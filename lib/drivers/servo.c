#include <servo.h>
#include "includes.h"


#define DDR_SERVO DDRE
#define P_SERVO PE3
#define PORT_SERVO PORTE

void servo(uint8_t angle)
{
    // Initialize as output
    DDR_SERVO |= (1 << P_SERVO);

    if (angle < 0)
        angle = 0;
    if (angle > 180)
        angle = 180;

    // Convert angle to pulse width in microseconds
    // 500us pulse width for 0 degrees, 2120us pulse width for 180 degrees
    uint16_t delay_us = ((uint16_t)angle) * 9 + 500;

    // Initialize Timer2 in normal 
    uint8_t TCCR2A_state= TCCR2A;//
    TCCR2A = 0;

    // Choose prescaler based on delay
    uint8_t TCCR2B_state= TCCR2B;//    
    TCCR2B = (1 << CS22) | (1 << CS21); // 256

    // Calculate the number of timer ticks needed for the specified delay
    uint8_t num_ticks = ((F_CPU / 1000000UL) * delay_us) / 256;

    for (uint16_t i = 0; i < 50; i++)//Wee assume it can get there in 1sec (50*20ms)
    {
        // Set PA1 high
        PORT_SERVO |= (1 << P_SERVO);
        // Reset the timer counter
        TCNT2 = 0;

        // Wait until the timer counter reaches the required ticks
        while (TCNT2 < num_ticks)
        {}

        // Set PA1 low
        PORT_SERVO &= ~(1 << P_SERVO);
        
        _delay_ms(18); // thats dirty TODO, make this pricise.
    }

   TCCR2A =TCCR2A_state ; //finished borrowing timer2. 
   TCCR2B =TCCR2B_state ; //
}