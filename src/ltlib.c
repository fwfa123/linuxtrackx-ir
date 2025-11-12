#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "ltlib_int.h"
#include "ipc_utils.h"
#include "utils.h"
#include "pref.h"

static struct mmap_s mmm;
static bool initialized = false;
static int notify_pipe = -1;

static int make_mmap()
{
  if(!ltr_int_mmap_file_exclusive(sizeof(struct mmap_s), &mmm)){
    ltr_int_my_perror("mmap_file: ");
    ltr_int_log_message("Couldn't mmap!\n");
    return -1;
  }
  return 0;
}

static void ltr_int_sanitize_name(char *name)
{
  char *forbidden = "\r\n";
  size_t len = strcspn(name, forbidden);
  name[len] = '\0';
}

// Helper function to search for an executable in PATH
static char *ltr_int_find_in_path(const char *executable)
{
  char *path_env = getenv("PATH");
  if(path_env == NULL){
    return NULL;
  }
  
  // Make a copy of PATH since strtok_r will modify it
  char *path_copy = ltr_int_my_strdup(path_env);
  if(path_copy == NULL){
    return NULL;
  }
  
  char *saveptr = NULL;
  char *dir;
  char *full_path = NULL;
  
  // Iterate through each directory in PATH using strtok_r (portable)
  dir = strtok_r(path_copy, ":", &saveptr);
  while(dir != NULL){
    if(*dir == '\0'){
      // Empty directory means current directory
      dir = ".";
    }
    
    // Construct full path: dir/executable
    size_t dir_len = strlen(dir);
    size_t exec_len = strlen(executable);
    size_t total_len = dir_len + 1 + exec_len + 1; // dir + '/' + executable + '\0'
    
    full_path = (char *)malloc(total_len);
    if(full_path == NULL){
      free(path_copy);
      return NULL;
    }
    
    snprintf(full_path, total_len, "%s/%s", dir, executable);
    
    // Check if file exists and is executable
    if(access(full_path, F_OK | X_OK) == 0){
      free(path_copy);
      return full_path;
    }
    
    free(full_path);
    full_path = NULL;
    
    // Get next directory
    dir = strtok_r(NULL, ":", &saveptr);
  }
  
  free(path_copy);
  return NULL;
}

linuxtrack_state_type ltr_wakeup(void);

static char *ltr_int_init_helper(const char *cust_section, bool standalone)
{
  char pid[16];
  char pipe0[16];
  char pipe1[16];
  int fd[2];
  bool is_child;
  bool created_new_mmap = false;
  
  // If already initialized, check if we should reuse or create new
  // Don't create a new one as that would lose the connection to the original
  if(initialized){
    if(standalone){
      // Already initialized in standalone mode, just return
      return mmm.fname;
    }
    // Already initialized, but switching to/checking client mode
    // Check if the existing mmap is in an error state - if so, create a new one
    struct ltr_comm *com_check = mmm.data;
    ltr_int_lockSemaphore(mmm.sem);
    linuxtrack_state_type current_state = com_check->state;
    ltr_int_unlockSemaphore(mmm.sem);
    
    // If state is err_NOT_INITIALIZED, the previous initialization failed
    // Create a new mmap instead of reusing the broken one
    if(current_state == err_NOT_INITIALIZED){
      ltr_int_log_message("Existing mmap is in error state, creating new shared memory region\n");
      ltr_int_unmap_file(&mmm);
      initialized = false;
      // Fall through to create new mmap
    } else {
      // Reuse existing mmm - don't create a new one
      // Return immediately to avoid creating duplicate slave connections
      // and leaking the existing notify_pipe file descriptor
      ltr_int_log_message("Already initialized, reusing existing shared memory region (state: %d)\n", current_state);
      return mmm.fname;
    }
  }
  
  // Create new shared memory region if not initialized or if previous one was in error state
  if(!initialized){
    if(make_mmap() != 0) return NULL;
    initialized = true;
    created_new_mmap = true;  // Track that we created it in this call
  }
  
  struct ltr_comm *com = mmm.data;
  // Acquire semaphore before modifying shared memory state to prevent data races
  ltr_int_lockSemaphore(mmm.sem);
  // Only reset state to INITIALIZING if it's currently in an error state
  // Preserve valid running states (RUNNING, PAUSED, INITIALIZING, STOPPED)
  if(com->state < 0 || com->state == STOPPED){
    com->state = INITIALIZING;
    com->preparing_start = true;
  } else {
    // State is already valid (RUNNING, PAUSED, or INITIALIZING), preserve it
    // Only set preparing_start if we're creating a new mmap
    if(created_new_mmap){
      com->preparing_start = true;
    }
  }
  ltr_int_unlockSemaphore(mmm.sem);
  if(standalone){
    if(pipe(fd) < 0){
      fd[0] = fd[1] = -1;
    }
    char *server = ltr_int_get_app_path("/ltr_server1");
    if(cust_section == NULL){
      cust_section = "Default";
    }
    char *section = ltr_int_my_strdup(cust_section);
    ltr_int_sanitize_name(section);
    snprintf(pid, sizeof(pid), "%lu", (unsigned long)getpid());
    snprintf(pipe0, sizeof(pipe0), "%d", fd[0]);
    snprintf(pipe1, sizeof(pipe1), "%d", fd[1]);
    char *args[] = {server, section, mmm.fname, pid, pipe0, pipe1, NULL};
    if(!ltr_int_fork_child(args, &is_child)){
      // Acquire semaphore before modifying shared memory state
      ltr_int_lockSemaphore(mmm.sem);
      com->state = err_NOT_INITIALIZED;
      ltr_int_unlockSemaphore(mmm.sem);
      free(server);
      free(section);
      // Close pipe file descriptors before returning
      if(fd[0] >= 0) close(fd[0]);
      if(fd[1] >= 0) close(fd[1]);
      // Clean up if we created the mmap in this call
      if(created_new_mmap){
        ltr_int_unmap_file(&mmm);
        initialized = false;
      }
      if(is_child){
        exit(1);
      }
      return NULL;
    }
    free(server);
    free(section);
    close(fd[1]);
    notify_pipe = fd[0];
    fcntl(notify_pipe, F_SETFL, fcntl(notify_pipe, F_GETFL) | O_NONBLOCK);
      } else {
        // Client mode: establish connection to existing server
        int socket_fd = ltr_int_connect_to_socket("/tmp/ltr_m_sock");
        if(socket_fd >= 0){
          close(socket_fd); // Close the test connection
          // Master server is running, spawn slave process to connect to it
          // The slave will receive pose data from master and write to shared memory
          if(pipe(fd) < 0){
            fd[0] = fd[1] = -1;
            ltr_int_log_message("Client mode: Failed to create pipe! (errno: %d, %s)\n", errno, strerror(errno));
            // Acquire semaphore before modifying shared memory state
            ltr_int_lockSemaphore(mmm.sem);
            com->state = err_NOT_INITIALIZED;
            ltr_int_unlockSemaphore(mmm.sem);
            // Only unmap if we created the mmap in this call
            // If reusing existing mmap, keep initialized=true and state=error
            // The next call will detect err_NOT_INITIALIZED and create a new mmap
            if(created_new_mmap){
              ltr_int_unmap_file(&mmm);
              initialized = false;
            } else {
              // Reusing existing mmap - keep initialized=true to prevent leak
              // State is already set to err_NOT_INITIALIZED above
              // Next call will detect the error state and create a new mmap
            }
            return NULL;
          }
          
          // Note: We don't call ltr_int_get_key() here because preferences haven't been loaded
          // in the parent process yet. Preferences are only loaded in the slave process.
          // The diagnostic logging below via ltr_int_get_app_path() will show the path being used.
          
          char *server = ltr_int_get_app_path("/ltr_server1");
          if(server == NULL){
            ltr_int_log_message("Client mode: Could not find ltr_server1 path via ltr_int_get_app_path!\n");
            
            // Fallback path resolution
            ltr_int_log_message("Client mode: Attempting fallback path resolution...\n");
            const char *fallback_paths[] = {
              "/usr/local/bin/ltr_server1",
              "/opt/linuxtrack/bin/ltr_server1",
              "/usr/bin/ltr_server1",
              NULL  // Placeholder for PATH lookup (handled separately)
            };
            
            server = NULL;
            // First try the hardcoded paths
            for(int i = 0; i < 3; i++){
              ltr_int_log_message("Client mode: Trying fallback path %d: %s\n", i, fallback_paths[i]);
              if(access(fallback_paths[i], F_OK | X_OK) == 0){
                server = ltr_int_my_strdup(fallback_paths[i]);
                ltr_int_log_message("Client mode: Found ltr_server1 at fallback path: %s\n", server);
                break;
              }
            }
            
            // If not found in hardcoded paths, search PATH
            if(server == NULL){
              ltr_int_log_message("Client mode: Searching PATH for ltr_server1...\n");
              server = ltr_int_find_in_path("ltr_server1");
              if(server != NULL){
                ltr_int_log_message("Client mode: Found ltr_server1 in PATH: %s\n", server);
              } else {
                ltr_int_log_message("Client mode: ltr_server1 not found in PATH\n");
              }
            }
            
            if(server == NULL){
              ltr_int_log_message("Client mode: All path resolution attempts failed!\n");
              // Acquire semaphore before modifying shared memory state
              ltr_int_lockSemaphore(mmm.sem);
              com->state = err_NOT_INITIALIZED;
              ltr_int_unlockSemaphore(mmm.sem);
              // Close pipe file descriptors before returning
              if(fd[0] >= 0) close(fd[0]);
              if(fd[1] >= 0) close(fd[1]);
              // Only unmap if we created the mmap in this call
              // If reusing existing mmap, keep initialized=true and state=error
              // The next call will detect err_NOT_INITIALIZED and create a new mmap
              if(created_new_mmap){
                ltr_int_unmap_file(&mmm);
                initialized = false;
              } else {
                // Reusing existing mmap - keep initialized=true to prevent leak
                // State is already set to err_NOT_INITIALIZED above
                // Next call will detect the error state and create a new mmap
              }
              return NULL;
            }
          } else {
            ltr_int_log_message("Client mode: ltr_int_get_app_path returned: %s\n", server);
          }
          
          // Verify the server executable exists and is executable before forking
          int access_result = access(server, F_OK | X_OK);
          if(access_result != 0){
            ltr_int_log_message("Client mode: access() check failed for %s (errno: %d, %s)\n", 
                               server, errno, strerror(errno));
            ltr_int_log_message("Client mode: ltr_server1 not found or not executable at: %s\n", server);
            // Acquire semaphore before modifying shared memory state
            ltr_int_lockSemaphore(mmm.sem);
            com->state = err_NOT_INITIALIZED;
            ltr_int_unlockSemaphore(mmm.sem);
            free(server);
            // Close pipe file descriptors before returning
            if(fd[0] >= 0) close(fd[0]);
            if(fd[1] >= 0) close(fd[1]);
            // Only unmap if we created the mmap in this call
            // If reusing existing mmap, keep initialized=true and state=error
            // The next call will detect err_NOT_INITIALIZED and create a new mmap
            if(created_new_mmap){
              ltr_int_unmap_file(&mmm);
              initialized = false;
            } else {
              // Reusing existing mmap - keep initialized=true to prevent leak
              // State is already set to err_NOT_INITIALIZED above
              // Next call will detect the error state and create a new mmap
            }
            return NULL;
          }
          ltr_int_log_message("Client mode: access() check passed for: %s\n", server);
          if(cust_section == NULL){
            cust_section = "Default";
          }
          char *section = ltr_int_my_strdup(cust_section);
          ltr_int_sanitize_name(section);
          snprintf(pid, sizeof(pid), "%lu", (unsigned long)getpid());
          snprintf(pipe0, sizeof(pipe0), "%d", fd[0]);
          snprintf(pipe1, sizeof(pipe1), "%d", fd[1]);
          char *args[] = {server, section, mmm.fname, pid, pipe0, pipe1, NULL};
          ltr_int_log_message("Client mode: Spawning slave process: %s %s %s\n", server, section, mmm.fname);
          ltr_int_log_message("Client mode: Slave process arguments: [0]=%s [1]=%s [2]=%s [3]=%s [4]=%s [5]=%s\n",
                             args[0], args[1], args[2], args[3], args[4], args[5]);
          if(!ltr_int_fork_child(args, &is_child)){
            ltr_int_log_message("Client mode: Failed to fork slave process! (is_child=%d, errno=%d: %s)\n", 
                               is_child, errno, strerror(errno));
            // Acquire semaphore before modifying shared memory state
            ltr_int_lockSemaphore(mmm.sem);
            com->state = err_NOT_INITIALIZED;
            ltr_int_unlockSemaphore(mmm.sem);
            free(server);
            free(section);
            // Close pipe file descriptors before returning
            if(fd[0] >= 0) close(fd[0]);
            if(fd[1] >= 0) close(fd[1]);
            // Only unmap if we created the mmap in this call
            // If reusing existing mmap, keep initialized=true and state=error
            // The next call will detect err_NOT_INITIALIZED and create a new mmap
            if(created_new_mmap){
              ltr_int_unmap_file(&mmm);
              initialized = false;
            } else {
              // Reusing existing mmap - keep initialized=true to prevent leak
              // State is already set to err_NOT_INITIALIZED above
              // Next call will detect the error state and create a new mmap
            }
            if(is_child){
              exit(1);
            }
            return NULL;
          }
          ltr_int_log_message("Client mode: Slave process forked successfully (is_child=%d)\n", is_child);
          free(server);
          free(section);
          close(fd[1]);
          notify_pipe = fd[0];
          fcntl(notify_pipe, F_SETFL, fcntl(notify_pipe, F_GETFL) | O_NONBLOCK);
        } else {
          // Socket connection failed - no pipe was created in this path
          ltr_int_log_message("Client mode: Could not connect to master socket /tmp/ltr_m_sock (errno: %d, %s)\n", 
                             errno, strerror(errno));
          // Acquire semaphore before modifying shared memory state
          ltr_int_lockSemaphore(mmm.sem);
          com->state = err_NOT_INITIALIZED;
          ltr_int_unlockSemaphore(mmm.sem);
          // Only unmap if we created the mmap in this call
          // If reusing existing mmap, keep initialized=true and state=error
          // The next call will detect err_NOT_INITIALIZED and create a new mmap
          if(created_new_mmap){
            ltr_int_unmap_file(&mmm);
            initialized = false;
          } else {
            // Reusing existing mmap - keep initialized=true to prevent leak
            // State is already set to err_NOT_INITIALIZED above
            // Next call will detect the error state and create a new mmap
          }
          return NULL;
        }
      }
  ltr_wakeup();
  return mmm.fname;
}

linuxtrack_state_type ltr_get_tracking_state(void);

linuxtrack_state_type ltr_init(const char *cust_section)
{
  // Check if a server is already running by testing if we can connect to the socket
  // This is more reliable than lock file detection
  int socket_fd = ltr_int_connect_to_socket("/tmp/ltr_m_sock");
  bool server_running = (socket_fd >= 0);
  if(socket_fd >= 0){
    close(socket_fd); // Close the test connection
  }
  
  if(server_running){
    // Server is running, use client mode
    char *result = ltr_int_init_helper(cust_section, false);
    if(result != NULL){
      return ltr_get_tracking_state();
    }else{
      return INITIALIZING;
    }
  }else{
    // No server running, use standalone mode
    if(ltr_int_init_helper(cust_section, true) != NULL){
      return ltr_get_tracking_state();
    }else{
      return INITIALIZING;
    }
  }
}

static const float c_EXT_LIMIT = 3.0f;
static const float c_EXT_ASYMPTOTE = 5.0f;

static float ltr_int_extrapolation_factor(int t1, int t2, int now)
{
  int dt12 = ltr_int_ts_diff(t1, t2);
  int dt = ltr_int_ts_diff(t2, now);
  if(dt == 0){
    return 0.0f;
  }
  float ext = (float)dt / dt12;
  //Should the extrapolation go further than c_EXTRAPOLATION_LIMIT
  //  times the frame interval, start to round out with asymptote
  //  of c_EXTRAPOLATION_ASYMPTOTE.
  //Start linear in the interval <0.0; c_EXTRAPOLATION_LIMIT>;
  //  at c_EXTRAPOLATION_LIMIT switch to -1/x type of curve
  if(ext > 3){
    // -1 / x => basic type of curve; we need to get the [1;-1] point
    //   to [c_EXTRAPOLATION_LIMIT; c_EXTRAPOLATION_LIMIT]
    // 
    ext = c_EXT_ASYMPTOTE - 
         (c_EXT_LIMIT * (c_EXT_ASYMPTOTE - c_EXT_LIMIT)) / ext;
  }
  return ext;
}

static inline float ltr_int_extrapolate(float v1, float v2, float ext)
{
  return v2 + (v2 - v1) * ext;
}

static void ltr_int_extrapolate_pose(
       linuxtrack_full_pose_t *pose,
       linuxtrack_pose_t *result)
{
  float ext = ltr_int_extrapolation_factor(pose->prev_timestamp, pose->timestamp, ltr_int_get_ts());
  result->yaw = ltr_int_extrapolate(pose->prev_pose.yaw, pose->pose.yaw, ext);
  result->pitch = ltr_int_extrapolate(pose->prev_pose.pitch, pose->pose.pitch, ext);
  result->roll = ltr_int_extrapolate(pose->prev_pose.roll, pose->pose.roll, ext);
  result->tx = ltr_int_extrapolate(pose->prev_pose.tx, pose->pose.tx, ext);
  result->ty = ltr_int_extrapolate(pose->prev_pose.ty, pose->pose.ty, ext);
  result->tz = ltr_int_extrapolate(pose->prev_pose.tz, pose->pose.tz, ext);
}


static void ltr_int_extrapolate_abs_pose(
       linuxtrack_full_pose_t *pose,
       linuxtrack_abs_pose_t *result)
{
  float ext = ltr_int_extrapolation_factor(pose->prev_timestamp, pose->timestamp, ltr_int_get_ts());
  result->abs_yaw = ltr_int_extrapolate(pose->prev_abs_pose.abs_yaw, pose->abs_pose.abs_yaw, ext);
  result->abs_pitch = ltr_int_extrapolate(pose->prev_abs_pose.abs_pitch, pose->abs_pose.abs_pitch, ext);
  result->abs_roll = ltr_int_extrapolate(pose->prev_abs_pose.abs_roll, pose->abs_pose.abs_roll, ext);
  result->abs_tx = ltr_int_extrapolate(pose->prev_abs_pose.abs_tx, pose->abs_pose.abs_tx, ext);
  result->abs_ty = ltr_int_extrapolate(pose->prev_abs_pose.abs_ty, pose->abs_pose.abs_ty, ext);
  result->abs_tz = ltr_int_extrapolate(pose->prev_abs_pose.abs_tz, pose->abs_pose.abs_tz, ext);
}



int ltr_get_pose(float *heading,
                         float *pitch,
                         float *roll,
                         float *tx,
                         float *ty,
                         float *tz,
                         uint32_t *counter)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return 0;
  struct ltr_comm tmp;
  ltr_int_lockSemaphore(mmm.sem);
  tmp = *com;
  //printf("OTHER_SIDE: %g %g %g\n", tmp.pose.yaw, tmp.pose.pitch, tmp.pose.roll);
  ltr_int_unlockSemaphore(mmm.sem);
  if(tmp.state >= LINUXTRACK_OK){
    uint32_t passed_counter = *counter;
    linuxtrack_pose_t tmp_pose;
    ltr_int_extrapolate_pose(&(tmp.full_pose), &tmp_pose);
    *heading = tmp_pose.yaw;
    *pitch = tmp_pose.pitch;
    *roll = tmp_pose.roll;
    *tx = tmp_pose.tx;
    *ty = tmp_pose.ty;
    *tz = tmp_pose.tz;
    *counter = tmp.full_pose.pose.counter;
    if(passed_counter != *counter){
      return 1;// flag new data
    }else{
      return 0;
    }
  }else{
    *heading = 0.0;
    *pitch = 0.0;
    *roll = 0.0;
    *tx = 0.0;
    *ty = 0.0;
    *tz = 0.0;
    *counter = 0;
    return 0;
  }
}

int ltr_get_pose_full(linuxtrack_pose_t *pose, float blobs[], int num_blobs, int *blobs_read)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return 0;
  struct ltr_comm tmp;
  ltr_int_lockSemaphore(mmm.sem);
  tmp = *com;
  ltr_int_unlockSemaphore(mmm.sem);
  if(tmp.state >= LINUXTRACK_OK){
    uint32_t prev_counter = pose->counter;
    ltr_int_extrapolate_pose(&(tmp.full_pose), pose);
    *blobs_read = (num_blobs < (int)tmp.full_pose.blobs) ? num_blobs : (int)tmp.full_pose.blobs;
    int i;
    for(i = 0; i < (*blobs_read) * BLOB_ELEMENTS; ++i){
      blobs[i] = tmp.full_pose.blob_list[i];
    }
    if(prev_counter != pose->counter){
      return 1;//new data
    }else{
      return 0;
    }
  }else{
    *blobs_read = 0;
    memset(pose, 0, sizeof(linuxtrack_pose_t));
    return 0;
  }
}

int ltr_get_abs_pose(float *heading,
                         float *pitch,
                         float *roll,
                         float *tx,
                         float *ty,
                         float *tz,
                         uint32_t *counter)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return 0;
  struct ltr_comm tmp;
  ltr_int_lockSemaphore(mmm.sem);
  tmp = *com;
  //printf("OTHER_SIDE: %g %g %g\n", tmp.pose.yaw, tmp.pose.pitch, tmp.pose.roll);
  ltr_int_unlockSemaphore(mmm.sem);
  if(tmp.state >= LINUXTRACK_OK){
    uint32_t passed_counter = *counter;
    linuxtrack_abs_pose_t tmp_pose;
    ltr_int_extrapolate_abs_pose(&(tmp.full_pose), &tmp_pose);
    *heading = tmp_pose.abs_yaw;
    *pitch = tmp_pose.abs_pitch;
    *roll = tmp_pose.abs_roll;
    *tx = tmp_pose.abs_tx;
    *ty = tmp_pose.abs_ty;
    *tz = tmp_pose.abs_tz;
    *counter = tmp.full_pose.pose.counter;
    if(passed_counter != *counter){
      return 1;// flag new data
    }else{
      return 0;
    }
  }else{
    *heading = 0.0;
    *pitch = 0.0;
    *roll = 0.0;
    *tx = 0.0;
    *ty = 0.0;
    *tz = 0.0;
    *counter = 0;
    return 0;
  }
}


linuxtrack_state_type ltr_suspend(void)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return err_NOT_INITIALIZED;
  ltr_int_lockSemaphore(mmm.sem);
  com->cmd = PAUSE_CMD;
  ltr_int_unlockSemaphore(mmm.sem);
  return LINUXTRACK_OK;
}

linuxtrack_state_type ltr_wakeup(void)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return err_NOT_INITIALIZED;
  ltr_int_lockSemaphore(mmm.sem);
  com->cmd = RUN_CMD;
  ltr_int_unlockSemaphore(mmm.sem);
  return LINUXTRACK_OK;
}

linuxtrack_state_type ltr_shutdown(void)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return err_NOT_INITIALIZED;
  ltr_int_lockSemaphore(mmm.sem);
  com->cmd = STOP_CMD;
  ltr_int_unlockSemaphore(mmm.sem);
  initialized = false;
  ltr_int_unmap_file(&mmm);
  return LINUXTRACK_OK;
}

linuxtrack_state_type ltr_recenter(void)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return err_NOT_INITIALIZED;
  ltr_int_lockSemaphore(mmm.sem);
  com->recenter = true;
  ltr_int_unlockSemaphore(mmm.sem);
  return LINUXTRACK_OK;
}

linuxtrack_state_type ltr_notification_on(void)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return err_NOT_INITIALIZED;
  ltr_int_lockSemaphore(mmm.sem);
  com->notify = true;
  ltr_int_unlockSemaphore(mmm.sem);
  return LINUXTRACK_OK;
}

int ltr_get_notify_pipe(void)
{
  return notify_pipe;
}

linuxtrack_state_type ltr_request_frames(void)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return err_NOT_INITIALIZED;
  ltr_int_lockSemaphore(mmm.sem);
  com->cmd = FRAMES_CMD;
  ltr_int_unlockSemaphore(mmm.sem);
  return LINUXTRACK_OK;
}

linuxtrack_state_type ltr_get_tracking_state(void)
{
  linuxtrack_state_type state = STOPPED;
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)){
    return err_NOT_INITIALIZED;
  }
  if(com->preparing_start){
    return INITIALIZING;
  }
  ltr_int_lockSemaphore(mmm.sem);
  state = com->state;
  ltr_int_unlockSemaphore(mmm.sem);
  return state;
}

void ltr_log_message(const char *format, ...)
{
  va_list ap;
  va_start(ap,format);
  ltr_int_valog_message(format, ap);
  va_end(ap);
}

const char *ltr_explain(linuxtrack_state_type status)
{
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
      res = "Internal error (symbol lookup). Please file an issue at Linuxtrack project page.";
      break;
    case err_NO_CONFIG:
      res = "Linuxtrack config not found. If you have Linuxtrack, run ltr_gui and set it up first.";
      break;
    case err_NOT_FOUND:
      res = "Linuxtrack was removed or relocated. If you relocated it,\n"
            "run ltr_gui from the new location, save preferences and try again.";
      break;
    case err_PROCESSING_FRAME:
      res = "Internal error (frame processing). Please file an issue at Linuxtrack project page.";
      break;
    default:
      printf("UNKNOWN status code. Please file an issue at Linuxtrack project page.\n");
      break;
  }
  return res;
}

static struct mmap_s mmap;

int ltr_get_frame(int *req_width, int *req_height, size_t buf_size, uint8_t *buffer)
{
  struct ltr_comm *com = mmm.data;
  if((!initialized) || (com == NULL)) return 0;
  struct ltr_comm tmp;
  ltr_int_lockSemaphore(mmm.sem);
  tmp = *com;
  ltr_int_unlockSemaphore(mmm.sem);
  if(tmp.state < LINUXTRACK_OK){
    return 0;
  }
  uint32_t p_w = tmp.full_pose.pose.resolution_x;
  uint32_t p_h = tmp.full_pose.pose.resolution_y;

  if(p_w * p_h > mmap.size){
    ltr_int_unmap_file(&mmap);
    int data_size = FRAME_BUFFERS * p_w * p_h + (3 * sizeof(uint32_t));
    char *fname = ltr_int_get_default_file_name("frames.dat");
    if(!ltr_int_mmap_file(fname, data_size, &mmap)){
      free(fname);
      return 0;
    }
    free(fname);
  }
  if(mmap.data == NULL){
    return 0;
  }
  uint32_t *data = (uint32_t*)mmap.data;

  *req_width = data[1];
  *req_height = data[2];
  unsigned int flag = data[0];
  uint32_t frame_size = (*req_width) * (*req_height);
  if((p_w * p_h) < frame_size){ //Size had to change, mmap not big enough
    return 0;
  }
  if(buf_size < frame_size){
    return 0;
  }
  uint8_t *buf = ((uint8_t*)mmap.data) + (3 * sizeof(uint32_t)) + flag * frame_size;
  memcpy(buffer, buf, frame_size);
  return 1;
}

int ltr_wait(int timeout)
{
  bool hup = false;
  int res = ltr_int_pipe_poll(notify_pipe, timeout, &hup);
  if(res > 0){
    uint8_t tmp[1024];
    ssize_t read_res = -1;
    while((read_res = read(notify_pipe, &tmp, sizeof(tmp))) > 0);
  }
  return res;
}

