#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 256
#define JOLTS_DIGITS 12

int main() {
  FILE *fp;

  if (!(fp = fopen("input", "r"))) {
    perror("Impossibile aprire il file\n");
  }

  char line[MAX_LINE_SIZE + 1];
  uint64_t sum = 0;

  while ((fgets(line, MAX_LINE_SIZE + 1, fp))) {
    line[strcspn(line, "\n")] = '\0';
    size_t line_len = strnlen(line, MAX_LINE_SIZE);

    if (line_len < JOLTS_DIGITS) {
      perror("Input troppo corto");
    }

    char *array[JOLTS_DIGITS];
    for (uint64_t i = 0; i < JOLTS_DIGITS; i++) {
      array[i] = line + line_len - (JOLTS_DIGITS - i);
    }

    char *current = array[0];
    while (current != line) {
      current--;
      if (*current >= *array[0]) {
        array[0] = current;
      }
    }

    for (uint64_t i = 1; i < JOLTS_DIGITS; i++) {
      current = array[i];
      while (current != array[i - 1]) {
        if (*current >= *array[i]) {
          array[i] = current;
        }
        current--;
      }
    }

    uint64_t value = 0;
    for (uint64_t i = 0; i < JOLTS_DIGITS; i++) {
      value += (array[i][0] - 48) * pow(10, JOLTS_DIGITS - i - 1);
    }

    sum += value;
  }

  printf("%llu\n", sum);
  return 0;
}
