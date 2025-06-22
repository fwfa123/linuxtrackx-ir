/*
 * Face Tracking Stub Implementation  
 * This file provides empty stubs for face tracking functionality to avoid OpenCV dependencies
 */

#include <stdbool.h>
#include <stdio.h>

// Stub functions for face tracking
bool ltr_int_ft_init(void)
{
    printf("Face tracking support stubbed - not available\n");
    return false;
}

bool ltr_int_ft_get_frame(void)
{
    return false;
}

void ltr_int_ft_close(void)
{
    // Nothing to do
}

bool ltr_int_ft_found(bool *have_camera, bool *have_permissions)
{
    *have_camera = false;
    *have_permissions = false;
    return false;
}

// Additional face tracking functions
bool ltr_int_ft_load_cascade(const char *cascade_path)
{
    (void)cascade_path;
    return false;
}

bool ltr_int_ft_set_device(int device_id)
{
    (void)device_id;
    return false;
}

bool ltr_int_ft_set_resolution(int width, int height)
{
    (void)width;
    (void)height;
    return false;
} 