/**
 * @file light.h
 * @brief Photoresistor driver for ATmega2560
 *
 * This file provides the API for initializing and reading the value from a photoresistor
 * connected to pin PK1 (ADC9) on the ATmega2560.
 *
 * @author Your Name
 * @date September 2023
 */

#pragma once

#include <stdint.h>

/**
 * @brief Initialize ADC for photoresistor
 *
 * This function initializes the ADC to read values from the photoresistor connected to pin PK1 (ADC9).
 */
void light_init(void);

/**
 * @brief Read value from photoresistor
 *
 * This function reads the ADC value from the photoresistor connected to pin PK1 (ADC9).
 *
 * @return 10-bit ADC value read from the photoresistor
 */
uint16_t light_read(void);

