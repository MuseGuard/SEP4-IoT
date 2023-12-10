#include "connection_control.h"
#include "includes.h"
#include "pc_comm.h"
#include "request_interpreter.h"
#include "wifi.h"

// remove
#include "security_system_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char buffer[25];
static server_callback_t application_callback_function;

void connection_control_callback() {
  application_callback_function(buffer);
}

bool connection_control_init(server_callback_t callback) {
  _delay_ms(3000);
  bool result = false;
  application_callback_function = callback;
  wifi_init();

  WIFI_ERROR_MESSAGE_t connect_to_AP =
        wifi_command_join_AP("madinnit", "12345678");
  
  if (connect_to_AP == WIFI_OK) {

    pc_comm_send_string_blocking("Connected to AP!\n");

    WIFI_ERROR_MESSAGE_t connect_to_server = wifi_command_create_TCP_connection(
        "34.88.237.151", 23, connection_control_callback, buffer);

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
}

bool connection_control_send_message(char *message) {
  WIFI_ERROR_MESSAGE_t result =
      wifi_command_TCP_transmit((uint8_t *)message, strlen(message));
  return result == WIFI_OK;
}
