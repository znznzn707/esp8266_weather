#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <lvgl.h>
#include <lv_port_disp.h>
#include <time.h>
#include "gui.h"
#include "my_sntp.h"
#include "weather.h"
#include "weather_white_img.h"
#include "weather_font.h"

static const char *s_TAG = "gui";

extern volatile time_t g_time;
extern volatile weather_now_resp_body_t g_now_resp_body;
LV_FONT_DECLARE(lv_font_montserrat_48)
extern const lv_font_t weather_font ;

// static lv_obj_t *s_weather_img; 
static lv_obj_t *s_time_label; 
static lv_obj_t *s_date_label; 
static lv_obj_t *s_week_label; 
static lv_obj_t *s_weather_label; 

static const char *s_text_color_p = "#00c779 "; //绿豆沙

static void get_week(char *week_buf, uint8_t week_num)
{
    const char *week_enum[] = {"日", "一", "二", "三", "四", "五", "六"};
    memcpy(week_buf, week_enum[week_num], 4);
}

static void display_date_time()
{
    char time_label_buf[20] = {0};
    char date_label_buf[25] = {0};
    char week_label_buf[20] = {0};
    char week_buf[4] = {0};
    struct tm timeinfo = {0};
    uint8_t color_len = strlen(s_text_color_p);
    
    memcpy(time_label_buf, s_text_color_p, color_len);
    memcpy(date_label_buf, s_text_color_p, color_len);
    memcpy(week_label_buf, s_text_color_p, color_len);
    
    localtime_r(&g_time, &timeinfo);
    strftime(time_label_buf + color_len, sizeof(time_label_buf) - color_len, "%H : %M#", &timeinfo);
    strftime(date_label_buf + color_len, sizeof(date_label_buf) - color_len, "%m/%d/%G#", &timeinfo);
    ESP_LOGI(s_TAG, "date_label_buf=%s", date_label_buf);
    
    get_week(week_buf, timeinfo.tm_wday);
    sprintf(week_label_buf + color_len, "周%s#", week_buf);
    ESP_LOGI(s_TAG, "week_label_buf=%s", week_label_buf);

    //时分秒
    if( !s_time_label )
    {
        s_time_label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(s_time_label, &lv_font_montserrat_48, LV_STATE_DEFAULT);
        lv_obj_set_pos(s_time_label, lv_pct(30), lv_pct(15));
        lv_label_set_recolor(s_time_label, true);
    }
    lv_label_set_text(s_time_label, time_label_buf);
    
    //月日年
    if( !s_date_label )
    {
        s_date_label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(s_date_label, &weather_font, LV_STATE_DEFAULT);
        lv_obj_align_to(s_date_label, s_time_label, LV_ALIGN_OUT_BOTTOM_LEFT, -30, 40);
        lv_label_set_recolor(s_date_label, true);
    }
    lv_label_set_text(s_date_label, date_label_buf);
    
    // 星期
    if (!s_week_label)
    {
        s_week_label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(s_week_label, &weather_font, LV_STATE_DEFAULT);
        lv_obj_align_to(s_week_label, s_date_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
        lv_label_set_recolor(s_week_label, true);
    }
    lv_label_set_text(s_week_label, week_label_buf);
}

static void display_weather()
{
    #if 0
    if (!s_weather_img)
    {
        s_weather_img = lv_img_create(lv_scr_act());
        lv_obj_set_pos(s_weather_img, lv_pct(70), lv_pct(50));
        lv_obj_set_size(s_weather_img, 50, 50);
    }
    lv_img_set_src(s_weather_img, &GET_IMG_DSC(g_now_resp_body.m_code));
    #endif // 0
    char weather_label_buf[50] = {0};
    uint8_t color_len = strlen(s_text_color_p);

    memcpy(weather_label_buf, s_text_color_p, color_len);

    ESP_LOGI(s_TAG, "m_code=%d, m_temp=%d, m_text=%s", g_now_resp_body.m_code,
             g_now_resp_body.m_temperature, g_now_resp_body.m_text);
    sprintf(weather_label_buf + color_len, "%s %d°C#", g_now_resp_body.m_text,
            g_now_resp_body.m_temperature);
    ESP_LOGI(s_TAG, "weather_label_buf=%s", weather_label_buf);

    if( !s_weather_label )
    {
        s_weather_label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(s_weather_label, &weather_font, LV_STATE_DEFAULT);
        lv_obj_align_to(s_weather_label, s_week_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        lv_label_set_recolor(s_weather_label, true);
    }
    lv_label_set_text(s_weather_label, weather_label_buf);
}
//---------------------------------------------------------------------------------- /**
/**
 * @brief 创建界面
 * 
 */
static void gui_create()
{

    //黑色背景
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); 
}

//---------------------------------------------------------------------------------- /**
/**
 * @brief 更新界面
 * 
 */
static void gui_update()
{

    display_date_time();
    display_weather();
}

//---------------------------------------------------------------------------------- /**
/**
 * @brief 初始化lvgl
 * 
 */
void gui_init()
{
    lv_init();
    lv_port_disp_init();
}

//---------------------------------------------------------------------------------- /**
/**
 * @brief gui任务
 * 
 */
void gui_task()
{
    uint32_t gui_delay_ms = 1000 * 30;   //30s
    gui_create();

    while (1)
    {
        gui_update();
        lv_tick_inc(gui_delay_ms);
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(gui_delay_ms));
    } 
}