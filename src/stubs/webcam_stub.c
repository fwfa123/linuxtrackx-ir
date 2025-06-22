/*
 * Webcam Tracking Stub Implementation
 * This file provides empty stubs for webcam functionality to avoid OpenCV dependencies
 */

#include <stdbool.h>
#include <stdio.h>

// Stub functions for webcam camera abstraction layer
bool ltr_int_wc_init(void)
{
    printf("Webcam support stubbed - not available\n");
    return false;
}

bool ltr_int_wc_get_frame(void)
{
    return false;
}

void ltr_int_wc_close(void)
{
    // Nothing to do
}

bool ltr_int_wc_found(bool *have_webcam, bool *have_permissions)
{
    *have_webcam = false;
    *have_permissions = false;
    return false;
}

// Additional webcam functions that might be called
int ltr_int_wc_get_device_list(void)
{
    return 0; // No devices
}

bool ltr_int_wc_set_device(int device_id)
{
    (void)device_id;
    return false;
}

bool ltr_int_wc_set_resolution(int width, int height)
{
    (void)width;
    (void)height; 
    return false;
}

bool ltr_int_wc_set_fps(int fps)
{
    (void)fps;
    return false;
} 