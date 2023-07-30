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

//----------------------------------------------------------------------------------
/**
 * @brief 屏幕初始化
 * 
 */
void lcd_init();

void lcd_fill();


void lcd_draw(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short* color);

#endif // !ST7789_H_
