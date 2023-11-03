/**
 * @file light.c
 * @brief Photoresistor driver implementation for ATmega2560
 *
 * This file provides the implementation for initializing and reading the value from a photoresistor
 * connected to pin PK1 (ADC9) on the ATmega2560.
 *
 * @author Your Name
 * @date September 2023
 */

#include "light.h"
#include "includes.h"

/**
 * @brief Initialize ADC for photoresistor
 *
 * This function initializes the ADC to read values from the photoresistor connected to pin PK1 (ADC9).
 */
void light_init(void) {

    //Vcc
    DDRK|=(1 << PK2);
    PORTK|=(1 << PK2);

    //GND
    DDRK|=(1 << PK1);

    // Set reference voltage to AVCC and left adjust ADC result
    // The  MUX1:5 should be set to 10000 for choosing ADC8, which ius placed on PK0 (look at page 283)
    ADMUX = (1 << REFS0);//|(1<<MUX1);
    ADCSRB = (1<<MUX5);
    // Enable ADC and set prescaler to 64 (16MHz/128 = 125kHz)
    // ADC must operate between 50kHz and 200kHz for its full 10-bit resolution
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1)| (1 << ADPS0);
    

    // Disable digital input on PK2 (ADC10) (page 287)
    // This will reduce power consumption on the pin
    DIDR2 = (1 << ADC8D);

}

/**
 * @brief Read value from photoresistor
 *
 * This function reads the ADC value from the photoresistor connected to pin PK1 (ADC9).
 *
 * @return 10-bit ADC value read from the photoresistor
 */
uint16_t light_read(void) {

uint32_t timeout = 40000;//if 2cc for incrementing and evaluation the timeout is 5ms
    // Start the conversion
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete
    while ((ADCSRA & (1 << ADSC))&& timeout > 0){timeout--;};

    // Read the 10-bit ADC value
    // ADCL must be read first, then ADCH
    uint16_t adc_value = ADCL;
    adc_value |= (ADCH << 8);

    return adc_value;
}
