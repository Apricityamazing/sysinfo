#include "mem.h"
#include "findvalues.h"
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MEMINFO *get_mem(void) {
  MEMINFO *memory = calloc(1, sizeof(MEMINFO));
  KeyValuePair *attributes[] = {
      create_keyvaluepair("MemTotal", SEARCH_FIRST, NULL),
      create_keyvaluepair("MemFree", SEARCH_FIRST, NULL),
      create_keyvaluepair("MemAvailable", SEARCH_FIRST, NULL),
  };

  uint8_t num_attributes = sizeof(attributes) / sizeof(KeyValuePair *);

  find_values("/proc/meminfo", num_attributes, attributes);

  for (int i = 0; i < num_attributes; i++) {
    if (strcmp(attributes[i]->key, "MemTotal") == 0) {
      if (attributes[i]->value != NULL) {
        errno = 0;
        memory->total_memory = strtof(attributes[i]->value, NULL) / 1024;
        if (errno != 0)
          perror("strtof");
      }
      destroy_keyvaluepair(attributes[i]);
      continue;
    } else if (strcmp(attributes[i]->key, "MemFree") == 0) {
      if (attributes[i]->value != NULL) {
        errno = 0;
        memory->free_memory = strtof(attributes[i]->value, NULL) / 1024;
        if (errno != 0)
          perror("strtof");
      }
      destroy_keyvaluepair(attributes[i]);
      continue;
    } else if (strcmp(attributes[i]->key, "MemAvailable") == 0) {
      if (attributes[i]->value != NULL) {
        errno = 0;
        memory->available_memory = strtof(attributes[i]->value, NULL) / 1024;
        if (errno != 0)
          perror("strtof");
      }
      destroy_keyvaluepair(attributes[i]);
      continue;
    }
  }
  memory->used_memory = ((memory->total_memory - memory->available_memory) /
                         memory->total_memory) *
                        100;
  return memory;
}
