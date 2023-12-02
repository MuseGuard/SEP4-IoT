#include "includes.h"
#include "monitoring_system_control.h"
#include "radar_control.h"
#include "connection_control.h"
#include "dht11.h"
#include "light.h"
#include "package_builder.h"

static uint8_t temperature_integer, temperature_decimal, humidity_integer,
    humidity_decimal;         // Static variables for Temperature Data
static uint16_t light_levels; // Static variable for Light Data

void monitoring_system_control_init() // Initializes all sensors
{
  dht11_init();
  light_init();
  radar_control_init();
}

void monitoring_system_control_execute() // Getting data from all sensors and
                                         // transmitimg over wifi
{
  dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer,
            &temperature_decimal);
  light_levels = light_read();

  // Give the package a new value
  Package package = package_builder_build_monitor(
      temperature_integer, temperature_decimal, humidity_integer, light_levels);
  connection_control_transmit(package);
  pc_comm_send_string_blocking(package.data);
};

void monitoring_radar_system()
{
  radar_control_start();
}
