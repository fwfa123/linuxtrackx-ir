/*
 * LinuxTrack Standalone GUI Stubs
 * 
 * This file provides stub implementations of LinuxTrack library functions
 * to allow the GUI to compile and run independently for testing purposes.
 * 
 * These stubs return safe default values and don't perform actual tracking.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef STANDALONE_GUI

// LinuxTrack API stubs
int ltr_init(const char *cust_section) {
    (void)cust_section;
    printf("[STUB] ltr_init called\n");
    return 0; // Success
}

int ltr_shutdown() {
    printf("[STUB] ltr_shutdown called\n");
    return 0;
}

int ltr_suspend() {
    printf("[STUB] ltr_suspend called\n");
    return 0;
}

int ltr_wakeup() {
    printf("[STUB] ltr_wakeup called\n");
    return 0;
}

int ltr_recenter() {
    printf("[STUB] ltr_recenter called\n");
    return 0;
}

int ltr_get_pose(float *heading, float *pitch, float *roll, 
                 float *tx, float *ty, float *tz, unsigned int *counter) {
    // Return some dummy tracking data
    if (heading) *heading = 0.0f;
    if (pitch) *pitch = 0.0f;
    if (roll) *roll = 0.0f;
    if (tx) *tx = 0.0f;
    if (ty) *ty = 0.0f;
    if (tz) *tz = 0.0f;
    if (counter) *counter = 0;
    return 0;
}

int ltr_get_pose_full(float *heading, float *pitch, float *roll,
                      float *tx, float *ty, float *tz,
                      float *raw_heading, float *raw_pitch, float *raw_roll,
                      float *raw_tx, float *raw_ty, float *raw_tz,
                      unsigned int *counter) {
    // Return dummy data for all parameters
    if (heading) *heading = 0.0f;
    if (pitch) *pitch = 0.0f;
    if (roll) *roll = 0.0f;
    if (tx) *tx = 0.0f;
    if (ty) *ty = 0.0f;
    if (tz) *tz = 0.0f;
    if (raw_heading) *raw_heading = 0.0f;
    if (raw_pitch) *raw_pitch = 0.0f;
    if (raw_roll) *raw_roll = 0.0f;
    if (raw_tx) *raw_tx = 0.0f;
    if (raw_ty) *raw_ty = 0.0f;
    if (raw_tz) *raw_tz = 0.0f;
    if (counter) *counter = 0;
    return 0;
}

// Configuration stubs
char* ltr_get_config_dir() {
    return strdup("/tmp/.linuxtrack_test");
}

int ltr_get_tracking_state() {
    return 0; // STOPPED
}

// Device enumeration stubs
typedef struct {
    char name[256];
    char id[64];
    int type;
} device_t;

int ltr_get_device_list(device_t **devices, int *count) {
    printf("[STUB] ltr_get_device_list called\n");
    if (devices && count) {
        *devices = NULL;
        *count = 0;
    }
    return 0;
}

// Profile management stubs
int ltr_get_profile_list(char ***profiles, int *count) {
    printf("[STUB] ltr_get_profile_list called\n");
    if (profiles && count) {
        *profiles = NULL;
        *count = 0;
    }
    return 0;
}

int ltr_set_active_profile(const char *profile) {
    printf("[STUB] ltr_set_active_profile: %s\n", profile ? profile : "NULL");
    return 0;
}

char* ltr_get_active_profile() {
    return strdup("Default");
}

// Server communication stubs
int ltr_start_server() {
    printf("[STUB] ltr_start_server called\n");
    return 0;
}

int ltr_stop_server() {
    printf("[STUB] ltr_stop_server called\n");
    return 0;
}

int ltr_server_running() {
    return 0; // Not running
}

// Calibration stubs
int ltr_start_calibration() {
    printf("[STUB] ltr_start_calibration called\n");
    return 0;
}

int ltr_stop_calibration() {
    printf("[STUB] ltr_stop_calibration called\n");
    return 0;
}

int ltr_calibration_running() {
    return 0; // Not running
}

// Log management stubs
int ltr_log_message(const char *message) {
    printf("[STUB LOG] %s\n", message ? message : "NULL");
    return 0;
}

char* ltr_get_log_messages() {
    return strdup("LinuxTrack GUI running in standalone mode\nNo actual tracking functionality available\n");
}

// Wine integration stubs
int ltr_install_wine_plugin() {
    printf("[STUB] ltr_install_wine_plugin called\n");
    return 0;
}

int ltr_uninstall_wine_plugin() {
    printf("[STUB] ltr_uninstall_wine_plugin called\n");
    return 0;
}

// Model management stubs
int ltr_get_model_list(char ***models, int *count) {
    printf("[STUB] ltr_get_model_list called\n");
    if (models && count) {
        *models = NULL;
        *count = 0;
    }
    return 0;
}

// Preference management stubs
int ltr_get_pref_val(const char *section, const char *key, char *buffer, int buffer_size) {
    printf("[STUB] ltr_get_pref_val: %s.%s\n", 
           section ? section : "NULL", key ? key : "NULL");
    if (buffer && buffer_size > 0) {
        strncpy(buffer, "0", buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
    return 0;
}

int ltr_set_pref_val(const char *section, const char *key, const char *value) {
    printf("[STUB] ltr_set_pref_val: %s.%s = %s\n", 
           section ? section : "NULL", 
           key ? key : "NULL", 
           value ? value : "NULL");
    return 0;
}

// Additional stubs that might be needed
int ltr_get_camera_info(char *name, int name_size, int *width, int *height) {
    printf("[STUB] ltr_get_camera_info called\n");
    if (name && name_size > 0) {
        strncpy(name, "Stub Camera", name_size - 1);
        name[name_size - 1] = '\0';
    }
    if (width) *width = 640;
    if (height) *height = 480;
    return 0;
}

int ltr_get_version(int *major, int *minor, int *patch) {
    if (major) *major = 0;
    if (minor) *minor = 99;
    if (patch) *patch = 20;
    return 0;
}

#endif // STANDALONE_GUI 