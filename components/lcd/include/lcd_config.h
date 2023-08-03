#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#define ST7789

#ifdef ST7789
#include "st7789.h"
#endif


#ifndef LCD_INIT_FUN
#define LCD_INIT_FUN()              lcd_init()
#endif // !LCD_INIT_FUN

#ifndef LCD_DRAW_FUN
#define LCD_DRAW_FUN(xsta, ysta, xend, yend, color) \
    lcd_draw(xsta, ysta, xend, yend, color)
#endif // !LCD_DRAW_FUN


#endif // !LCD_CONFIG_H_
