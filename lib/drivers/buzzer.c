#include "buzzer.h"
#include "includes.h"

#define BUZ_BIT PE5
#define BUZ_DDR DDRE
#define BUZ_PORT PORTE

void buzzer_beep(){

    //Save the state of the 2 registers
    uint8_t DDR_state = BUZ_DDR;
    uint8_t PORT_state = BUZ_PORT;


    BUZ_DDR |= (1<<BUZ_BIT); //init to be an output
    BUZ_PORT&=~(1<<BUZ_BIT); //Turn On (Active low)
    _delay_ms(5);
    BUZ_PORT|=(1<<BUZ_BIT); //Turn Off (Active low)

    // Restore the previous state. 
    BUZ_DDR = DDR_state;
    BUZ_PORT = PORT_state;
}