#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "tir_protocol.h"

static void test_checksum(void)
{
  const uint8_t firmware[] = {0x00, 0x01, 0x02};
  assert(ltr_tir_firmware_checksum(firmware, sizeof(firmware)) == 0x0033);
  assert(ltr_tir_firmware_checksum(NULL, 0) == 0);
  assert(ltr_tir_firmware_checksum(NULL, 1) == 0);
}

static void test_status(void)
{
  const uint8_t status_packet[] = {0x07, 0x20, 0x00, 0x01, 0x12, 0x34, 0x02};
  ltr_tir_status status;
  assert(ltr_tir_parse_status_packet(status_packet, sizeof(status_packet), &status));
  assert(status.fw_loaded);
  assert(status.cfg_flag == 0x02);
  assert(status.fw_cksum == 0x1234);

  const uint8_t extended_status[] = {0x09, 0x20, 0x00, 0x00, 0, 0, 0};
  assert(ltr_tir_parse_status_packet(extended_status, sizeof(extended_status), &status));
  assert(!status.fw_loaded);
  assert(!ltr_tir_parse_status_packet(status_packet, 6, &status));

  uint8_t invalid[7];
  memcpy(invalid, status_packet, sizeof(invalid));
  invalid[1] = 0x40;
  assert(!ltr_tir_parse_status_packet(invalid, sizeof(invalid), &status));
}

static void test_headers(void)
{
  const uint8_t tir5_header[] = {0x00, 0x00, 0x00, 0xAA};
  assert(ltr_tir_validate_tir5_header(tir5_header, sizeof(tir5_header)));
  assert(!ltr_tir_validate_tir5_header(tir5_header, 3));

  const uint8_t sn4_header[] = {0x01, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0xAB};
  assert(ltr_tir_validate_sn4_header(sn4_header, sizeof(sn4_header)));
  assert(!ltr_tir_validate_sn4_header(sn4_header, 7));
}

static void test_packet_construction(void)
{
  uint8_t packet[6];
  assert(ltr_tir_build_led_packet(0xFF, 0x0F, packet) == 3);
  const uint8_t led_expected[] = {0x10, 0xF0, 0x00};
  assert(memcmp(packet, led_expected, sizeof(led_expected)) == 0);

  assert(ltr_tir_build_threshold_packet(TIR5V2, 10, packet) == 4);
  const uint8_t tir5_threshold[] = {0x15, 30, 0x01, 0x00};
  assert(memcmp(packet, tir5_threshold, sizeof(tir5_threshold)) == 0);

  assert(ltr_tir_build_threshold_packet(TIR2, 10, packet) == 3);
  const uint8_t tir2_threshold[] = {0x15, 40, 0x01};
  assert(memcmp(packet, tir2_threshold, sizeof(tir2_threshold)) == 0);

  uint8_t high[6];
  uint8_t low[6];
  ltr_tir_build_exposure_packets(0x1234, high, low);
  const uint8_t expected_high[] = {0x23, 0x42, 0x08, 0x12, 0x00, 0x00};
  const uint8_t expected_low[] = {0x23, 0x42, 0x10, 0x34, 0x00, 0x00};
  assert(memcmp(high, expected_high, sizeof(high)) == 0);
  assert(memcmp(low, expected_low, sizeof(low)) == 0);
}

static void test_stripe_decoding(void)
{
  ltr_tir_stripe stripe;

  const uint8_t tir2_payload[] = {0x05, 0x10, 0x14};
  assert(ltr_tir_decode_tir2_stripe(tir2_payload, &stripe));
  assert(stripe.vline == 5);
  assert(stripe.hstart == 16);
  assert(stripe.hstop == 20);
  assert(stripe.points == 5);
  assert(stripe.sum == 5);
  assert(stripe.sum_x == 10);

  const uint8_t tir4_payload[] = {0x10, 0x20, 0x30, 0xF8};
  assert(ltr_tir_decode_tir4_stripe(tir4_payload, &stripe));
  assert(stripe.vline == 0x110);
  assert(stripe.hstart == 0x320);
  assert(stripe.hstop == 0x330);
  assert(stripe.points == 17);
  assert(stripe.sum == 17);
  assert(stripe.sum_x == 136);

  const uint8_t tir5_payload[] = {0x12, 0xA3, 0xE5, 0x6D,
                                  0x01, 0x02, 0x80, 0x34};
  assert(ltr_tir_decode_tir5_stripe(tir5_payload, &stripe));
  assert(stripe.hstart == 0x4A);
  assert(stripe.vline == 287);
  assert(stripe.points == 173);
  assert(stripe.hstop == 246);
  assert(stripe.sum_x == 655877);
  assert(stripe.sum == 52);

  assert(!ltr_tir_decode_tir5_stripe(NULL, &stripe));
  assert(!ltr_tir_decode_tir4_stripe(tir4_payload, NULL));
}

int main(void)
{
  test_checksum();
  test_status();
  test_headers();
  test_packet_construction();
  test_stripe_decoding();
  printf("test_tir_protocol passed\n");
  return 0;
}
