#include <stdint.h>

void display_controller_write_word(char *word);
uint8_t *display_controller_convert_word_to_numbers(char *word);

void display_controller_show_pin_code_position(uint8_t *pin_code,
                                               uint8_t current_position);