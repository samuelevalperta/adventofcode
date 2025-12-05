#include <stdio.h>
#include <stdlib.h>

#define MAX_RANGES 200

int unify_ranges(uint64_t ranges[], size_t *plen) {
  int len = *plen;
  uint8_t unified;
  int tot_unified = 0;

  for (size_t i = 0; i < len * 2; i += 2) {
    unified = 0;
    size_t j = 0;

    if (ranges[i] == 0 && ranges[i + 1] == 0) {
      continue;
    }

    for (j = 0; j < len * 2; j += 2) {
      if (j == i) {
        continue;
      }

      if (ranges[i] < ranges[j]) {
        // Inizia prima di uno esistente

        if (ranges[i + 1] < ranges[j + 1]) {
          // Finisce anche prima di uno esistente
          continue;
        } else if (ranges[i + 1] >= ranges[j] &&
                   ranges[i + 1] <= ranges[j + 1]) {
          // Inizia prima e finisce dentro
          ranges[j] = ranges[i];
          unified = 1;
        } else {
          // Inizia prima e finisce dopo
          ranges[j] = ranges[i];
          ranges[j + 1] = ranges[i + 1];
          unified = 1;
        }
      } else if (ranges[i] >= ranges[j] && ranges[i] <= ranges[j + 1]) {
        // Inizia in mezzo ad uno esistente
        if (ranges[i + 1] <= ranges[j + 1]) {
          // Inizia in mezzo e finisce in mezzo
          unified = 1;
        } else if (ranges[i + 1] > ranges[j + 1]) {
          // Inizia in mezzo e finisce dopo
          ranges[j + 1] = ranges[i + 1];
          unified = 1;
        }
      }
      if (unified) {
        ranges[i] = 0;
        ranges[i + 1] = 0;
        tot_unified++;
        break;
      }
    }
  }

  return tot_unified;
}

int main() {
  FILE *fp;

  if (!(fp = fopen("input", "r"))) {
    perror("input");
    exit(1);
  };

  // +1 goes because the fscanf reads one more line then the ranges ones
  uint64_t ranges[2 * MAX_RANGES + 1];
  size_t i = 0;

  while ((fscanf(fp, "%llu-%llu", &ranges[i], &ranges[i + 1])) == 2) {
    i += 2;
  }

  size_t total_ranges = i / 2;

  while ((unify_ranges(ranges, &total_ranges)))
    ;

  int64_t tot_ids = 0;
  for (int i = 0; i < total_ranges * 2; i += 2) {
    if (ranges[i] == 0 && ranges[i + 1] == 0) {
      continue;
    }
    tot_ids += (ranges[i + 1] - ranges[i] + 1);
  }

  printf("%llu\n", tot_ids);

  return 0;
}
