#include "buttons.h"
#include "includes.h"

#define B_1 PF1
#define B_2 PF2
#define B_3 PF3

#define B_DDR DDRF
#define B_PORT PORTF
#define B_PIN PINF


void buttons_init(){
    B_DDR&=~((1<<B_1) | (1<<B_2) | (1<<B_3)) ; // Make them 3 buttons to be input. 
    B_PORT|=((1<<B_1) | (1<<B_2) | (1<<B_3)) ; // enable the pullup resistor. 
}

uint8_t buttons_1_pressed(){
    return !(B_PIN & (1 << B_1));
}

uint8_t buttons_2_pressed(){
    return !(B_PIN & (1 << B_2));
}

uint8_t buttons_3_pressed(){
    return !(B_PIN & (1 << B_3));
}

