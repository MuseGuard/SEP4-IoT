#include <stdint.h>

void display_control_write_word(char *word);
uint8_t *display_control_convert_word_to_numbers(char *word);

/*
// Example for a word with 4 letters:
uint8_t *numbers = display_control_convert_word_to_numbers("hello");

  while (1)
  {
  display_setValues(numbers[0], numbers[1], numbers[2], numbers[3]);
  _delay_ms(1000);
  }

 */