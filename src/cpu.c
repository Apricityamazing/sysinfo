#include "cpu.h"
#include "getvalue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CPUINFO *get_cpu(void) {
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  if (cpuinfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  CPUINFO *cpu = calloc(1, sizeof(CPUINFO));
  char buffer[256];

  while (fgets(buffer, sizeof(buffer), cpuinfo) != NULL) {
    if (cpu->model == NULL && strstr(buffer, "model name") != NULL) {
      cpu->model = get_value(buffer);
    }
    if (strstr(buffer, "processor") != NULL) {
      cpu->threads++;
    }
    if (cpu->cores == NULL && strstr(buffer, "cpu cores") != NULL) {
      cpu->cores = get_value(buffer);
    }
  }
  fclose(cpuinfo);
  return cpu;
}
