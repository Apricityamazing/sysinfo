#ifndef CPU_H
#define CPU_H
#include <sys/types.h>
typedef struct {
  char *model;
  char *cores;
  char *threads;
} CPUINFO;

CPUINFO *get_cpu(void);
void destroy_cpu(CPUINFO *cpu);
#endif
