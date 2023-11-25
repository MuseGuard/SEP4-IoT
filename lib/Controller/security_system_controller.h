#pragma once
#include <stdbool.h>
#include <stdint.h>

bool security_system_controller_check_pin_code(uint8_t *expected_code,
                                               uint8_t *input_code);

void security_system_controller_activate();
void security_system_controller_evaluate();
void security_system_controller_toggle_status();
void security_system_controller_send_notification();
void securiy_system_controller_change_pin_code(uint8_t *new_pin);