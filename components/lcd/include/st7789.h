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


//进入睡眠
#define LCD_CMD_SLPIN                (0x10)
//退出睡眠
#define LCD_CMD_SLPOUT               (0x11)
//Display Inversion On 
#define LCD_CMD_INVON                (0x21)
//Display Off
#define LCD_CMD_DISPOFF              (0x28)
//Display On
#define LCD_CMD_DISPON               (0x29)
//列地址设置
#define LCD_CMD_CASET                (0x2A)
//行地址设置
#define LCD_CMD_RASET                (0x2B)
//内存写入
#define LCD_CMD_RAMWR                (0x2C)
/*内存数据存取控制
Bit D7- Page Address Order 
“0” = Top to Bottom (When MADCTL D7=”0”). 
“1” = Bottom to Top (When MADCTL D7=”1”). 
Bit D6- Column Address Order 
“0” = Left to Right (When MADCTL D6=”0”). 
“1” = Right to Left (When MADCTL D6=”1”). 
Bit D5- Page/Column Order 
“0” = Normal Mode (When MADCTL D5=”0”). 
“1” = Reverse Mode (When MADCTL D5=”1”) 
Bit D4- Line Address Order 
“0” = LCD Refresh Top to Bottom (When MADCTL D4=”0”) 
“1” = LCD Refresh Bottom to Top (When MADCTL D4=”1”) 
Bit D3- RGB/BGR Order 
“0” = RGB (When MADCTL D3=”0”) 
“1” = BGR (When MADCTL D3=”1”) 
Bit D2- Display Data Latch Data Order 
“0” = LCD Refresh Left to Right (When MADCTL D2=”0”) 
“1” = LCD Refresh Right to Left (When MADCTL D2=”1”) 
*/
#define LCD_CMD_MADCTL               (0x36)
/*
Interface Pixel Format
bit [7] - Set to ‘0’ 
bit [6:4] RGB interface color format:
‘101’ = 65K of RGB interface
‘110’ = 262K of RGB interface 
bit [3] - Set to ‘0’ 
bit[2:0] Control interface color format:
‘011’ = 12bit/pixel 
‘101’ = 16bit/pixel 
‘110’ = 18bit/pixel 
‘111’ = 16M truncated 
*/
#define LCD_CMD_COLMOD               (0x3A)
/*
屏参设置
BPA[6:0]: Back porch setting in normal mode. The minimum setting is 0x01. 
FPA[6:0]: Front porch setting in normal mode. The minimum setting is 0x01. 
PSEN: Enable separate porch control.  0 Disable ; 1 Enable 
BPB[3:0]: Back porch setting in idle mode. The minimum setting is 0x01. 
FPB[3:0]: Front porch setting in idle mode. The minimum setting is 0x01. 
BPC[3:0]: Back porch setting in partial mode. The minimum setting is 0x01. 
FPC[3:0]: Front porch setting in partial mode. The minimum setting is 0x01. 
*/
#define LCD_CMD_PORCTRL              (0xB2)
/*
Gate Control 
D7    D6     D5    D4    D3    D2     D1      D0 
0    VGHS2  VGHS1 VGHS0  0    VGLS2  VGLS1   VGLS0
*/
#define LCD_CMD_GCTR                 (0xB7)
/* VCOM Setting */ 
#define LCD_CMD_VCOMS                (0xBB)
/*
VDV and VRH Command Enable
CMDEN: VDV and VRH command write enable. 
CMDEN=”0”: VDV and VRH register value comes from NVM. 
CMDEN=”1”, VDV and VRH register value comes from command write. 
*/
#define LCD_CMD_VDVVRHEN             (0xC2)
/*
VRH 设置
0 0 VRHS5 VRHS4 VRHS3 VRHS2 VRHS1 VRHS0
*/
#define LCD_CMD_VRHS                 (0xC3)
/*
VDV 设置
0 0 VDVS5 VDVS4 VDVS3 VDVS2 VDVS1 VDVS0
*/
#define LCD_CMD_VDVS                 (0xC4)
/*
Frame Rate Control in Normal Mode 
NLA2 NLA1 NLA0 RTNA4 RTNA3 RTNA2 RTNA1 RTNA0
*/
#define LCD_CMD_FRCTRL               (0xC6)
/*
Power Control 1
par1: 1 0 1 0 0 1 0 0     
par2:AVDD1 AVDD0 AVCL1 AVCL0 0 0 VDS1 VDS0 
*/
#define LCD_CMD_PWCTRL1              (0xD0)
/*
Positive Voltage Gamma Control 
*/
#define LCD_CMD_PVGAMCTRL            (0xE0)
/*
Negative Voltage Gamma Control 
*/
#define LCD_CMD_NVGAMCTRL            (0xE1)


//---------------------------------------------------------------------------------- /**
 /* @brief 屏幕初始化 * 
 */
void lcd_init();

void lcd_fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color);


//---------------------------------------------------------------------------------
/**
 * @brief 
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
