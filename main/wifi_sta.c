#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <event_groups.h>
#include "wifi_sta.h"

#define WIFI_STA_CONED_BIT      (1 << 0)
#define WIFI_STA_DISCONED_BIT   (1 << 1)


static const char *s_pbyTag = "wifi_sta";


static EventGroupHandle_t s_hWifiStaEventGroup;
static wifi_config_t s_wifiConfig = {
    .sta = {
        .ssid = "abc",
        .password = "12345678",  
    },
};

static void wifi_event_handler(void *event_handler_arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data)
{
    if(event_base == WIFI_EVENT)
    {
        if(event_id == WIFI_EVENT_STA_START)
        {
            esp_wifi_connect(); //连接ap
        } else if(event_id == WIFI_EVENT_STA_CONNECTED)
        {
            xEventGroupSetBits(s_hWifiStaEventGroup, WIFI_STA_CONED_BIT);
        }else if(event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            xEventGroupSetBits(s_hWifiStaEventGroup, WIFI_STA_DISCONED_BIT);
        }
    }else if(event_base == IP_EVENT)
    {
        if(event_id == IP_EVENT_STA_GOT_IP)
        {
            ip_event_got_ip_t *pIpEventGotIp = (ip_event_got_ip_t *) event_data;
            ESP_LOGI(s_pbyTag, "got ip=%s\n", ip4addr_ntoa( &pIpEventGotIp->ip_info.ip ) );
        }
    }
}
/**
 * @brief wifi sta初始化
 *
 */
void wifi_init_sta()
{
    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    s_hWifiStaEventGroup = xEventGroupCreate();
    EventBits_t eventBits;

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // 创建默认事件循环
    ESP_ERROR_CHECK(esp_wifi_init(&wifiInitConfig));

    // 注册事件处理
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &s_wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start()); // 启动wifi，但还没有连接

    eventBits = xEventGroupWaitBits(s_hWifiStaEventGroup,
                                    WIFI_STA_CONED_BIT | WIFI_STA_DISCONED_BIT,
                                    pdFALSE,
                                    pdFALSE,
                                    portMAX_DELAY);

    if (eventBits & WIFI_STA_CONED_BIT)
    {
        ESP_LOGI(s_pbyTag, "wifi sta has connected to ap...\n");
    }
    else if (eventBits & WIFI_STA_DISCONED_BIT)
    {
        ESP_LOGI(s_pbyTag, "wifi sta diconnected to ap...\n");
    }

    // 卸载事件
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler));
    vEventGroupDelete(s_hWifiStaEventGroup);
    // ESP_ERROR_CHECK();
}