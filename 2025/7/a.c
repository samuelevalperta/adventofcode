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
  printf("%d\n", line_len);

  fseek(fp, line_len + 2, SEEK_CUR);

  int split = 0;
  char bitmap[line_len];
  memset(bitmap, 0, line_len * sizeof(char));
  bitmap[beam_start - 1] = 1;

  printf("Bitmap");
  for (int i = 0; i < line_len; i++) {
    printf("%hhx", bitmap[i]);
  }
  printf("\n");

  while (fgets(line, MAX_LINE_LEN, fp)) {
    // Substitute the '\n' with null terminator
    line[line_len] = '\0';
    printf("Line: %s\n", line);

    for (int i = 0; i < line_len; i++) {
      if (line[i] == '^' && bitmap[i] > 0) {
        if (i > 1) {
          bitmap[i - 1] = (++bitmap[i - 1]);
        }

        if (i < line_len - 1) {
          bitmap[i + 1] = (++bitmap[i + 1]);
        }

        bitmap[i] = 0;

        split++;
      }
    }

    printf("Bitmap");
    for (int i = 0; i < line_len; i++) {
      printf("%hhx", bitmap[i]);
    }
    printf("\n");
  }

  printf("%d\n", split);
  return 0;
}
