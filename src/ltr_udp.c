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
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "linuxtrack.h"

// Default Opentrack UDP port
#define DEFAULT_PORT 4242
#define DEFAULT_HOST "127.0.0.1"

static volatile bool keep_running = true;
static int sock_fd = -1;
static struct sockaddr_in dest_addr;

void signal_handler(int dummy) {
  (void)dummy;
  keep_running = false;
}

// OpenTrack UDP Packet Structure: 6 doubles (Standard)
// Order: X, Y, Z, Yaw, Pitch, Roll
typedef struct __attribute__((packed)) {
  double x;     // cm
  double y;     // cm
  double z;     // cm
  double yaw;   // degrees
  double pitch; // degrees
  double roll;  // degrees
} opentrack_udp_t;

// Freetrack UDP Packet Structure (Alternative)
// Needs explicit 4-byte fields
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

int main(int argc, char *argv[]) {
  linuxtrack_pose_t pose;
  // Blobs buffer required by API even if we don't use it
  float blobs[30];
  int blobs_read;
  const char *target_ip = DEFAULT_HOST;
  int target_port = DEFAULT_PORT;
  proto_t protocol = PROTO_OPENTRACK;
  const char *profile_name = NULL;  // NULL = use "Default" profile

  // Parse simple arguments
  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--proto=freetrack", 17) == 0) {
      protocol = PROTO_FREETRACK;
    } else if (strncmp(argv[i], "--ip=", 5) == 0) {
      target_ip = argv[i] + 5;
    } else if (strncmp(argv[i], "--port=", 7) == 0) {
      target_port = atoi(argv[i] + 7);
    } else if (strncmp(argv[i], "--profile=", 10) == 0) {
      profile_name = argv[i] + 10;
    } else {
      // Assume port if just a number
      int p = atoi(argv[i]);
      if (p > 1024)
        target_port = p;
    }
  }

  // Setup signal handling
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  // Initialize UDP socket
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0) {
    perror("Failed to create socket");
    return 1;
  }

  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(target_port);
  if (inet_pton(AF_INET, target_ip, &dest_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    return 1;
  }

  printf("ltr_udp v1.2.0 starting...\n");
  printf("Protocol: %s\n", (protocol == PROTO_OPENTRACK) ? "OpenTrack (UDP)" : "FreeTrack");
  printf("Target:   %s:%d\n", target_ip, target_port);
  printf("Profile:  %s\n", profile_name ? profile_name : "Default");
  printf("Press Ctrl+C to stop.\n");

  // Initialize Linuxtrack with specified profile
  printf("ltr_udp: Initializing Linuxtrack with profile '%s'...\n",
         profile_name ? profile_name : "Default");
  linuxtrack_state_type state = linuxtrack_init(profile_name);
  if (state < LINUXTRACK_OK) {
    fprintf(stderr, "Failed to initialize linuxtrack: %s\n",
            linuxtrack_explain(state));
    return 1;
  }

  // Wait for tracker to start
  printf("ltr_udp: Waiting for tracker state RUNNING/PAUSED...\n");
  int timeout = 0;
  while (timeout < 50 && keep_running) {
    state = linuxtrack_get_tracking_state();
    if (state == RUNNING || state == PAUSED) {
      printf("ltr_udp: Tracker IS ACTIVE! (State: %d)\n", state);
      break;
    }
    usleep(200000); // 200ms
    timeout++;
  }

  if (state != RUNNING && state != PAUSED) {
    fprintf(stderr, "Tracker failed to start. Is ltr_server1 running?\n");
    linuxtrack_shutdown();
    return 1;
  }

  // Enable notifications so we get pose updates
  linuxtrack_notification_on();
  printf("ltr_udp: Notifications enabled, starting main loop.\n");

  // Main Loop
  long frame_count = 0;

  while (keep_running) {
    // Wait for frame (blocking wait to reduce CPU usage)
    // Wait BEFORE getting data, match original osc_server cadence
    linuxtrack_wait(10);

    int result = linuxtrack_get_pose_full(&pose, blobs, 10, &blobs_read);

    // Debug: show result every 100 iterations
    static long debug_count = 0;
    if (debug_count % 100 == 0) {
      printf("ltr_udp: get_pose_full returned %d, blobs=%d, yaw=%.2f\n", result,
             blobs_read, pose.yaw);
    }
    debug_count++;

    // ALWAYS send packets (even if result==0) for testing
    {
      ssize_t sent_bytes = 0;

      if (protocol == PROTO_OPENTRACK) {
        opentrack_udp_t packet;
        // Linuxtrack mm -> cm
        packet.x = (double)pose.tx / 10.0;
        packet.y = (double)pose.ty / 10.0;
        packet.z = (double)pose.tz / 10.0;

        // OpenTrack convention: positive yaw = look right
        // Linuxtrack convention: positive yaw = look left (from face tracking geometry)
        // Invert yaw to match OpenTrack expectations
        packet.yaw = -(double)pose.yaw;
        packet.pitch = (double)pose.pitch;
        packet.roll = (double)pose.roll;

        sent_bytes = sendto(sock_fd, &packet, sizeof(packet), 0,
                            (struct sockaddr *)&dest_addr, sizeof(dest_addr));
      } else {
        freetrack_udp_t packet;
        packet.data_id = 2; // ID?
        packet.cam_width = 0;
        packet.cam_height = 0;
        // FreeTrack convention: same as OpenTrack (positive yaw = look right)
        packet.yaw = -(float)pose.yaw;
        packet.pitch = (float)pose.pitch;
        packet.roll = (float)pose.roll;
        packet.x =
            (float)pose.tx; // FT might want mm? or cm? Stick to raw mm for now
        packet.y = (float)pose.ty;
        packet.z = (float)pose.tz;

        sent_bytes = sendto(sock_fd, &packet, sizeof(packet), 0,
                            (struct sockaddr *)&dest_addr, sizeof(dest_addr));
      }

      if (sent_bytes < 0) {
        perror("Error sending UDP packet");
      }

      // Debug output every ~60 frames (approx 1-2 sec)
      if (frame_count % 60 == 0) {
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
