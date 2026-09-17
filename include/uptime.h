#ifndef UPTIME_H
#define UPTIME_H

#define SEC_IN_WEEK 604800
#define SEC_IN_DAY 86400
#define SEC_IN_HOUR 3600
#define SEC_IN_MINUTE 60

char *uptime_str(float uptime);
char *get_uptime(void);
#endif
