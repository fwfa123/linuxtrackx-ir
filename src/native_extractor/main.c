#define _POSIX_C_SOURCE 200809L

#include "native_installer.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

static void usage(FILE *stream, const char *program)
{
  fprintf(stream,
          "Usage: %s --installer FILE --destination DIR [OPTIONS]\n"
          "       %s --installer FILE --list\n\n"
          "Extract verified TrackIR firmware directly from a supported Windows\n"
          "installer without running Wine or vendor code.\n\n"
          "Options:\n"
          "  -i, --installer FILE     NaturalPoint TrackIR installer\n"
          "  -d, --destination DIR   New or existing firmware directory\n"
          "  -l, --list              Identify and list outputs without extracting\n"
          "  -h, --help              Show this help\n",
          program, program);
}

int main(int argc, char **argv)
{
  const char *installer = NULL;
  const char *destination = NULL;
  bool list_only = false;
  int option;
  static const struct option options[] = {
    {"installer", required_argument, NULL, 'i'},
    {"destination", required_argument, NULL, 'd'},
    {"list", no_argument, NULL, 'l'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
  };

  while ((option = getopt_long(argc, argv, "i:d:lh", options, NULL)) != -1) {
    switch (option) {
      case 'i': installer = optarg; break;
      case 'd': destination = optarg; break;
      case 'l': list_only = true; break;
      case 'h': usage(stdout, argv[0]); return 0;
      default: usage(stderr, argv[0]); return 2;
    }
  }
  if (installer == NULL || (!list_only && destination == NULL) || optind != argc) {
    usage(stderr, argv[0]);
    return 2;
  }
  return ltr_native_extract_installer(installer, destination,
                                      list_only) == 0 ? 0 : 1;
}
