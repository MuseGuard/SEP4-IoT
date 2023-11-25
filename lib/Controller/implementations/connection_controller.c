#include "connection_controller.h"
#include "includes.h"
#include "pc_comm.h"
#include "wifi.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "security_system_controller.h"

static char buffer[15];

void connection_controller_callbackFunc() {
  pc_comm_send_string_blocking(buffer);
  if (strcmp(buffer, "2iotplease") == 0) {
    security_system_control_remote_toggle();
    connection_controller_transmit((Package){.data = "4SecurityStatusChanged", .size = sizeof("4SecurityStatusChanged")});
  } else if (strcmp(buffer, "hello") == 0) {
    pc_comm_send_string_blocking("Hello from server!\n");
  } else if (strcmp(buffer, "ping") == 0) {
    pc_comm_send_string_blocking("Pong!\n");
  } else {
    pc_comm_send_string_blocking(buffer);
  }
}

bool connection_controller_init(void) {
  _delay_ms(3000);
  bool result = false;
  wifi_init();

  // Connecting to Rado's phone
  WIFI_ERROR_MESSAGE_t connect_to_AP =
      wifi_command_join_AP("madinnit", "12345678");

  // Connecting to Bozhidar's phone
  // WIFI_ERROR_MESSAGE_t connect_to_AP = wifi_command_join_AP("Xr",
  // "12345678");

  if (connect_to_AP == WIFI_OK) {

    pc_comm_send_string_blocking("Connected to AP!\n");
    WIFI_ERROR_MESSAGE_t connect_to_server = wifi_command_create_TCP_connection(
        "192.168.214.98", 23, connection_controller_callbackFunc, buffer);
    // wifi_command_create_TCP_connection("172.20.10.3", 23, NULL, NULL);
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

bool connection_controller_transmit(Package package) {
  wifi_command_TCP_transmit((uint8_t *)package.data, package.size);
  return true;
}
