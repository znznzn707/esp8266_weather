#ifndef ST7789_H_
#define ST7789_H_

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#define LCD_W                         240
#define LCD_H                         240

#define LCD_GPIO_CS                   1
#define LCD_GPIO_DC                   14
#define LCD_GPIO_RST                  12
#define LCD_GPIO_SDA                  13
#define LCD_GPIO_SCL                  15
#define LCD_GPIO_BLK                  6


#define LCD_SCL_LOW()                gpio_set_level(LCD_GPIO_SCL, 0)
#define LCD_SCL_HIGH()               gpio_set_level(LCD_GPIO_SCL, 1) 

#define LCD_SDA_LOW()                gpio_set_level(LCD_GPIO_SDA, 0)
#define LCD_SDA_HIGH()               gpio_set_level(LCD_GPIO_SDA, 1)

#define LCD_RST_LOW()                gpio_set_level(LCD_GPIO_RST, 0)
#define LCD_RST_HIGH()               gpio_set_level(LCD_GPIO_RST, 1)

#define LCD_DC_LOW()                 gpio_set_level(LCD_GPIO_DC, 0)
#define LCD_DC_HIGH()                gpio_set_level(LCD_GPIO_DC, 1)

//----------------------------------------------------------------------------------
/**
 * @brief 屏幕初始化
 * 
 */
void lcd_init();

void lcd_fill();


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
void lcd_draw(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short* color);

//---------------------------------------------------------------------------------
/**
 * @brief 写8位数据
 * 
 * @param data8 
 */
void lcd_write_data8(unsigned char data8);

//---------------------------------------------------------------------------------
/**

 * @brief 写16位数据
 * 
 * @param data16 
 */
void lcd_write_data16(unsigned short data16);

//---------------------------------------------------------------------------------
/**
 * @brief 写8位指令
 * 
 * @param cmd 
 */
void lcd_write_cmd(unsigned char cmd);

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
                  unsigned short xend, unsigned short yend);

#endif // !ST7789_H_
