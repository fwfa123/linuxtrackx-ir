#ifndef LTR_UNIX_CONFIG_HOME_H
#define LTR_UNIX_CONFIG_HOME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Wine/Proton often clears HOME for Win32 PEs; USERPROFILE then points at the
 * bottle (e.g. C:\users\steamuser), not the real Linux home where tir_firmware
 * and gamedata.txt live. Set LINUXTRACK_UNIX_HOME to the Unix home path (same
 * as `echo $HOME` on the host) so NPClient, Tester64, and friends read
 * ~/.config/linuxtrack/... correctly. Checked before HOME, then USERPROFILE. */
static inline const char *ltr_unix_home_for_config(void)
{
  static char unix_user_home[512];
  const char *h = getenv("LINUXTRACK_UNIX_HOME");
  if(h && *h){
    return h;
  }
  h = getenv("HOME");
  if(h && *h
#ifdef __MINGW32__
     && h[0] == '/'
#endif
  ){
    return h;
  }
#ifdef __MINGW32__
  h = getenv("USER");
  if(h && *h && strcmp(h, "steamuser") != 0){
    snprintf(unix_user_home, sizeof(unix_user_home), "/home/%s", h);
    return unix_user_home;
  }
  h = getenv("LOGNAME");
  if(h && *h && strcmp(h, "steamuser") != 0){
    snprintf(unix_user_home, sizeof(unix_user_home), "/home/%s", h);
    return unix_user_home;
  }
#endif
  h = getenv("USERPROFILE");
  if(h && *h){
    return h;
  }
  return ".";
}

#endif
