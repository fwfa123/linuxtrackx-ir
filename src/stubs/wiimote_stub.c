/*
 * Wiimote/Joystick Tracking Stub Implementation
 * This file provides empty stubs for Wiimote/joystick functionality to avoid Bluetooth/cwiid dependencies
 */

#include <stdbool.h>
#include <stdio.h>

// Stub functions for Wiimote/joystick tracking
bool ltr_int_joy_init(void)
{
    printf("Wiimote/joystick support stubbed - not available\n");
    return false;
}

bool ltr_int_joy_get_frame(void)
{
    return false;
}

void ltr_int_joy_close(void)
{
    // Nothing to do
}

bool ltr_int_joy_found(bool *have_joystick, bool *have_permissions)
{
    *have_joystick = false;
    *have_permissions = false;
    return false;
}

// Additional joystick functions
int ltr_int_joy_get_device_count(void)
{
    return 0; // No devices
}

bool ltr_int_joy_set_device(int device_id)
{
    (void)device_id;
    return false;
}

bool ltr_int_joy_calibrate(void)
{
    return false;
}

// Wiimote specific functions
bool ltr_int_wii_init(void)
{
    printf("Wiimote support stubbed - not available\n");
    return false;
}

bool ltr_int_wii_connect(void)
{
    return false;
}

void ltr_int_wii_close(void)
{
    // Nothing to do
}

bool ltr_int_wii_found(bool *have_wiimote, bool *have_bluetooth)
{
    *have_wiimote = false;
    *have_bluetooth = false;
    return false;
} 