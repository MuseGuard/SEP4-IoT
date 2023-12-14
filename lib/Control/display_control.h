#pragma once
#include <stdint.h>

void display_control_init(void);
void display_control_write_word(char *word);
uint8_t *display_control_convert_word_to_numbers(char *word);

void display_control_show_pin_code_position(uint8_t *pin_code,
                                            uint8_t current_position);