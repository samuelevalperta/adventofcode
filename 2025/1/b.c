#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *fp = fopen("input", "r");

  if (fp == NULL) {
    printf("Unable to open input file\n");
    exit(1);
  }

  size_t max_len = 10;
  char line[max_len];
  int current = 50;
  int counter = 0;

  while (fgets(line, max_len, fp)) {
    line[strcspn(line, "\n")] = '\0';

    int value = atoi(&line[1]);

    counter += value / 100;
    value = value % 100;

    if (line[0] == 'L') {
      if (current && current < value) {
        counter++;
      }
      current -= value;
    } else if (line[0] == 'R') {
      if (100 - current < value) {
        counter++;
      }
      current += value;
    } else {
      printf("Something went wrong while parsing the input file\n");
      exit(1);
    }

    // Find the real value using the modulo 100 operation
    // https://stackoverflow.com/a/42131603
    current = (current % 100 + 100) % 100;

    if (current == 0) {
      counter++;
    }
  }

  printf("%d\n", counter);
  fclose(fp);

  return 0;
}
