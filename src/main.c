#include <stdcountof.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Add uptime support
typedef struct {
  char model[256];
  char cores[8];
  int threads;
  int uptime;
  float total_memory;
  float free_memory;
  float available_memory;
  float used_memory;
} sysinfo;

void get_value(char *buffer, char *output, size_t output_size) {
  char *ptr = buffer;
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
  sysinfo info;
  memset(&info, 0,
         sizeof(info)); // Zeros out all values so no garbage values are set

  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  char buffer[256];

  if (cpuinfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, sizeof(buffer), cpuinfo) != NULL) {
    if (strstr(buffer, "model name") != NULL) {
      get_value(buffer, info.model, sizeof(info.model));
    }
    if (strstr(buffer, "processor") != NULL) {
      info.threads++;
    }
    if (strstr(buffer, "cpu cores") != NULL) {
      get_value(buffer, info.cores, sizeof(info.cores));
    }
  }

  fclose(cpuinfo);

  FILE *meminfo = fopen("/proc/meminfo", "r");
  if (meminfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, sizeof(buffer), meminfo) != NULL) {
    if (strstr(buffer, "MemTotal") != NULL) {
      char total_memory[256];
      get_value(buffer, total_memory, sizeof(total_memory));
      info.total_memory = (float)atoi(total_memory) / 1024;
    }
    if (strstr(buffer, "MemFree") != NULL) {
      char free_memory[256];
      get_value(buffer, free_memory, sizeof(free_memory));
      info.free_memory = (float)atoi(free_memory) / 1024;
    }
    if (strstr(buffer, "MemAvailable") != NULL) {
      char available_memory[256];
      get_value(buffer, available_memory, sizeof(available_memory));
      info.available_memory = (float)atoi(available_memory) / 1024;
    }
  }

  float used_memory =
      ((info.total_memory - info.available_memory) / info.total_memory) * 100;
  fclose(meminfo);

  FILE *uptime = fopen("/proc/sysinfo", "r");
  if (uptime == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  fclose(uptime);

  printf("CPU: %s\n", info.model);
  printf("Cores: %s\n", info.cores);
  printf("Threads: %d\n", info.threads);
  if (info.total_memory >= 1024) {
    info.total_memory = (info.total_memory / 1024);
    printf("Total Memory: %.2f GiB\n", info.total_memory);
  } else {
    printf("Total Memory: %d MiB\n", (int)info.total_memory);
  }
  if (info.free_memory >= 1024) {
    info.free_memory = (info.free_memory / 1024);
    printf("Free Memory: %.2f GiB\n", info.free_memory);
  } else {
    printf("Free Memory: %d MiB\n", (int)info.free_memory);
  }
  printf("Used Memory: %d%%\n", (int)used_memory);

  exit(EXIT_SUCCESS);
}
