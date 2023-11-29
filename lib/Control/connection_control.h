#pragma once
#include "package_builder.h"
#include <stdbool.h>

bool connection_control_init(void);
bool connection_control_transmit(Package package);
bool connection_control_send_message(char *message);
