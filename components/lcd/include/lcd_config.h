#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#define ST7789

#ifdef ST7789
#include "st7789.h"
#endif


#define LCD_INIT_FUN()              lcd_init()
#define LCD_FILL_FUN()              lcd_fill()


#endif // !LCD_CONFIG_H_
