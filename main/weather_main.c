#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_log.h>

#include "wifi_sta.h"

static const char *s_pbyTag = "weather_main";

void app_main()
{
    ESP_LOGI(s_pbyTag, "app_main...\n");

    wifi_init_sta();

    while (1)
    {
    }
    
}