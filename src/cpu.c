#include "cpu.h"
#include "findvalues.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

CPUINFO *get_cpu(void) {
  CPUINFO *cpu = calloc(1, sizeof(CPUINFO));

  KeyValuePair *attributes[] = {
      create_keyvaluepair("model name", SEARCH_FIRST),
      create_keyvaluepair("cpu cores", SEARCH_FIRST),
      create_keyvaluepair("processor", SEARCH_COUNT),
  };

  uint8_t num_attributes = 3;

  find_values("/proc/cpuinfo", num_attributes, attributes);

  for (int i = 0; i < num_attributes; i++) {
    if (strcmp(attributes[i]->key, "model name") == 0) {
      if (attributes[i]->value != NULL) {
        cpu->model = realloc(cpu->model, strlen(attributes[i]->value) + 1);
      }
      strcpy(cpu->model, attributes[i]->value);
      destroy_keyvaluepair(attributes[i]);
    } else if (strcmp(attributes[i]->key, "cpu cores") == 0) {
      if (attributes[i]->value != NULL) {
        cpu->cores = realloc(cpu->cores, strlen(attributes[i]->value) + 1);
      }
      cpu->cores = strcpy(cpu->cores, attributes[i]->value);
      destroy_keyvaluepair(attributes[i]);
    } else if (strcmp(attributes[i]->key, "processor") == 0) {
      cpu->threads = realloc(cpu->threads, strlen(attributes[i]->value) + 1);
      strcpy(cpu->threads, attributes[i]->value);
      destroy_keyvaluepair(attributes[i]);
    }
  }
  return cpu;
}
