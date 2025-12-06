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

  int values[line_count - 1];
  int offset = 0, max_value_digits;
  uint64_t partial = 0, total = 0;
  int num_read_char;
  int count = 0;
  while (sscanf(op_pointer, " %c", &op) != EOF) {
    count++;
    max_value_digits = 0;
    for (int i = 0; i < line_count - 1; i++) {
      sscanf(file + MAX_LINE_LEN * i + offset, "%d%n", &values[i],
             &num_read_char);
      printf("Read valued: %d\n", values[i]);
      if (num_read_char > max_value_digits) {
        max_value_digits = num_read_char;
      }

      if (op == '*') {
        partial *= values[i];
      } else {
        partial += values[i];
      }
    }

    if (op == '*') {
      partial = 1;
      for (int i = 0; i < line_count - 1; i++) {
        partial *= values[i];
      }
    } else {
      partial = 0;
      for (int i = 0; i < line_count - 1; i++) {
        partial += values[i];
      }
    }
    /////

    for (int i = 0; i < line_count - 1; i++) {
      printf("%d%c", values[i], op);
    }
    printf("=");

    /////

    total += partial;
    printf("%llu\n", partial);
    offset += max_value_digits;
    op_pointer += max_value_digits;
  }

  printf("%s\n", file + MAX_LINE_LEN * 4);
  printf("%llu\n", total);
  printf("Read %d operations\n", count);
  return 0;
}
