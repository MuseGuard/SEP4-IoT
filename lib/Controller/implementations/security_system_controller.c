#ifndef WINDOWS_TEST
#include "security_system_controller.h"
#include "buttons.h"
#include "buttons_controller.h"
#include "display.h"
#include "display_controller.h"
#include "pc_comm.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    pc_comm_send_string_blocking("Unlocked\n");
    display_controller_write_word("Unlocked");
    _delay_ms(2000);
  } else {
    pc_comm_send_string_blocking("Err\n");
    display_controller_write_word("Err");
    _delay_ms(2000);
  }

  free(input);
}

#endif