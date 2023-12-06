#include "connection_control.h"
#include "includes.h"
#include "request_interpreter.h"
#include "security_system_control.h"
#include "wifi.h"

static char buffer[25];

// TODO APPLICATION
void connection_control_callbackFunc() {
  pc_comm_send_string_blocking(buffer);
  pc_comm_send_string_blocking("\n");
  if (strcmp(buffer, "ChangeSecurityStatus") == 0) {
    char *message = security_system_control_toggle_status(true);
    pc_comm_send_string_blocking(message);
    connection_control_send_message(message);
    free(message);
  } else if (strstr(buffer, "ChangePIN") == buffer) {
    uint8_t *pin_code = request_interpreter_get_pin(buffer);
    security_system_control_change_pin_code(pin_code);
    free(pin_code);
  } else {
    connection_control_send_message("Invalid command!");
  }
}

// TODO add function as argument
bool connection_control_init(void) {
  _delay_ms(3000);
  bool result = false;
  wifi_init();

  WIFI_ERROR_MESSAGE_t connect_to_AP =
      wifi_command_join_AP("madinnit", "12345678");

  if (connect_to_AP == WIFI_OK) {

    pc_comm_send_string_blocking("Connected to AP!\n");

    WIFI_ERROR_MESSAGE_t connect_to_server = wifi_command_create_TCP_connection(
        "192.168.214.98", 23, connection_control_callbackFunc, buffer);

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
