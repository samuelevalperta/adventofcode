#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096

int main() {

  FILE *fp;
  char line[MAX_LINE_LEN];
  char *file;

  if (!(fp = fopen("input", "r"))) {
    perror("input");
    exit(1);
  }

  int line_count = 0;
  while ((fgets(line, MAX_LINE_LEN, fp))) {
    line_count++;
    file = realloc(file, MAX_LINE_LEN * line_count);
    strncpy(file + (MAX_LINE_LEN * (line_count - 1)), line, MAX_LINE_LEN);
  }

  fclose(fp);

  char op;
  char *op_pointer = file + MAX_LINE_LEN * (line_count - 1);
  uint64_t offset = 0;
  int consumed;
  uint64_t tot = 0;

  while (sscanf(op_pointer, "%c %n", &op, &consumed) != EOF) {
    op_pointer += consumed;
    uint64_t partial;

    if (op == '*') {
      partial = 1;
    } else {
      partial = 0;
    }

    for (int i = 0; i < consumed - 1; i++) {
      uint64_t value = 0;
      for (int j = 0; j < line_count - 1; j++) {
        char digit;
        if ((digit = *(file + (MAX_LINE_LEN * j) + offset + i)) != ' ' ||
            !value) {
          value = value * 10 + atoi(&digit);
        } else {
          break;
        }
      }

      if (op == '*') {
        partial *= value;
      } else {
        partial += value;
      }
    }

    tot += partial;
    offset += consumed;
  }

  printf("%llu\n", tot);

  return 0;
}
