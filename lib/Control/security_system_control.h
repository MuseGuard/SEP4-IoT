#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef void (*pir_detection_callback_t)(void);

bool security_system_control_check_pin_code(uint8_t *input_code);
void security_system_control_activate();
char *security_system_control_toggle_status(bool remote);
void security_system_control_send_notification();
char *security_system_control_change_pin_code(uint8_t *new_pin);
bool security_system_control_is_on();
void security_system_control_init(pir_detection_callback_t callback);