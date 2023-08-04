#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <lvgl.h>
#include <lv_port_disp.h>
#include "gui.h"
#include "weather_img.h"

static const char *s_TAG = "gui";

#define scr_act_width()        lv_obj_get_width( lv_scr_act() )
#define scr_act_height()       lv_obj_get_height( lv_scr_act() )

extern const lv_img_dsc_t WORD_ALIGNED_ATTR weather_unknown_img_dsc;
static lv_obj_t *s_weather_img; 

//---------------------------------------------------------------------------------- /**
/**
 * @brief 创建界面
 * 
 */
static void gui_create()
{
    int8_t height_ratio = 15;
    int8_t upper_height = scr_act_height() * height_ratio / 100;
    
    ESP_LOGI(s_TAG, "width=%d, height=%d", scr_act_width(), scr_act_height());
    
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0); //0x000000 = 黑色

    // lv_obj_t *label = lv_label_create(lv_scr_act());
    // lv_obj_set_pos(label, 10, 50);                                      //文本位置
    // lv_obj_set_size(label, 240, 60);                                    //标签框大小
    // lv_label_set_text(label, "#ff0080 abcde##00ff80 12345#");           //字体颜色
    // lv_label_set_recolor(label, true);                                  //使能颜色修改

    s_weather_img = lv_img_create(lv_scr_act());

    // lv_img_set_src(s_weather_img, &GET_IMG_DSC(9));
    lv_img_set_src(s_weather_img, &GET_UNKNOWN_IMG_DSC);
    lv_obj_align(s_weather_img, LV_ALIGN_CENTER, 0, 0);

}

//---------------------------------------------------------------------------------- /**
/**
 * @brief 更新界面
 * 
 */
static void gui_update()
{
    static int8_t weather_img_cnt = 0;

    lv_img_set_src(s_weather_img, &GET_IMG_DSC(weather_img_cnt++));
    
    if(weather_img_cnt >= IMG_DSC_CNT)
    {
        weather_img_cnt = 0;
    }
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