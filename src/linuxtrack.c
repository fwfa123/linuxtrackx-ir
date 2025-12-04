/*
The MIT License

Copyright (c) 2009 Tulthix, uglyDwarf

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/





#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef __MINGW32__
#include <dlfcn.h>
#endif
#include <unistd.h>
#include <string.h>
#include "linuxtrack.h"
#include "utils.h"

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#define BLOB_ELEMENTS 3

typedef linuxtrack_state_type (*ltr_gp_t)(void);
typedef linuxtrack_state_type (*ltr_init_t)(const char *cust_section);
typedef int (*ltr_get_pose_t)(float *heading,
                         float *pitch,
                         float *roll,
                         float *tx,
                         float *ty,
                         float *tz,
                         uint32_t *counter);
typedef int (*ltr_get_pose_full_t)(linuxtrack_pose_t *pose, float blobs[], int num_blobs, int *blobs_read);
typedef linuxtrack_state_type (*ltr_get_tracking_state_t)(void);
typedef const char *(*ltr_explain_t)(linuxtrack_state_type);
typedef int (*ltr_get_frame_t)(int *req_width, int *req_height, size_t buf_size, uint8_t *buffer);
typedef int (*ltr_get_notify_pipe_t)(void);
typedef int (*ltr_wait_t)(int timeout);


static ltr_init_t ltr_init_fun = NULL;
static ltr_gp_t ltr_shutdown_fun = NULL;
static ltr_gp_t ltr_suspend_fun = NULL;
static ltr_gp_t ltr_wakeup_fun = NULL;
static ltr_gp_t ltr_recenter_fun = NULL;
static ltr_get_pose_t ltr_get_pose_fun = NULL;
static ltr_get_pose_full_t ltr_get_pose_full_fun = NULL;
static ltr_get_tracking_state_t ltr_get_tracking_state_fun = NULL;
static ltr_explain_t ltr_explain_fun = NULL;
static ltr_get_pose_t ltr_get_abs_pose_fun = NULL;
static ltr_gp_t ltr_request_frames_fun = NULL;
static ltr_get_frame_t ltr_get_frame_fun = NULL;
static ltr_gp_t ltr_notification_on_fun = NULL;
static ltr_get_notify_pipe_t ltr_get_notify_pipe_fun = NULL;
static ltr_wait_t ltr_wait_fun = NULL;

static void *lib_handle = NULL;

struct func_defs_t{
  char *name;
  void *ref;
  int mandatory;
};

static struct func_defs_t functions[] =
{
  {(char*)"ltr_init", (void*)&ltr_init_fun, 1},
  {(char*)"ltr_shutdown", (void*)&ltr_shutdown_fun, 1},
  {(char*)"ltr_suspend", (void *)&ltr_suspend_fun, 1},
  {(char*)"ltr_wakeup", (void *)&ltr_wakeup_fun, 1},
  {(char*)"ltr_recenter", (void *)&ltr_recenter_fun, 1},
  {(char*)"ltr_get_pose", (void *)&ltr_get_pose_fun, 1},
  {(char*)"ltr_get_pose_full", (void *)&ltr_get_pose_full_fun, 1},
  {(char*)"ltr_get_tracking_state", (void *)&ltr_get_tracking_state_fun, 1},
  {(char*)"ltr_explain", (void *)&ltr_explain_fun, 0},
  {(char*)"ltr_get_abs_pose", (void *)&ltr_get_abs_pose_fun, 1},
  {(char*)"ltr_request_frames", (void *)&ltr_request_frames_fun, 0},
  {(char*)"ltr_get_frame", (void *)&ltr_get_frame_fun, 0},
  {(char*)"ltr_notification_on", (void *)&ltr_notification_on_fun, 0},
  {(char*)"ltr_get_notify_pipe", (void *)&ltr_get_notify_pipe_fun, 0},
  {(char*)"ltr_wait", (void *)&ltr_wait_fun, 0},
  {(char*)NULL, NULL, 0}
};

static const char *lib_locations[] = {
"/Frameworks/liblinuxtrack.0.dylib",
"/lib/linuxtrack/liblinuxtrack.so.0",
"/lib32/linuxtrack/liblinuxtrack32.so.0",
"/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0",
"/lib/i386-linux-gnu/linuxtrack/liblinuxtrack32.so.0",
"/lib/x86_64-linux-gnu/linuxtrack/liblinuxtrack.so.0",
/* old paths */
"/lib/liblinuxtrack.so.0",
"/lib32/liblinuxtrack.so.0",
"/lib32/liblinuxtrack32.so.0",
"/lib/i386-linux-gnu/liblinuxtrack.so.0",
"/lib/x86_64-linux-gnu/liblinuxtrack.so.0",
NULL
};

static FILE *log_f = NULL;
static char logfname[] = "/tmp/linuxtrack.logXXXXXX";

static void linuxtrack_log(const char *format, ...)
{
  if(log_f == NULL){
    FILE *tmpf;
    int tmpfd = mkstemp(logfname);
    if(tmpfd != -1){
      tmpf = fdopen(tmpfd, "a");
      if(tmpf != NULL){
        log_f = tmpf;
      }
    }
  }
  va_list ap;
  va_start(ap,format);
  vfprintf(log_f, format, ap);
  fflush(log_f);
  va_end(ap);
}

linuxtrack_state_type linuxtrack_shutdown(void)
{
  linuxtrack_state_type res;
  if(ltr_shutdown_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  res = ltr_shutdown_fun();

  if(lib_handle != NULL){
    void *handle = lib_handle;
    lib_handle = NULL;
    ltr_init_fun = NULL;
    ltr_shutdown_fun = NULL;
    ltr_suspend_fun = NULL;
    ltr_wakeup_fun = NULL;
    ltr_recenter_fun = NULL;
    ltr_get_pose_fun = NULL;
    ltr_get_pose_full_fun = NULL;
    ltr_get_tracking_state_fun = NULL;
    ltr_explain_fun = NULL;
#ifndef __MINGW32__
    dlclose(handle);
#endif
  }
  return res;
}

linuxtrack_state_type linuxtrack_suspend(void)
{
  if(ltr_suspend_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_suspend_fun();
}

linuxtrack_state_type linuxtrack_wakeup(void)
{
  if(ltr_wakeup_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_wakeup_fun();
}

linuxtrack_state_type linuxtrack_recenter(void)
{
  if(ltr_recenter_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_recenter_fun();
}


//RetVal 0 means no new data
int linuxtrack_get_pose(float *heading,
                         float *pitch,
                         float *roll,
                         float *tx,
                         float *ty,
                         float *tz,
                         uint32_t *counter)
{
  if(ltr_get_pose_fun == NULL){
    *heading = *pitch = *roll = *tx = *ty = *tz = 0.0f;
    *counter = 0;
    return 0;
  }
  return ltr_get_pose_fun(heading, pitch, roll, tx, ty, tz, counter);
}

//RetVal 0 means no new data
int linuxtrack_get_abs_pose(float *heading,
                            float *pitch,
                            float *roll,
                            float *tx,
                            float *ty,
                            float *tz,
                            uint32_t *counter)
{
  if(ltr_get_abs_pose_fun == NULL){
    *heading = *pitch = *roll = *tx = *ty = *tz = 0.0f;
    *counter = 0;
    return 0;
  }
  return ltr_get_abs_pose_fun(heading, pitch, roll, tx, ty, tz, counter);
}

//RetVal 0 means no new data
int linuxtrack_get_pose_full(linuxtrack_pose_t *pose, float blobs[], int num_blobs, int *blobs_read)
{
  if(ltr_get_pose_full_fun == NULL){
    memset(pose, 0, sizeof(linuxtrack_pose_t));
    int i;
    for(i = 0; i < num_blobs * BLOB_ELEMENTS; ++i){
      blobs[i] = 0.0f;
    }
    return 0;
  }
  return ltr_get_pose_full_fun(pose, blobs, num_blobs, blobs_read);
}


linuxtrack_state_type linuxtrack_get_tracking_state(void)
{
  if(ltr_get_tracking_state_fun == NULL){
    linuxtrack_log("ltr_get_tracking_state_fun is NULL!\n");
    return err_NOT_INITIALIZED;
  }
  linuxtrack_state_type res = ltr_get_tracking_state_fun();
  linuxtrack_log("ltr_get_tracking_state_fun returned: %d\n", res);
  return res;
}


static int linuxtrack_load_functions(void *handle)
{
#ifndef __MINGW32__
  int i = 0;
  void *symbol;
  while((functions[i]).name != NULL){
    dlerror();
    symbol = dlsym(handle, (functions[i]).name);
    const char *error = dlerror();
    fprintf(stderr, "DEBUG: Trying to load symbol '%s': %p, error: %s\n", (functions[i]).name, symbol, error ? error : "none");
    linuxtrack_log("Trying to load symbol '%s': %p, error: %s\n", (functions[i]).name, symbol, error ? error : "none");
    if(symbol == NULL){
      fprintf(stderr, "DEBUG: Couldn't load symbol '%s': %s\n", (functions[i]).name, error);
      linuxtrack_log("Couldn't load symbol '%s': %s\n", (functions[i]).name, error);
      if((functions[i]).mandatory){
        fprintf(stderr, "DEBUG: Symbol '%s' is mandatory, failing\n", (functions[i]).name);
        linuxtrack_log("Symbol '%s' is mandatory, failing\n", (functions[i]).name);
        return err_SYMBOL_LOOKUP;
      }
    } else {
      fprintf(stderr, "DEBUG: Loaded symbol '%s' OK\n", (functions[i]).name);
      linuxtrack_log("Loaded symbol '%s' OK\n", (functions[i]).name);
    }
    *((void **)(functions[i]).ref) = symbol;
    ++i;
  }
  fprintf(stderr, "DEBUG: All symbols processed, ltr_init_fun = %p\n", ltr_init_fun);
  return LINUXTRACK_OK;
#else
  (void)handle; // Suppress unused parameter warning
  // For MinGW builds, we can't use dlopen/dlsym
  // The wine_bridge should link statically or use a different approach
  return err_SYMBOL_LOOKUP;
#endif
}


static char *construct_name(const char *path, const char *sep, const char *name)
{
  size_t len = strlen(path) + strlen(sep) + strlen(name) + 1;
  char *res = (char *)malloc(len);
  snprintf(res, len, "%s%s%s", path, sep, name);
  return res;
}

/**
 * Extract install prefix from LIB_PATH compile-time define
 * LIB_PATH is defined as "${CMAKE_INSTALL_PREFIX}/lib/linuxtrack/"
 * This function extracts the prefix by removing "/lib/linuxtrack/" suffix
 */
static char *get_install_prefix_from_lib_path(void)
{
#ifdef LIB_PATH
  char *lib_path = strdup(LIB_PATH);
  if(lib_path == NULL){
    return NULL;
  }
  
  // Remove trailing slash if present
  size_t len = strlen(lib_path);
  if(len > 0 && lib_path[len - 1] == '/'){
    lib_path[len - 1] = '\0';
    len--;
  }
  
  // Check if it ends with "/lib/linuxtrack" and extract prefix
  const char *suffix = "/lib/linuxtrack";
  size_t suffix_len = strlen(suffix);
  if(len >= suffix_len && strcmp(lib_path + len - suffix_len, suffix) == 0){
    lib_path[len - suffix_len] = '\0';
    fprintf(stderr, "DEBUG: Extracted install prefix from LIB_PATH: %s\n", lib_path);
    return lib_path;
  }
  
  // If it doesn't match expected pattern, try to find "/lib/linuxtrack" in the path
  char *lib_pos = strstr(lib_path, suffix);
  if(lib_pos != NULL){
    *lib_pos = '\0';
    fprintf(stderr, "DEBUG: Extracted install prefix from LIB_PATH (pattern match): %s\n", lib_path);
    return lib_path;
  }
  
  // If we can't extract prefix, return NULL to use fallback
  free(lib_path);
  fprintf(stderr, "DEBUG: Could not extract install prefix from LIB_PATH: %s\n", LIB_PATH);
#endif
  return NULL;
}

/**
 * Build dynamic library search paths based on CMake install location
 * This ensures libraries can be found regardless of where CMake installs them
 */
static void build_dynamic_search_paths(char ***paths, int *count, const char *libname)
{
  *count = 0;
  *paths = NULL;
  
  // Get install prefix from LIB_PATH
  char *install_prefix = get_install_prefix_from_lib_path();
  if(install_prefix == NULL){
    fprintf(stderr, "DEBUG: Could not determine install prefix, skipping dynamic paths\n");
    return;
  }
  
  // Allocate array for paths (we'll build multiple paths)
  // Estimate: ~10 paths per prefix (standard + multiarch + 32-bit variants)
  int max_paths = 20;
  *paths = (char **)malloc(max_paths * sizeof(char *));
  if(*paths == NULL){
    free(install_prefix);
    return;
  }
  
  // Build paths based on install prefix
  // Standard 64-bit library location
  char *std_path = construct_name(install_prefix, "/lib/linuxtrack/", libname);
  if(std_path != NULL && *count < max_paths - 1){
    (*paths)[(*count)++] = std_path;
  } else if(std_path != NULL) {
    free(std_path);  // Free if we can't add it
  }
  
  // Standard 32-bit library location (if looking for 32-bit library)
  if(strstr(libname, "32") != NULL){
    // Already looking for 32-bit library, paths are already added above
  } else {
    // If looking for 64-bit lib, also check for 32-bit variant in same location
    if(*count < max_paths - 1){
      char *std32_path = construct_name(install_prefix, "/lib/linuxtrack/", "liblinuxtrack32.so.0");
      if(std32_path != NULL){
        (*paths)[(*count)++] = std32_path;
      }
    }
  }
  
  // Multiarch paths (Debian/Ubuntu style)
  const char *multiarch_dirs[] = {
    "i386-linux-gnu",      // 32-bit on 64-bit system
    "x86_64-linux-gnu",    // 64-bit
    NULL
  };
  
  for(int i = 0; multiarch_dirs[i] != NULL && *count < max_paths - 1; i++){
    char *multiarch_path = NULL;
    // Construct multiarch path: prefix/lib/multiarch_dir/linuxtrack/libname
    if(asprintf(&multiarch_path, "%s/lib/%s/linuxtrack/%s", 
                install_prefix, multiarch_dirs[i], libname) >= 0){
      if(*count < max_paths - 1){
        (*paths)[(*count)++] = multiarch_path;
      } else {
        free(multiarch_path);  // Free if we can't add it
      }
    }
    
    // If looking for 64-bit library, also check for 32-bit variant in i386-linux-gnu
    if(*count < max_paths - 1 && strstr(libname, "32") == NULL && strcmp(multiarch_dirs[i], "i386-linux-gnu") == 0){
      char *lib32_multiarch = NULL;
      if(asprintf(&lib32_multiarch, "%s/lib/%s/linuxtrack/liblinuxtrack32.so.0",
                  install_prefix, multiarch_dirs[i]) >= 0){
        if(*count < max_paths - 1){
          (*paths)[(*count)++] = lib32_multiarch;
        } else {
          free(lib32_multiarch);  // Free if we can't add it
        }
      }
    }
  }
  
  // Alternative lib32 directory (if LIB32DIR was set during CMake build)
  if(*count < max_paths - 1){
    char *lib32_path = construct_name(install_prefix, "/lib32/linuxtrack/", libname);
    if(lib32_path != NULL){
      (*paths)[(*count)++] = lib32_path;
    }
  }
  
  // lib64 variant (Fedora/RHEL style)
  if(*count < max_paths - 1){
    char *lib64_path = construct_name(install_prefix, "/lib64/linuxtrack/", libname);
    if(lib64_path != NULL){
      (*paths)[(*count)++] = lib64_path;
    }
  }
  
  // If install_prefix is not /opt, also check /opt paths (for Steam Proton compatibility)
  if(strcmp(install_prefix, "/opt") != 0){
    if(*count < max_paths - 1){
      char *opt_path = construct_name("/opt", "/lib/linuxtrack/", libname);
      if(opt_path != NULL){
        (*paths)[(*count)++] = opt_path;
      }
    }
    
    // Also check for 32-bit variant in /opt if looking for 64-bit
    if(*count < max_paths - 1 && strstr(libname, "32") == NULL){
      char *opt32_path = construct_name("/opt", "/lib/linuxtrack/", "liblinuxtrack32.so.0");
      if(opt32_path != NULL){
        (*paths)[(*count)++] = opt32_path;
      }
    }
  }
  
  // If install_prefix is not /usr, also check /usr paths (for relocatable installs)
  if(strcmp(install_prefix, "/usr") != 0 && strcmp(install_prefix, "/usr/local") != 0){
    if(*count < max_paths - 1){
      char *usr_path = construct_name("/usr", "/lib/linuxtrack/", libname);
      if(usr_path != NULL){
        (*paths)[(*count)++] = usr_path;
      }
    }
    
    if(*count < max_paths - 1){
      char *usr_local_path = construct_name("/usr/local", "/lib/linuxtrack/", libname);
      if(usr_local_path != NULL){
        (*paths)[(*count)++] = usr_local_path;
      }
    }
  }
  
  // Terminate array (safe because we always ensure count < max_paths - 1)
  (*paths)[*count] = NULL;
  
  fprintf(stderr, "DEBUG: Built %d dynamic search paths for %s\n", *count, libname);
  free(install_prefix);
}


static void* linuxtrack_try_library(const char *path)
{
#ifndef __MINGW32__
  void *handle = NULL;
  fprintf(stderr, "DEBUG: Trying library: %s\n", path);
  linuxtrack_log("Trying to load '%s'... ", path);
  if(access(path, F_OK) != 0){
    fprintf(stderr, "DEBUG: Library not found: %s\n", path);
    linuxtrack_log("Not found.\n");
    return NULL;
  }
  dlerror();
  handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
  if(handle != NULL){
    fprintf(stderr, "DEBUG: Library loaded successfully: %s\n", path);
    linuxtrack_log("Loaded OK.\n");
    return handle;
  }
  char *dlerr = dlerror();
  fprintf(stderr, "DEBUG: Failed to load library %s: %s\n", path, dlerr ? dlerr : "unknown error");
  linuxtrack_log("Couldn't load library - %s!\n", dlerr);
  return NULL;
#else
  (void)path; // Suppress unused parameter warning
  linuxtrack_log("Dynamic library loading not supported on MinGW.\n");
  return NULL;
#endif
}

char *linuxtrack_get_prefix()
{
  /* Try to read prefix from config file first */
  char *fname = ltr_int_get_default_file_name(NULL);
  if(fname != NULL){
    FILE *f = fopen(fname, "r");
    if(f != NULL){
      char key[2048];
      char val[2048];
      while(!feof(f)){
        if(fscanf(f, "%2040s", key) == 1){
          if(strcasecmp(key, "PREFIX") == 0){
            if(fgets(key, 2040, f) != NULL){
              if(sscanf(key, " = \"%[^\"\n]", val) > 0){
                fprintf(stderr, "DEBUG: linuxtrack_get_prefix read from config: %s\n", val);
                fclose(f);
                free(fname);
                return strdup(val);
              }
            }
          }
        }
      }
      fclose(f);
    }
    free(fname);
  }
  
  /* Fallback to hardcoded /usr if config reading fails */
  fprintf(stderr, "DEBUG: linuxtrack_get_prefix using fallback: /usr\n");
  return strdup("/usr");
}

/**
 * Detect if we're running under Steam Proton's pressure-vessel container
 * Steam Proton uses a containerized environment where /usr is not accessible
 * but /opt is accessible. This function checks for Steam Proton-specific
 * environment variables and path patterns.
 */
static int is_steam_proton_environment(void)
{
  // Check for STEAM_COMPAT_DATA_PATH environment variable
  if(getenv("STEAM_COMPAT_DATA_PATH") != NULL) {
    fprintf(stderr, "DEBUG: Detected Steam Proton via STEAM_COMPAT_DATA_PATH\n");
    return 1;
  }
  
  // Check if WINEPREFIX contains "compatdata" (Steam Proton prefix pattern)
  char *wineprefix = getenv("WINEPREFIX");
  if(wineprefix != NULL && strstr(wineprefix, "compatdata") != NULL) {
    fprintf(stderr, "DEBUG: Detected Steam Proton via WINEPREFIX containing 'compatdata'\n");
    return 1;
  }
  
  return 0;
}

static void* linuxtrack_find_library(linuxtrack_state_type *problem)
{
  /*
  //search order:
  //  1. LINUXTRACK_LIBS environment variable
  //       development, backward compatibility and weird locations handling
  //  2. Wine-specific paths (when running under Wine, checked via WINEPREFIX)
  //  3. prefix from config file (with relative library paths)
  //  4. absolute fallback paths (common distro layouts)
  //       worth in Linux only, since on Mac we never install to system libraries
  */
  void *handle = NULL;
  char *name = NULL;
  char *prefix;

  /*Look for LINUXTRACK_LIBS*/
  char *lp = getenv("LINUXTRACK_LIBS");
  if(lp != NULL){
    fprintf(stderr, "DEBUG: Found LINUXTRACK_LIBS environment variable: %s\n", lp);
    char *path = strdup(lp);
    char *part = path;
    while(1){
      part = strtok(part, ":");
      if((part == NULL) || ((handle = linuxtrack_try_library(part)) != NULL)){
        break;
      }
      part = NULL;
    }
    free(path);
    if(handle != NULL){
      fprintf(stderr, "DEBUG: Successfully loaded library from LINUXTRACK_LIBS\n");
      return handle;
    }
    fprintf(stderr, "DEBUG: Failed to load library from LINUXTRACK_LIBS\n");
  } else {
    fprintf(stderr, "DEBUG: LINUXTRACK_LIBS environment variable not set\n");
  }

  /* Check if we're running under Wine and add Wine-specific search paths (step 2) */
  char *wine_check = getenv("WINEPREFIX");
  if(wine_check != NULL) {
    fprintf(stderr, "DEBUG: Detected Wine environment (WINEPREFIX=%s), checking Wine-specific library paths\n", wine_check);
    
    /* First, try dynamic paths based on CMake install location (for liblinuxtrack32.so.0) */
    char **dynamic_paths = NULL;
    int dynamic_count = 0;
    build_dynamic_search_paths(&dynamic_paths, &dynamic_count, "liblinuxtrack32.so.0");
    if(dynamic_paths != NULL && dynamic_count > 0){
      fprintf(stderr, "DEBUG: Trying %d dynamic paths for liblinuxtrack32.so.0 (32-bit)\n", dynamic_count);
      for(int i = 0; i < dynamic_count && dynamic_paths[i] != NULL; i++){
        if((handle = linuxtrack_try_library(dynamic_paths[i])) != NULL){
          fprintf(stderr, "DEBUG: Successfully loaded liblinuxtrack32.so.0 from dynamic path: %s\n", dynamic_paths[i]);
          // Free all paths
          for(int j = 0; j < dynamic_count; j++){
            if(dynamic_paths[j] != NULL) free(dynamic_paths[j]);
          }
          free(dynamic_paths);
          return handle;
        }
      }
      // Free all paths
      for(int i = 0; i < dynamic_count; i++){
        if(dynamic_paths[i] != NULL) free(dynamic_paths[i]);
      }
      free(dynamic_paths);
      dynamic_paths = NULL;
      dynamic_count = 0;
    } else {
      fprintf(stderr, "DEBUG: Could not build dynamic paths for liblinuxtrack32.so.0, will try static paths\n");
    }
    
    /* Also try dynamic paths for 64-bit library (liblinuxtrack.so.0) */
    build_dynamic_search_paths(&dynamic_paths, &dynamic_count, "liblinuxtrack.so.0");
    if(dynamic_paths != NULL && dynamic_count > 0){
      fprintf(stderr, "DEBUG: Trying %d dynamic paths for liblinuxtrack.so.0 (64-bit)\n", dynamic_count);
      for(int i = 0; i < dynamic_count && dynamic_paths[i] != NULL; i++){
        if((handle = linuxtrack_try_library(dynamic_paths[i])) != NULL){
          fprintf(stderr, "DEBUG: Successfully loaded liblinuxtrack.so.0 from dynamic path: %s\n", dynamic_paths[i]);
          // Free all paths
          for(int j = 0; j < dynamic_count; j++){
            if(dynamic_paths[j] != NULL) free(dynamic_paths[j]);
          }
          free(dynamic_paths);
          return handle;
        }
      }
      // Free all paths
      for(int i = 0; i < dynamic_count; i++){
        if(dynamic_paths[i] != NULL) free(dynamic_paths[i]);
      }
      free(dynamic_paths);
    } else {
      fprintf(stderr, "DEBUG: Could not build dynamic paths for liblinuxtrack.so.0, will try static paths\n");
    }
    
    /* Fallback to static Wine-specific paths for compatibility */
    fprintf(stderr, "DEBUG: Trying static Wine-specific paths (both 32-bit and 64-bit)\n");
    
    /* Check if we're running under Steam Proton */
    int is_steam_proton = is_steam_proton_environment();
    
    /* Build path array based on environment */
    /* For Steam Proton: prioritize /opt paths (accessible in container, /usr is not accessible) */
    /* For regular Wine: prioritize /opt (default install location), then /usr/local (backward compatibility), then /usr (system) */
    static const char *wine_lib_locations_steam[] = {
      "/opt/lib/linuxtrack/liblinuxtrack32.so.0",         /* Steam Proton: accessible in container - PRIORITY */
      "/opt/lib/linuxtrack/liblinuxtrack.so.0",           /* Steam Proton: accessible in container - PRIORITY */
      "/usr/local/lib/linuxtrack/liblinuxtrack32.so.0",   /* Not accessible in container, but kept for completeness */
      "/usr/local/lib/linuxtrack/liblinuxtrack.so.0",    /* Not accessible in container, but kept for completeness */
      "/usr/lib/linuxtrack/liblinuxtrack32.so.0",        /* Not accessible in container, but kept for completeness */
      "/usr/lib/linuxtrack/liblinuxtrack.so.0",          /* Not accessible in container, but kept for completeness */
      NULL
    };
    
    static const char *wine_lib_locations_regular[] = {
      "/opt/lib/linuxtrack/liblinuxtrack32.so.0",        /* Regular Wine: default install location - PRIORITY */
      "/opt/lib/linuxtrack/liblinuxtrack.so.0",          /* Regular Wine: default install location - PRIORITY */
      "/usr/local/lib/linuxtrack/liblinuxtrack32.so.0",  /* Regular Wine: backward compatibility */
      "/usr/local/lib/linuxtrack/liblinuxtrack.so.0",    /* Regular Wine: backward compatibility */
      "/usr/lib/linuxtrack/liblinuxtrack32.so.0",        /* Regular Wine: system install */
      "/usr/lib/linuxtrack/liblinuxtrack.so.0",          /* Regular Wine: system install */
      NULL
    };
    
    const char **wine_lib_locations = is_steam_proton ? wine_lib_locations_steam : wine_lib_locations_regular;
    
    int i = 0;
    while(wine_lib_locations[i] != NULL){
      if((handle = linuxtrack_try_library((char*)wine_lib_locations[i++])) != NULL){
        fprintf(stderr, "DEBUG: Successfully loaded library from Wine-specific static path\n");
        return handle;
      }
    }
    fprintf(stderr, "DEBUG: Wine-specific paths exhausted, continuing to prefix-based search\n");
  }

  prefix = linuxtrack_get_prefix();
  if(prefix == NULL){
    *problem = err_NO_CONFIG;
    return NULL;
  }
  fprintf(stderr, "DEBUG: linuxtrack_find_library using prefix: %s\n", prefix);
  int i = 0;
  while(lib_locations[i] != NULL){
    name = construct_name(prefix, "/../", lib_locations[i++]);
    if((handle = linuxtrack_try_library(name)) != NULL){
      free(name);
      free(prefix);
      return handle;
    }
    free(name);
  }
  free(prefix);
  
  /* Try dynamic paths based on CMake install location (for non-Wine environments) */
  char **dynamic_paths = NULL;
  int dynamic_count = 0;
  build_dynamic_search_paths(&dynamic_paths, &dynamic_count, "liblinuxtrack.so.0");
  if(dynamic_paths != NULL && dynamic_count > 0){
    fprintf(stderr, "DEBUG: Trying %d dynamic search paths for liblinuxtrack.so.0 (64-bit) based on CMake install location...\n", dynamic_count);
    for(int j = 0; j < dynamic_count && dynamic_paths[j] != NULL; j++){
      if((handle = linuxtrack_try_library(dynamic_paths[j])) != NULL){
        fprintf(stderr, "DEBUG: Successfully loaded liblinuxtrack.so.0 from dynamic path: %s\n", dynamic_paths[j]);
        // Free all paths
        for(int k = 0; k < dynamic_count; k++){
          if(dynamic_paths[k] != NULL) free(dynamic_paths[k]);
        }
        free(dynamic_paths);
        return handle;
      }
    }
    // Free all paths
    for(int j = 0; j < dynamic_count; j++){
      if(dynamic_paths[j] != NULL) free(dynamic_paths[j]);
    }
    free(dynamic_paths);
  } else {
    fprintf(stderr, "DEBUG: Could not build dynamic paths for liblinuxtrack.so.0, will try fallback static paths\n");
  }
  
  /* Absolute fallbacks independent of prefix to support common distro layouts */
  static const char *alt_lib_locations[] = {
    "/opt/lib/linuxtrack/liblinuxtrack.so.0",          /* Default install location - PRIORITY */
    "/opt/lib/linuxtrack/liblinuxtrack32.so.0",         /* Default install location 32-bit - PRIORITY */
    "/usr/local/lib64/linuxtrack/liblinuxtrack.so.0",  /* Fedora local installs */
    "/usr/local/lib/linuxtrack/liblinuxtrack.so.0",     /* Arch/Debian local installs */
    "/usr/local/lib64/linuxtrack/liblinuxtrack32.so.0",  /* Fedora local installs 32-bit */
    "/usr/local/lib/linuxtrack/liblinuxtrack32.so.0",     /* Arch/Debian local installs 32-bit */
    "/usr/lib64/linuxtrack/liblinuxtrack.so.0",        /* Fedora/RHEL 64-bit */
    "/usr/lib/linuxtrack/liblinuxtrack.so.0",          /* Arch/Debian */
    "/lib64/linuxtrack/liblinuxtrack.so.0",            /* Alternative Fedora location */
    "/lib/linuxtrack/liblinuxtrack.so.0",              /* Alternative Arch/Debian location */
    "/usr/lib/x86_64-linux-gnu/linuxtrack/liblinuxtrack.so.0", /* Debian/Ubuntu 64-bit */
    "/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0",  /* Debian/Ubuntu 32-bit */
    "/usr/lib64/linuxtrack/liblinuxtrack32.so.0",        /* Fedora/RHEL 32-bit */
    "/usr/lib/linuxtrack/liblinuxtrack32.so.0",          /* Arch/Debian 32-bit */
    "/lib64/linuxtrack/liblinuxtrack32.so.0",            /* Alternative Fedora location 32-bit */
    "/lib/linuxtrack/liblinuxtrack32.so.0",              /* Alternative Arch/Debian location 32-bit */
    "/usr/lib/x86_64-linux-gnu/linuxtrack/liblinuxtrack32.so.0", /* Debian/Ubuntu 32-bit */
    "/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack32.so.0",  /* Debian/Ubuntu 32-bit */
    NULL
  };
  fprintf(stderr, "DEBUG: Trying fallback library locations...\n");
  i = 0;
  while(alt_lib_locations[i] != NULL){
    if((handle = linuxtrack_try_library((char*)alt_lib_locations[i++])) != NULL){
      fprintf(stderr, "DEBUG: Successfully loaded library from fallback location\n");
      return handle;
    }
  }
  fprintf(stderr, "DEBUG: All library search attempts failed\n");
  *problem = err_NOT_FOUND;
  return NULL;
}



static linuxtrack_state_type linuxtrack_load_library()
{
#ifndef __MINGW32__
  fprintf(stderr, "DEBUG: linuxtrack_load_library called\n");
  linuxtrack_state_type problem;
  lib_handle = linuxtrack_find_library(&problem);
  if(lib_handle == NULL){
    fprintf(stderr, "DEBUG: linuxtrack_find_library failed with problem: %d\n", problem);
    linuxtrack_log("Couldn't load liblinuxtrack, headtracking will not be available!\n");
    return problem;
  }
  fprintf(stderr, "DEBUG: Library handle obtained: %p\n", lib_handle);
  dlerror(); /*clear any existing error...*/
  fprintf(stderr, "DEBUG: About to call linuxtrack_load_functions\n");
  linuxtrack_state_type load_result = linuxtrack_load_functions(lib_handle);
  fprintf(stderr, "DEBUG: linuxtrack_load_functions returned: %d\n", load_result);
  if(load_result != LINUXTRACK_OK){
    fprintf(stderr, "DEBUG: Function loading failed\n");
    linuxtrack_log("Couldn't load liblinuxtrack functions, headtracking will not be available!\n");
    return load_result;
  }
  fprintf(stderr, "DEBUG: All functions loaded successfully\n");
  return LINUXTRACK_OK;
#else
  // For MinGW builds, dynamic loading is not supported
  // The wine_bridge should be designed to work without the full linuxtrack library
  linuxtrack_log("Dynamic library loading not supported on MinGW, using stubs.\n");
  return LINUXTRACK_OK;
#endif
}

linuxtrack_state_type linuxtrack_init(const char *cust_section)
{
  fprintf(stderr, "DEBUG: linuxtrack_init called with section: %s\n", cust_section ? cust_section : "NULL");
  linuxtrack_state_type res = linuxtrack_load_library();
  if(res < LINUXTRACK_OK){
    fprintf(stderr, "DEBUG: linuxtrack_load_library failed: %d\n", res);
    return res;
  }
  if(ltr_init_fun == NULL){
    fprintf(stderr, "DEBUG: ltr_init_fun is NULL!\n");
    linuxtrack_log("ltr_init_fun is NULL!\n");
    return err_SYMBOL_LOOKUP;
  }
  fprintf(stderr, "DEBUG: About to call ltr_init_fun\n");
  linuxtrack_state_type init_res = ltr_init_fun(cust_section);
  fprintf(stderr, "DEBUG: ltr_init_fun returned: %d\n", init_res);
  linuxtrack_log("ltr_init_fun returned: %d (%s)\n", init_res, linuxtrack_explain(init_res));
  return init_res;
}

const char *linuxtrack_explain(linuxtrack_state_type status)
{
  if(ltr_explain_fun != NULL){
    return ltr_explain_fun(status);
  }
  const char *res = NULL;
  switch(status){
    case INITIALIZING:
      res = "Linuxtrack is initializing.";
      break;
    case RUNNING:
      res = "Linuxtrack is running.";
      break;
    case PAUSED:
      res = "Linuxtrack is paused.";
      break;
    case STOPPED:
      res = "Linuxtrack is stopped.";
      break;
    case err_NOT_INITIALIZED:
      res = "Linuxtrack function was called without proper initialization.";
      break;
    case err_SYMBOL_LOOKUP:
      res = "Internal error (symbol lookup). Please file an issue at http://linuxtrack.eu";
      break;
    case err_NO_CONFIG:
      res = "Linuxtrack config not found. If you have installed Linuxtrack,\n"
            "run ltr_gui and set it up first.";
      break;
    case err_NOT_FOUND:
      res = "Linuxtrack was removed or relocated. If you relocated it,\n"
            "run ltr_gui from the new location, save preferences and try again.";
      break;
    case err_PROCESSING_FRAME:
      res = "Internal error (frame processing). Please file an issue at http://linuxtrack.eu";
      break;
    default:
      printf("UNKNOWN status code. Please file an issue at http://linuxtrack.eu\n");
     break;
  }
  return res;
}


linuxtrack_state_type linuxtrack_request_frames(void)
{
  if(ltr_request_frames_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_request_frames_fun();
}

int linuxtrack_get_frame(int *req_width, int *req_height, size_t buf_size, uint8_t *buffer)
{
  if(ltr_get_frame_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_get_frame_fun(req_width, req_height, buf_size, buffer);
}

linuxtrack_state_type linuxtrack_notification_on(void)
{
  if(ltr_notification_on_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_notification_on_fun();
}

int linuxtrack_get_notify_pipe(void)
{
  if(ltr_get_notify_pipe_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_get_notify_pipe_fun();
}

int linuxtrack_wait(int timeout)
{
  if(ltr_wait_fun == NULL){
    return err_NOT_INITIALIZED;
  }
  return ltr_wait_fun(timeout);
}
