#include "display_control.h"
#include "display.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void display_control_write_word(char *word) {
  uint8_t *nums = display_control_convert_word_to_numbers(word);
  uint8_t nums_len = sizeof(nums) / sizeof(nums[0]);

  // TODO:

  free(nums); // Clean up
}

uint8_t *display_control_convert_word_to_numbers(char *word) {
  uint8_t len = (uint8_t)strlen(word);
  uint8_t *result = malloc(len * sizeof(uint8_t));
  for (int i = 0; i < len; i++) {
    char current_letter = word[i];
    uint8_t numeric_value = 0;

    if (current_letter >= 'A' && current_letter <= 'z') {
      numeric_value = current_letter % 32 + 9;
      /*  'A' and 'a' in decimal %32 are 1 -> instead of
            toLowerCase() / toUpperCase() */
      //  9 is the offset to the first letter in the display_data array
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

      default:
        break;
      }
    }
    result[i] = numeric_value;
  }
  return result;
}