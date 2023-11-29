#include "pir.h"
#include "includes.h"

//Vcc
#define DDR_Vcc DDRK
#define PORT_Vcc PORTK
#define P_Vcc PK5

//GND
#define DDR_Gnd DDRK //DDRK
#define P_Gnd PK3 //PK7

//signal
#define DDR_sig DDRK
#define P_sig PK4
#define PORT_sig PORTK
#define PIN_sig PINK




static pir_callback_t pir_callback = NULL;

#ifndef WINDOWS_TEST
ISR(PCINT2_vect) {
    // Check if PK4 changed state
    if (PINK & (1 << PK4)) {
        // High: Motion detected
        if (pir_callback) {
            pir_callback();
        }
    } else {
        // Low: No motion
        // You can also call a different callback here if needed
    }
}
#endif

void pir_init(pir_callback_t callback) {

//VccPK5
DDR_Vcc|=(1<<P_Vcc);
PORT_Vcc|=(1<<P_Vcc);
//GndPK3
DDR_Gnd|=(1<<P_Gnd);


//Set PK4 as input and enable pullupresistor
    DDR_sig &= ~(1 << P_sig);
    PORT_sig |= (1 << P_sig);

    // Enable pin change interrupt on PCINT20 (PK4)
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);

    // Set the callback
    pir_callback = callback;

    // Enable global interrupts
    sei();
}