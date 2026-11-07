#import "TrackIRUSBTransport.h"

#import <IOKit/IOKitLib.h>
#import <IOKit/IOReturn.h>

#include "TrackIRUSB.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define USB_IMPL_ONLY
#include "usb_ifc.h"
#include "utils.h"

/*
 * This file is the C ABI adapter loaded by tir_driver.c as libltusb1 on
 * macOS.  The protocol engine remains in libtir; only the USB ownership and
 * transfer implementation is Objective-C++ here.
 */

static TrackIRUSBTransport *trackir_transport = nil;
static bool usb_initialized = false;

/* tir_hw.c uses this as its receive buffer size.  Keep the native backend
 * bounded to the same size rather than accepting arbitrary allocations from
 * a dynamically loaded caller. */
static const size_t kMaxBulkTransferSize = 16u * 1024u;
static const uint16_t kTrackIRProductIDs[] = {
  TRACKIR_USB_TIR5V3_PRODUCT_ID,
  TRACKIR_USB_TIR5V2_PRODUCT_ID,
};

static dev_found model_for_product_id(uint16_t product_id)
{
  switch (product_id) {
    case TRACKIR_USB_TIR5V2_PRODUCT_ID:
      return TIR5V2;
    case TRACKIR_USB_TIR5V3_PRODUCT_ID:
      return TIR5V3;
    default:
      return NOT_TIR;
  }
}

static const char *model_name(dev_found model)
{
  switch (model) {
    case TIR5V2:
      return "TIR5V2";
    case TIR5V3:
      return "TIR5V3";
    default:
      return "unknown TrackIR model";
  }
}

static void log_error(const char *operation, NSError *error)
{
  if (error == nil) {
    ltr_int_log_message("IOUSBHost %s failed\n", operation);
    return;
  }

  const char *description = error.localizedDescription.UTF8String;
  if (description == nullptr) {
    description = "unknown error";
  }
  ltr_int_log_message("IOUSBHost %s failed: %s (0x%08x)\n", operation,
                      description, (unsigned int)error.code);
}

static bool is_access_error(NSError *error)
{
  if (error == nil) {
    return false;
  }

  IOReturn code = (IOReturn)error.code;
  return code == kIOReturnNotPermitted || code == kIOReturnExclusiveAccess;
}

static const char *init_options_name(IOUSBHostObjectInitOptions options)
{
  if ((options & IOUSBHostObjectInitOptionsDeviceCapture) != 0) {
    return "DeviceCapture";
  }
  if ((options & IOUSBHostObjectInitOptionsDeviceSeize) != 0) {
    return "DeviceSeize";
  }
  return "ordinary ownership";
}

static CFMutableDictionaryRef create_matching_dictionary(uint16_t product_id)
{
  return [IOUSBHostInterface
      createMatchingDictionaryWithVendorID:@(TRACKIR_USB_VENDOR_ID)
                                 productID:@(product_id)
                                  bcdDevice:nil
                            interfaceNumber:@(TRACKIR_USB_INTERFACE_NUMBER)
                         configurationValue:@(TRACKIR_USB_CONFIGURATION_VALUE)
                            interfaceClass:nil
                         interfaceSubclass:nil
                          interfaceProtocol:nil
                                      speed:nil
                             productIDArray:nil];
}

static bool open_matching_interface(IOUSBHostObjectInitOptions options,
                                    uint16_t product_id,
                                    bool *saw_service, bool *access_error)
{
  if (saw_service != nullptr) {
    *saw_service = false;
  }
  if (access_error != nullptr) {
    *access_error = false;
  }

  CFMutableDictionaryRef matching = create_matching_dictionary(product_id);
  if (matching == nullptr) {
    ltr_int_log_message("Could not create the IOUSBHost matching dictionary.\n");
    return false;
  }

  io_iterator_t iterator = IO_OBJECT_NULL;
  kern_return_t result =
      IOServiceGetMatchingServices(MACH_PORT_NULL, matching, &iterator);
  if (result != KERN_SUCCESS) {
    ltr_int_log_message("IOUSBHost service matching failed: 0x%08x\n",
                        (unsigned int)result);
    return false;
  }

  ltr_int_log_message("Trying IOUSBHost %s for %04x:%04x.\n",
                      init_options_name(options), TRACKIR_USB_VENDOR_ID,
                      product_id);
  io_service_t service = IO_OBJECT_NULL;
  while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
    if (saw_service != nullptr) {
      *saw_service = true;
    }

    NSError *error = nil;
    TrackIRUSBTransport *candidate =
        [[TrackIRUSBTransport alloc] initWithIOService:service
                                               options:options
                                                 error:&error];
    IOObjectRelease(service);

    if (candidate != nil && candidate.open) {
      trackir_transport = candidate;
      ltr_int_log_message(
          "TrackIR interface opened through IOUSBHost: %04x:%04x, "
          "configuration %u, interface %u, bulk OUT 0x%02x, bulk IN 0x%02x.\n",
          trackir_transport.vendorID, trackir_transport.productID,
          trackir_transport.configurationValue,
          trackir_transport.interfaceNumber,
          trackir_transport.bulkOutEndpointAddress,
          trackir_transport.bulkInEndpointAddress);
      IOObjectRelease(iterator);
      return true;
    }

    if (is_access_error(error) && access_error != nullptr) {
      *access_error = true;
    }
    log_error("opening the TrackIR interface", error);
  }

  IOObjectRelease(iterator);
  return false;
}

/* LinuxTrack prefers the newest TrackIR 5 revision. Keep that ordering on
 * macOS, and do not silently fall back to an older device when a higher
 * priority matching interface is present but cannot be opened. This preserves
 * a useful permission error for the device the caller actually needs. */
static bool open_preferred_matching_interface(
    IOUSBHostObjectInitOptions options, dev_found *model, bool *saw_service,
    bool *access_error)
{
  if (model != nullptr) {
    *model = NOT_TIR;
  }
  if (saw_service != nullptr) {
    *saw_service = false;
  }
  if (access_error != nullptr) {
    *access_error = false;
  }

  for (uint16_t product_id : kTrackIRProductIDs) {
    bool product_saw_service = false;
    bool product_access_error = false;
    if (open_matching_interface(options, product_id, &product_saw_service,
                                &product_access_error)) {
      if (model != nullptr) {
        *model = model_for_product_id(trackir_transport.productID);
      }
      if (saw_service != nullptr) {
        *saw_service = true;
      }
      if (access_error != nullptr) {
        *access_error = product_access_error;
      }
      return true;
    }

    if (saw_service != nullptr) {
      *saw_service = *saw_service || product_saw_service;
    }
    if (access_error != nullptr) {
      *access_error = *access_error || product_access_error;
    }

    if (product_saw_service) {
      if (model != nullptr) {
        *model = model_for_product_id(product_id);
      }
      return false;
    }
  }
  return false;
}

static bool valid_endpoint(int endpoint, uint8_t expected, const char *direction)
{
  if (endpoint < 0 || endpoint > UINT8_MAX || endpoint != expected) {
    ltr_int_log_message("IOUSBHost %s endpoint 0x%x does not match 0x%02x\n",
                        direction, endpoint, expected);
    return false;
  }
  return true;
}

static bool valid_bulk_size(size_t size, const char *direction)
{
  if (size > kMaxBulkTransferSize) {
    ltr_int_log_message("IOUSBHost %s transfer is too large: %zu bytes\n",
                        direction, size);
    return false;
  }
  return true;
}

static bool valid_timeout(long timeout, NSTimeInterval *seconds)
{
  if (timeout < 0 || seconds == nullptr) {
    return false;
  }

  /* Match libusb_ifc.c: a zero timeout means its normal 500 ms timeout. */
  long effective_timeout = timeout == 0 ? 500 : timeout;
  *seconds = (NSTimeInterval)effective_timeout / 1000.0;
  return true;
}

extern "C" {

bool ltr_int_init_usb(void)
{
  @autoreleasepool {
    if (usb_initialized) {
      ltr_int_log_message("IOUSBHost backend already initialized.\n");
      return true;
    }

    /* A failed previous open can leave an object only if the caller did not
     * reach finish_usb.  Release that state before starting a new discovery. */
    if (trackir_transport != nil) {
      [trackir_transport destroy];
      trackir_transport = nil;
    }

    usb_initialized = true;
    ltr_int_log_message(
        "Initializing IOUSBHost backend; ordinary ownership is attempted "
        "before DeviceCapture.\n");
    return true;
  }
}

dev_found ltr_int_find_tir(void)
{
  @autoreleasepool {
    if (!usb_initialized) {
      ltr_int_log_message("IOUSBHost discovery requested before init.\n");
      return NOT_TIR;
    }

    if (trackir_transport != nil && trackir_transport.open) {
      dev_found model = model_for_product_id(trackir_transport.productID);
      if (model != NOT_TIR) {
        ltr_int_log_message(
            "Reusing the open IOUSBHost %s interface.\n", model_name(model));
        return model;
      }
      ltr_int_log_message(
          "The open IOUSBHost interface has an unsupported TrackIR product "
          "ID: %04x.\n", trackir_transport.productID);
      return NOT_TIR;
    }

    dev_found model = NOT_TIR;
    bool saw_service = false;
    bool access_denied = false;
    if (open_preferred_matching_interface(IOUSBHostObjectInitOptionsNone,
                                          &model, &saw_service,
                                          &access_denied)) {
      return model;
    }

    if (!saw_service) {
      ltr_int_log_message(
          "No supported TrackIR 5 IOUSBHost interface was found "
          "(TIR5V3/TIR5V2).\n");
      return NOT_TIR;
    }

    ltr_int_log_message(
        "Ordinary IOUSBHost ownership failed for %s%s; retrying with "
        "DeviceCapture.\n",
        model_name(model),
        access_denied ? " because the interface is already owned or not permitted"
                      : "");
    dev_found capture_model = NOT_TIR;
    if (open_preferred_matching_interface(
            IOUSBHostObjectInitOptionsDeviceCapture, &capture_model, nullptr,
            nullptr)) {
      return capture_model;
    }
    if (model == NOT_TIR) {
      model = capture_model;
    }

    if (geteuid() != 0) {
      ltr_int_log_message(
          "IOUSBHost DeviceCapture for %s could not be authorized. Quit and "
          "run 'sudo ltr_server1' if the device is still owned by another "
          "client.\n", model_name(model));
      return model == NOT_TIR ? NOT_TIR : (dev_found)(model | NOT_PERMITTED);
    }

    ltr_int_log_message(
        "IOUSBHost DeviceCapture for %s failed even with root; check the "
        "device and its current USB owner.\n", model_name(model));
    return NOT_TIR;
  }
}

bool ltr_int_reset_device(void)
{
  @autoreleasepool {
    if (trackir_transport == nil || !trackir_transport.open) {
      ltr_int_log_message("IOUSBHost reset requested without an open device.\n");
      return false;
    }

    /* IOUSBHostDevice reset terminates the interface and requires a fresh
     * service/object after re-enumeration.  The legacy callback has no way to
     * return that replacement context, so do not issue a reset through an
     * interface that the protocol engine is about to use. */
    ltr_int_log_message(
        "IOUSBHost reset requested; reset/reopen is not available through the "
        "legacy callback yet. Continuing without reset.\n");
    return false;
  }
}

bool ltr_int_prepare_device(unsigned int config, unsigned int interface_number)
{
  @autoreleasepool {
    if (trackir_transport == nil || !trackir_transport.open) {
      ltr_int_log_message("IOUSBHost prepare requested without an open device.\n");
      return false;
    }
    if (config != trackir_transport.configurationValue ||
        interface_number != trackir_transport.interfaceNumber ||
        trackir_transport.alternateSetting != TRACKIR_USB_ALTERNATE_SETTING) {
      ltr_int_log_message(
          "IOUSBHost prepare identity mismatch: config %u/%u, interface %u/%u, "
          "alternate %u/%u.\n",
          config, trackir_transport.configurationValue, interface_number,
          trackir_transport.interfaceNumber, trackir_transport.alternateSetting,
          (unsigned int)TRACKIR_USB_ALTERNATE_SETTING);
      return false;
    }

    /* IOUSBHostInterface was opened against the requested configuration and
     * already owns the interface; there is no separate claim operation. */
    ltr_int_log_message("IOUSBHost TrackIR interface prepared.\n");
    return true;
  }
}

bool ltr_int_send_data(int out_ep, unsigned char data[], size_t size)
{
  @autoreleasepool {
    if (trackir_transport == nil || !trackir_transport.open ||
        !valid_endpoint(out_ep, trackir_transport.bulkOutEndpointAddress,
                        "bulk OUT") ||
        !valid_bulk_size(size, "bulk OUT") || (size > 0 && data == nullptr)) {
      return false;
    }

    NSData *payload = [NSData dataWithBytes:data length:size];
    NSUInteger transferred = 0;
    NSError *error = nil;
    BOOL success = [trackir_transport sendBulkData:payload
                                   bytesTransferred:&transferred
                                             timeout:0.5
                                              error:&error];
    if (!success) {
      log_error("bulk OUT", error);
      return false;
    }
    if (transferred != size) {
      ltr_int_log_message(
          "IOUSBHost bulk OUT completed short: %lu/%zu bytes\n",
          (unsigned long)transferred, size);
      return false;
    }
    return true;
  }
}

bool ltr_int_receive_data(int in_ep, unsigned char data[], size_t size,
                          size_t *transferred, long timeout)
{
  @autoreleasepool {
    if (transferred != nullptr) {
      *transferred = 0;
    }

    NSTimeInterval timeout_seconds = 0.0;
    if (transferred == nullptr || trackir_transport == nil ||
        !trackir_transport.open ||
        !valid_endpoint(in_ep, trackir_transport.bulkInEndpointAddress,
                        "bulk IN") ||
        !valid_bulk_size(size, "bulk IN") || (size > 0 && data == nullptr) ||
        !valid_timeout(timeout, &timeout_seconds)) {
      return false;
    }

    NSMutableData *payload = [NSMutableData dataWithLength:size];
    NSUInteger received = 0;
    NSError *error = nil;
    BOOL success = [trackir_transport receiveBulkData:payload
                                      bytesTransferred:&received
                                                timeout:timeout_seconds
                                                 error:&error];

    size_t copied = received > size ? size : (size_t)received;
    if (copied > 0) {
      memcpy(data, payload.bytes, copied);
    }
    *transferred = copied;

    /* libusb_ifc.c treats a read timeout as a successful zero-byte poll. */
    if (!success && error != nil && (IOReturn)error.code == kIOReturnTimeout) {
      return true;
    }
    if (!success) {
      log_error("bulk IN", error);
      return false;
    }
    return true;
  }
}

bool ltr_int_ctrl_data(uint8_t req_type, uint8_t req, uint16_t val,
                       uint16_t index, unsigned char data[], size_t size)
{
  @autoreleasepool {
    if (trackir_transport == nil || !trackir_transport.open ||
        size > UINT16_MAX || size > kMaxBulkTransferSize ||
        (size > 0 && data == nullptr)) {
      return false;
    }

    IOUSBDeviceRequest request = {};
    request.bmRequestType = req_type;
    request.bRequest = req;
    request.wValue = val;
    request.wIndex = index;
    request.wLength = (uint16_t)size;

    NSMutableData *payload = [NSMutableData dataWithLength:size];
    if (size > 0 && (req_type & 0x80u) == 0) {
      memcpy(payload.mutableBytes, data, size);
    }

    NSUInteger transferred = 0;
    NSError *error = nil;
    BOOL success = [trackir_transport sendControlRequest:request
                                                    data:payload
                                        bytesTransferred:&transferred
                                                  timeout:0.5
                                                   error:&error];
    if (!success) {
      log_error("control transfer", error);
      return false;
    }

    if (size > 0 && (req_type & 0x80u) != 0) {
      size_t copied = transferred > size ? size : (size_t)transferred;
      memcpy(data, payload.bytes, copied);
    }
    return true;
  }
}

void ltr_int_finish_usb(unsigned int interface_number)
{
  @autoreleasepool {
    (void)interface_number;
    if (trackir_transport != nil) {
      ltr_int_log_message("Closing the IOUSBHost TrackIR interface.\n");
      [trackir_transport destroy];
      trackir_transport = nil;
    }
    usb_initialized = false;
  }
}

} /* extern "C" */
