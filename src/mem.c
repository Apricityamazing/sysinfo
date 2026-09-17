#include "mem.h"
#include "getvalue.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MEMINFO *get_mem(void) {
  MEMINFO *memory = calloc(1, sizeof(MEMINFO));
  FILE *meminfo = fopen("/proc/meminfo", "r");
  char buffer[256];
  if (meminfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  while (fgets(buffer, sizeof(buffer), meminfo) != NULL) {
    if (strstr(buffer, "MemTotal") != NULL) {
      char *total_memory = get_value(buffer);
      memory->total_memory = strtof(total_memory, NULL) / 1024;
      if (errno != 0)
        perror("strtof");
      free(total_memory);
    }
    if (strstr(buffer, "MemFree") != NULL) {
      char *free_memory = get_value(buffer);
      memory->free_memory = strtof(free_memory, NULL) / 1024;
      if (errno != 0)
        perror("strtof");
      free(free_memory);
    }
    if (strstr(buffer, "MemAvailable") != NULL) {
      char *available_memory = get_value(buffer);
      memory->available_memory = strtof(available_memory, NULL) / 1024;
      if (errno != 0)
        perror("strtof");
      free(available_memory);
    }
  }
  memory->used_memory = ((memory->total_memory - memory->available_memory) /
                         memory->total_memory) *
                        100;
  fclose(meminfo);
  return memory;
}
