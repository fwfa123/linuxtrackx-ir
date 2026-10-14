#ifndef TIR_TRANSPORT__H
#define TIR_TRANSPORT__H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "usb_ifc.h"

#define LTR_USB_TRANSPORT_ABI_VERSION 1u

/*
 * Information returned by transport discovery.  The legacy Linux backend
 * only returned dev_found, but a native backend also needs the USB identity
 * that was actually matched for diagnostics and model-specific handling.
 */
typedef struct {
  uint16_t vendor_id;
  uint16_t product_id;
  dev_found model;
  bool permission_denied;
} ltr_usb_device_info;

typedef bool (ltr_usb_init_fn)(void *context);
typedef dev_found (ltr_usb_find_tir_fn)(void *context, ltr_usb_device_info *info);
typedef bool (ltr_usb_reset_fn)(void *context);
typedef bool (ltr_usb_prepare_fn)(void *context, unsigned int config,
                                  unsigned int interface_number);
typedef bool (ltr_usb_send_fn)(void *context, int endpoint,
                               unsigned char data[], size_t size);
typedef bool (ltr_usb_receive_fn)(void *context, int endpoint,
                                  unsigned char data[], size_t size,
                                  size_t *transferred, long timeout);
typedef bool (ltr_usb_control_fn)(void *context, uint8_t request_type,
                                  uint8_t request, uint16_t value,
                                  uint16_t index, unsigned char data[],
                                  size_t size);
typedef void (ltr_usb_finish_fn)(void *context, unsigned int interface_number);

/*
 * Platform-neutral transport contract used by the TrackIR protocol engine.
 * The context is owned by the backend. Platform-specific implementations can
 * therefore keep their handles private without exposing them to tir_hw.
 */
typedef struct {
  uint32_t abi_version;
  void *context;
  ltr_usb_init_fn *init;
  ltr_usb_find_tir_fn *find_tir;
  ltr_usb_reset_fn *reset;
  ltr_usb_prepare_fn *prepare;
  ltr_usb_send_fn *send;
  ltr_usb_receive_fn *receive;
  ltr_usb_control_fn *control;
  ltr_usb_finish_fn *finish;
} ltr_usb_transport;

/* The current dynamic-loader backend still exports these legacy functions. */
typedef struct {
  init_usb_fun *init;
  find_tir_fun *find_tir;
  reset_device_fun *reset;
  prepare_device_fun *prepare;
  send_data_fun *send;
  receive_data_fun *receive;
  ctrl_data_fun *control;
  finish_usb_fun *finish;
} ltr_usb_legacy_api;

/* Build a transport facade around the existing dynamically loaded symbols. */
void ltr_usb_transport_from_legacy(ltr_usb_transport *transport,
                                   ltr_usb_legacy_api *legacy);

bool ltr_usb_transport_is_valid(const ltr_usb_transport *transport);
bool ltr_usb_transport_init(const ltr_usb_transport *transport);
dev_found ltr_usb_transport_find_tir(const ltr_usb_transport *transport,
                                     ltr_usb_device_info *info);
bool ltr_usb_transport_reset(const ltr_usb_transport *transport);
bool ltr_usb_transport_prepare(const ltr_usb_transport *transport,
                               unsigned int config, unsigned int interface_number);
bool ltr_usb_transport_send(const ltr_usb_transport *transport, int endpoint,
                            unsigned char data[], size_t size);
bool ltr_usb_transport_receive(const ltr_usb_transport *transport, int endpoint,
                               unsigned char data[], size_t size,
                               size_t *transferred, long timeout);
bool ltr_usb_transport_control(const ltr_usb_transport *transport,
                               uint8_t request_type, uint8_t request,
                               uint16_t value, uint16_t index,
                               unsigned char data[], size_t size);
void ltr_usb_transport_finish(const ltr_usb_transport *transport,
                              unsigned int interface_number);

#endif
