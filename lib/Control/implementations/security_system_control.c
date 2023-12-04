// #ifndef WINDOWS_TEST
#include "security_system_control.h"
#include "buttons_control.h"
#include "buzzer.h"
#include "connection_control.h"
#include "display.h"
#include "display_control.h"
#include "includes.h"
#include "message_builder.h"
#include "pir.h"

static uint8_t pin_code[4] = {1, 2, 3, 4};
static bool status = false;

static bool is_pir_calibrating = true;

// Send notification to the server when motion is detected
void security_system_control_send_notification() {
  if (status && !is_pir_calibrating) {
    sei();
    buzzer_beep();
    connection_control_send_message("Motion Detected");
    cli();
  }
}

void security_system_control_activate() {
  pir_init(security_system_control_send_notification);
  int i = 10;

  while (i != 0) {
    display_int(i);
    _delay_ms(1000);
    i--;
  }
  is_pir_calibrating = false;

  pc_comm_send_string_blocking("PIR Activated\n");
};

bool security_system_control_check_pin_code(uint8_t *input_code) {
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

/* void security_system_control_evaluate() {

  uint8_t *input = buttons_control_pin_code_input();
  bool areEqual = security_system_control_check_pin_code(input);
  char str[20];
  sprintf(str, "Are equal: %d\n", areEqual);
  pc_comm_send_string_blocking(str);
  if (areEqual) {
    security_system_control_toggle_status(false);
  } else {
    pc_comm_send_string_blocking("Err\n");
    display_control_write_word("Err");
    free(input);
    security_system_control_evaluate();
  }

  free(input);
} */

char *security_system_control_toggle_status(bool remote) {
  status = !status; // toggle the status
  if (status) {
    security_system_control_activate();
    pc_comm_send_string_blocking("Unlocked\n");
  } else {
    pc_comm_send_string_blocking("Locked\n");
  }
  return remote ? "SSCRemote" : "SSCLocal";

  // connection_control_send_message(remote ? "SSCRemote" : "SSCLocal");
}

char *security_system_control_change_pin_code(uint8_t *new_pin) {
  memcpy(&pin_code, new_pin, 4);
  free(new_pin);
  char *str = malloc(20 * sizeof(char));
  sprintf(str, "NewPIN=%d%d%d%d\n", pin_code[0], pin_code[1], pin_code[2],
          pin_code[3]);

  pc_comm_send_string_blocking(str);
  // connection_control_send_message(str);
  return str;
}

bool security_system_control_is_on() { return status; }

// #endif