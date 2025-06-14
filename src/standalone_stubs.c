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
#include <unistd.h>
#include "joy_driver_prefs.h"
#include "dyn_load.h"

#ifdef STANDALONE_GUI

// Forward declarations for stub functions
int ltr_int_tir_found(bool *have_firmware, bool *have_permissions);

// Forward declarations for internal types
typedef struct {
    float heading, pitch, roll;
    float tx, ty, tz;
    float raw_heading, raw_pitch, raw_roll;
    float raw_tx, raw_ty, raw_tz;
    unsigned int counter;
} linuxtrack_full_pose_t;

typedef struct {
    int width, height;
    int pixfmt;
    float fps;
    unsigned char *bitmap;
} frame_type;

typedef enum {
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
    AXIS_RX = 3,
    AXIS_RY = 4,
    AXIS_RZ = 5
} axis_t;

typedef enum {
    AXIS_PARAM_ENABLED = 0,
    AXIS_PARAM_SYMMETRIC = 1,
    AXIS_PARAM_REVERSE = 2,
    AXIS_PARAM_SENSITIVITY = 3,
    AXIS_PARAM_DEADZONE = 4,
    AXIS_PARAM_FILTER_FACTOR = 5
} axis_param_t;

typedef enum {
    CAP_DEVICE = 0,
    FACE_TRACKER = 1,
    WIIMOTE = 2,
    JOYSTICK = 3
} deviceType_t;

// Internal LinuxTrack API stubs - these are the functions the GUI actually calls

// Basic system functions
int ltr_int_check_root() {
    printf("[STUB] ltr_int_check_root called\n");
    return 0; // Not root
}

int ltr_int_log_message(const char *message) {
    printf("[STUB LOG] %s\n", message ? message : "NULL");
    return 0;
}

// Preference management
int ltr_int_read_prefs(const char *filename, bool force) {
    printf("[STUB] ltr_int_read_prefs called with filename=%s, force=%d\n", 
           filename ? filename : "NULL", force);
    
    // Check if we have the linuxtrack.conf file
    char *cwd = getcwd(NULL, 0);
    char path[1024];
    if (filename) {
        if (filename[0] == '/') {
            // Absolute path
            strncpy(path, filename, sizeof(path) - 1);
            path[sizeof(path) - 1] = '\0';
        } else {
            // Relative path - look in test_data
            snprintf(path, sizeof(path), "%s/test_data/usr/share/linuxtrack/%s", cwd, filename);
        }
    } else {
        // Default path - look for linuxtrack1.conf in ~/.config/linuxtrack/
        snprintf(path, sizeof(path), "%s/.config/linuxtrack/linuxtrack1.conf", getenv("HOME"));
    }
    free(cwd);
    
    // Check if file exists
    if (access(path, R_OK) == 0) {
        printf("[STUB LOG] Found preference file: %s\n", path);
        return 1; // Success
    } else {
        printf("[STUB LOG] Pref file not found: %s\n", path);
        return 0; // Failure
    }
}

int ltr_int_save_prefs() {
    printf("[STUB] ltr_int_save_prefs called\n");
    return 1; // Success
}

int ltr_int_free_prefs() {
    printf("[STUB] ltr_int_free_prefs called\n");
    return 1; // Success
}

int ltr_int_new_prefs() {
    printf("[STUB] ltr_int_new_prefs called\n");
    return 1; // Success
}

int ltr_int_need_saving() {
    return 0; // No save needed
}

int ltr_int_prefs_changed() {
    return 0; // No changes
}

char* ltr_int_get_key(const char *section_name, const char *key_name) {
    printf("[STUB] ltr_int_get_key: %s.%s\n", 
           section_name ? section_name : "NULL", key_name ? key_name : "NULL");
    
    // Return some default values for common keys
    if (section_name && key_name) {
        if (strcmp(section_name, "Global") == 0 && strcmp(key_name, "Device-ID") == 0) {
            return strdup("Webcam");
        }
        if (strcmp(section_name, "Global") == 0 && strcmp(key_name, "Prefix") == 0) {
            return NULL; // Will trigger prefix update in checkPrefix
        }
    }
    return NULL;
}





int ltr_int_change_key(const char *section, const char *key, const char *value) {
    printf("[STUB] ltr_int_change_key: %s.%s = %s\n", 
           section ? section : "NULL", 
           key ? key : "NULL", 
           value ? value : "NULL");
    return 1; // Success
}

int ltr_int_change_key_int(const char *section, const char *key, int value) {
    printf("[STUB] ltr_int_change_key_int: %s.%s = %d\n", 
           section ? section : "NULL", 
           key ? key : "NULL", 
           value);
    return 1; // Success
}

int ltr_int_change_key_flt(const char *section, const char *key, float value) {
    printf("[STUB] ltr_int_change_key_flt: %s.%s = %f\n", 
           section ? section : "NULL", 
           key ? key : "NULL", 
           value);
    return 1; // Success
}

char* ltr_int_add_unique_section(const char *section) {
    printf("[STUB] ltr_int_add_unique_section: %s\n", section ? section : "NULL");
    if (section) {
        return strdup(section); // Return the same section name
    }
    return NULL;
}

int ltr_int_find_section(const char *section, char *buffer, int buffer_size) {
    printf("[STUB] ltr_int_find_section: %s\n", section ? section : "NULL");
    if (buffer && buffer_size > 0) {
        strncpy(buffer, "Default", buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
    return 0;
}

char** ltr_int_find_sections(const char *pattern, int *count) {
    printf("[STUB] ltr_int_find_sections: %s\n", pattern ? pattern : "NULL");
    if (count) *count = 0;
    return NULL;
}

char** ltr_int_get_section_list(int *count) {
    printf("[STUB] ltr_int_get_section_list called\n");
    if (count) *count = 0;
    return NULL;
}

const char* ltr_int_get_data_path_prefix() {
    // Return path to our test data directory
    char *cwd = getcwd(NULL, 0);
    char *path = malloc(strlen(cwd) + 50);
    sprintf(path, "%s/test_data/usr/share/linuxtrack/", cwd);
    free(cwd);
    return path;
}

const char* ltr_int_get_lib_path() {
    return strdup("/usr/lib/linuxtrack/");
}

const char* ltr_int_get_logfile_name() {
    return strdup("/tmp/linuxtrack.log");
}

// Tracking state and control
int ltr_int_get_tracking_state() {
    return 0; // STOPPED
}

int ltr_int_request_shutdown() {
    printf("[STUB] ltr_int_request_shutdown called\n");
    return 0;
}

int ltr_int_suspend_cmd() {
    printf("[STUB] ltr_int_suspend_cmd called\n");
    return 0;
}

int ltr_int_wakeup_cmd() {
    printf("[STUB] ltr_int_wakeup_cmd called\n");
    return 0;
}

int ltr_int_recenter_cmd() {
    printf("[STUB] ltr_int_recenter_cmd called\n");
    return 0;
}

// Master thread and server
int ltr_int_master() {
    printf("[STUB] ltr_int_master called (sleeping)\n");
    // Simulate server loop with sleep
    while (1) {
        sleep(1);
    }
    return 0;
}

bool ltr_int_gui_lock(bool do_lock) {
    printf("[STUB] ltr_int_gui_lock called with do_lock=%d (allowing GUI)\n", do_lock);
    return true; // Allow GUI to run in standalone mode
}

void ltr_int_gui_lock_clean() {
    printf("[STUB] ltr_int_gui_lock_clean called\n");
    // Safe to do nothing - no actual lock file to clean
}

typedef void (*ltr_state_changed_t)(void *);
typedef void (*ltr_new_frame_t)(frame_type *, void *);
typedef void (*ltr_new_pose_t)(linuxtrack_full_pose_t *, void *);

int ltr_int_set_callback_hooks(ltr_state_changed_t state_cb, void *state_param,
                               ltr_new_frame_t frame_cb, void *frame_param,
                               ltr_new_pose_t pose_cb, void *pose_param) {
    printf("[STUB] ltr_int_set_callback_hooks called\n");
    (void)state_cb; (void)state_param;
    (void)frame_cb; (void)frame_param;
    (void)pose_cb; (void)pose_param;
    return 0;
}

int ltr_int_get_camera_update() {
    return 0; // No update
}

// Axes and profile management
int ltr_int_init_axes(const char *profile) {
    printf("[STUB] ltr_int_init_axes: %s\n", profile ? profile : "NULL");
    return 0;
}

int ltr_int_close_axes() {
    printf("[STUB] ltr_int_close_axes called\n");
    return 0;
}

int ltr_int_axes_from_default() {
    printf("[STUB] ltr_int_axes_from_default called\n");
    return 0;
}

int ltr_int_postprocess_axes(linuxtrack_full_pose_t *pose) {
    printf("[STUB] ltr_int_postprocess_axes called\n");
    (void)pose;
    return 0;
}

float ltr_int_get_axis_param(axis_t axis, axis_param_t param) {
    printf("[STUB] ltr_int_get_axis_param: axis=%d, param=%d\n", axis, param);
    return 1.0f; // Default value
}

int ltr_int_set_axis_param(axis_t axis, axis_param_t param, float value) {
    printf("[STUB] ltr_int_set_axis_param: axis=%d, param=%d, value=%f\n", 
           axis, param, value);
    return 0;
}

bool ltr_int_get_axis_bool_param(axis_t axis, axis_param_t param) {
    printf("[STUB] ltr_int_get_axis_bool_param: axis=%d, param=%d\n", axis, param);
    return false;
}

int ltr_int_set_axis_bool_param(axis_t axis, axis_param_t param, bool value) {
    printf("[STUB] ltr_int_set_axis_bool_param: axis=%d, param=%d, value=%d\n", 
           axis, param, value);
    return 0;
}

int ltr_int_change(void) {
    printf("[STUB] ltr_int_change called\n");
    return 0;
}

float ltr_int_val_on_axis(axis_t axis, float value) {
    printf("[STUB] ltr_int_val_on_axis: axis=%d, value=%f\n", axis, value);
    return value;
}

bool ltr_int_is_symetrical(axis_t axis) {
    printf("[STUB] ltr_int_is_symetrical: axis=%d\n", axis);
    return false;
}

// Model and announcement functions
int ltr_int_announce_model_change() {
    printf("[STUB] ltr_int_announce_model_change called\n");
    return 0;
}

// Configuration flags
bool ltr_int_use_alter() {
    return false;
}

int ltr_int_set_use_alter(bool value) {
    printf("[STUB] ltr_int_set_use_alter: %d\n", value);
    return 0;
}

bool ltr_int_use_oldrot() {
    return false;
}

int ltr_int_set_use_oldrot(bool value) {
    printf("[STUB] ltr_int_set_use_oldrot: %d\n", value);
    return 0;
}

bool ltr_int_do_tr_align() {
    return false;
}

int ltr_int_set_tr_align(bool value) {
    printf("[STUB] ltr_int_set_tr_align: %d\n", value);
    return 0;
}

float ltr_int_get_focal_length() {
    return 800.0f; // Default focal length
}

int ltr_int_set_focal_length(float value) {
    printf("[STUB] ltr_int_set_focal_length: %f\n", value);
    return 0;
}

// Library loading functions
void* ltr_int_load_library(char *lib_name, lib_fun_def_t *func_defs) {
    printf("[STUB] ltr_int_load_library: %s (SAFE MODE - No actual loading)\n", lib_name ? lib_name : "NULL");
    
    // In standalone mode, we don't actually load hardware libraries
    if (lib_name) {
        // For joystick library, populate function pointers with our stubs
        if (strstr(lib_name, "libjoy")) {
            printf("[STUB LOG] Joystick library requested - providing stub functions\n");
            if (func_defs) {
                while (func_defs->name != NULL) {
                    printf("[STUB LOG] Setting up function: %s\n", func_defs->name);
                    if (strcmp(func_defs->name, "ltr_int_joy_enum_joysticks") == 0) {
                        *(void **)(func_defs->ref) = (void*)ltr_int_joy_enum_joysticks;
                    } else if (strcmp(func_defs->name, "ltr_int_joy_free_joysticks") == 0) {
                        *(void **)(func_defs->ref) = (void*)ltr_int_joy_free_joysticks;
                    } else if (strcmp(func_defs->name, "ltr_int_joy_enum_axes") == 0) {
                        *(void **)(func_defs->ref) = (void*)ltr_int_joy_enum_axes;
                    } else if (strcmp(func_defs->name, "ltr_int_joy_free_axes") == 0) {
                        *(void **)(func_defs->ref) = (void*)ltr_int_joy_free_axes;
                    } else {
                        printf("[STUB LOG] Unknown joystick function: %s\n", func_defs->name);
                        *(void **)(func_defs->ref) = NULL;
                    }
                    ++func_defs;
                }
            }
            return (void*)0x1; // Fake handle for joystick library
        }
        
        // For TrackIR library, populate function pointers with our stubs  
        if (strstr(lib_name, "libtir")) {
            printf("[STUB LOG] TrackIR library requested - providing stub functions (no hardware)\n");
            if (func_defs) {
                while (func_defs->name != NULL) {
                    printf("[STUB LOG] Setting up function: %s\n", func_defs->name);
                    if (strcmp(func_defs->name, "ltr_int_tir_found") == 0) {
                        *(void **)(func_defs->ref) = (void*)ltr_int_tir_found;
                    } else {
                        printf("[STUB LOG] Unknown TIR function: %s\n", func_defs->name);
                        *(void **)(func_defs->ref) = NULL;
                    }
                    ++func_defs;
                }
            }
            return (void*)0x1; // Fake handle for TIR library
        }
        
        // For other hardware libraries, return NULL to indicate they're not available
        if (strstr(lib_name, "libtir") || strstr(lib_name, "libwc") || strstr(lib_name, "libwii")) {
            printf("[STUB LOG] Hardware library %s not loaded in standalone mode\n", lib_name);
            return NULL; // Indicate library not available
        }
    }
    return (void*)0x1; // Fake handle for other libraries
}

int ltr_int_unload_library(void *handle, lib_fun_def_t *func_defs) {
    printf("[STUB] ltr_int_unload_library: %p\n", handle);
    // Clear function pointers like the real implementation
    if (func_defs) {
        while (func_defs->name != NULL) {
            *(void **)(func_defs->ref) = NULL;
            ++func_defs;
        }
    }
    return 0;
}

// Device-specific functions - TIR
int ltr_int_tir_init_prefs(const char *section) {
    printf("[STUB] ltr_int_tir_init_prefs: %s\n", section ? section : "NULL");
    return 0;
}

int ltr_int_tir_get_threshold() { return 128; }
int ltr_int_tir_set_threshold(int value) { 
    printf("[STUB] ltr_int_tir_set_threshold: %d\n", value);
    return 0; 
}

int ltr_int_tir_get_max_blob() { return 100; }
int ltr_int_tir_set_max_blob(int value) { 
    printf("[STUB] ltr_int_tir_set_max_blob: %d\n", value);
    return 0; 
}

int ltr_int_tir_get_min_blob() { return 10; }
int ltr_int_tir_set_min_blob(int value) { 
    printf("[STUB] ltr_int_tir_set_min_blob: %d\n", value);
    return 0; 
}

int ltr_int_tir_get_ir_brightness() { return 50; }
int ltr_int_tir_set_ir_brightness(int value) { 
    printf("[STUB] ltr_int_tir_set_ir_brightness: %d\n", value);
    return 0; 
}

int ltr_int_tir_get_status_brightness() { return 50; }
int ltr_int_tir_set_status_brightness(int value) { 
    printf("[STUB] ltr_int_tir_set_status_brightness: %d\n", value);
    return 0; 
}

bool ltr_int_tir_get_status_indication() { return false; }
int ltr_int_tir_set_status_indication(bool value) { 
    printf("[STUB] ltr_int_tir_set_status_indication: %d\n", value);
    return 0; 
}

bool ltr_int_tir_get_use_grayscale() { return false; }
int ltr_int_tir_set_use_grayscale(bool value) { 
    printf("[STUB] ltr_int_tir_set_use_grayscale: %d\n", value);
    return 0; 
}

// Device-specific functions - Wiimote  
int ltr_int_wii_init_prefs(const char *section) {
    printf("[STUB] ltr_int_wii_init_prefs: %s\n", section ? section : "NULL");
    return 0;
}

int ltr_int_get_run_indication() { return 0; }
int ltr_int_set_run_indication(int value) { 
    printf("[STUB] ltr_int_set_run_indication: %d\n", value);
    return 0; 
}

int ltr_int_get_pause_indication() { return 0; }
int ltr_int_set_pause_indication(int value) { 
    printf("[STUB] ltr_int_set_pause_indication: %d\n", value);
    return 0; 
}

// Device-specific functions - Webcam
int ltr_int_wc_init_prefs(const char *section) {
    printf("[STUB] ltr_int_wc_init_prefs: %s\n", section ? section : "NULL");
    return 0;
}

int ltr_int_wc_close_prefs() {
    printf("[STUB] ltr_int_wc_close_prefs called\n");
    return 0;
}

int ltr_int_wc_get_pixfmt() { return 0; }
int ltr_int_wc_set_pixfmt(int value) { 
    printf("[STUB] ltr_int_wc_set_pixfmt: %d\n", value);
    return 0; 
}

const char* ltr_int_wc_get_resolution() { return strdup("640x480"); }
int ltr_int_wc_set_resolution(const char *value) { 
    printf("[STUB] ltr_int_wc_set_resolution: %s\n", value ? value : "NULL");
    return 0; 
}

float ltr_int_wc_get_fps() { return 30.0f; }
int ltr_int_wc_set_fps(float value) { 
    printf("[STUB] ltr_int_wc_set_fps: %f\n", value);
    return 0; 
}

int ltr_int_wc_get_threshold() { return 128; }
int ltr_int_wc_set_threshold(int value) { 
    printf("[STUB] ltr_int_wc_set_threshold: %d\n", value);
    return 0; 
}

int ltr_int_wc_get_max_blob() { return 100; }
int ltr_int_wc_set_max_blob(int value) { 
    printf("[STUB] ltr_int_wc_set_max_blob: %d\n", value);
    return 0; 
}

int ltr_int_wc_get_min_blob() { return 10; }
int ltr_int_wc_set_min_blob(int value) { 
    printf("[STUB] ltr_int_wc_set_min_blob: %d\n", value);
    return 0; 
}

const char* ltr_int_wc_get_cascade() { return strdup("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml"); }
int ltr_int_wc_set_cascade(const char *value) { 
    printf("[STUB] ltr_int_wc_set_cascade: %s\n", value ? value : "NULL");
    return 0; 
}

float ltr_int_wc_get_eff() { return 0.8f; }
int ltr_int_wc_set_eff(float value) { 
    printf("[STUB] ltr_int_wc_set_eff: %f\n", value);
    return 0; 
}

int ltr_int_wc_get_optim_level() { return 0; }
int ltr_int_wc_set_optim_level(int value) { 
    printf("[STUB] ltr_int_wc_set_optim_level: %d\n", value);
    return 0; 
}

// Device-specific functions - Joystick
bool ltr_int_joy_init_prefs() {
    printf("[STUB] ltr_int_joy_init_prefs called\n");
    return true; // Success
}

ifc_type_t ltr_int_joy_get_ifc() { return e_EVDEV; }
bool ltr_int_joy_set_ifc(ifc_type_t value) { 
    printf("[STUB] ltr_int_joy_set_ifc: %d\n", value);
    return true; 
}

int ltr_int_joy_get_pitch_axis() { return 0; }
bool ltr_int_joy_set_pitch_axis(int value) { 
    printf("[STUB] ltr_int_joy_set_pitch_axis: %d\n", value);
    return true; 
}

int ltr_int_joy_get_yaw_axis() { return 0; }
bool ltr_int_joy_set_yaw_axis(int value) { 
    printf("[STUB] ltr_int_joy_set_yaw_axis: %d\n", value);
    return true; 
}

int ltr_int_joy_get_roll_axis() { return 0; }
bool ltr_int_joy_set_roll_axis(int value) { 
    printf("[STUB] ltr_int_joy_set_roll_axis: %d\n", value);
    return true; 
}

int ltr_int_joy_get_tx_axis() { return 0; }
bool ltr_int_joy_set_tx_axis(int value) { 
    printf("[STUB] ltr_int_joy_set_tx_axis: %d\n", value);
    return true; 
}

int ltr_int_joy_get_ty_axis() { return 0; }
bool ltr_int_joy_set_ty_axis(int value) { 
    printf("[STUB] ltr_int_joy_set_ty_axis: %d\n", value);
    return true; 
}

int ltr_int_joy_get_tz_axis() { return 0; }
bool ltr_int_joy_set_tz_axis(int value) { 
    printf("[STUB] ltr_int_joy_set_tz_axis: %d\n", value);
    return true; 
}

float ltr_int_joy_get_angle_base() { return 1.0f; }
float ltr_int_joy_get_trans_base() { return 1.0f; }
int ltr_int_joy_get_pps() { return 60; }
bool ltr_int_joy_set_pps(int value) { 
    printf("[STUB] ltr_int_joy_set_pps: %d\n", value);
    return true; 
}

// Utility functions
int ltr_int_dump_prefs() {
    printf("[STUB] ltr_int_dump_prefs called\n");
    return 0;
}

void ltr_int_array_cleanup(void *array) {
    printf("[STUB] ltr_int_array_cleanup: %p\n", array);
    if (array) free(array);
}

// Firmware extraction functions
int extract_blob(const char *game_name, const char *output_dir) {
    printf("[STUB] extract_blob: %s -> %s\n", 
           game_name ? game_name : "NULL", 
           output_dir ? output_dir : "NULL");
    return 0;
}

int get_game_data(const char *game_name, void **data, int *size) {
    printf("[STUB] get_game_data: %s\n", game_name ? game_name : "NULL");
    if (data) *data = NULL;
    if (size) *size = 0;
    return 0;
}

// Joystick enumeration functions for GUI
joystickNames_t *ltr_int_joy_enum_joysticks(ifc_type_t ifc) {
    printf("[STUB] ltr_int_joy_enum_joysticks: ifc=%d (no hardware available)\n", ifc);
    joystickNames_t *names = malloc(sizeof(joystickNames_t));
    if (names) {
        names->nameList = NULL;
        names->nameListSize = 0;
        names->namesFound = 0;
    }
    return names;
}

void ltr_int_joy_free_joysticks(joystickNames_t *nl) {
    printf("[STUB] ltr_int_joy_free_joysticks called\n");
    if (nl) {
        if (nl->nameList) {
            for (size_t i = 0; i < nl->namesFound; i++) {
                if (nl->nameList[i]) {
                    free(nl->nameList[i]);
                }
            }
            free(nl->nameList);
        }
        free(nl);
    }
}

bool ltr_int_joy_enum_axes(ifc_type_t ifc, const char *name, axes_t *axes) {
    printf("[STUB] ltr_int_joy_enum_axes: ifc=%d, name=%s (no hardware)\n", 
           ifc, name ? name : "NULL");
    if (axes) {
        axes->axesList = NULL;
        axes->axisNames = NULL;
        axes->min = NULL;
        axes->max = NULL;
        axes->axes = 0;
    }
    return false; // No hardware available
}

void ltr_int_joy_free_axes(axes_t axes) {
    printf("[STUB] ltr_int_joy_free_axes called\n");
    // Safe to do nothing - our stub doesn't allocate anything
    (void)axes;
}

// TrackIR detection function for GUI
int ltr_int_tir_found(bool *have_firmware, bool *have_permissions) {
    printf("[STUB] ltr_int_tir_found called (no TrackIR hardware in standalone mode)\n");
    if (have_firmware) {
        *have_firmware = true; // Pretend firmware is OK to avoid dialog
    }
    if (have_permissions) {
        *have_permissions = true; // Pretend permissions are OK to avoid dialog  
    }
    return 0; // Return 0 = no TrackIR device found (which is correct for standalone mode)
}

#endif // STANDALONE_GUI 