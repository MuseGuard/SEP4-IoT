#pragma once
#include <stdint.h>

void radar_control_init();
void radar_control_measurement(uint8_t angle);
void radar_control_start();