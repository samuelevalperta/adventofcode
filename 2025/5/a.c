#include <stdio.h>
#include <stdlib.h>

#define MAX_RANGES 200

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
  uint64_t fresh_ingredients = 0;

  uint64_t id = ranges[total_ranges * 2];
  for (size_t i = 0; i < total_ranges * 2; i += 2) {
    if (id >= ranges[i] && id <= ranges[i + 1]) {
      fresh_ingredients++;
      break;
    }
  }

  while ((fscanf(fp, "%llu", &id) == 1)) {
    for (size_t i = 0; i < total_ranges * 2; i = i + 2) {
      if (id >= ranges[i] && id <= ranges[i + 1]) {
        fresh_ingredients++;
        break;
      }
    }
  }

  printf("%llu\n", fresh_ingredients);

  return 0;
}
