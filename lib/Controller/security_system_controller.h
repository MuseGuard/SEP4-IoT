#pragma once
#include <stdbool.h>
#include <stdint.h>

bool check_pin_code(uint8_t *expected_code, uint8_t *input_code);

void security_system_control_unlock();

char *security_system_control_get_pin_code();