#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <lvgl.h>
#include <lv_port_disp.h>
#include "gui.h"
#include "weather_img.h"
#include "weather_font.h"

static const char *s_TAG = "gui";

#define scr_act_width()        lv_obj_get_width( lv_scr_act() )
#define scr_act_height()       lv_obj_get_height( lv_scr_act() )

LV_FONT_DECLARE(lv_font_montserrat_48)
extern const lv_img_dsc_t WORD_ALIGNED_ATTR weather_unknown_img_dsc;
extern const lv_font_t weather_font ;

static lv_obj_t *s_weather_img; 
static lv_obj_t *s_time_label; 

//---------------------------------------------------------------------------------- /**
/**
 * @brief 创建界面
 * 
 */
static void gui_create()
{
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); //0x000000 = 黑色

    s_time_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(s_time_label, &lv_font_montserrat_48, LV_STATE_DEFAULT);
    lv_label_set_text(s_time_label, "#00ff80 00:00:00#");
    lv_obj_set_pos(s_time_label, lv_pct(15), lv_pct(20));
    lv_label_set_recolor(s_time_label, true);
    
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
        lv_tick_inc(5);
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    } 
}