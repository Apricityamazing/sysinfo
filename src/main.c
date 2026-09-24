#include "cpu.h"
#include "mem.h"
#include "uptime.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdcountof.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void) {
  MEMINFO *memory = get_mem();
  CPUINFO *cpu = get_cpu();
  char *uptime = get_uptime();
  printf("CPU: %s\n", cpu->model);
  free(cpu->model);
  printf("Cores: %s\n", cpu->cores);
  free(cpu->cores);
  printf("Threads: %s\n", cpu->threads);
  free(cpu->threads);
  if (memory->total_memory >= 1024) {
    printf("Total Memory: %.2f GiB\n", (memory->total_memory / 1024));
  } else {
    printf("Total Memory: %d MiB\n", (int)memory->total_memory);
  }
  if (memory->free_memory >= 1024) {
    printf("Free Memory: %.2f GiB\n", (memory->free_memory / 1024));
  } else {
    printf("Free Memory: %d MiB\n", (int)memory->free_memory);
  }
  printf("Used Memory: %d%%\n", (int)memory->used_memory);
  printf("Uptime: %s\n", uptime);

  free(cpu);
  free(memory);
  free(uptime);
  exit(EXIT_SUCCESS);
}
