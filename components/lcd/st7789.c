#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <driver/gpio.h>
#include <esp_log.h>

#include "st7789.h"

#define SOFT_SPI

#define DELAY_MS(ms)                    vTaskDelay(pdMS_TO_TICKS(ms))          

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

void lcd_draw(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short* color)
{

}
