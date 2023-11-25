#include "request_interpreter.h"
#include <stdlib.h>

uint8_t *request_interpreter_get_pin(char *request){
  char *equal_sign = strchr(request, '='); // Find the pointer of '='
  char *request_pin_code = equal_sign + 1; // Get the pointer of the first symbol after '='
 
  uint8_t* new_pin = malloc(4 * sizeof(uint8_t));
  for (int i = 0; i < 4; i++) {
    new_pin[i] = request_pin_code[i] - '0'; // convert char to int (ASCII)
  }
  
  return new_pin;
}