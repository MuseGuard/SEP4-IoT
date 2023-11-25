#pragma once
#include <stdbool.h>
#include <stdint.h>

bool security_system_check_pin_code(uint8_t *expected_code,
                                    uint8_t *input_code);

void security_system_control_activate();
void security_system_control_evaluate();
void security_system_control_toggle_status();
void security_system_control_send_notification();