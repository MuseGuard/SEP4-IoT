#ifndef WINDOWS_TEST
#include "security_system_controller.h"
#include "buttons.h"
#include "buttons_controller.h"
#include "buzzer.h"
#include "display.h"
#include "display_controller.h"
#include "pc_comm.h"
#include "pir.h"
#include "tone.h"
#include "util/delay.h"
#include "wifi.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void security_system_send_notification() {
  sei();
  wifi_command_TCP_transmit((uint8_t *)"Motion detected\n", 17);
  tone_play(1000, 500);
  // pc_comm_send_string_blocking("Motion Detected\n");
  // display_controller_write_word("AAAA");
  cli();
}

void security_system_control_init() {
  int i = 10;
  
  while (i != 0) {
    display_int(i);
    _delay_ms(1000);
    i--;
  }

  wifi_command_TCP_transmit((uint8_t *)"PIR Activated\n", 15);
  pc_comm_send_string_blocking("PIR Activated\n");
  pir_init(security_system_send_notification);
  tone_init();
};

bool check_pin_code(uint8_t *expected_code, uint8_t *input_code) {
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

void security_system_control_unlock() {

  uint8_t pin_code[4] = {1, 2, 3, 4};
  uint8_t *input = buttons_control_pin_code_input();
  bool areEqual = check_pin_code((uint8_t *)pin_code, input);

  if (areEqual) {
    wifi_command_TCP_transmit((uint8_t *)"Unlocked\n", 10);
    pc_comm_send_string_blocking("Unlocked\n");
    display_controller_write_word("Unlocked");
    security_system_control_init();
    // _delay_ms(2000);
  } else {
    wifi_command_TCP_transmit((uint8_t *)"Err\n", 5);
    pc_comm_send_string_blocking("Err\n");
    display_controller_write_word("Err");
    free(input);
    security_system_control_unlock();
    // _delay_ms(2000);
  }

  free(input);
}

#endif