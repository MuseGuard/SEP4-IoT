#include "connection_controller.h"
#include "pc_comm.h"
#include "wifi.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

bool connection_controller_init(void) {
  _delay_ms(3000);
  bool result = false;
  wifi_init();

  WIFI_ERROR_MESSAGE_t connect_to_AP =
      wifi_command_join_AP("madinnit", "12345678");

  if (connect_to_AP == WIFI_OK) {
    pc_comm_send_string_blocking("Connected to AP!\n");
    WIFI_ERROR_MESSAGE_t connect_to_server =
        wifi_command_create_TCP_connection("192.168.214.218", 23, NULL, NULL);
    if (connect_to_server == WIFI_OK) {
      pc_comm_send_string_blocking("Connected to server!\n");
      result = true;
    } else {
      result = false;
      pc_comm_send_string_blocking("Error connecting to server!\n");
    }

  } else {
    result = false;
    pc_comm_send_string_blocking("Error connecting to Wifi AP!\n");
  }

  return result;

  /*   wifi_command_join_AP("XR", "12345678");
    wifi_command_create_TCP_connection("192.168.90.98", 23, NULL, NULL); */
}
bool connection_controller_transmit(char *package, int length) {
  wifi_command_TCP_transmit((uint8_t *)package, length);
}
