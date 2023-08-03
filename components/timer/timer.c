#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <driver/hw_timer.h>
#include "timer.h"

static const char *s_TAG = "timer";


static void timer_callback(void *arg)
{
}

void timer_init()
{
    ESP_LOGI(s_TAG, "init timer...");
    hw_timer_init(timer_callback, NULL);
    hw_timer_alarm_us(1000000, true);
}