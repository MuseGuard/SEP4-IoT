#pragma once
#include "stdint.h"

void application_init();
void application_run();
void application_evaluate_pin();
void application_get_pin();
uint8_t* application_take_pin_input();
    // void application_motion_detected();