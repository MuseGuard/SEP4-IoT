#include "display.h"
#include "includes.h"
// LATCH
#define LATCH_BIT PG5
#define LATCH_DDR DDRG
#define LATCH_PORT PORTG

// DATA
#define DATA_BIT PH5 // Serial Data Input.
#define DATA_DDR DDRH
#define DATA_PORT PORTH

// CLOCK
#define CLOCK_BIT PH4 // SFTCLK stands for Shift Clock or Shift Register Clock
#define CLOCK_DDR DDRH
#define CLOCK_PORT PORTH

const static uint8_t hex_digits[] = {
    0b00111111, // Index 0 (0x00): 0
    0b00000110, // Index 1 (0x01): 1
    0b01011011, // Index 2 (0x02): 2
    0b01001111, // Index 3 (0x03): 3
    0b01100110, // Index 4 (0x04): 4
    0b01101101, // Index 5 (0x05): 5
    0b01111101, // Index 6 (0x06): 6
    0b00000111, // Index 7 (0x07): 7
    0b01111111, // Index 8 (0x08): 8
    0b01101111, // Index 9 (0x09): 9
    0b01110111, // Index 10 (0x0A): A
    0b01111100, // Index 11 (0x0B): B
    0b00111001, // Index 12 (0x0C): C
    0b01011110, // Index 13 (0x0D): D
    0b01111001, // Index 14 (0x0E): E
    0b01110001, // Index 15 (0x0F): F
    0b00111101, // Index 16 (0x10): G
    0b01110100, // Index 17 (0x11): H (h)
    0b00000110, // Index 18 (0x12): I
    0b00001110, // Index 19 (0x13): J
    0b01110101, // Index 20 (0x14): K
    0b00111000, // Index 21 (0x15): L
    0b01010101, // Index 22 (0x16): M
    0b01010100, // Index 23 (0x17): N (n)
    0b01011100, // Index 24 (0x18): O (o)
    0b01110011, // Index 25 (0x19): P
    0b01100111, // Index 26 (0x1A): Q (q)
    0b01010000, // Index 27 (0x1B): R (r)
    0b01101101, // Index 28 (0x1C): S
    0b01111000, // Index 29 (0x1D): T (t)
    0b00111110, // Index 30 (0x1E): U
    0b00011100, // Index 31 (0x1F): V (v)
    0b01101010, // Index 32 (0x20): W
    0b01110110, // Index 33 (0x21): X
    0b01100110, // Index 34 (0x22): Y
    0b00011011, // Index 35 (0x23): Z
    0b01000000, // Index 36 (0x24): '-' (Negative sign)
    0b00000000, // Index 37 (0x25): ' '(Empty space)
    0b10000000,  // Index 38 (0x26): '.' (Decimal point)
    0b00001000,  // Index 39 (0x27): '_' (Underscore)
};

uint8_t static display_data[] = {0x3, 0x3, 0x3, 0x3};

void display_setValues(uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4) {
  display_data[0] = seg1;
  display_data[1] = seg2;
  display_data[2] = seg3;
  display_data[3] = seg4;
}

// Function to update the display_data[] array with the digits of a signed
// integer value Input: value - a signed integer between -999 and 9999
void display_int(int16_t value) {
  if (value > 9999 || value < -999) {
    return; // Ignore out-of-range values
  }

  uint8_t is_negative = 0; // Flag to indicate if the input value is negative

  if (value < 0) {
    is_negative = 1; // Set the negative flag
    value = -value;  // Convert the value to its positive equivalent
  }

  // Initialize display_data[] with a blank space (0x11) -> (0x25)
  display_data[0] = display_data[1] = display_data[2] = display_data[3] = 0x25;

  if (is_negative) {
    if (value < 10) {
      display_data[0] = 37; // empty
      display_data[1] = 37; // empty
      display_data[2] = 36; // minus
    } else if (value < 100) {
      display_data[0] = 37; // empty
      display_data[1] = 36; // minus
    } else
      display_data[0] = 36; // minus
  }

  // Iterate over each digit from the least significant digit to the most
  // significant digit
  if (value == 0)
    display_data[3] = 0;
  else {
    for (uint8_t i = 0; value > 0; i++) {
      display_data[3 - i] =
          value %
          10; // Get the current digit and store it in the display_data[] array
      value /= 10; // Remove the current digit from the value
    }
  }
  _delay_ms(1); // Added mimimum delay so the display always shows the values
}

void shift_out(uint8_t data);
void pulse_latch();

void display_init() {
  LATCH_DDR |= (1 << LATCH_BIT);
  DATA_DDR |= (1 << DATA_BIT);
  CLOCK_DDR |= (1 << CLOCK_BIT);

  // Set up Timer1 for CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Set the compare value for 1 ms interrupt
  OCR1A = 16000000 / 8 / 1000 - 1;

  // Enable the Timer1 compare match A interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Set the prescaler to 8
  TCCR1B |= (1 << CS11);

  sei();
  display_data[0] = display_data[1] = display_data[2] = display_data[3] = 37;
}
#ifndef WINDOWS_TEST
ISR(TIMER1_COMPA_vect) {
  uint8_t static current_digit = 0;
  LATCH_PORT &= ~(1 << LATCH_BIT);
  shift_out(~hex_digits[display_data[current_digit]]);
  shift_out(1 << current_digit);
  LATCH_PORT |= (1 << LATCH_BIT);
  // pulse_latch();

  current_digit = (current_digit + 1) % 4;
}
#endif

void shift_out(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    if (data & (1 << (7 - i))) {
      DATA_PORT |= (1 << DATA_BIT);
    } else {
      DATA_PORT &= ~(1 << DATA_BIT);
    }

    // Pulse the clock pin
    CLOCK_PORT |= (1 << CLOCK_BIT);
    // _delay_us(1);
    CLOCK_PORT &= ~(1 << CLOCK_BIT);
    //_delay_us(1);
  }
}

void pulse_latch() {
  LATCH_PORT |= (1 << LATCH_BIT);
  //_delay_us(1);
  LATCH_PORT &= ~(1 << LATCH_BIT);
  // _delay_us(1);
}