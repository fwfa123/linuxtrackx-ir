#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ltr_int_get_default_file_name(const char *fname)
{
  const char *home = getenv("HOME");
  if(home == NULL){
    home = getenv("USERPROFILE");
  }
  if(home == NULL){
    return NULL;
  }
  if(fname == NULL){
    fname = "linuxtrack1.conf";
  }
  size_t need = strlen(home) + strlen("/.config/linuxtrack/") + strlen(fname) + 1;
  char *path = (char*)malloc(need);
  if(path == NULL){
    return NULL;
  }
  snprintf(path, need, "%s/.config/linuxtrack/%s", home, fname);
  return path;
}
