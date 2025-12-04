#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 256

int main() {
  FILE *fp;

  if (!(fp = fopen("input", "r"))) {
    perror("Impossibile aprire il file\n");
  }

  char line[MAX_LINE_SIZE + 1];
  int sum = 0;

  while ((fgets(line, MAX_LINE_SIZE + 1, fp))) {
    line[strcspn(line, "\n")] = '\0';
    size_t line_len = strnlen(line, MAX_LINE_SIZE);

    // printf("Size: %zu\n", line_len);
    // printf("Line: %s\n", line);
    char *first = line + line_len - 2, *second = first + 1, *current = first;

    // printf("%s\nFirst: %c, Second:%c, Current:%c\n", line, first[0],
    // second[0],
    //        current[0]);

    while (current != line) {
      current--;
      if (*current >= *first) {
        first = current;
      }
    }

    current = second - 1;
    while (current != first) {
      if (*current >= *second) {
        second = current;
      }
      current--;
    }

    int value = (int)((first[0]) - 48) * 10 + (int)(second[0] - 48);
    // printf("Value: %u\n", value);
    sum += value;
  }

  printf("%d\n", sum);
  return 0;
}
