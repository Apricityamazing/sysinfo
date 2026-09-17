#ifndef MEMINFO_H
#define MEMINFO_H
typedef struct {
  float total_memory;
  float free_memory;
  float available_memory;
  float used_memory;
} MEMINFO;
MEMINFO *get_mem(void);
#endif
