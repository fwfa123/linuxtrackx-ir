/*
 * ltr_udp.c
 *
 * Bridge to send Linuxtrack pose data via UDP in the format supported by
 * X4: Foundations and other OpenTrack-compatible games.
 *
 * Original author: StarTuz on GitHub
 *      Repository: https://github.com/StarTuz/linuxtrack-Qt6-Wayland
 *     Commit Hash: 515803af3d263391d8f447b55d14885a95aeafba
 *   Integrated by: Mike Becker (https://gitlab.com/uap-universe/)
 *
 * Protocol: 6 doubles (little-endian)
 * [X, Y, Z, Yaw, Pitch, Roll]
 * X,Y,Z in cm.
 * Yaw, Pitch, Roll in degrees.
 *
 * Connects to localhost:4242 by default.
 */

#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "linuxtrack.h"

/* Default Opentrack UDP port */
#define DEFAULT_PORT 4242
#define DEFAULT_HOST "127.0.0.1"

static volatile bool keep_running = true;
static int sock_fd = -1;
static struct sockaddr_in dest_addr;
static bool verbose = false;

void signal_handler(int dummy) {
  (void)dummy;
  keep_running = false;
}

/* OpenTrack UDP Packet Structure: 6 doubles (Standard)
 * Order: X, Y, Z, Yaw, Pitch, Roll */
typedef struct __attribute__((packed)) {
  double x;     /* cm */
  double y;     /* cm */
  double z;     /* cm */
  double yaw;   /* degrees */
  double pitch; /* degrees */
  double roll;  /* degrees */
} opentrack_udp_t;

/* Freetrack UDP Packet Structure (Alternative) */
typedef struct __attribute__((packed)) {
  int32_t data_id;
  float cam_width;
  float cam_height;
  float yaw;
  float pitch;
  float roll;
  float x;
  float y;
  float z;
} freetrack_udp_t;

typedef enum { PROTO_OPENTRACK, PROTO_FREETRACK } proto_t;

/* Return value after --key= or next argv for --key value; advances *i when needed. */
static const char *opt_value(int argc, char *argv[], int *i, const char *eq)
{
  if (eq != NULL && eq[0] != '\0') {
    return eq;
  }
  if (*i + 1 < argc) {
    (*i)++;
    return argv[*i];
  }
  return NULL;
}

static void str_tolower_copy(char *dst, size_t dst_sz, const char *src)
{
  size_t n = 0;
  if (dst_sz == 0) {
    return;
  }
  while (src[n] != '\0' && n + 1 < dst_sz) {
    dst[n] = (char)tolower((unsigned char)src[n]);
    n++;
  }
  dst[n] = '\0';
}

static bool parse_proto(const char *value, proto_t *protocol)
{
  char lower[32];
  if (value == NULL || protocol == NULL) {
    return false;
  }
  str_tolower_copy(lower, sizeof(lower), value);
  if (strcmp(lower, "freetrack") == 0) {
    *protocol = PROTO_FREETRACK;
    return true;
  }
  if (strcmp(lower, "opentrack") == 0) {
    *protocol = PROTO_OPENTRACK;
    return true;
  }
  return false;
}

static void print_usage(const char *argv0)
{
  fprintf(stderr,
          "Usage: %s [--proto=opentrack|freetrack] [--ip=HOST] [--port=PORT] "
          "[--profile=NAME] [--verbose] [PORT]\n",
          argv0);
}

int main(int argc, char *argv[]) {
  linuxtrack_pose_t pose;
  /* Blobs buffer required by API even if we don't use it */
  float blobs[30];
  int blobs_read;
  const char *target_ip = DEFAULT_HOST;
  int target_port = DEFAULT_PORT;
  proto_t protocol = PROTO_OPENTRACK;
  const char *profile_name = NULL; /* NULL = use "Default" profile */
  const char *dbg = getenv("LINUXTRACK_DBG");
  const char *udp_verbose = getenv("LTR_UDP_VERBOSE");

  if ((dbg != NULL && strchr(dbg, 'u') != NULL) ||
      (udp_verbose != NULL && udp_verbose[0] != '\0' && udp_verbose[0] != '0')) {
    verbose = true;
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      print_usage(argv[0]);
      return 0;
    }
    if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
      verbose = true;
      continue;
    }

    if (strncmp(argv[i], "--proto=", 8) == 0 || strcmp(argv[i], "--proto") == 0) {
      const char *eq = (strncmp(argv[i], "--proto=", 8) == 0) ? argv[i] + 8 : NULL;
      const char *val = opt_value(argc, argv, &i, eq);
      if (!parse_proto(val, &protocol)) {
        fprintf(stderr, "Invalid --proto value (use opentrack or freetrack)\n");
        return 1;
      }
      continue;
    }
    if (strncmp(argv[i], "--ip=", 5) == 0 || strcmp(argv[i], "--ip") == 0) {
      const char *eq = (strncmp(argv[i], "--ip=", 5) == 0) ? argv[i] + 5 : NULL;
      const char *val = opt_value(argc, argv, &i, eq);
      if (val == NULL || val[0] == '\0') {
        fprintf(stderr, "Missing value for --ip\n");
        return 1;
      }
      target_ip = val;
      continue;
    }
    if (strncmp(argv[i], "--port=", 7) == 0 || strcmp(argv[i], "--port") == 0) {
      const char *eq = (strncmp(argv[i], "--port=", 7) == 0) ? argv[i] + 7 : NULL;
      const char *val = opt_value(argc, argv, &i, eq);
      if (val == NULL) {
        fprintf(stderr, "Missing value for --port\n");
        return 1;
      }
      target_port = atoi(val);
      continue;
    }
    if (strncmp(argv[i], "--profile=", 10) == 0 || strcmp(argv[i], "--profile") == 0) {
      const char *eq = (strncmp(argv[i], "--profile=", 10) == 0) ? argv[i] + 10 : NULL;
      const char *val = opt_value(argc, argv, &i, eq);
      if (val == NULL || val[0] == '\0') {
        fprintf(stderr, "Missing value for --profile\n");
        return 1;
      }
      profile_name = val;
      continue;
    }

    /* Positional port shorthand */
    {
      char *end = NULL;
      long p = strtol(argv[i], &end, 10);
      if (end != argv[i] && *end == '\0' && p > 1024 && p <= 65535) {
        target_port = (int)p;
        continue;
      }
    }

    fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
    print_usage(argv[0]);
    return 1;
  }

  if (target_port < 1 || target_port > 65535) {
    fprintf(stderr, "Invalid UDP port: %d\n", target_port);
    return 1;
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0) {
    perror("Failed to create socket");
    return 1;
  }

  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons((uint16_t)target_port);
  if (inet_pton(AF_INET, target_ip, &dest_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    return 1;
  }

  printf("ltr_udp starting...\n");
  printf("Protocol: %s\n", (protocol == PROTO_OPENTRACK) ? "OpenTrack (UDP)" : "FreeTrack");
  printf("Target:   %s:%d\n", target_ip, target_port);
  printf("Profile:  %s\n", profile_name ? profile_name : "Default");
  printf("Press Ctrl+C to stop.\n");

  printf("ltr_udp: Initializing Linuxtrack with profile '%s'...\n",
         profile_name ? profile_name : "Default");
  linuxtrack_state_type state = linuxtrack_init(profile_name);
  if (state < LINUXTRACK_OK) {
    fprintf(stderr, "Failed to initialize linuxtrack: %s\n",
            linuxtrack_explain(state));
    return 1;
  }

  printf("ltr_udp: Waiting for tracker state RUNNING/PAUSED...\n");
  int timeout = 0;
  while (timeout < 50 && keep_running) {
    state = linuxtrack_get_tracking_state();
    if (state == RUNNING || state == PAUSED) {
      printf("ltr_udp: Tracker is active (state: %d)\n", state);
      break;
    }
    usleep(200000); /* 200ms */
    timeout++;
  }

  if (state != RUNNING && state != PAUSED) {
    fprintf(stderr, "Tracker failed to start. Is ltr_server1 running?\n");
    linuxtrack_shutdown();
    return 1;
  }

  linuxtrack_notification_on();
  printf("ltr_udp: Notifications enabled, starting main loop.\n");

  long frame_count = 0;

  while (keep_running) {
    linuxtrack_wait(10);

    int result = linuxtrack_get_pose_full(&pose, blobs, 10, &blobs_read);
    (void)result;

    {
      ssize_t sent_bytes = 0;

      if (protocol == PROTO_OPENTRACK) {
        opentrack_udp_t packet;
        /* Linuxtrack mm -> cm */
        packet.x = (double)pose.tx / 10.0;
        packet.y = (double)pose.ty / 10.0;
        packet.z = (double)pose.tz / 10.0;

        /* OpenTrack: positive yaw = look right; invert LinuxTrack yaw */
        packet.yaw = -(double)pose.yaw;
        packet.pitch = (double)pose.pitch;
        packet.roll = (double)pose.roll;

        sent_bytes = sendto(sock_fd, &packet, sizeof(packet), 0,
                            (struct sockaddr *)&dest_addr, sizeof(dest_addr));
      } else {
        freetrack_udp_t packet;
        packet.data_id = 2;
        packet.cam_width = 0;
        packet.cam_height = 0;
        packet.yaw = -(float)pose.yaw;
        packet.pitch = (float)pose.pitch;
        packet.roll = (float)pose.roll;
        packet.x = (float)pose.tx;
        packet.y = (float)pose.ty;
        packet.z = (float)pose.tz;

        sent_bytes = sendto(sock_fd, &packet, sizeof(packet), 0,
                            (struct sockaddr *)&dest_addr, sizeof(dest_addr));
      }

      if (sent_bytes < 0) {
        perror("Error sending UDP packet");
      }

      if (verbose && frame_count % 60 == 0) {
        printf("ltr_udp: Sent package %ld | Yaw: %.2f Pitch: %.2f Roll: %.2f\n",
               frame_count, pose.yaw, pose.pitch, pose.roll);
      }
      frame_count++;
    }
  }

  printf("\nShutting down...\n");
  linuxtrack_shutdown();
  close(sock_fd);
  return 0;
}
