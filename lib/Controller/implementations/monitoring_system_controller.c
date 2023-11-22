#include "connection_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include "dht11.h"
#include "light.h"

static uint8_t temperature_integer, temperature_decimal, humidity_integer, humidity_decimal; //Static variables for Temperature Data
static uint16_t light_levels; // Static variable for Light Data

void monitoring_system_controller_init() //Initializes all sensors
{
  dht11_init();
  light_init();
}

void monitoring_system_controller_execute() //Getting data from all sensors and transmitimg over wifi
{

  dht11_get(&humidity_integer, &humidity_decimal ,& temperature_integer,
            &temperature_decimal);
  light_levels = light_read();

  // Give the package a new value
  Package package = package_builder_build(temperature_integer, temperature_decimal, humidity_integer, light_levels);
  connection_controller_transmit(package);
  pc_comm_send_string_blocking(package.data);
};
