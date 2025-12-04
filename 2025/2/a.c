#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define MAX_DIGITS_UINT64 21

uint64_t is_invalid(uint64_t i) {
  char str[MAX_DIGITS_UINT64]; // UINT64_MAX has 20 digits + 1 for the null term

  sprintf(str, "%llu", i);
  size_t digits = strnlen(str, MAX_DIGITS_UINT64);

  if (digits % 2 != 0) {
    return 0;
  }

  size_t half_digits = digits / 2;
  for (size_t j = 0; j < half_digits; j++) {
    if (str[j] != str[j + half_digits]) {
      return 0;
    }
  }

  return 1;
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
        result += i;
      }
    }
    p += (consumed + 1);
  }

  printf("%llu\n", result);
  return 0;
}
