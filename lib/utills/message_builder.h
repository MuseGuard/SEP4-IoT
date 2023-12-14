#pragma once

#include <stdint.h>

char* message_builder_build_monitor(int8_t temperature_integer, uint8_t temperature_decimal, uint8_t humidity_integer, uint16_t light_levels);