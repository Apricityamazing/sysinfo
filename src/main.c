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
  float total_memint;
  float free_memint;
  float available_memint;
  if (meminfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, sizeof(line), meminfo) != NULL) {
    if (strstr(line, "MemTotal") != NULL) {
      char total_memory[256];
      get_value(line, total_memory, sizeof(total_memory));
      total_memint = (float)atoi(total_memory) / 1024;
    }
    if (strstr(line, "MemFree") != NULL) {
      char free_memory[256];
      get_value(line, free_memory, sizeof(free_memory));
      free_memint = (float)atoi(free_memory) / 1024;
    }
    if (strstr(line, "MemAvailable") != NULL) {
      char available_memory[256];
      get_value(line, available_memory, sizeof(available_memory));
      available_memint = (float)atoi(available_memory) / 1024;
    }
  }

  fclose(meminfo);

  float used_memory = ((total_memint - available_memint) / total_memint) * 100;
  printf("CPU: %s\n", model);
  printf("Cores: %s\n", cores);
  printf("Threads: %d\n", threads);
  if (total_memint >= 1024) {
    total_memint = (total_memint / 1024);
    printf("Total Memory: %.2f GiB\n", total_memint);
  } else {
    printf("Total Memory: %d MiB\n", (int)total_memint);
  }
  if (free_memint >= 1024) {
    free_memint = (free_memint / 1024);
    printf("Free Memory: %.2f GiB\n", free_memint);
  } else {
    printf("Free Memory: %d MiB\n", (int)free_memint);
  }
  printf("Used Memory: %d%%\n", (int)used_memory);

  return EXIT_SUCCESS;
}
