#ifndef LTR_UNIX_CONFIG_HOME_H
#define LTR_UNIX_CONFIG_HOME_H

#include <stdlib.h>

/* Wine/Proton often clears HOME for Win32 PEs; USERPROFILE then points at the
 * bottle (e.g. C:\users\steamuser), not the real Linux home where tir_firmware
 * and gamedata.txt live. Set LINUXTRACK_UNIX_HOME to the Unix home path (same
 * as `echo $HOME` on the host) so NPClient, Tester64, and friends read
 * ~/.config/linuxtrack/... correctly. Checked before HOME, then USERPROFILE. */
static inline const char *ltr_unix_home_for_config(void)
{
  const char *h = getenv("LINUXTRACK_UNIX_HOME");
  if(h && *h){
    return h;
  }
  h = getenv("HOME");
  if(h && *h){
    return h;
  }
  h = getenv("USERPROFILE");
  if(h && *h){
    return h;
  }
  return ".";
}

#endif
