#import "TrackIRUSBTransport.h"

#import <IOKit/IOKitLib.h>

#include <stdio.h>

#include "TrackIRUSB.h"

static NSError *transport_error(IOReturn code, NSString *description)
{
  return [NSError errorWithDomain:IOUSBHostErrorDomain
                              code:code
                          userInfo:@{
                            NSLocalizedDescriptionKey: description,
                          }];
}

static void set_transport_error(NSError **error, IOReturn code,
                                NSString *description)
{
  if (error != nullptr) {
    *error = transport_error(code, description);
  }
}

@interface TrackIRUSBTransport ()

@property(nonatomic, strong) IOUSBHostInterface *hostInterface;
@property(nonatomic, strong) IOUSBHostPipe *bulkInPipe;
@property(nonatomic, strong) IOUSBHostPipe *bulkOutPipe;

@property(nonatomic, readwrite) uint16_t vendorID;
@property(nonatomic, readwrite) uint16_t productID;
@property(nonatomic, readwrite) uint8_t configurationValue;
@property(nonatomic, readwrite) uint8_t interfaceNumber;
@property(nonatomic, readwrite) uint8_t alternateSetting;
@property(nonatomic, readwrite) uint8_t interfaceClass;
@property(nonatomic, readwrite) uint8_t interfaceSubclass;
@property(nonatomic, readwrite) uint8_t interfaceProtocol;
@property(nonatomic, readwrite) NSUInteger endpointCount;
@property(nonatomic, readwrite) uint8_t bulkInEndpointAddress;
@property(nonatomic, readwrite) uint8_t bulkInAttributes;
@property(nonatomic, readwrite) uint16_t bulkInMaxPacketSize;
@property(nonatomic, readwrite) uint8_t bulkInInterval;
@property(nonatomic, readwrite) uint8_t bulkOutEndpointAddress;
@property(nonatomic, readwrite) uint8_t bulkOutAttributes;
@property(nonatomic, readwrite) uint16_t bulkOutMaxPacketSize;
@property(nonatomic, readwrite) uint8_t bulkOutInterval;

@end

@implementation TrackIRUSBTransport

- (nullable instancetype)initWithIOService:(io_service_t)service
                                    options:(IOUSBHostObjectInitOptions)options
                                      error:(NSError **)error
{
  self = [super init];
  if (self == nil) {
    return nil;
  }

  NSError *initError = nil;
  IOUSBHostInterface *interface =
      [[IOUSBHostInterface alloc] initWithIOService:service
                                            options:options
                                              queue:nil
                                              error:&initError
                                    interestHandler:^(IOUSBHostObject *,
                                                      uint32_t messageType,
                                                      void *) {
    fprintf(stderr, "TrackIR: IOUSBHost interest message 0x%08x\n",
            messageType);
  }];
  if (interface == nil) {
    if (error != nullptr) {
      *error = initError;
    }
    return nil;
  }

  const IOUSBDeviceDescriptor *deviceDescriptor = interface.deviceDescriptor;
  const IOUSBConfigurationDescriptor *configurationDescriptor =
      interface.configurationDescriptor;
  const IOUSBInterfaceDescriptor *interfaceDescriptor =
      interface.interfaceDescriptor;
  if (deviceDescriptor == nullptr || configurationDescriptor == nullptr ||
      interfaceDescriptor == nullptr) {
    [interface destroy];
    set_transport_error(error, kIOReturnNotFound,
                        @"USB descriptors are unavailable");
    return nil;
  }

  if (deviceDescriptor->idVendor != TRACKIR_USB_VENDOR_ID ||
      !TRACKIR_USB_IS_SUPPORTED_PRODUCT_ID(deviceDescriptor->idProduct) ||
      configurationDescriptor->bConfigurationValue !=
          TRACKIR_USB_CONFIGURATION_VALUE ||
      interfaceDescriptor->bInterfaceNumber != TRACKIR_USB_INTERFACE_NUMBER ||
      interfaceDescriptor->bAlternateSetting != TRACKIR_USB_ALTERNATE_SETTING) {
    [interface destroy];
    set_transport_error(error, kIOReturnBadArgument,
                        @"matched interface has an unexpected identity");
    return nil;
  }

  IOUSBHostPipe *bulkInPipe = nil;
  IOUSBHostPipe *bulkOutPipe = nil;
  uint8_t bulkInAttributes = 0;
  uint16_t bulkInMaxPacketSize = 0;
  uint8_t bulkInInterval = 0;
  uint8_t bulkOutAttributes = 0;
  uint16_t bulkOutMaxPacketSize = 0;
  uint8_t bulkOutInterval = 0;
  NSUInteger endpointCount = 0;

  const IOUSBDescriptorHeader *current = nullptr;
  while (true) {
    const IOUSBEndpointDescriptor *endpoint = IOUSBGetNextEndpointDescriptor(
        configurationDescriptor, interfaceDescriptor, current);
    if (endpoint == nullptr) {
      break;
    }
    current = reinterpret_cast<const IOUSBDescriptorHeader *>(endpoint);
    ++endpointCount;

    NSError *pipeError = nil;
    IOUSBHostPipe *pipe = [interface copyPipeWithAddress:
                                      endpoint->bEndpointAddress
                                                 error:&pipeError];
    if (pipe == nil) {
      [interface destroy];
      if (error != nullptr) {
        *error = pipeError;
      }
      return nil;
    }

    if ((endpoint->bmAttributes & 0x03u) != 0x02u) {
      continue;
    }
    if ((endpoint->bEndpointAddress & 0x80u) != 0) {
      if (bulkInPipe == nil) {
        bulkInPipe = pipe;
        bulkInAttributes = endpoint->bmAttributes;
        bulkInMaxPacketSize = endpoint->wMaxPacketSize;
        bulkInInterval = endpoint->bInterval;
      }
    } else if (bulkOutPipe == nil) {
      bulkOutPipe = pipe;
      bulkOutAttributes = endpoint->bmAttributes;
      bulkOutMaxPacketSize = endpoint->wMaxPacketSize;
      bulkOutInterval = endpoint->bInterval;
    }
  }

  if (bulkInPipe == nil || bulkOutPipe == nil) {
    [interface destroy];
    set_transport_error(error, kIOReturnNotFound,
                        @"matched interface has no bulk IN and OUT pair");
    return nil;
  }

  self.hostInterface = interface;
  self.bulkInPipe = bulkInPipe;
  self.bulkOutPipe = bulkOutPipe;
  self.vendorID = deviceDescriptor->idVendor;
  self.productID = deviceDescriptor->idProduct;
  self.configurationValue = configurationDescriptor->bConfigurationValue;
  self.interfaceNumber = interfaceDescriptor->bInterfaceNumber;
  self.alternateSetting = interfaceDescriptor->bAlternateSetting;
  self.interfaceClass = interfaceDescriptor->bInterfaceClass;
  self.interfaceSubclass = interfaceDescriptor->bInterfaceSubClass;
  self.interfaceProtocol = interfaceDescriptor->bInterfaceProtocol;
  self.endpointCount = endpointCount;
  self.bulkInEndpointAddress = bulkInPipe.endpointAddress;
  self.bulkInAttributes = bulkInAttributes;
  self.bulkInMaxPacketSize = bulkInMaxPacketSize;
  self.bulkInInterval = bulkInInterval;
  self.bulkOutEndpointAddress = bulkOutPipe.endpointAddress;
  self.bulkOutAttributes = bulkOutAttributes;
  self.bulkOutMaxPacketSize = bulkOutMaxPacketSize;
  self.bulkOutInterval = bulkOutInterval;
  return self;
}

- (BOOL)open
{
  return self.hostInterface != nil && self.bulkInPipe != nil &&
         self.bulkOutPipe != nil;
}

- (BOOL)ensureOpen:(NSError **)error
{
  if (self.open) {
    return YES;
  }
  set_transport_error(error, kIOReturnNotOpen,
                      @"TrackIR IOUSBHost transport is closed");
  return NO;
}

- (BOOL)sendBulkData:(NSData *)data
    bytesTransferred:(NSUInteger *)bytesTransferred
              timeout:(NSTimeInterval)timeout
               error:(NSError **)error
{
  if (![self ensureOpen:error]) {
    return NO;
  }
  NSMutableData *mutableData = [data mutableCopy];
  return [self.bulkOutPipe sendIORequestWithData:mutableData
                                  bytesTransferred:bytesTransferred
                                 completionTimeout:timeout
                                             error:error];
}

- (BOOL)receiveBulkData:(NSMutableData *)data
       bytesTransferred:(NSUInteger *)bytesTransferred
                 timeout:(NSTimeInterval)timeout
                  error:(NSError **)error
{
  if (![self ensureOpen:error]) {
    return NO;
  }
  return [self.bulkInPipe sendIORequestWithData:data
                                 bytesTransferred:bytesTransferred
                                completionTimeout:timeout
                                            error:error];
}

- (BOOL)sendControlRequest:(IOUSBDeviceRequest)request
                       data:(NSMutableData *)data
           bytesTransferred:(NSUInteger *)bytesTransferred
                     timeout:(NSTimeInterval)timeout
                       error:(NSError **)error
{
  if (![self ensureOpen:error]) {
    return NO;
  }
  return [self.hostInterface sendDeviceRequest:request
                                           data:data
                               bytesTransferred:bytesTransferred
                              completionTimeout:timeout
                                           error:error];
}

- (void)destroy
{
  if (self.hostInterface != nil) {
    [self.hostInterface destroy];
  }
  self.bulkInPipe = nil;
  self.bulkOutPipe = nil;
  self.hostInterface = nil;
}

- (void)dealloc
{
  [self destroy];
}

@end
