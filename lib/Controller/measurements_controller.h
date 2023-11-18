#pragma once
#include <stdint.h>
void measurements_controller_init();
uint8_t *measurements_controller_get_temperature_and_humididty();
uint16_t measurements_controller_get_lightlevels();
