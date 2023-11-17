#pragma once
#include <stdbool.h>

bool connection_controller_init(void);
bool connection_controller_transmit(char *package, int length);
