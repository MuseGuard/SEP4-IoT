// #ifndef WINDOWS_TEST
#include "security_system_control.h"
#include "includes.h"

#include "buttons_control.h"
#include "buzzer.h"
#include "connection_control.h"
#include "display.h"
#include "display_control.h"
#include "package_builder.h"
#include "pir.h"

static uint8_t pin_code[4] = {1, 2, 3, 4};
static bool status = false;
static bool is_pir_calibrating = true;

// Send notification to the server when motion is detected
void security_system_control_send_notification() {
  if (status && !is_pir_calibrating) {
    sei();
    buzzer_beep();
    Package package = package_builder_build_motion_detected();
    connection_control_transmit(package);
    pc_comm_send_string_blocking("Motion detected\n");
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

void security_system_control_evaluate() {

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
/*     connection_control_send_message("Err"); */
    free(input);
    security_system_control_evaluate();
  }

  free(input);
}

void security_system_control_toggle_status(bool remote) {
  status = !status; // toggle the status

  if (status) {
    security_system_control_activate();
    pc_comm_send_string_blocking("Unlocked\n");
  } else {
    pc_comm_send_string_blocking("Locked\n");
  }
  connection_control_send_message(remote ? "SSCRemote" : "SSCLocal");
}

void security_system_control_change_pin_code(uint8_t *new_pin) {
  memcpy(&pin_code, new_pin, 4);
  free(new_pin);
  char str[20];
  sprintf(str, "NewPIN=%d%d%d%d\n", pin_code[0], pin_code[1], pin_code[2],
          pin_code[3]);

  pc_comm_send_string_blocking(str);
  connection_control_send_message(str);
}

void security_system_control_override_pin_code() {
  if (status) {
    _delay_ms(200);
    display_control_write_word("Edit");
    _delay_ms(1000);
    uint8_t *new_pin_code = buttons_control_pin_code_input();
    security_system_control_change_pin_code(new_pin_code);
    display_control_write_word("OK");
  } else {
    pc_comm_send_string_blocking("Unlock the device first\n");
    display_control_write_word("Err");
    _delay_ms(1000);
  }
}
// #endif