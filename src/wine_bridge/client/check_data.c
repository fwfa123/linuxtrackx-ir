#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "rest.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
  printf("DEBUG: check_data.exe starting...\n");
  fflush(stdout);
  
  // Add detailed environment debugging
  printf("DEBUG: Environment variables check:\n");
  printf("DEBUG: HOME = %s\n", getenv("HOME") ? getenv("HOME") : "NULL");
  printf("DEBUG: USERPROFILE = %s\n", getenv("USERPROFILE") ? getenv("USERPROFILE") : "NULL");
  printf("DEBUG: USER = %s\n", getenv("USER") ? getenv("USER") : "NULL");
  printf("DEBUG: PWD = %s\n", getenv("PWD") ? getenv("PWD") : "NULL");
  printf("DEBUG: WINEPREFIX = %s\n", getenv("WINEPREFIX") ? getenv("WINEPREFIX") : "NULL");
  fflush(stdout);
  
  //signatures
  char verse1[200], verse2[200];
  game_desc_t gd;
  
  printf("DEBUG: About to initialize game_desc_t structure...\n");
  fflush(stdout);
  
  // Initialize the structure to prevent undefined behavior
  memset(&gd, 0, sizeof(game_desc_t));
  
  printf("DEBUG: game_desc_t structure initialized\n");
  fflush(stdout);
  
  // Fix for Wine environment: getenv("HOME") can return NULL
  printf("DEBUG: Getting HOME environment variable...\n");
  fflush(stdout);
  char *home = getenv("HOME");
  if (home == NULL) {
    printf("DEBUG: HOME is NULL, trying USERPROFILE...\n");
    fflush(stdout);
    // Fallback for Wine environments where HOME is not set
    home = getenv("USERPROFILE");
    if (home == NULL) {
      printf("DEBUG: USERPROFILE is NULL, trying USER...\n");
      fflush(stdout);
      // Try to construct home from USER
      char *user = getenv("USER");
      if (user != NULL) {
        printf("DEBUG: Using USER to construct home path\n");
        fflush(stdout);
        home = "/home/";
        // Note: This is a simplified approach - in practice we'd need to allocate memory
        // For now, let's use a more robust fallback
      }
      if (home == NULL) {
        printf("DEBUG: All fallbacks failed, using current directory\n");
        fflush(stdout);
        // Final fallback to current directory
        home = ".";
      }
    }
  }
  
  printf("DEBUG: Using home directory: %s\n", home);
  fflush(stdout);
  
  // Get current working directory for debugging
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("DEBUG: Current working directory: %s\n", cwd);
  } else {
    printf("DEBUG: Failed to get current working directory\n");
  }
  fflush(stdout);
  
  // Check if firmware directory exists before trying to access gamedata.txt
  printf("DEBUG: Allocating memory for firmware_path...\n");
  fflush(stdout);
  char *firmware_path = malloc(200 + strlen(home));
  if (firmware_path == NULL) {
    printf("DEBUG: Memory allocation failed for firmware_path!\n");
    fflush(stdout);
    MessageBox(NULL,
    "Memory allocation failed!\nInstallation may be incomplete.",
    "Linuxtrack-wine check",
    MB_OK);
    return 1;
  }
  
  printf("DEBUG: Building firmware path...\n");
  fflush(stdout);
  sprintf(firmware_path, "%s/.config/linuxtrack/tir_firmware", home);
  printf("DEBUG: Firmware path: %s\n", firmware_path);
  fflush(stdout);
  
  // Check if the firmware directory exists
  printf("DEBUG: Checking if firmware directory exists...\n");
  fflush(stdout);
  if (access(firmware_path, F_OK) != 0) {
    printf("DEBUG: Firmware directory does not exist!\n");
    fflush(stdout);
    MessageBox(NULL,
    "LinuxTrack firmware files not found!\n\n"
    "Please run the main LinuxTrack application first to extract the firmware files,\n"
    "then run this wine installer again.",
    "Linuxtrack-wine check",
    MB_OK);
    free(firmware_path);
    return 0; // Exit gracefully, don't crash
  }
  
  printf("DEBUG: Firmware directory exists\n");
  fflush(stdout);
  
  // Check if gamedata.txt exists
  printf("DEBUG: Allocating memory for gamedata_path...\n");
  fflush(stdout);
  char *gamedata_path = malloc(200 + strlen(home));
  if (gamedata_path == NULL) {
    printf("DEBUG: Memory allocation failed for gamedata_path!\n");
    fflush(stdout);
    MessageBox(NULL,
    "Memory allocation failed!\nInstallation may be incomplete.",
    "Linuxtrack-wine check",
    MB_OK);
    free(firmware_path);
    return 1;
  }
  
  printf("DEBUG: Building gamedata path...\n");
  fflush(stdout);
  sprintf(gamedata_path, "%s/.config/linuxtrack/tir_firmware/gamedata.txt", home);
  printf("DEBUG: Gamedata path: %s\n", gamedata_path);
  fflush(stdout);
  
  if (access(gamedata_path, F_OK) != 0) {
    printf("DEBUG: Gamedata file does not exist!\n");
    fflush(stdout);
    MessageBox(NULL,
    "LinuxTrack game data file not found!\n\n"
    "Please run the main LinuxTrack application first to extract the firmware files,\n"
    "then run this wine installer again.",
    "Linuxtrack-wine check",
    MB_OK);
    free(firmware_path);
    free(gamedata_path);
    return 0; // Exit gracefully, don't crash
  }
  
  printf("DEBUG: Gamedata file exists\n");
  fflush(stdout);
  
  free(gamedata_path);
  
  // Now try to get game data - this should work since we've verified the file exists
  printf("DEBUG: About to call game_data_get_desc...\n");
  fflush(stdout);
  
  bool game_data_result = game_data_get_desc(1001, &gd);
  printf("DEBUG: game_data_get_desc returned: %s\n", game_data_result ? "true" : "false");
  fflush(stdout);
  
  printf("DEBUG: About to call getSomeSeriousPoetry...\n");
  fflush(stdout);
  bool poetry_result = getSomeSeriousPoetry(verse1, verse2);
  printf("DEBUG: getSomeSeriousPoetry returned: %s\n", poetry_result ? "true" : "false");
  fflush(stdout);
  
  if(game_data_result && poetry_result){
    printf("DEBUG: Both functions succeeded, all is OK\n");
    fflush(stdout);
    //data available, all is OK
  }else{
    printf("DEBUG: One or both functions failed\n");
    fflush(stdout);
    MessageBox(NULL,
    "To fully utilize linuxtrack-wine,\ninstall the support data in ltr_gui!",
    "Linuxtrack-wine check",
    MB_OK);
  }
  
  printf("DEBUG: Allocating memory for path1...\n");
  fflush(stdout);
  char *path1 = malloc(200 + strlen(home));
  if (path1 == NULL) {
    printf("DEBUG: Memory allocation failed for path1!\n");
    fflush(stdout);
    MessageBox(NULL,
    "Memory allocation failed!\nInstallation may be incomplete.",
    "Linuxtrack-wine check",
    MB_OK);
    free(firmware_path);
    return 1;
  }
  
  printf("DEBUG: Building TIRViews.dll path...\n");
  fflush(stdout);
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/TIRViews.dll", home);
  printf("DEBUG: TIRViews.dll path: %s\n", path1);
  fflush(stdout);
  
  printf("DEBUG: Creating symlink for TIRViews.dll...\n");
  printf("DEBUG: Target path: %s\n", path1);
  printf("DEBUG: Link name: TIRViews.dll\n");
  printf("DEBUG: Current working directory: %s\n", cwd);
  fflush(stdout);
  if(symlink(path1, "TIRViews.dll") != 0){
    printf("DEBUG: Failed to create symlink for TIRViews.dll (error: %d)\n", errno);
    fflush(stdout);
    
    // Check if error is EEXIST (file already exists) - this is not a problem
    if(errno == EEXIST) {
      printf("DEBUG: TIRViews.dll symlink already exists (this is normal)\n");
      fflush(stdout);
    } else {
      // Show detailed error information to user only for real errors
      char error_msg[512];
      sprintf(error_msg, 
        "Failed to create symlink to TIRViews.dll!\n\n"
        "Error code: %d\n"
        "Target path: %s\n"
        "Working directory: %s\n\n"
        "This usually means the TrackIR firmware files are not properly installed.\n"
        "Please run the TrackIR firmware extraction in the LinuxTrack GUI first.",
        errno, path1, cwd);
      
      MessageBox(NULL, error_msg, "Linuxtrack-wine check", MB_OK);
    }
  } else {
    printf("DEBUG: Successfully created symlink for TIRViews.dll\n");
    fflush(stdout);
  }

  printf("DEBUG: Building MFC library paths...\n");
  fflush(stdout);
  
  // Create MFC42 symlink (simplified approach)
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
  printf("DEBUG: mfc42u.dll path: %s\n", path1);
  fflush(stdout);
  
  printf("DEBUG: Creating symlink for mfc42u.dll...\n");
  fflush(stdout);
  if(symlink(path1, "mfc42u.dll") != 0){
    printf("DEBUG: Failed to create symlink for mfc42u.dll (error: %d)\n", errno);
    fflush(stdout);
    
    // Check if error is EEXIST (file already exists) - this is not a problem
    if(errno == EEXIST) {
      printf("DEBUG: mfc42u.dll symlink already exists (this is normal)\n");
      fflush(stdout);
    } else {
      // Show detailed error information to user only for real errors
      char error_msg[512];
      sprintf(error_msg, 
        "Failed to create symlink to mfc42u.dll!\n\n"
        "Error code: %d\n"
        "Target path: %s\n"
        "Working directory: %s\n\n"
        "This usually means the MFC42 library is not properly installed.\n"
        "Please install MFC42 using winetricks:\n"
        "winetricks mfc42\n"
        "Or run the MFC42 installation in the LinuxTrack GUI first.",
        errno, path1, cwd);
      
      MessageBox(NULL, error_msg, "Linuxtrack-wine check", MB_OK);
    }
  } else {
    printf("DEBUG: Successfully created symlink for mfc42u.dll\n");
    fflush(stdout);
  }

  printf("DEBUG: Cleaning up memory...\n");
  fflush(stdout);
  free(path1);
  free(firmware_path);
  
  printf("DEBUG: check_data.exe completed successfully\n");
  fflush(stdout);
  return 0;
}

