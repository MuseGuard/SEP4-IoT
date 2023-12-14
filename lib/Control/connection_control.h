#pragma once
#include "message_builder.h"
#include <stdbool.h>

typedef void (*server_callback_t)(char *);

void connection_control_callback();
bool connection_control_init(server_callback_t callback);
bool connection_control_send_message(char *message);
