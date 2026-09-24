#include "findvalues.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
typedef enum {
  SEARCH_FIRST,
  SEARCH_COUNT,
} SearchMode;

typedef struct {
  char *key;
  char *value;
  SearchMode mode;
} KeyValuePairs;
*/

KeyValuePair *create_keyvaluepair(const char *key, SearchMode mode) {
  KeyValuePair *kvp = calloc(1, sizeof(KeyValuePair));
  kvp->key = key;
  kvp->mode = mode;
  return kvp;
}

void destroy_keyvaluepair(KeyValuePair *keyvaluepair) {
  free(keyvaluepair->value);
  free(keyvaluepair);
}

char *search_first(char *filepath, char *buffer, size_t bufsize,
                   const char *key) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, bufsize, file) != NULL) {
    if (strstr(buffer, key)) {
      char *ptr = buffer;
      // Removes leading 'key :'
      strsep(&ptr, ":");
      char *value = strsep(&ptr, "\n");
      if (value != NULL) {
        // Removes leading whitespace
        while (*value == ' ' || *value == '\t') {
          value++;
        }
      }
      char *output = malloc(strlen(value) + 1);
      strcpy(output, value);
      return output;
    }
  }
  return NULL;
}

char *search_count(char *filepath, char *buffer, size_t bufsize,
                   const char *key) {
  FILE *file = fopen(filepath, "r");
  int i = 0;
  if (file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, bufsize, file)) {
    if (strstr(buffer, key)) {
      i++;
    }
  }
  char count[16];
  snprintf(count, sizeof(count), "%d", i);
  // Allocates memory which needs to be freed
  char *output = malloc(strlen(count) + 1);
  strcpy(output, count);
  return output;
}

void find_values(char *filepath, int num_pairs, KeyValuePair *keys[]) {
  for (int i = 0; i < num_pairs; i++) {
    char buffer[256];
    char *found_value;
    switch (keys[i]->mode) {
    case SEARCH_FIRST:
      found_value =
          search_first(filepath, buffer, sizeof(buffer), keys[i]->key);
      keys[i]->value = found_value;
      break;
    case SEARCH_COUNT:
      found_value =
          search_count(filepath, buffer, sizeof(buffer), keys[i]->key);
      keys[i]->value = found_value;
      break;
    default:
      fprintf(stderr, "Invalid mode argument \n");
      break;
    }
  }
}
