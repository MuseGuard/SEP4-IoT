// #ifndef WINDOWS_TEST
#include "security_system_control.h"
#include "buzzer.h"
#include "display.h"
#include "includes.h"
#include "pc_comm.h"
#include "pir.h"

static uint8_t pin_code[4] = {1, 2, 3, 4};

static bool status = false;
static bool is_pir_calibrating = true;

static pir_detection_callback_t pir_callback;

// Send notification to the server when motion is detected
void security_system_control_send_notification() {
  if (status && !is_pir_calibrating) {
    buzzer_beep();
    pir_callback();
  }
}

void security_system_control_activate() {
  pir_init(security_system_control_send_notification);
  int i = 10;

  pc_comm_send_string_blocking("PIR Activated\n");
  while (i != 0) {
    display_int(i);
    _delay_ms(1000);
    i--;
  }
  is_pir_calibrating = false;
};

bool security_system_control_check_pin_code(uint8_t *input_code) {
  bool areEqual = false;
  char str[37];
  sprintf(str, "Expeced: %d%d%d%d. Received%d%d%d%d\n", pin_code[0],
          pin_code[1], pin_code[2], pin_code[3], input_code[0], input_code[1],
          input_code[2], input_code[3]);
  pc_comm_send_string_blocking(str);
  for (int i = 0; i < 4; i++) {
    if (pin_code[i] == input_code[i]) {
      areEqual = true;
    } else {
      areEqual = false;
      break;
    }
  }
  return areEqual;
}

char *security_system_control_toggle_status(bool remote) {
  status = !status; // toggle the status

  if (status) {
    is_pir_calibrating = true;
    security_system_control_activate();
    pc_comm_send_string_blocking("Unlocked\n");
  } else {
    pc_comm_send_string_blocking("Locked\n");
  }

  char *message = malloc(10 * sizeof(char));
  sprintf(message, remote ? "SSCRemote" : "SSCLocal");

  return message;
}

char *security_system_control_change_pin_code(uint8_t *new_pin) {
  memcpy(&pin_code, new_pin, 4);
  free(new_pin);
  char *str = malloc(20);

  sprintf(str, "NewPIN=%d%d%d%d\n", pin_code[0], pin_code[1], pin_code[2],
          pin_code[3]);
          pc_comm_send_string_blocking(str);
  return str;
}

bool security_system_control_is_on() { return status; }

void security_system_control_init(pir_detection_callback_t callback) {
  pir_callback = callback;
}

// #endif