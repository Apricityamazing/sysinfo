#include "uptime.h"
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char *get_uptime(void) {
  FILE *uptime = fopen("/proc/uptime", "r");
  char buffer[256];
  fgets(buffer, sizeof(buffer), uptime);
  char *ptr = buffer;
  strsep(&ptr, " ");
  float uptimeflt = strtof(buffer, NULL);
  if (errno != 0)
    perror("strtof");
  char *str = uptime_str(uptimeflt);
  fclose(uptime);
  return str;
}

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
