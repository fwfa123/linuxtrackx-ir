#ifndef TRACKIR_USERSPACE_USB_TRANSPORT_H
#define TRACKIR_USERSPACE_USB_TRANSPORT_H

#import <Foundation/Foundation.h>
#import <IOUSBHost/IOUSBHost.h>

NS_ASSUME_NONNULL_BEGIN

@interface TrackIRUSBTransport : NSObject

- (nullable instancetype)initWithIOService:(io_service_t)service
                                    options:(IOUSBHostObjectInitOptions)options
                                      error:(NSError * _Nullable * _Nullable)error;

@property(nonatomic, readonly) BOOL open;
@property(nonatomic, readonly) uint16_t vendorID;
@property(nonatomic, readonly) uint16_t productID;
@property(nonatomic, readonly) uint8_t configurationValue;
@property(nonatomic, readonly) uint8_t interfaceNumber;
@property(nonatomic, readonly) uint8_t alternateSetting;
@property(nonatomic, readonly) uint8_t interfaceClass;
@property(nonatomic, readonly) uint8_t interfaceSubclass;
@property(nonatomic, readonly) uint8_t interfaceProtocol;
@property(nonatomic, readonly) NSUInteger endpointCount;
@property(nonatomic, readonly) uint8_t bulkInEndpointAddress;
@property(nonatomic, readonly) uint8_t bulkInAttributes;
@property(nonatomic, readonly) uint16_t bulkInMaxPacketSize;
@property(nonatomic, readonly) uint8_t bulkInInterval;
@property(nonatomic, readonly) uint8_t bulkOutEndpointAddress;
@property(nonatomic, readonly) uint8_t bulkOutAttributes;
@property(nonatomic, readonly) uint16_t bulkOutMaxPacketSize;
@property(nonatomic, readonly) uint8_t bulkOutInterval;

- (BOOL)sendBulkData:(NSData *)data
    bytesTransferred:(NSUInteger * _Nullable)bytesTransferred
              timeout:(NSTimeInterval)timeout
               error:(NSError * _Nullable * _Nullable)error;

- (BOOL)receiveBulkData:(NSMutableData *)data
       bytesTransferred:(NSUInteger * _Nullable)bytesTransferred
                 timeout:(NSTimeInterval)timeout
                  error:(NSError * _Nullable * _Nullable)error;

- (BOOL)sendControlRequest:(IOUSBDeviceRequest)request
                       data:(NSMutableData * _Nullable)data
           bytesTransferred:(NSUInteger * _Nullable)bytesTransferred
                     timeout:(NSTimeInterval)timeout
                       error:(NSError * _Nullable * _Nullable)error;

/* Destroy the IOUSBHost object and release captured ownership. */
- (void)destroy;

@end

NS_ASSUME_NONNULL_END

#endif /* TRACKIR_USERSPACE_USB_TRANSPORT_H */
