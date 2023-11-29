#pragma once
#include <stdbool.h>
#include <stdint.h>

bool security_system_control_check_pin_code(uint8_t *input_code);
void security_system_control_activate();
void security_system_control_evaluate();
void security_system_control_toggle_status(bool remote);
void security_system_control_send_notification();
void security_system_control_change_pin_code(uint8_t *new_pin);
void security_system_control_override_pin_code();