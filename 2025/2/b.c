#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define MAX_DIGITS_UINT64 21

uint64_t is_invalid(uint64_t i) {
  /* InvalidID with 2 digits are the ones and only divisible by 11 */
  if (i < 100) {
    if (i % 11 == 0) {
      return 1;
    }
    return 0;
  }

  char str[MAX_DIGITS_UINT64]; // UINT64_MAX has 20 digits + 1 for the null term

  sprintf(str, "%llu", i);
  size_t digits = strnlen(str, MAX_DIGITS_UINT64);

  /* Check if all the digits are the same, made here so the next cycle can stop
   * a digits/2 instead of digits. */
  for (char c = 48; c < 58; c++) {
    char tmp[digits];
    memset(tmp, c, digits);
    tmp[digits] = '\0';

    if (!strncmp(str, tmp, digits)) {
      return 1;
    }
  }

  /* Loop on factor of the digits - excluding digits itsfelf */
  for (uint64_t i = 2; i <= (digits / 2); i++) {
    if (digits % i == 0) {
      /* there can be an (digits/i) len string that concatenated i times
       * generate the ID */
      uint64_t possible_len = digits / i;
      uint8_t is_invalid = 1;
      for (uint64_t j = 0; j < (i - 1); j++) {
        if (strncmp(&str[j * possible_len], &str[(j + 1) * possible_len],
                    possible_len)) {
          is_invalid = 0;
          break;
        }
      }

      if (is_invalid) {
        return 1;
      }
    }
  }

  return 0;
}

int main() {
  uint64_t result = 0;
  FILE *fp;

  if (!(fp = fopen("input", "r"))) {
    perror("input");
    exit(1);
  };

  char input[4096];
  char *p = input;

  if (!fgets(input, 4096, fp)) {
    perror("File vuoto");
    exit(1);
  }

  uint64_t start, end;
  int consumed;
  while (sscanf(p, "%llu-%llu%n", &start, &end, &consumed) == 2) {
    for (uint64_t i = start; i <= end; i++) {
      if (is_invalid(i)) {
        // printf("%llu\n", i);
        result += i;
      }
    }
    p += (consumed + 1);
  }

  printf("%llu\n", result);
  return 0;
}
