#ifndef TIR_PROTOCOL__H
#define TIR_PROTOCOL__H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "usb_ifc.h"

typedef struct {
  bool fw_loaded;
  uint8_t cfg_flag;
  uint16_t fw_cksum;
} ltr_tir_status;

typedef struct {
  uint32_t vline;
  uint32_t hstart;
  uint32_t hstop;
  uint32_t sum_x;
  uint32_t sum;
  uint32_t points;
} ltr_tir_stripe;

uint16_t ltr_tir_firmware_checksum(const uint8_t *firmware, size_t size);
bool ltr_tir_parse_status_packet(const uint8_t *data, size_t size,
                                 ltr_tir_status *status);
bool ltr_tir_validate_tir5_header(const uint8_t *data, size_t size);
bool ltr_tir_validate_sn4_header(const uint8_t *data, size_t size);

size_t ltr_tir_build_led_packet(unsigned int leds, unsigned int mask,
                                uint8_t packet[3]);
size_t ltr_tir_build_threshold_packet(dev_found device, unsigned int value,
                                      uint8_t packet[4]);
void ltr_tir_build_exposure_packets(unsigned int exposure,
                                     uint8_t high_packet[6],
                                     uint8_t low_packet[6]);

bool ltr_tir_decode_tir2_stripe(const uint8_t payload[3], ltr_tir_stripe *stripe);
bool ltr_tir_decode_tir4_stripe(const uint8_t payload[4], ltr_tir_stripe *stripe);
bool ltr_tir_decode_tir5_stripe(const uint8_t payload[8], ltr_tir_stripe *stripe);

#endif
