#include "ltr_srv_comm.h"
#include "ltr_srv_slave.h"
#include "ltr_srv_master.h"
#include "utils.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
#ifndef DARWIN
  ltr_int_check_root();
#endif
  signal(SIGPIPE, SIG_IGN);

  if (argc >= 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
    printf("ltr_server1 - LinuxTrack tracking daemon\n");
    printf("Usage: ltr_server1\n");
    printf("  Run with no arguments. Usually started from ltr_gui or by the client library.\n");
    return 0;
  }

  if (argc == 1) {
    ltr_int_master(true);
  } else if (argc >= 6) {
    /* Arguments are passed by the master when spawning a slave; not for direct use. */
    ltr_int_slave(argv[1], argv[2], argv[3], argv[4], argv[5]);
  } else {
    fprintf(stderr, "Invalid arguments. Use --help for usage.\n");
    return 1;
  }
  return 0;
}

