#include "display.h"
#include "includes.h"
//LATCH
#define LATCH_BIT PG5
#define LATCH_DDR DDRG
#define LATCH_PORT PORTG

//DATA
#define DATA_BIT PH5  // Serial Data Input.
#define DATA_DDR DDRH
#define DATA_PORT PORTH

// CLOCK
#define CLOCK_BIT PH4 // SFTCLK stands for Shift Clock or Shift Register Clock
#define CLOCK_DDR DDRH
#define CLOCK_PORT PORTH

const static uint8_t hex_digits[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b01000000, // '-' (Negative sign)
    0b00000000  // (Empty space)
};

uint8_t static display_data[] = {0x3, 0x3, 0x3, 0x3};

void display_setValues(uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4)
{
    display_data[0] = seg1;
    display_data[1] = seg2;
    display_data[2] = seg3;
    display_data[3] = seg4;
}

// Function to update the display_data[] array with the digits of a signed integer value
// Input: value - a signed integer between -999 and 9999
void display_int(int16_t value)
{
    if (value > 9999 || value < -999)
    {
        return; // Ignore out-of-range values
    }

    uint8_t is_negative = 0; // Flag to indicate if the input value is negative

    if (value < 0)
    {
        is_negative = 1; // Set the negative flag
        value = -value;  // Convert the value to its positive equivalent
    }

    // Initialize display_data[] with a blank space (0x11)
    display_data[0] = display_data[1] = display_data[2] = display_data[3] = 0x11;

    if (is_negative)
    {
        if (value < 10)
        {
            display_data[0] = 17; // empty
            display_data[1] = 17; // empty
            display_data[2] = 16; // minus
        }
        else if (value < 100)
        {
            display_data[0] = 17; // empty
            display_data[1] = 16; // minus
        }
        else
            display_data[0] = 16; // minus
    }

    // Iterate over each digit from the least significant digit to the most significant digit
    if (value == 0)
        display_data[3] = 0;
    else
        for (uint8_t i = 0; value > 0; i++)
        {
            display_data[3 - i] = value % 10; // Get the current digit and store it in the display_data[] array
            value /= 10;                      // Remove the current digit from the value
        }
}

void shift_out(uint8_t data);
void pulse_latch();

void display_init()
{
    LATCH_DDR |= (1 << LATCH_BIT);
    DATA_DDR |= (1 << DATA_BIT); 
    CLOCK_DDR|= (1 << CLOCK_BIT);

    // Set up Timer1 for CTC mode (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12);

    // Set the compare value for 1 ms interrupt
    OCR1A = 16000000 / 8 / 1000 - 1;

    // Enable the Timer1 compare match A interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Set the prescaler to 8
    TCCR1B |= (1 << CS11);

    sei();
    display_data[0] = display_data[1] = display_data[2] = display_data[3] = 17;
}
#ifndef WINDOWS_TEST
ISR(TIMER1_COMPA_vect)
{
    uint8_t static current_digit = 0;
    LATCH_PORT &= ~(1 << LATCH_BIT);
    shift_out(~hex_digits[display_data[current_digit]]);
    shift_out(1 << current_digit);
    LATCH_PORT |= (1 << LATCH_BIT);
    // pulse_latch();

    current_digit = (current_digit + 1) % 4;
}
#endif

void shift_out(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1 << (7 - i)))
        {
            DATA_PORT |= (1 << DATA_BIT);
        }
        else
        {
            DATA_PORT &= ~(1 << DATA_BIT);
        }

        // Pulse the clock pin
        CLOCK_PORT |= (1 << CLOCK_BIT);
        // _delay_us(1);
        CLOCK_PORT &= ~(1 << CLOCK_BIT);
        //_delay_us(1);
    }
}

void pulse_latch()
{
    LATCH_PORT |= (1 << LATCH_BIT);
    //_delay_us(1);
    LATCH_PORT &= ~(1 << LATCH_BIT);
    // _delay_us(1);
}