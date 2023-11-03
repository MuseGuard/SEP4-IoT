#include <stdint.h>

void leds_init(void);// initialize the LED’s to be output. 

void leds_turnOn(uint8_t led_no);//takes in the value 1, 2, 3 or 4, each number corresponding to a LED. 

void leds_turnOff(uint8_t led_no);//takes in the value 1, 2, 3 or 4, each number corresponding to a LED. 

void leds_toggle(uint8_t led_no);//takes in the value 1, 2, 3 or 4, each number corresponding to a LED. 