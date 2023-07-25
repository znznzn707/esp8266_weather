#ifndef MY_SNTP_H
#define MY_SNTP_H

#include <time.h>

void sntp_initialize();

void obtain_time(time_t *now, struct tm *timeinfo);

#endif // !MY_SNTP_H
