#ifndef CPU_H
#define CPU_H
#include <sys/types.h>
typedef struct {
  char *model;
  char *cores;
  u_int8_t threads;
} CPUINFO;

CPUINFO *get_cpu(void);
#endif
