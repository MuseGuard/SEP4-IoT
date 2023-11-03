#include <stdint.h>
/**
 * @brief Initialize the display (the 4 7-segments)
 * 
 */
void display_init(void);

/**
 * @brief 
 * 
 * @param seg should be a number from 0-17. 0-15 corrospond to the hex-digit, 16 is '-' and 17 is whitespace ' '
 */
void display_setValues(uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4 );

/**
 * @brief Can show an integer. 
 * 
 * @param value between -999 to 9999
 */
void display_int(int16_t value);