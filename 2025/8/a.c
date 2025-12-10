#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SIZE <= 2^31
#define SIZE 1000
#define MAX_UNION 1000

void *xmalloc(size_t size) {
  void *p = malloc(size);
  if (!p) {
    fprintf(stderr, "Malloc: unable to allocate %zu bytes\n", size);
    exit(EXIT_FAILURE);
  }
  return p;
}

typedef struct {
  int x;
  int y;
  int z;
} Point;

uint64_t calculate_distance(Point *p1, Point *p2) {
  long dx = p1->x - p2->x;
  long dy = p1->y - p2->y;
  long dz = p1->z - p2->z;
  return (uint64_t)(dx * dx + dy * dy + dz * dz);
}

// A Node of a min-heap data structure
typedef struct {
  uint64_t distance;
  uint32_t idx1;
  uint32_t idx2;
} Node;

void heapify_up(Node *heap, size_t i) {
  if (i == 0) {
    return;
  }

  size_t parent = (i - 1) / 2;
  if (heap[i].distance < heap[parent].distance) {
    // Swap
    Node tmp = heap[i];
    heap[i] = heap[parent];
    heap[parent] = tmp;
    heapify_up(heap, parent);
  }
}

void heapify_down(Node *heap, size_t *len, size_t idx) {
  size_t smallest = idx;
  size_t left = 2 * idx + 1;
  size_t right = 2 * idx + 2;
  Node tmp;

  if (left < *len && heap[left].distance < heap[idx].distance) {
    smallest = left;
  }

  if (right < *len && heap[right].distance < heap[smallest].distance) {
    smallest = right;
  }

  if (smallest != idx) {
    tmp = heap[idx];
    heap[idx] = heap[smallest];
    heap[smallest] = tmp;
    heapify_down(heap, len, smallest);
  }
}

void heap_insert(Node *heap, Node *to_insert, size_t *len) {
  heap[(*len)++] = *to_insert;
  heapify_up(heap, *len - 1);
}

Node *heap_extract_min(Node *heap, size_t *len, Node *node) {
  if (*len == 0) {
    return 0;
  }

  size_t last = *len - 1;
  *node = heap[0];
  heap[0] = heap[last];

  *len = last;

  heapify_down(heap, len, 0);

  return node;
}

/* Disjoint-Set Forest
We use a dsf where each node is an int.
If the node value is negative then the node is a root of a set and the
inverse of the value is the set size. Otherwise the value represent the
representative of the set.
*/

int *dsf_init(size_t len) {
  int *p = xmalloc(sizeof(int) * len);
  /*
  We need all the representative to be root with size 1 => rep = -1
  Memset usually works for single bytes, but (unsigned char)(-1) = 0xFF and
  (int)(-1) = 0XFFFFFFFF
  */
  memset(p, -1, sizeof(int) * len);
  return p;
}

int dsf_find_rep(int *dsf, size_t len, size_t idx) {
  if (!(idx < len)) {
    fprintf(stderr, "Invalid index to find\n");
    exit(EXIT_FAILURE);
  }

  if (dsf[idx] < 0) {
    return idx;
  }

  // Path compression
  return dsf[idx] = dsf_find_rep(dsf, len, dsf[idx]);
}

int dsf_join(int *dsf, size_t len, size_t i, size_t j) {
  if (!(i < len && j < len)) {
    fprintf(stderr, "Invalid index while trying to join\n");
    exit(EXIT_FAILURE);
  }

  size_t repi = dsf_find_rep(dsf, len, i);
  size_t repj = dsf_find_rep(dsf, len, j);

  if (repi == repj) {
    return 0;
  }

  /*
    Here both dsf[repj] and dsf[repi] are root of a some different sets because
    otherwise they wouldn't be representative. We keep the left one to be the
    representative of the union set. We add the size of the second set to the
    first one and then we set the seecond one to point to the first one.

    Note:
    dsf[repi] = dsf[repi] - (-dsf[repj]) = dsf[repi] + dsf[repj]
  */
  if (dsf[repi] < dsf[repj]) {
    dsf[repi] += dsf[repj];
    dsf[repj] = repi;
  } else {
    dsf[repj] += dsf[repi];
    dsf[repi] = repj;
  }

  return 1;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s inputfile\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "r");

  if (!fp) {
    perror("Open input file\n");
    exit(1);
  }

  Point *points = xmalloc(SIZE * sizeof(*points));
  size_t last_idx = 0;

  if (fscanf(fp, "%d,%d,%d", &points[last_idx].x, &points[last_idx].y,
             &points[last_idx].z) != 3) {
    fprintf(stderr, "Error while reading the first point\n");
    exit(EXIT_FAILURE);
  };
  last_idx++;

  size_t unique_distances = SIZE * (SIZE - 1) / 2;
  size_t heap_len = 0;
  Node *heap = xmalloc(unique_distances * sizeof(Node));
  Node *current_node = xmalloc(sizeof(Node));

  while (fscanf(fp, "%d,%d,%d", &points[last_idx].x, &points[last_idx].y,
                &points[last_idx].z) == 3) {
    size_t points_len = last_idx + 1;
    for (size_t i = 0; i < points_len; i++) {

      if (last_idx == i) {
        continue;
      }

      current_node->distance =
          calculate_distance(&points[last_idx], &points[i]);

      current_node->idx1 = last_idx;
      current_node->idx2 = i;

      heap_insert(heap, current_node, &heap_len);
    }

    last_idx++;
  }

  int *dsf = dsf_init(SIZE);
  for (size_t i = 0; i < MAX_UNION; i++) {
    if ((current_node = heap_extract_min(heap, &heap_len, current_node)) == 0) {
      fprintf(stderr, "Something went wrong during min extraction\n");
      exit(EXIT_FAILURE);
    };

    dsf_join(dsf, SIZE, current_node->idx1, current_node->idx2);
  }

  int val1 = 0, val2 = 0, val3 = 0;
  uint64_t tot = 0;

  for (size_t i = 0; i < SIZE; i++) {
    if (dsf[i] < 0) {
      int set_size = -dsf[i];

      if (set_size >= val1) {
        val3 = val2;
        val2 = val1;
        val1 = set_size;
      } else if (set_size >= val2) {
        val3 = val2;
        val2 = set_size;
      } else if (set_size >= val3) {
        val3 = set_size;
      }
    }
  }

  tot = val1 * val2 * val3;
  printf("%llu\n", tot);

  free(heap);
  free(dsf);
  free(current_node);

  return 0;
}
