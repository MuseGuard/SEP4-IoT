#pragma once
#include <inttypes.h>



void adxl345_init(void);



void adxl345_write_register(uint8_t reg, uint8_t value);

/// @brief It returns the acceleration in the 3 direction. the number is a int16_t, 
/// meaning the range is from −32,768 to +32,767. The maximum value corrosponds to 
/// 4g, and the minimum value to -4g. To change the unit to m/s^2 the values should 
/// be devided by 32,767/(9.82*4) = 834
/// @param x 
/// @param y 
/// @param z 
void adxl345_read_xyz(int16_t *x, int16_t *y, int16_t *z);
