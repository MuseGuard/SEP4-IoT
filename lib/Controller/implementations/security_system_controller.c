// #ifndef WINDOWS_TEST
#include "security_system_controller.h"
#include "buttons_controller.h"
#include "buzzer.h"
#include "connection_controller.h"
#include "display.h"
#include "display_controller.h"
#include "includes.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "pir.h"
#include "wifi.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t pin_code[4] = {1, 2, 3, 4};
static bool status = false;

// Send notification to the server when motion is detected
void security_system_controller_send_notification() {
  if (status) {
    sei();
    buzzer_beep();
    Package package = package_builder_build_motion_detected();
    connection_controller_transmit(package);
    cli();
  }
}

void security_system_controller_activate() {
  int i = 10;

  while (i != 0) {
    display_int(i);
    _delay_ms(1000);
    i--;
  }

  pir_init(security_system_controller_send_notification);

  Package package = package_builder_build_acknowledgement("PIR Activated");
  connection_controller_transmit(package);
  pc_comm_send_string_blocking("PIR Activated\n");
};

bool security_system_controller_check_pin_code(uint8_t *input_code) {
  bool areEqual = false;
  char str[20];
  sprintf(str, "Expected %d%d%d%d\n", pin_code[0], pin_code[1], pin_code[2],
          pin_code[3]);
  pc_comm_send_string_blocking(str);
  for (int i = 0; i < 4; i++) {
    char str[20];
    sprintf(str, "Expected %d, but was %d\n", pin_code[i], input_code[i]);
    pc_comm_send_string_blocking(str);

    if (pin_code[i] == input_code[i]) {
      areEqual = true;
    } else {
      areEqual = false;
      break;
    }
  }
  return areEqual;
}

void security_system_controller_evaluate() {

  uint8_t *input = buttons_control_pin_code_input();
  bool areEqual = security_system_controller_check_pin_code(input);
  char str[20];
  sprintf(str, "Are equal: %d\n", areEqual);
  pc_comm_send_string_blocking(str);
  if (areEqual) {
    security_system_controller_toggle_status();
  } else {
    pc_comm_send_string_blocking("Err\n");
    display_controller_write_word("Err");
    wifi_command_TCP_transmit((uint8_t *)"Err\n", 5);
    free(input);
    security_system_controller_evaluate();
  }

  free(input);
  pc_comm_send_string_blocking("Security system controller: evaluate\n");
}

void security_system_controller_toggle_status() {
  status = !status; // toggle the status
  if (status) {
    // wifi_command_TCP_transmit((uint8_t *)"Unlocked\n", 10);
    pc_comm_send_string_blocking("Unlocked\n");
    // security_system_controller_activate();
  } else {
    // wifi_command_TCP_transmit((uint8_t *)"Locked\n", 8);
    pc_comm_send_string_blocking("Locked\n");
    /* Package package = package_builder_build_acknowledgement("PIR
    Deactivated"); connection_controller_transmit(package); */
  }
}

void securiy_system_controller_change_pin_code(uint8_t *new_pin) {
  pc_comm_send_string_blocking("Entered func\n");
  if (new_pin == NULL) {
    pc_comm_send_string_blocking("Entered if\n");
    return;
  }

  memcpy(&pin_code, new_pin, 4);
  free(new_pin);
  char str[20];
  sprintf(str, "New pin code: %d%d%d%d\n", pin_code[0], pin_code[1],
          pin_code[2], pin_code[3]);
  pc_comm_send_string_blocking(str);
}

void security_system_controller_override_pin_code() {
  if (status) {
    _delay_ms(200);
    display_controller_write_word("Edit");
    _delay_ms(1000);
    uint8_t *new_pin_code = buttons_control_pin_code_input();
    pc_comm_send_string_blocking("Entered override mode\n");
    securiy_system_controller_change_pin_code(new_pin_code);
  } else {
    pc_comm_send_string_blocking("Unlock the device first\n");
    display_controller_write_word("Err");
    _delay_ms(1000);
  }
}
// #endif