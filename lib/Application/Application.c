#include "Application.h"
#include "buttons_control.h"
#include "connection_control.h"
#include "display.h"
#include "display_control.h"
#include "includes.h"
#include "monitoring_system_control.h"
#include "request_interpreter.h"
#include "security_system_control.h"
#include "pc_comm.h"
#ifndef WINDOWS_TEST_APPLICATION
#include "periodic_task.h"
#endif

// Take measurements from the sensors(Temp, Humidity & Light) and send them to
// the server
void application_take_measurements() {
  char *message = monitoring_system_control_execute();
  connection_control_send_message(message);
  pc_comm_send_string_blocking(message);
}

// Change the pin code on the device (On-Site)
void application_get_new_pin() 
{
    if (security_system_control_is_on()) 
    {
        _delay_ms(200);
        display_control_write_word("Edit");
        uint8_t *new_pin_code = application_take_pin_input();
        char *message = security_system_control_change_pin_code(new_pin_code);
        pc_comm_send_string_blocking(message);
        connection_control_send_message(message);
        free(message);
        display_control_write_word("OK");
    } 
    else
    {
        pc_comm_send_string_blocking("Unlock the device first\n");
        display_control_write_word("Err");
        _delay_ms(1000);
    }
}

uint8_t *application_take_pin_input() {
  uint8_t *pin_code =
      malloc(4 * sizeof(uint8_t));          // allocate memory for pin_code
  memset(pin_code, 0, 4 * sizeof(uint8_t)); // initialize with 4 zeros

  int current_position = 0;

  while (current_position < 4) {
    display_setValues(pin_code[0], pin_code[1], pin_code[2], pin_code[3]);

    uint8_t button_number = buttons_control_listen();
    if (button_number == 0) {
      continue;
    }

    if (button_number == 1) {
      pin_code[current_position]++;
      pin_code[current_position] = pin_code[current_position] % 10;
    } else {
      if (button_number == 2) {
        current_position--;
      } else if (button_number == 3) {
        current_position++;
      }
      display_control_show_pin_code_position(pin_code, current_position);
    }
    _delay_ms(150);
  }

  char message[25];
  sprintf(message, "Input: %d%d%d%d\n", pin_code[0], pin_code[1], pin_code[2],
          pin_code[3]);
  pc_comm_send_string_blocking(message);
  return pin_code;
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
  case 2:
    apllication_on_message_received_callback("ChangePIN=1250");
    break;
  default:
    break;
  }
  _delay_ms(100);
  display_setValues(33, 18, 37, 37); // Show "Hi  "
}

// Check if the pin code is correct
void application_evaluate_pin() {
  uint8_t *input = application_take_pin_input();
  bool areEqual = security_system_control_check_pin_code(input);
  free(input);
  if (areEqual) {
    char *message = security_system_control_toggle_status(false);
    pc_comm_send_string_blocking(message);
    connection_control_send_message(message);
    free(message);
    display_control_write_word("OK");
  } else {
    pc_comm_send_string_blocking("Err\n");
    display_control_write_word("Err");
  }
}

void apllication_on_message_received_callback(char *buffer) {
  pc_comm_send_string_blocking(buffer);
  pc_comm_send_string_blocking("\n");
  if (strcmp(buffer, "ChangeSecurityStatus") == 0) {
    security_system_control_toggle_status(true);
    char *message = security_system_control_toggle_status(true);
    pc_comm_send_string_blocking(message);
    connection_control_send_message(message);
    free(message);
  } else if (strstr(buffer, "ChangePIN") == buffer) {
    uint8_t *pin_code = request_interpreter_get_pin(buffer);
    char *message = security_system_control_change_pin_code(pin_code);
    pc_comm_send_string_blocking(message);
    connection_control_send_message(message);
    free(message);
  } else {
    connection_control_send_message("Invalid command!");
  }
}

void application_pir_callback() {
  connection_control_send_message("Motion Detected");
  pc_comm_send_string_blocking("Motion Detected\n");
}

void application_init() {
  //pc_comm_init(9600, NULL);
  display_control_init();
  connection_control_init(apllication_on_message_received_callback);
  security_system_control_init(application_pir_callback);
  monitoring_system_control_init();
  buttons_control_init();
#ifndef WINDOWS_TEST_APPLICATION
  timer_init_a(application_take_measurements, 5000);
  #endif
}

// Run the application after initialization
void application_run() { application_buttons_listen(); }
