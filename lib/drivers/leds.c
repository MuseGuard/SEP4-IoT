#include "leds.h"
#include "includes.h"

#define LED_BIT1 PB7
#define LED_BIT2 PB6
#define LED_BIT3 PB5
#define LED_BIT4 PB4

#define LED_DDR DDRB
#define LED_PORT PORTB

void leds_init(void){

      // LEDS, create driver later
  LED_DDR |= (1<<LED_BIT1)|(1<<LED_BIT2)|(1<<LED_BIT3)|(1<<LED_BIT4); //Output
  LED_PORT |= (1<<LED_BIT1)|(1<<LED_BIT2)|(1<<LED_BIT3)|(1<<LED_BIT4); //turnOff (Active Low)


}

void leds_turnOn(uint8_t led_no){//Active low
 switch (led_no)
 {
 case 1:
    LED_PORT&=~((1<<LED_BIT1));
    break;
case 2:
    LED_PORT&=~((1<<LED_BIT2));
    break;
case 3:
    LED_PORT&=~((1<<LED_BIT3));
    break;
case 4:
    LED_PORT&=~((1<<LED_BIT4));
    break;
 }

}

void leds_turnOff(uint8_t led_no){//active low
 switch (led_no)
 {
 case 1:
    LED_PORT|=((1<<LED_BIT1));
    break;
case 2:
    LED_PORT|=((1<<LED_BIT2));
    break;
case 3:
    LED_PORT|=((1<<LED_BIT3));
    break;
case 4:
    LED_PORT|=((1<<LED_BIT4));
    break;
 }

}

void leds_toggle(uint8_t led_no){
 switch (led_no)
 {
 case 1:
    LED_PORT^=((1<<LED_BIT1));
    break;
case 2:
    LED_PORT^=((1<<LED_BIT2));
    break;
case 3:
    LED_PORT^=((1<<LED_BIT3));
    break;
case 4:
    LED_PORT^=((1<<LED_BIT4));
    break;
 }

}