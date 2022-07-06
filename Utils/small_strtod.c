/*
 * small_strtod.c
 *
 *  Created on: May 11, 2022
 *      Author: matth
 */

#include "small_strtod.h"

#include <math.h>

#include "stdbool.h"

#define TO_INT(ch) ch - '0'

#define IS_DIGIT(ch) (ch >= '0' && ch <= '9')

double small_strtod(char* str, char** endptr) {
  char* current = str;
  double sum = 0;
  bool before_decimal = true;
  int place = 1;
  bool is_negative = false;
  bool number_started = false;
  while (*current != '\0') {
    // always skip over whitespace
    if (*current == ' ') {
      // Reject anything that isn't a digit, decimal, or negative
    } else if (!IS_DIGIT(*current) && *current != '.' && *current != '-') {
      break;

      // check negative sign
    } else if (*current == '-') {
      if (number_started) {
        break;
      } else {
        is_negative = true;
        number_started = true;
      }

      // check decimal
    } else if (*current == '.') {
      if (!before_decimal) {
        break;
      } else {
        before_decimal = false;
        number_started = true;
      }

      // must be a digit
    } else {
      number_started = true;
      if (before_decimal) {
        // Multiply by 10, then add our number
        // So like 1021 = 102 * 10 + 1
        sum *= 10;
        sum += TO_INT(*current);
      } else {
        // Place 1 = 1/10
        // place 2 = 1/(10*10)
        // etc
        sum += (TO_INT(*current)) / (float)pow(10, place);
        place++;
      }
    }
    ++current;
  }
  *endptr = current;

  return sum * (is_negative ? -1.0 : 1.0);
}
