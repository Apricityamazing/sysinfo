#include <inttypes.h>
#include <stdarg.h>
#include <stdcountof.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define SEC_IN_WEEK 604800
#define SEC_IN_DAY 86400
#define SEC_IN_HOUR 3600
#define SEC_IN_MINUTE 60

typedef struct {
  char model[256];
  char *uptime;
  char cores[8];
  u_int8_t threads;
  float total_memory;
  float free_memory;
  float available_memory;
  float used_memory;
} sysinfo;

char *uptime_str(float uptime) {
  u_int64_t seconds_int = (u_int64_t)uptime;
  u_int16_t weeks = seconds_int / SEC_IN_WEEK;
  u_int16_t days = (seconds_int % SEC_IN_WEEK) / SEC_IN_DAY;
  u_int8_t hours = (seconds_int % SEC_IN_WEEK % SEC_IN_DAY) / SEC_IN_HOUR;
  u_int8_t minutes = (seconds_int % SEC_IN_DAY % SEC_IN_HOUR) / SEC_IN_MINUTE;
  u_int8_t seconds = (seconds_int % SEC_IN_DAY % SEC_IN_HOUR % SEC_IN_MINUTE);

  char *uptime_str = NULL; // Set to null to get rid of garbage values
  if (weeks > 0) {
    char *weeks_str;
    asprintf(&weeks_str, "%" PRIu16 "w", weeks);
    asprintf(&uptime_str, "%s", weeks_str);
    free(weeks_str);
  }
  if (days > 0) {
    char *days_str;
    asprintf(&days_str, "%" PRIu16 "d", days);
    if (uptime_str == NULL) {
      asprintf(&uptime_str, "%s", days_str);
    } else {
      char *temp; // Stops undefined behavior from putting uptime_str in both
                  // the left and the right sides
      asprintf(&temp, "%s %s", uptime_str, days_str);
      free(uptime_str);
      uptime_str = temp; // Prevents a memory leak
    }
    free(days_str);
  }
  if (hours > 0) {
    char *hours_str;
    asprintf(&hours_str, "%" PRIu8 "h", hours);
    if (uptime_str == NULL) {
      asprintf(&uptime_str, "%s", hours_str);
    } else {
      char *temp;
      asprintf(&temp, "%s %s", uptime_str, hours_str);
      free(uptime_str);
      uptime_str = temp;
    }
    free(hours_str);
  }
  if (minutes > 0) {
    char *minutes_str;
    asprintf(&minutes_str, "%" PRIu8 "m", minutes);
    if (uptime_str == NULL) {
      asprintf(&uptime_str, "%s", minutes_str);
    } else {
      char *temp;
      asprintf(&temp, "%s %s", uptime_str, minutes_str);
      free(uptime_str);
      uptime_str = temp;
    }
    free(minutes_str);
  }
  if (seconds > 0) {
    char *seconds_str;
    asprintf(&seconds_str, "%" PRIu8 "s", seconds);
    if (uptime_str == NULL) {
      asprintf(&uptime_str, "%s", seconds_str);
    } else {
      char *temp;
      asprintf(&temp, "%s %s", uptime_str, seconds_str);
      free(uptime_str);
      uptime_str = temp;
    }
    free(seconds_str);
  }
  return uptime_str;
}

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

  FILE *uptime = fopen("/proc/uptime", "r");
  if (uptime == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  fgets(buffer, sizeof(buffer), uptime);
  char *ptr = buffer;
  strsep(&ptr, " ");
  float uptimeflt = atof(buffer);
  info.uptime = uptime_str(uptimeflt);
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
  printf("Uptime: %s\n", info.uptime);

  free(info.uptime);
  exit(EXIT_SUCCESS);
}
