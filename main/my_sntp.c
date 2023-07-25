#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>
#include <lwip/apps/sntp.h>
#include "my_sntp.h"

static const char *s_TAG = "my_sntp";


void sntp_initialize()
{
    ESP_LOGI(s_TAG, "init sntp...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    //设置东八区
    setenv("TZ", "CST-8", 1);
    tzset();
}

void obtain_time(time_t *now, struct tm *timeinfo)
{
    time_t now_tmp = 0;

    time(&now_tmp);
    if (now)
    {
        *now = now_tmp;
    }
    if (timeinfo)
    {
        localtime_r(&now_tmp, timeinfo);
    }
}