#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_http_client.h>
#include <cJSON.h>
#include "weather_api.h"

static const char *s_TAG = "weather_api";


//----------------------------------------------------------------------------------------
/**
 * @brief http事件处理函数
 * 
 * @param evt 
 * @return esp_err_t 
 */
static esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) 
    {
        case HTTP_EVENT_ERROR://错误事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED://连接成功事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT://发送头事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER://接收头事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA://接收数据事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                ESP_LOGI(s_TAG, "%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH://会话完成事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED://断开事件
            ESP_LOGI(s_TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}


//------------------------------------------------------------------------------------
/**
 * @brief 获取当天天气实况
 * 
 * @param key 私钥
 * @param location 位置 英文/中文 
 * @param language 语言
 * @param unit c 表示温度；f 表示湿度
 * @param now_resp_body 
 */
void weather_get_now(char *key, char *location, char *language, char *unit,
                     weather_now_resp_body_t *now_resp_body)
{
    char base_url[] = "http://api.seniverse.com/v3/weather";
    char url[130] = {0};
    char resp_body_json[300] = {0};

    esp_http_client_handle_t http_client_handle = NULL;
    esp_http_client_config_t http_client_config = {
        .method = HTTP_METHOD_GET,
        .event_handler = _http_event_handle,
    };

    cJSON *root = NULL;
    cJSON *results_arr = NULL;
    cJSON *result_obj = NULL;
    cJSON *now_obj = NULL;
    cJSON *text_item = NULL;
    cJSON *code_item = NULL;
    cJSON *temperature_item = NULL;

    if (!key || !location || !language || !unit)
    {
            ESP_LOGE(s_TAG, "request param error...");
            return;
    }
    sprintf(url, "%s/now.json?key=%s&location=%s&language=%s&unit=%s",
            base_url, key, location, language, unit);
    ESP_LOGI(s_TAG, "url=%s", url);

    http_client_config.url = url;
    http_client_handle = esp_http_client_init(&http_client_config);
    ESP_ERROR_CHECK( esp_http_client_perform(http_client_handle));
    esp_http_client_read(http_client_handle, resp_body_json, 300);
    ESP_LOGI(s_TAG, "resp data=%s\n", resp_body_json);
    
    ESP_ERROR_CHECK( esp_http_client_cleanup(http_client_handle) );

    if (now_resp_body)
    {
            root = cJSON_Parse(resp_body_json);
            if (root)
            {
                results_arr = cJSON_GetObjectItem(root, "results");
                result_obj = cJSON_GetArrayItem(results_arr, 0);
                now_obj = cJSON_GetObjectItem(result_obj, "now");

                text_item = cJSON_GetObjectItem(now_obj, "text");
                code_item = cJSON_GetObjectItem(now_obj, "code");
                temperature_item = cJSON_GetObjectItem(now_obj, "temperature");

                ESP_LOGI(s_TAG, "text = %s, code = %s, temperature = %s\n",
                         text_item->valuestring, code_item->valuestring, temperature_item->valuestring);

                strcpy(now_resp_body->m_text, text_item->valuestring);
                now_resp_body->m_code = atoi(code_item->valuestring);
                now_resp_body->m_temperature = atoi(temperature_item->valuestring);

                cJSON_Delete(root);
            }
    }
}

//TODO
//---------------------------------------------------------------------------------------
/**
 * @brief 获取未来三天的天气
 * 
 * @param key 私钥
 * @param location 位置 英文/中文 
 * @param daily_resp_body 
 */
void weather_get_daily(char *key, char *location, char *language,
                       weather_daily_resp_body_t *daily_resp_body)
{
}