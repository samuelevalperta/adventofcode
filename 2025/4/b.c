#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 136
#define COLS 136

int main() {

  FILE *fp;
  char wall[ROWS][COLS];
  char weigths[ROWS][COLS];

  if (!(fp = fopen("input", "r"))) {
    perror("Impossibile aprire il file\n");
    exit(1);
  }

  /* Parsing del file */
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      wall[i][j] = fgetc(fp);
    }
    /* Consumo il newline */
    char c;
    if ((c = fgetc(fp) != '\n')) {
      printf("Impossibile fare il parsing. Trovato: '%x'\n", c);
      exit(1);
    };
  }
  fclose(fp);

  /* Stampo il muro */
  // for (int i = 0; i < ROWS; i++) {
  //   for (int j = 0; j < COLS; j++) {
  //     printf("%c", wall[i][j]);
  //   }
  //   printf("\n");
  // }

  int result = 0;
  int partial = 1;

  while (partial != 0) {
    partial = 0;

    memset(weigths, 0, ROWS * COLS);

    /* Bordo superiore */
    for (int i = 1; i < COLS - 1; i++) {
      if (wall[0][i] == '@') {
        weigths[0][i - 1]++;
        weigths[0][i + 1]++;
        weigths[1][i - 1]++;
        weigths[1][i]++;
        weigths[1][i + 1]++;
      }
    }

    /* Bordo inferiore */
    for (int i = 1; i < COLS - 1; i++) {
      if (wall[ROWS - 1][i] == '@') {
        weigths[ROWS - 1][i - 1]++;
        weigths[ROWS - 1][i + 1]++;
        weigths[ROWS - 2][i - 1]++;
        weigths[ROWS - 2][i]++;
        weigths[ROWS - 2][i + 1]++;
      }
    }

    /* Bordo sx */
    for (int i = 1; i < ROWS - 1; i++) {
      if (wall[i][0] == '@') {
        weigths[i - 1][0]++;
        weigths[i + 1][0]++;
        weigths[i - 1][1]++;
        weigths[i][1]++;
        weigths[i + 1][1]++;
      }
    }

    /* Bordo dx */
    for (int i = 1; i < ROWS - 1; i++) {
      if (wall[i][COLS - 1] == '@') {
        weigths[i - 1][COLS - 1]++;
        weigths[i + 1][COLS - 1]++;
        weigths[i - 1][COLS - 2]++;
        weigths[i][COLS - 2]++;
        weigths[i + 1][COLS - 2]++;
      }
    }

    /* Angoli */
    if (wall[0][0] == '@') {
      weigths[0][1]++;
      weigths[1][1]++;
      weigths[1][0]++;
    }

    if (wall[0][COLS - 1] == '@') {
      weigths[0][COLS - 2]++;
      weigths[1][COLS - 1]++;
      weigths[1][COLS - 2]++;
    }

    if (wall[ROWS - 1][0] == '@') {
      weigths[ROWS - 1][1]++;
      weigths[ROWS - 2][1]++;
      weigths[ROWS - 2][0]++;
    }

    if (wall[ROWS - 1][COLS - 1] == '@') {
      weigths[ROWS - 1][COLS - 2]++;
      weigths[ROWS - 2][COLS - 1]++;
      weigths[ROWS - 2][COLS - 2]++;
    }

    /* Itero il muro esclusi i bordi */
    for (int i = 1; i < ROWS - 1; i++) {
      for (int j = 1; j < COLS - 1; j++) {
        if (wall[i][j] == '@') {
          weigths[i - 1][j - 1]++;
          weigths[i - 1][j]++;
          weigths[i - 1][j + 1]++;
          weigths[i][j - 1]++;
          weigths[i][j + 1]++;
          weigths[i + 1][j - 1]++;
          weigths[i + 1][j]++;
          weigths[i + 1][j + 1]++;
        }
      }
    }

    /* Stampo i pesi*/
    // for (int i = 0; i < ROWS; i++) {
    //   for (int j = 0; j < COLS; j++) {
    //     printf("%d", weigths[i][j]);
    //   }
    //   printf("\n");
    // }

    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (weigths[i][j] < 4 && wall[i][j] == '@') {
          wall[i][j] = 'x';
          partial++;
        }
      }
    }

    result += partial;
  }

  printf("%d\n", result);
}
