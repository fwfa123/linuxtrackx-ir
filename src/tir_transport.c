#include "tir_transport.h"

#include <string.h>

static ltr_usb_legacy_api *legacy_api(void *context)
{
  return (ltr_usb_legacy_api *)context;
}

static bool legacy_init(void *context)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  return api != NULL && api->init != NULL && api->init();
}

static uint16_t product_id_for_model(dev_found model)
{
  switch(model){
    case TIR2: return 0x0150;
    case TIR3: return 0x0155;
    case TIR4: return 0x0156;
    case TIR5: return 0x0157;
    case TIR5V2: return 0x0158;
    case TIR5V3: return 0x0159;
    case SMARTNAV3: return 0x0105;
    case SMARTNAV4: return 0x0106;
    default: return 0;
  }
}

static dev_found legacy_find_tir(void *context, ltr_usb_device_info *info)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  if(api == NULL || api->find_tir == NULL){
    return NOT_TIR;
  }

  dev_found result = api->find_tir();
  if(info != NULL){
    memset(info, 0, sizeof(*info));
    info->model = result;
    info->permission_denied = (result & NOT_PERMITTED) != 0;

    dev_found model = result;
    if(info->permission_denied){
      model = (dev_found)(model ^ NOT_PERMITTED);
    }
    info->model = model;
    info->vendor_id = model == NOT_TIR ? 0 : 0x131D;
    info->product_id = product_id_for_model(model);
  }
  return result;
}

static bool legacy_reset(void *context)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  return api != NULL && api->reset != NULL && api->reset();
}

static bool legacy_prepare(void *context, unsigned int config,
                           unsigned int interface_number)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  return api != NULL && api->prepare != NULL &&
    api->prepare(config, interface_number);
}

static bool legacy_send(void *context, int endpoint, unsigned char data[], size_t size)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  return api != NULL && api->send != NULL && api->send(endpoint, data, size);
}

static bool legacy_receive(void *context, int endpoint, unsigned char data[],
                           size_t size, size_t *transferred, long timeout)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  if(api == NULL || api->receive == NULL){
    if(transferred != NULL){
      *transferred = 0;
    }
    return false;
  }
  return api->receive(endpoint, data, size, transferred, timeout);
}

static bool legacy_control(void *context, uint8_t request_type, uint8_t request,
                           uint16_t value, uint16_t index, unsigned char data[],
                           size_t size)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  return api != NULL && api->control != NULL &&
    api->control(request_type, request, value, index, data, size);
}

static void legacy_finish(void *context, unsigned int interface_number)
{
  ltr_usb_legacy_api *api = legacy_api(context);
  if(api != NULL && api->finish != NULL){
    api->finish(interface_number);
  }
}

void ltr_usb_transport_from_legacy(ltr_usb_transport *transport,
                                   ltr_usb_legacy_api *legacy)
{
  if(transport == NULL){
    return;
  }

  memset(transport, 0, sizeof(*transport));
  transport->abi_version = LTR_USB_TRANSPORT_ABI_VERSION;
  transport->context = legacy;
  transport->init = legacy_init;
  transport->find_tir = legacy_find_tir;
  transport->reset = legacy_reset;
  transport->prepare = legacy_prepare;
  transport->send = legacy_send;
  transport->receive = legacy_receive;
  transport->control = legacy_control;
  transport->finish = legacy_finish;
}

bool ltr_usb_transport_is_valid(const ltr_usb_transport *transport)
{
  return transport != NULL &&
    transport->abi_version == LTR_USB_TRANSPORT_ABI_VERSION &&
    transport->init != NULL &&
    transport->find_tir != NULL &&
    transport->reset != NULL &&
    transport->prepare != NULL &&
    transport->send != NULL &&
    transport->receive != NULL &&
    transport->finish != NULL;
}

bool ltr_usb_transport_init(const ltr_usb_transport *transport)
{
  return ltr_usb_transport_is_valid(transport) &&
    transport->init(transport->context);
}

dev_found ltr_usb_transport_find_tir(const ltr_usb_transport *transport,
                                     ltr_usb_device_info *info)
{
  if(!ltr_usb_transport_is_valid(transport)){
    return NOT_TIR;
  }
  return transport->find_tir(transport->context, info);
}

bool ltr_usb_transport_reset(const ltr_usb_transport *transport)
{
  return ltr_usb_transport_is_valid(transport) &&
    transport->reset(transport->context);
}

bool ltr_usb_transport_prepare(const ltr_usb_transport *transport,
                               unsigned int config, unsigned int interface_number)
{
  return ltr_usb_transport_is_valid(transport) &&
    transport->prepare(transport->context, config, interface_number);
}

bool ltr_usb_transport_send(const ltr_usb_transport *transport, int endpoint,
                            unsigned char data[], size_t size)
{
  return ltr_usb_transport_is_valid(transport) && data != NULL &&
    transport->send(transport->context, endpoint, data, size);
}

bool ltr_usb_transport_receive(const ltr_usb_transport *transport, int endpoint,
                               unsigned char data[], size_t size,
                               size_t *transferred, long timeout)
{
  if(transferred != NULL){
    *transferred = 0;
  }
  if(!ltr_usb_transport_is_valid(transport) || data == NULL ||
     transferred == NULL){
    return false;
  }
  return transport->receive(transport->context, endpoint, data, size,
                            transferred, timeout);
}

bool ltr_usb_transport_control(const ltr_usb_transport *transport,
                               uint8_t request_type, uint8_t request,
                               uint16_t value, uint16_t index,
                               unsigned char data[], size_t size)
{
  return ltr_usb_transport_is_valid(transport) && transport->control != NULL &&
    transport->control(transport->context, request_type, request, value,
                       index, data, size);
}

void ltr_usb_transport_finish(const ltr_usb_transport *transport,
                              unsigned int interface_number)
{
  if(ltr_usb_transport_is_valid(transport)){
    transport->finish(transport->context, interface_number);
  }
}
