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

    lcd_gpio_config.pin_bit_mask = (1ULL << LCD_GPIO_SCL);
    gpio_config(&lcd_gpio_config);

    lcd_gpio_config.pin_bit_mask = (1ULL << LCD_GPIO_SDA);
    gpio_config(&lcd_gpio_config);

    lcd_gpio_config.pin_bit_mask = (1ULL << LCD_GPIO_DC);
    gpio_config(&lcd_gpio_config);

    lcd_gpio_config.pin_bit_mask = (1ULL << LCD_GPIO_RST);
    gpio_config(&lcd_gpio_config);

	gpio_set_level(LCD_GPIO_RST | LCD_GPIO_DC | LCD_GPIO_SDA | LCD_GPIO_SCL, 1);
}


//----------------------------------------------------------------------------------
/**
 * @brief lcd内部配置
 * 
 */
static void lcd_config_init()
{
    LCD_RST_LOW();    //复位
    DELAY_MS(100);
	LCD_RST_HIGH();
    DELAY_MS(100);

    lcd_write_cmd(LCD_CMD_SLPOUT);     //退出睡眠
    DELAY_MS(120);


    lcd_write_cmd(LCD_CMD_MADCTL);     
    lcd_write_data8(0x00);
    lcd_write_cmd(LCD_CMD_COLMOD);     
    lcd_write_data8(0x05);       //pixel 16bit

    lcd_write_cmd(LCD_CMD_PORCTRL);   //屏参设置  
    lcd_write_data8(0x0C);            //BPA
    lcd_write_data8(0x0C);            //FPA
    lcd_write_data8(0x00);            //PSEN
    lcd_write_data8(0x33);            //bit[7:4]BPB, bit[3:0]FPB
    lcd_write_data8(0x33);            //bit[7:4]BPC, bit[3:0]FPC

    lcd_write_cmd(LCD_CMD_GCTR);   //设置VGH,VGL 
    lcd_write_data8(0x35);            
    lcd_write_cmd(LCD_CMD_VCOMS);   
    lcd_write_data8(0x32);      //VCOM 1.35V     
    lcd_write_cmd(LCD_CMD_VDVVRHEN);   
    lcd_write_data8(0x01);     
    lcd_write_cmd(LCD_CMD_VRHS);   
    lcd_write_data8(0x15);     //gvdd 4.6+( vcom+vcom offset+0.5vdv)
    lcd_write_cmd(LCD_CMD_VDVS);   
    lcd_write_data8(0x20);     //vdv=0v
    lcd_write_cmd(LCD_CMD_FRCTRL);   
    lcd_write_data8(0x0F);     //FR=60hz

	lcd_write_cmd(LCD_CMD_PWCTRL1);			
	lcd_write_data8(0xA4);  //固定
	lcd_write_data8(0xA1); 

	lcd_write_cmd(LCD_CMD_PVGAMCTRL);
	lcd_write_data8(0xD0);   
	lcd_write_data8(0x08);   
	lcd_write_data8(0x0E);   
	lcd_write_data8(0x09);   
	lcd_write_data8(0x09);   
	lcd_write_data8(0x05);   
	lcd_write_data8(0x31);   
	lcd_write_data8(0x33);   
	lcd_write_data8(0x48);   
	lcd_write_data8(0x17);   
	lcd_write_data8(0x14);   
	lcd_write_data8(0x15);   
	lcd_write_data8(0x31);   
	lcd_write_data8(0x34);   

	lcd_write_cmd(LCD_CMD_NVGAMCTRL);     
	lcd_write_data8(0xD0);   
	lcd_write_data8(0x08);   
	lcd_write_data8(0x0E);   
	lcd_write_data8(0x09);   
	lcd_write_data8(0x09);   
	lcd_write_data8(0x15);   
	lcd_write_data8(0x31);   
	lcd_write_data8(0x33);   
	lcd_write_data8(0x48);   
	lcd_write_data8(0x17);   
	lcd_write_data8(0x14);   
	lcd_write_data8(0x15);   
	lcd_write_data8(0x31);   
	lcd_write_data8(0x34);

	lcd_write_cmd(LCD_CMD_INVON); 
	lcd_write_cmd(LCD_CMD_DISPON);
}

//----------------------------------------------------------------------------------
/**
 * @brief 屏幕初始化
 * 
 */
void lcd_init()
{
    lcd_gpio_init();
    lcd_config_init();
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
	unsigned int i,all;
	unsigned short* p_color =color;

    lcd_set_addr(xsta, ysta, xend, yend);
    all = (xend - xsta + 1) * (yend - ysta + 1);
    for(i = 0; i < all; i++)
    {
        lcd_write_data16(*p_color++);
    }
}



//---------------------------------------------------------------------------------
/**
 * @brief spi写一字节
 * 
 * @param data 
 */
static void spi_write_byte(unsigned char data)
{
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        LCD_SCL_LOW();
        if (data & 0x80)
        {
            LCD_SDA_HIGH();
        }
        else
        {
            LCD_SDA_LOW();
        }
        LCD_SCL_HIGH();
        data <<= 1;
    }
}

//---------------------------------------------------------------------------------
/**
 * @brief 写8位数据
 * 
 * @param data8 
 */
void lcd_write_data8(unsigned char data8)
{
    spi_write_byte(data8);
}

//---------------------------------------------------------------------------------
/**

 * @brief 写16位数据
 * 
 * @param data16 
 */
void lcd_write_data16(unsigned short data16)
{
    spi_write_byte(data16 >> 8);
    spi_write_byte(data16);
}

//---------------------------------------------------------------------------------
/**
 * @brief 写8位指令
 * 
 * @param cmd 
 */
void lcd_write_cmd(unsigned char cmd)
{
    LCD_DC_LOW();
    spi_write_byte(cmd);
    LCD_DC_HIGH();
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
    lcd_write_cmd(LCD_CMD_CASET);
    lcd_write_data16(xsta);
    lcd_write_data16(xend);
    lcd_write_cmd(LCD_CMD_RASET);
    lcd_write_data16(ysta);
    lcd_write_data16(yend);
    lcd_write_cmd(LCD_CMD_RAMWR);
}


void lcd_fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color)
{
    unsigned short i, j;
    lcd_set_addr(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            lcd_write_data16(color);
        }
    }
}