#pragma once
#include <stdbool.h>
#include "package_builder.h"

bool connection_controller_init(void);
bool connection_controller_transmit(Package package);
bool connection_controller_send_message(char *message);
