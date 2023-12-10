#pragma once
#include "stdint.h"

void application_take_measurements();
void application_get_new_pin();
uint8_t *application_take_pin_input();
void application_buttons_listen();
void application_evaluate_pin();
void apllication_on_message_received_callback(char *buffer);
void application_pir_callback();
void application_init();
void application_run();
// void application_motion_detected();