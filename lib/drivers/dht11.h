/**
 * @file dht11.h
 * @brief DHT11 Sensor Driver Header File
 * 
 * This header file provides the interface for interfacing with a DHT11 
 * humidity and temperature sensor.
 */

#pragma once

#include <stdint.h>

/**
 * @brief DHT11 Error messages.
 */
typedef enum{
    DHT11_OK,                         /**< Command successful. */
    DHT11_FAIL,                       /**< General failure or operation not successful. */
} DHT11_ERROR_MESSAGE_t;

/**
 * @brief Fetches humidity and temperature readings from the DHT11 sensor.
 * 
 * This function fetches the humidity and temperature readings from the DHT11 sensor.
 * The function can only be called once every 2 seconds to avoid sensor lock-up.
 * 
 * To only get specific data, pass NULL for the arguments that are not needed.
 * For example, to only get the humidity integer part, pass NULL for all other arguments.
 * 
 * @param[out] humidity_integer Pointer where the integer part of the humidity will be stored. Pass NULL if not needed.
 * @param[out] humidity_decimal Pointer where the decimal part of the humidity will be stored. Pass NULL if not needed.
 * @param[out] temperature_integer Pointer where the integer part of the temperature will be stored. Pass NULL if not needed.
 * @param[out] temperature_decimal Pointer where the decimal part of the temperature will be stored. Pass NULL if not needed.
 * 
 * @return DHT11_ERROR_MESSAGE_t Status of the read operation (DHT11_OK if successful, DHT11_FAIL otherwise).
 * 
 * @note This function should be called once every 2 seconds at most.
 * 
 * @code{.c}
 *  Example usage:
 *      uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
 *      char str[64];
 *      if (dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal) == DHT11_OK) {
 *          sprintf(str, "Humidity = %d.%d%% and the temperature = %d.%d C\n\n", 
 *              humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
 *      }
 * @endcode
 */
DHT11_ERROR_MESSAGE_t dht11_get(uint8_t* humidity_integer, uint8_t*  humidity_decimal, uint8_t* temperature_integer, uint8_t* temperature_decimal);


void dht11_init();