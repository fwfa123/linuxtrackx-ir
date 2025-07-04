#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "rest.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  //signatures
  char verse1[200], verse2[200];
  game_desc_t gd;
  if(game_data_get_desc(1001, &gd) && getSomeSeriousPoetry(verse1, verse2)){
    //data available, all is OK
  }else{
    MessageBox(NULL,
    "To fully utilize linuxtrack-wine,\ninstall the support data in ltr_gui!",
    "Linuxtrack-wine check",
    MB_OK);
  }
  char *home = getenv("HOME");
  char *path1 = malloc(200 + strlen(home));
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/TIRViews.dll", home);
#ifndef __MINGW32__
  if(symlink(path1, "TIRViews.dll") != 0){
    MessageBox(NULL,
    "Failed to create symlink to TIRViews.dll!\nSome games will not have headtracking available.",
    "Linuxtrack-wine check",
    MB_OK);
  }
#else
  // On MinGW/Windows, copy the file instead of creating a symlink
  if(!CopyFile(path1, "TIRViews.dll", FALSE)){
    MessageBox(NULL,
    "Failed to copy TIRViews.dll!\nSome games will not have headtracking available.",
    "Linuxtrack-wine check",
    MB_OK);
  }
#endif
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
#ifndef __MINGW32__
  if(symlink(path1, "mfc42u.dll") != 0){
    MessageBox(NULL,
    "Failed to create symlink to mfc42u.dll!\n"
    "Try to install TIRViews support in ltr_gui,\n"
    "or install mfc42 into this bottle using winetricks.",
    "Linuxtrack-wine check",
    MB_OK);
  }
#else
  // On MinGW/Windows, copy the file instead of creating a symlink
  if(!CopyFile(path1, "mfc42u.dll", FALSE)){
    MessageBox(NULL,
    "Failed to copy mfc42u.dll!\n"
    "Try to install TIRViews support in ltr_gui,\n"
    "or install mfc42 into this bottle using winetricks.",
    "Linuxtrack-wine check",
    MB_OK);
  }
#endif
  return 0;
}

