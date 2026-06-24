#ifndef TIR_DRIVER_PREFS__H
#define TIR_DRIVER_PREFS__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool ltr_int_tir_init_prefs();

int ltr_int_tir_get_max_blob();
bool ltr_int_tir_set_max_blob(int val);

int ltr_int_tir_get_min_blob();
bool ltr_int_tir_set_min_blob(int val);

int ltr_int_tir_get_status_brightness();
bool ltr_int_tir_set_status_brightness(int val);

int ltr_int_tir_get_ir_brightness();
bool ltr_int_tir_set_ir_brightness(int val);

int ltr_int_tir_get_threshold();
bool ltr_int_tir_set_threshold(int val);

bool ltr_int_tir_get_status_indication();
bool ltr_int_tir_set_status_indication(bool ind);

bool ltr_int_tir_set_use_grayscale(bool gs);
bool ltr_int_tir_get_use_grayscale();

int ltr_int_tir_get_video_on_delay();
bool ltr_int_tir_set_video_on_delay(int val);

bool ltr_int_tir_get_usb_reset();
bool ltr_int_tir_usb_reset_is_set();
bool ltr_int_tir_set_usb_reset(bool val);

#ifdef __cplusplus
}
#endif

#endif