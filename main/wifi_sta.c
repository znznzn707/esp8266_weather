#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <event_groups.h>
#include <esp_smartconfig.h>
#include "wifi_sta.h"

#define WIFI_STA_CONED_BIT      (1 << 0)
#define WIFI_STA_DISCONED_BIT   (1 << 1)


static const char *s_pbyTag = "wifi_sta";

static EventGroupHandle_t s_hWifiStaEventGroup;

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
    }else if(event_base == SC_EVENT)
    {
        if(event_id == SC_EVENT_GOT_SSID_PSWD)
        {
            wifi_config_t wifi_config = {0};
            smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
            memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
            memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
            ESP_LOGI(s_pbyTag, "got ssid=%s\n", evt->ssid );
            ESP_LOGI(s_pbyTag, "got password=%s\n", evt->password );
            
            ESP_ERROR_CHECK( esp_wifi_disconnect() );
            ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
            ESP_ERROR_CHECK( esp_wifi_connect() ); //连接ap
        }
    }
}


static void smart_init_config()
{
    smartconfig_start_config_t smartconfig_start_config = SMARTCONFIG_START_CONFIG_DEFAULT();

    EventBits_t eventBits;

    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_AIRKISS) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) );
    ESP_ERROR_CHECK( esp_smartconfig_start(&smartconfig_start_config) ) ;
    ESP_LOGI(s_pbyTag, "smart config started...\n");

    eventBits = xEventGroupWaitBits(s_hWifiStaEventGroup,
                                    WIFI_STA_CONED_BIT | WIFI_STA_DISCONED_BIT,
                                    pdTRUE,
                                    pdFALSE,
                                    portMAX_DELAY);

    if (eventBits & WIFI_STA_CONED_BIT)
    {
        ESP_LOGI(s_pbyTag, "smart config has connected to ap...\n");
    }
    else if (eventBits & WIFI_STA_DISCONED_BIT)
    {
        ESP_LOGI(s_pbyTag, "smart config diconnected to ap...\n");
    }
    ESP_ERROR_CHECK(esp_event_handler_unregister( SC_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler) );
}

/**
 * @brief wifi sta初始化
 *
 */
void wifi_init_sta()
{
    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t wifiConfig = {
        .sta = {
            .ssid = "abc",
            .password = "12345678",
        },
    };

    s_hWifiStaEventGroup = xEventGroupCreate();
    EventBits_t eventBits;

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // 创建默认事件循环
    ESP_ERROR_CHECK(esp_wifi_init(&wifiInitConfig));

    // 注册事件处理
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start()); // 启动wifi，但还没有连接
    ESP_LOGI(s_pbyTag, "wifi init sta started...\n");

    eventBits = xEventGroupWaitBits(s_hWifiStaEventGroup,
                                    WIFI_STA_CONED_BIT | WIFI_STA_DISCONED_BIT,
                                    pdTRUE,
                                    pdFALSE,
                                    portMAX_DELAY);

    if (eventBits & WIFI_STA_CONED_BIT)
    {
        ESP_LOGI(s_pbyTag, "wifi init sta has connected to ap...\n");
    }
    else if (eventBits & WIFI_STA_DISCONED_BIT)
    {
        ESP_LOGI(s_pbyTag, "wifi init sta diconnected to ap...\n");
        smart_init_config();
    }

    // 卸载事件
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler));
    vEventGroupDelete(s_hWifiStaEventGroup);
    
    vTaskDelay( pdMS_TO_TICKS(1000) );
}