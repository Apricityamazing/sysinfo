#include <stdcountof.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_value(char *line, char *output, size_t output_size) {
  char line_copy[256];
  strcpy(line_copy, line);

  char *ptr = line_copy;
  // Removes leading model name :
  strsep(&ptr, ":");
  char *value = strsep(&ptr, "\n");
  if (value != NULL) {
    // Removes leading whitespace
    while (*value == ' ' || *value == '\t') {
      value++;
    }
  }
  strncpy(output, value, output_size - 1);
  output[output_size - 1] = '\0'; // Makes sure its null terminated
}

int main(void) {
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  char line[256];
  char model[256];
  char cores[8];
  int threads = 0;
  if (cpuinfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, sizeof(line), cpuinfo) != NULL) {
    if (strstr(line, "model name") != NULL) {
      get_value(line, model, sizeof(model));
    }
    if (strstr(line, "processor") != NULL) {
      threads++;
    }
    if (strstr(line, "cpu cores") != NULL) {
      get_value(line, cores, sizeof(cores));
    }
  }

  fclose(cpuinfo);

  FILE *meminfo = fopen("/proc/meminfo", "r");
  char total_memory[256];
  char free_memory[256];
  char available_memory[256];

  if (meminfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, sizeof(line), meminfo) != NULL) {
    if (strstr(line, "MemTotal") != NULL) {
      get_value(line, total_memory, sizeof(total_memory));
    }
    if (strstr(line, "MemFree") != NULL) {
      get_value(line, free_memory, sizeof(total_memory));
    }
    if (strstr(line, "MemAvailable") != NULL) {
      get_value(line, available_memory, sizeof(total_memory));
    }
  }

  printf("CPU: %s\n", model);
  printf("Cores: %s\n", cores);
  printf("Threads: %d\n", threads);
  printf("Total Memory: %s\n", total_memory);
  printf("Free Memory: %s\n", free_memory);
  printf("Available Memory: %s \n", available_memory);
  return EXIT_SUCCESS;
}
