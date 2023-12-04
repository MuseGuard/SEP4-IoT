#include "display_control.h"
#include "includes.h"

#include "display.h"
#include <ctype.h>

void display_control_init() { display_init(); }

void display_control_write_word(char *word) {
  uint8_t *nums = display_control_convert_word_to_numbers(word);
  uint8_t nums_len = (uint8_t)strlen(word);

  if (nums_len < 4) {
    /* Creating a temprorary array with 4 elements. Fill the first lemenets
    with the values from nums, and the rest with 37 (space) */
    uint8_t *temp = malloc(4 * sizeof(uint8_t));
    for (uint8_t i = 0; i < 4; i++) {
      if (i < nums_len) {

        temp[i] = nums[i];
      } else {
        temp[i] = 37;
      }
    }
    free(nums);
    nums = temp;
    nums_len = 4;
  }

  for (uint8_t i = 0; i < nums_len - 3; i++) {

    display_setValues(nums[i], nums[i + 1], nums[i + 2], nums[i + 3]);
    _delay_ms(1000);
  }

  display_setValues(37, 37, 37, 37); // Clear the display (show 4 spaces)
  free(nums);                        // Clean up memory
}

uint8_t *display_control_convert_word_to_numbers(char *word) {
  uint8_t len = (uint8_t)strlen(word);
  uint8_t *result = malloc(len * sizeof(uint8_t));

  for (int i = 0; i < len; i++) {
    char current_letter = word[i];
    uint8_t numeric_value = 0;

    if (isalpha(current_letter)) {
      numeric_value = toupper(current_letter) - 'A' + 10;
    } else if (isdigit(current_letter)) {
      numeric_value = current_letter % 48;
    } else {
      switch (current_letter) {
      case '-':
        numeric_value = 36;
        break;
      case ' ':
        numeric_value = 37;
        break;
      case '.':
        numeric_value = 38;
        break;
      case '_':
        numeric_value = 39;
        break;
      default:
        break;
      }
    }
    result[i] = numeric_value;
  }
  return result;
}

void display_control_show_pin_code_position(uint8_t *pin_code,
                                            uint8_t current_position) {
  switch (current_position) {
  case 0:
    display_setValues(39, pin_code[1], pin_code[2], pin_code[3]);
    break;
  case 1:
    display_setValues(pin_code[0], 39, pin_code[2], pin_code[3]);
    break;
  case 2:
    display_setValues(pin_code[0], pin_code[1], 39, pin_code[3]);
    break;
  case 3:
    display_setValues(pin_code[0], pin_code[1], pin_code[2], 39);
    break;
  default:
    break;
  }
  _delay_ms(1);
}