#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 142

int main() {
  FILE *fp;

  if (!(fp = fopen("input", "r"))) {
    perror("Reading input");
    exit(1);
  }

  char line[MAX_LINE_LEN];
  int consumed = 0;

  while (!consumed) {
    fscanf(fp, "%*[.]S%n", &consumed);
  }

  // Consume all the characters till the end of the line and count them to
  // obtain the total line_len
  int line_len = consumed;
  int beam_start = consumed;

  fscanf(fp, "%s%n", line, &consumed);
  line_len = line_len + consumed;

  fseek(fp, line_len + 2, SEEK_CUR);

  uint64_t bitmap[line_len];
  memset(bitmap, 0, line_len * sizeof(uint64_t));
  bitmap[beam_start - 1] = 1;

  while (fgets(line, MAX_LINE_LEN, fp)) {
    // Substitute the '\n' with null terminator
    line[line_len] = '\0';

    for (int i = 0; i < line_len; i++) {
      if (line[i] == '^' && bitmap[i] > 0) {
        if (i > 1) {
          bitmap[i - 1] += bitmap[i];
        }

        if (i < line_len - 1) {
          bitmap[i + 1] += bitmap[i];
        }

        bitmap[i] = 0;
      }
    }
  }

  uint64_t sum = 0;
  for (int i = 0; i < line_len; i++) {
    sum += bitmap[i];
  }

  printf("%llu\n", sum + 1);
  return 0;
}
