#ifndef WINDOWS_TEST
#include "connection_controller.h"
#include "measurements_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include <avr/interrupt.h>
#include <stdlib.h>

void monitoring_system_controller_execute() {
  uint8_t *temperature_and_humidity =
      measurements_controller_get_temperature_and_humidity();
  uint16_t light_levels = measurements_controller_get_lightlevels();
  Package package =
      package_builder_build(temperature_and_humidity, light_levels);

  // Free the memory allocated by the measurements_controller
  free(temperature_and_humidity);
  pc_comm_send_string_blocking(package.data);
  connection_controller_transmit(package);
};
#endif