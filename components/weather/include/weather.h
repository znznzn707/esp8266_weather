#ifndef WEATHER_H_
#define WEATHER_H_

#include "weather_api.h"

void weather_task();

extern volatile weather_now_resp_body_t g_now_resp_body;

#endif // !WEATHER_H_
