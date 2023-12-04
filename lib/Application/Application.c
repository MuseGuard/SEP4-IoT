#include "Application.h"
#include "buttons_control.h"
#include "connection_control.h"
#include "display_control.h"
#include "includes.h"
#include "monitoring_system_control.h"
#include "periodic_task.h"
#include "security_system_control.h"

/* void application_motion_detected() {
  connection_control_send_message("Motion detected");
} */

// Take measurements from the sensors(Temp, Humidity & Light) and send them to
// the server
void application_take_measurements() {
  char *message = monitoring_system_control_execute();
  connection_control_send_message(message);
  pc_comm_send_string_blocking(message);
}


// Change the pin code on the device (On-Site)
void application_get_new_pin() {
  if (security_system_control_is_on()) {
    _delay_ms(200);
    display_control_write_word("Edit");
    uint8_t *new_pin_code = buttons_control_pin_code_input();
    char *message = security_system_control_change_pin_code(new_pin_code);
    pc_comm_send_string_blocking(message);
    connection_control_send_message(message);
    free(message);
    display_control_write_word("OK");
  } else {
    pc_comm_send_string_blocking("Unlock the device first\n");
    display_control_write_word("Err");
    _delay_ms(1000);
  }
}

// Listen for button presses on the device
void application_buttons_listen() {
  uint8_t button_number = buttons_control_listen();
  switch (button_number) {
  case 1:
    application_evaluate_pin();
    break;
  case 3:
    application_get_new_pin();
    break;
  default:
    break;
  }
  // display_setValues(33, 18, 37, 37); // Show "Hi  "
}


// Check if the pin code is correct
void application_evaluate_pin() {
  uint8_t *input = buttons_control_pin_code_input();
  bool areEqual = security_system_control_check_pin_code(input);

  // CLEAR
  char str[20];
  sprintf(str, "Are equal: %d\n", areEqual);
  pc_comm_send_string_blocking(str);

  if (areEqual) {
    char *message = security_system_control_toggle_status(false);
    connection_control_send_message(message);
    display_control_write_word("OK");
  } else {
    pc_comm_send_string_blocking("Err\n");
    display_control_write_word("Err");
  }
}

// Initialize all the modules
void application_init() {
  display_control_init();
  connection_control_init();
  monitoring_system_control_init();
  buttons_control_init();


  timer_init_a(application_take_measurements, 5000);
}

// Run the application after initialization
void application_run() {
  application_buttons_listen();
  _delay_ms(100);
}