#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "imx6u.h"

/* 颜色 */
#define LCD_BLUE 0x000000FF
#define LCD_GREEN 0x0000FF00
#define LCD_RED 0x00FF0000
#define LCD_CYAN 0x0000FFFF
#define LCD_MAGENTA 0x00FF00FF
#define LCD_YELLOW 0x00FFFF00
#define LCD_LIGHTBLUE 0x008080FF
#define LCD_LIGHTGREEN 0x0080FF80
#define LCD_LIGHTRED 0x00FF8080
#define LCD_LIGHTCYAN 0x0080FFFF
#define LCD_LIGHTMAGENTA 0x00FF80FF
#define LCD_LIGHTYELLOW 0x00FFFF80
#define LCD_DARKBLUE 0x00000080
#define LCD_DARKGREEN 0x00008000
#define LCD_DARKRED 0x00800000
#define LCD_DARKCYAN 0x00008080
#define LCD_DARKMAGENTA 0x00800080
#define LCD_DARKYELLOW 0x00808000
#define LCD_WHITE 0x00FFFFFF
#define LCD_LIGHTGRAY 0x00D3D3D3
#define LCD_GRAY 0x00808080
#define LCD_DARKGRAY 0x00404040
#define LCD_BLACK 0x00000000
#define LCD_BROWN 0x00A52A2A
#define LCD_ORANGE 0x00FFA500
#define LCD_TRANSPARENT 0x00000000

typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t hspw;
    uint16_t hbpd;
    uint16_t hfpd;
    uint16_t vspw;
    uint16_t vbpd;
    uint16_t vfpd;
    uint32_t foreground;
    uint32_t background;
    uint8_t pixsize; // 每个像素所占字节数
    uint32_t framebuffer; // 帧缓冲区地址
} lcd_type_t;

extern lcd_type_t lcd;

void lcd_init(void);
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
uint32_t lcd_read_pixel(uint16_t x, uint16_t y);
void lcd_fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void lcd_clear(uint32_t color);

#endif // !__BSP_LCD_H