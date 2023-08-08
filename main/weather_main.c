#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>

#include "wifi_sta.h"
#include "my_sntp.h"
#include "weather_api.h"
#include "weather.h"
#include "lcd_config.h"
#include "timer.h"
#include "gui.h"

static const char *s_pbyTag = "weather_main";

extern volatile time_t g_time;


void time_task()
{
    time_t now = 0;
    struct tm timeinfo = {0};
    char strftime_datebuf[32] = {0}; // 保存日期
    char strftime_timebuf[32] = {0}; // 保存日期

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        // time(&now);
        // localtime_r(&now, &timeinfo);
        obtain_time(&now, &timeinfo);
        strftime(strftime_datebuf, sizeof(strftime_datebuf), "%F", &timeinfo); // 取出日期/年月日
        strftime(strftime_timebuf, sizeof(strftime_timebuf), "%T", &timeinfo); // 取出时间/时分秒
        ESP_LOGI(s_pbyTag, "year=%d", timeinfo.tm_year);
        ESP_LOGI(s_pbyTag, "now1=%s", strftime_datebuf);
        ESP_LOGI(s_pbyTag, "now2=%s", strftime_timebuf);
        // ESP_LOGI(s_pbyTag, "now=%ld", now);
        // ESP_LOGI(s_pbyTag, "year=%d, mon=%d, mday=%d, hour=%d, min=%d, sec=%d",
                //  timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday,
                //  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }
}

#if 0
static void lv_demo_task(void *pvParameters)
{
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); //0x000000 = 黑色
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);                //滚动模式
    lv_label_set_recolor(label, true);                                  //使能颜色修改
    lv_obj_set_pos(label, 10, 50);                                      //文本位置
    lv_obj_set_size(label, 240, 60);                                    //标签框大小
    LV_FONT_DECLARE(lv_font_montserrat_16)                              //设置字体
    lv_label_set_text(label, "#ff0080 abcde##00ff80 12345#");           //字体颜色
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
}
#endif // 0

void app_main()
{
    ESP_LOGI(s_pbyTag, "app_main...\n");

    wifi_init_sta();
    lcd_init();
    gui_init();
    
    xTaskCreate(weather_task, "weather_task", 2048, NULL, 11, NULL);
   
    sntp_initialize();
    obtain_time(&g_time, NULL);
    timer_init();

    // xTaskCreate(time_task, "time_task", 2048, NULL, 10, NULL);
    xTaskCreate(gui_task, "gui_task", 2048, NULL, 10, NULL);
    
    
    while (1)
    {
    }
    
}