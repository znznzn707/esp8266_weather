#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <driver/gpio.h>
#include <esp_log.h>

#include "st7789.h"

#define SOFT_SPI

#define DELAY_MS(ms)                    vTaskDelay(pdMS_TO_TICKS(ms))          

//----------------------------------------------------------------------------------
/**
 * @brief 屏幕gpio初始化
 * 
 */
static void lcd_gpio_init()
{
    gpio_config_t lcd_gpio_config = {0};
    lcd_gpio_config.mode = GPIO_MODE_OUTPUT;
    lcd_gpio_config.intr_type = GPIO_INTR_DISABLE;
    lcd_gpio_config.pull_down_en = 0;
    lcd_gpio_config.pull_up_en = 1;

    lcd_gpio_config.pin_bit_mask = (1ULL << LCD_GPIO_SCL) | (1ULL << LCD_GPIO_DC) |
                                   (1ULL << LCD_GPIO_RST) | (1ULL << LCD_GPIO_SDA);
    gpio_config(&lcd_gpio_config);
	gpio_set_level(LCD_GPIO_RST | LCD_GPIO_DC | LCD_GPIO_SDA | LCD_GPIO_SCL, 1);
}

//----------------------------------------------------------------------------------
/**
 * @brief 屏幕初始化
 * 
 */
void lcd_init()
{
    lcd_gpio_init();
}

//---------------------------------------------------------------------------------
/**
 * @brief 在指定区域填充颜色
 * 
 * @param xsta 
 * @param ysta 
 * @param xend 
 * @param yend 
 * @param color 
 */
void lcd_draw(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short* color)
{

}



//---------------------------------------------------------------------------------
/**
 * @brief spi写一字节
 * 
 * @param data 
 */
static void spi_write_byte(unsigned char data)
{

}

//---------------------------------------------------------------------------------
/**
 * @brief 写8位数据
 * 
 * @param data8 
 */
void lcd_write_data8(unsigned char data8)
{

}

//---------------------------------------------------------------------------------
/**

 * @brief 写16位数据
 * 
 * @param data16 
 */
void lcd_write_data16(unsigned short data16)
{

}

//---------------------------------------------------------------------------------
/**
 * @brief 写8位指令
 * 
 * @param cmd 
 */
void lcd_write_cmd(unsigned char cmd)
{

}

//---------------------------------------------------------------------------------
/**
 * @brief 设置起始和结束地址
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 */
void lcd_set_addr(unsigned short xsta, unsigned short ysta,
                  unsigned short xend, unsigned short yend)
{
}
