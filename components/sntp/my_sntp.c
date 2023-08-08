#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>
#include <lwip/apps/sntp.h>
#include "my_sntp.h"

static const char *s_TAG = "my_sntp";
volatile time_t g_time;

//------------------------------------------------------------------------------------
/**
 * @brief sntp初始化
 * 
 */
void sntp_initialize()
{
    ESP_LOGI(s_TAG, "init sntp...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // sntp_setservername(0, "pool.ntp.org");
    sntp_setservername(0, "1.cn.pool.ntp.org");
    sntp_init();

    //设置东八区
    setenv("TZ", "CST-8", 1);
    tzset();

    g_time = 0;
}



//------------------------------------------------------------------------------------
/**
 * @brief 获取时间，参数可传一个
 * 
 * @param now 时间戳
 * @param timeinfo 时间结构体
 */
void obtain_time(time_t *now, struct tm *timeinfo)
{
    time_t now_tmp = 0;
    struct tm timeinfo_tmp = {0};

    do
    {
        time(&now_tmp);
        localtime_r(&now_tmp, &timeinfo_tmp);
        vTaskDelay(pdMS_TO_TICKS(500));
    } while (timeinfo_tmp.tm_year == 70);

    if (now)
    {
        *now = now_tmp;
    }
    if (timeinfo)
    {
        *timeinfo = timeinfo_tmp;
    }
}