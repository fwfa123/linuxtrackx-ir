#include "tir_protocol.h"

#include <string.h>

uint16_t ltr_tir_firmware_checksum(const uint8_t *firmware, size_t size)
{
  uint32_t checksum = 0;
  if(firmware == NULL && size != 0){
    return 0;
  }

  while(size > 0){
    uint32_t byte = *firmware++;
    checksum += byte;
    checksum ^= byte << 4;
    --size;
  }
  return (uint16_t)(checksum & 0xffffu);
}

bool ltr_tir_parse_status_packet(const uint8_t *data, size_t size,
                                 ltr_tir_status *status)
{
  if(data == NULL || status == NULL || size < 7 || data[0] < 0x07 ||
     data[1] != 0x20){
    return false;
  }
  status->fw_loaded = data[3] == 1;
  status->cfg_flag = data[6];
  status->fw_cksum = (uint16_t)(((uint16_t)data[4] << 8) | data[5]);
  return true;
}

bool ltr_tir_validate_tir5_header(const uint8_t *data, size_t size)
{
  return data != NULL && size >= 4 &&
    (uint8_t)(data[0] ^ data[1] ^ data[2] ^ data[3]) == 0xAA;
}

bool ltr_tir_validate_sn4_header(const uint8_t *data, size_t size)
{
  if(data == NULL || size < 8){
    return false;
  }
  uint8_t checksum = 0;
  for(size_t i = 0; i < 8; ++i){
    checksum ^= data[i];
  }
  return checksum == 0xAA;
}

size_t ltr_tir_build_led_packet(unsigned int leds, unsigned int mask,
                                uint8_t packet[3])
{
  if(packet == NULL){
    return 0;
  }
  packet[0] = 0x10;
  packet[1] = (uint8_t)(leds & 0xf0u);
  packet[2] = (uint8_t)(mask & 0xf0u);
  return 3;
}

size_t ltr_tir_build_threshold_packet(dev_found device, unsigned int value,
                                      uint8_t packet[4])
{
  if(packet == NULL){
    return 0;
  }
  if(value > 253){
    value = 253;
  }
  packet[0] = 0x15;
  packet[1] = (uint8_t)value;
  packet[2] = 0x01;
  packet[3] = 0x00;

  if(device > TIR2){
    if(value < 30){
      value = 30;
      packet[1] = (uint8_t)value;
    }
    return 4;
  }
  if(value < 40){
    packet[1] = 40;
  }
  return 3;
}

void ltr_tir_build_exposure_packets(unsigned int exposure,
                                    uint8_t high_packet[6],
                                    uint8_t low_packet[6])
{
  static const uint8_t high_template[6] = {0x23, 0x42, 0x08, 0x00, 0x00, 0x00};
  static const uint8_t low_template[6] = {0x23, 0x42, 0x10, 0x00, 0x00, 0x00};
  if(high_packet != NULL){
    memcpy(high_packet, high_template, sizeof(high_template));
    high_packet[3] = (uint8_t)(exposure >> 8);
  }
  if(low_packet != NULL){
    memcpy(low_packet, low_template, sizeof(low_template));
    low_packet[3] = (uint8_t)exposure;
  }
}

bool ltr_tir_decode_tir2_stripe(const uint8_t payload[3], ltr_tir_stripe *stripe)
{
  if(payload == NULL || stripe == NULL){
    return false;
  }
  stripe->vline = payload[0];
  stripe->hstart = payload[1];
  stripe->hstop = payload[2];
  stripe->sum = stripe->hstop - stripe->hstart + 1;
  stripe->sum_x = stripe->sum * (stripe->sum - 1) / 2;
  stripe->points = stripe->sum;
  return true;
}

bool ltr_tir_decode_tir4_stripe(const uint8_t payload[4], ltr_tir_stripe *stripe)
{
  if(payload == NULL || stripe == NULL){
    return false;
  }
  uint8_t rest = payload[3];
  stripe->vline = payload[0];
  stripe->hstart = payload[1];
  stripe->hstop = payload[2];
  if(rest & 0x20) stripe->vline |= 0x100;
  if(rest & 0x80) stripe->hstart |= 0x100;
  if(rest & 0x40) stripe->hstop |= 0x100;
  if(rest & 0x10) stripe->hstart |= 0x200;
  if(rest & 0x08) stripe->hstop |= 0x200;
  stripe->sum = stripe->hstop - stripe->hstart + 1;
  stripe->sum_x = stripe->sum * (stripe->sum - 1) / 2;
  stripe->points = stripe->sum;
  return true;
}

bool ltr_tir_decode_tir5_stripe(const uint8_t payload[8], ltr_tir_stripe *stripe)
{
  if(payload == NULL || stripe == NULL){
    return false;
  }
  stripe->hstart = ((uint32_t)payload[0] << 2) |
                   ((uint32_t)payload[1] >> 6);
  stripe->vline = (((uint32_t)payload[1] & 0x3F) << 3) |
                  (((uint32_t)payload[2] & 0xE0) >> 5);
  stripe->points = (((uint32_t)payload[2] & 0x1F) << 5) |
                   ((uint32_t)payload[3] >> 3);
  stripe->hstop = stripe->points + stripe->hstart - 1;
  stripe->sum_x = (((uint32_t)payload[3] & 7) << 17) |
                  ((uint32_t)payload[4] << 9) |
                  ((uint32_t)payload[5] << 1) |
                  ((uint32_t)payload[6] >> 7);
  stripe->sum = (((uint32_t)payload[6] & 0x7F) << 8) | payload[7];
  return true;
}
