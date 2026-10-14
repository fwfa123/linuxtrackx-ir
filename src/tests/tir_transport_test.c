#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "tir_transport.h"

static bool initialized;
static bool reset;
static bool prepared;
static bool finished;
static int last_endpoint;
static unsigned int last_config;
static unsigned int last_interface;

static bool fake_init(void)
{
  initialized = true;
  return true;
}

static dev_found fake_find_tir(void)
{
  return TIR5V2;
}

static bool fake_reset(void)
{
  reset = true;
  return true;
}

static bool fake_prepare(unsigned int config, unsigned int interface_number)
{
  prepared = true;
  last_config = config;
  last_interface = interface_number;
  return true;
}

static bool fake_send(int endpoint, unsigned char data[], size_t size)
{
  last_endpoint = endpoint;
  return size == 2 && data[0] == 0x12 && data[1] == 0x34;
}

static bool fake_receive(int endpoint, unsigned char data[], size_t size,
                         size_t *transferred, long timeout)
{
  (void)timeout;
  last_endpoint = endpoint;
  if(size < 2){
    *transferred = 0;
    return false;
  }
  data[0] = 0x56;
  data[1] = 0x78;
  *transferred = 2;
  return true;
}

static bool fake_control(uint8_t request_type, uint8_t request, uint16_t value,
                         uint16_t index, unsigned char data[], size_t size)
{
  return request_type == 0x40 && request == 1 && value == 2 && index == 3 &&
    size == 1 && data[0] == 4;
}

static void fake_finish(unsigned int interface_number)
{
  finished = true;
  last_interface = interface_number;
}

int main(void)
{
  ltr_usb_legacy_api legacy = {
    .init = fake_init,
    .find_tir = fake_find_tir,
    .reset = fake_reset,
    .prepare = fake_prepare,
    .send = fake_send,
    .receive = fake_receive,
    .control = fake_control,
    .finish = fake_finish
  };
  ltr_usb_transport transport;
  ltr_usb_transport_from_legacy(&transport, &legacy);

  assert(ltr_usb_transport_is_valid(&transport));
  assert(ltr_usb_transport_init(&transport));
  assert(initialized);

  ltr_usb_device_info info;
  assert(ltr_usb_transport_find_tir(&transport, &info) == TIR5V2);
  assert(info.vendor_id == 0x131D);
  assert(info.product_id == 0x0158);
  assert(info.model == TIR5V2);
  assert(!info.permission_denied);

  assert(ltr_usb_transport_reset(&transport));
  assert(reset);
  assert(ltr_usb_transport_prepare(&transport, 1, 0));
  assert(prepared && last_config == 1 && last_interface == 0);

  unsigned char outgoing[] = {0x12, 0x34};
  assert(ltr_usb_transport_send(&transport, 0x01, outgoing, sizeof(outgoing)));
  assert(last_endpoint == 0x01);

  unsigned char incoming[2];
  size_t transferred = 99;
  assert(ltr_usb_transport_receive(&transport, 0x82, incoming, sizeof(incoming),
                                   &transferred, 500));
  assert(last_endpoint == 0x82 && transferred == 2);
  assert(incoming[0] == 0x56 && incoming[1] == 0x78);

  unsigned char control_data[] = {4};
  assert(ltr_usb_transport_control(&transport, 0x40, 1, 2, 3,
                                   control_data, sizeof(control_data)));

  ltr_usb_transport_finish(&transport, 0);
  assert(finished && last_interface == 0);
  printf("test_tir_transport passed\n");
  return 0;
}
