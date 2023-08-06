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
#include "weather_img.h"
#include "weather_font.h"

static const char *s_TAG = "gui";

extern volatile time_t g_time;
LV_FONT_DECLARE(lv_font_montserrat_48)
extern const lv_font_t weather_font ;

static lv_obj_t *s_weather_img; 
static lv_obj_t *s_time_label; 
static lv_obj_t *s_date_label; 
// static const char *s_text_color_p = "#ffffff "; //白色
static const char *s_text_color_p = "#00c779 "; //绿豆沙

static void display_date_time()
{
    char time_label_buf[20] = {0};
    char date_label_buf[25] = {0};
    struct tm timeinfo = {0};
    uint8_t color_len = strlen(s_text_color_p);
    
    memcpy(time_label_buf, s_text_color_p, color_len);
    memcpy(date_label_buf, s_text_color_p, color_len);
    localtime_r(&g_time, &timeinfo);
    strftime(time_label_buf + color_len, sizeof(time_label_buf) - color_len, "%T#", &timeinfo);
    strftime(date_label_buf + color_len, sizeof(date_label_buf) - color_len, "%m月%d日#", &timeinfo);
    ESP_LOGI(s_TAG, "date_label_buf=%s", date_label_buf);
    
    //时分秒
    if( !s_time_label )
    {
        s_time_label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(s_time_label, &lv_font_montserrat_48, LV_STATE_DEFAULT);
        lv_obj_set_pos(s_time_label, lv_pct(10), lv_pct(15));
        lv_label_set_recolor(s_time_label, true);
    }
    lv_label_set_text(s_time_label, time_label_buf);
    
    //月日周
    if( !s_date_label )
    {
        s_date_label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(s_date_label, &weather_font, LV_STATE_DEFAULT);
        lv_obj_align_to(s_date_label, s_time_label, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 20);
        lv_label_set_recolor(s_date_label, true);
    }
    lv_label_set_text(s_date_label, date_label_buf);
}

static void display_weather()
{

}
//---------------------------------------------------------------------------------- /**
/**
 * @brief 创建界面
 * 
 */
static void gui_create()
{

    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); //0x000000 = 黑色

    // s_weather_img = lv_img_create(lv_scr_act());

    // lv_img_set_src(s_weather_img, &GET_UNKNOWN_IMG_DSC);
    // lv_obj_set_pos(s_weather_img, 240-60, 120);

    // lv_obj_t *font_label = lv_label_create(lv_scr_act());
    // lv_obj_set_style_text_font(font_label, &weather_font, LV_STATE_DEFAULT);
    // lv_label_set_text(font_label, "#ff0080 晴间多云大部阴雷阵雨伴有冰雹小中大暴特冻夹雪浮尘扬沙尘龙卷冷热未知强雾霾风飓热带°C0123456789.:#");
    // lv_label_set_long_mode(font_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // lv_obj_set_pos(font_label, 0, 200);
    // lv_obj_set_size(font_label, 240, 40);                                    //标签框大小
    // lv_label_set_recolor(font_label, true);                                  //使能颜色修改
}

//---------------------------------------------------------------------------------- /**
/**
 * @brief 更新界面
 * 
 */
static void gui_update()
{
    // static int8_t weather_img_cnt = 0;

    // lv_img_set_src(s_weather_img, &GET_IMG_DSC(weather_img_cnt++));
    
    // if(weather_img_cnt >= IMG_DSC_CNT)
    // {
    //     weather_img_cnt = 0;
    // }
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
    gui_create();

    while (1)
    {
        gui_update();
        lv_tick_inc(3);
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(3));
    } 
}