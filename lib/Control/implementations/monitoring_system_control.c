#include "monitoring_system_control.h"
#include "includes.h"

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
}

void monitoring_system_control_execute() // Getting data from all sensors and
                                         // transmitimg over wifi
{

  DHT11_ERROR_MESSAGE_t measurement_result =
      dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer,
                &temperature_decimal);
  if (measurement_result != DHT11_OK) {
    connection_control_send_message("Error reading DHT11 sensor!");
  } else {
    light_levels = light_read();

    // Give the package a new value
    Package package =
        package_builder_build_monitor(temperature_integer, temperature_decimal,
                                      humidity_integer, light_levels);
    connection_control_transmit(package);
    pc_comm_send_string_blocking(package.data);
  }
};
