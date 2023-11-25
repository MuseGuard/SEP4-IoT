// #ifndef WINDOWS_TEST
#include "security_system_controller.h"
#include "buttons.h"
#include "buttons_controller.h"
#include "buzzer.h"
#include "connection_controller.h"
#include "display.h"
#include "display_controller.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "pir.h"
#include "util/delay.h"
#include "wifi.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t pin_code[4] = {1, 2, 3, 4};
static bool status = false;

// Send notification to the server when motion is detected
void security_system_send_notification() {
  if (status) {
    sei();
    buzzer_beep();
    Package package = package_builder_build_motion_detected();
    connection_controller_transmit(package);
    cli();
  }
}

void security_system_control_activate() {
  int i = 10;

  while (i != 0) {
    display_int(i);
    _delay_ms(1000);
    i--;
  }

  pir_init(security_system_send_notification);

  Package package = package_builder_build_acknowledgement("PIR Activated");
  connection_controller_transmit(package);
  pc_comm_send_string_blocking("PIR Activated\n");
};

bool security_system_check_pin_code(uint8_t *expected_code,
                                    uint8_t *input_code) {
  bool areEqual = false;
  for (int i = 0; i < 4; i++) {
    char str[20];
    sprintf(str, "Expected %d, but was %d\n", expected_code[i], input_code[i]);
    pc_comm_send_string_blocking(str);

    if (expected_code[i] == input_code[i]) {
      areEqual = true;
    } else {
      areEqual = false;
      break;
    }
  }
  return areEqual;
}

void security_system_control_evaluate() {

  uint8_t *input = buttons_control_pin_code_input();
  bool areEqual = security_system_check_pin_code((uint8_t *)pin_code, input);

  if (areEqual) {
    security_system_control_toggle_status();
  } else {
    wifi_command_TCP_transmit((uint8_t *)"Err\n", 5);
    pc_comm_send_string_blocking("Err\n");
    display_controller_write_word("Err");
  }

  free(input);
  security_system_control_evaluate();
}

void security_system_control_toggle_status() {
  status = !status; // toggle the status
  if (status) {
    wifi_command_TCP_transmit((uint8_t *)"Unlocked\n", 10);
    pc_comm_send_string_blocking("Unlocked\n");
    security_system_control_activate();
  } else {
    wifi_command_TCP_transmit((uint8_t *)"Locked\n", 8);
    pc_comm_send_string_blocking("Locked\n");
    Package package = package_builder_build_acknowledgement("PIR Deactivated");
    connection_controller_transmit(package);
  }
}
// #endif