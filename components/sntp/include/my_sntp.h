#ifndef MY_SNTP_H
#define MY_SNTP_H

#include <time.h>

//------------------------------------------------------------------------------------
/**
 * @brief sntp初始化
 * 
 */
void sntp_initialize();


//------------------------------------------------------------------------------------
/**
 * @brief 获取时间，参数可传一个
 * 
 * @param now 时间戳
 * @param timeinfo 时间结构体
 */
void obtain_time(time_t *now, struct tm *timeinfo);

extern volatile time_t g_time;

#endif // !MY_SNTP_H
