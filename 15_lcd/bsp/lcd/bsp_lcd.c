#include "bsp_lcd.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "stdio.h"

typedef enum {
    LCD_ID_4342,
    LCD_ID_7084,
    LCD_ID_7016,
    NC = 0xFF
} LCD_ID;

lcd_type_t lcd = {
    .background = LCD_WHITE,
    .foreground = LCD_BLACK,
    .pixsize = 4, // ARGB8888
    .framebuffer = 0x89000000
};

static LCD_ID lcd_read_id(void);
static void lcd_gpio_init(void);
static void lcd_reset(void);
static void lcd_reset_stop(void);
static void lcd_enable(void);
static void lcd_clk_init(uint8_t loopDiv, uint8_t prediv, uint8_t div);

void lcd_init(void)
{
    LCD_ID id = lcd_read_id();
    printf("LCD ID: %d\r\n", id);

    if (id == NC)
        return;

    lcd_gpio_init();
    lcd_reset();
    delay_ms(10);
    lcd_reset_stop();

    switch (id) {
    case LCD_ID_4342:
        lcd.width = 480;
        lcd.height = 272;
        lcd.hspw = 1;
        lcd.hbpd = 40;
        lcd.hfpd = 5;
        lcd.vspw = 1;
        lcd.vbpd = 8;
        lcd.vfpd = 8;
        lcd_clk_init(27, 8, 8);
        break;
    case LCD_ID_7084:
        lcd.width = 800;
        lcd.height = 480;
        lcd.hspw = 1;
        lcd.hbpd = 46;
        lcd.hfpd = 210;
        lcd.vspw = 1;
        lcd.vbpd = 23;
        lcd.vfpd = 22;
        lcd_clk_init(50, 6, 6);
        break;
    case LCD_ID_7016:
        lcd.width = 1024;
        lcd.height = 600;
        lcd.hspw = 20;
        lcd.hbpd = 140;
        lcd.hfpd = 160;
        lcd.vspw = 3;
        lcd.vbpd = 20;
        lcd.vfpd = 12;
        lcd_clk_init(32, 3, 5);
        break;
    default:
        break;
    }

    /* 配置LCDIF控制器 */
    LCDIF->CTRL = 0;
    LCDIF->CTRL |= (1 << 5) | (3 << 8) | (3 << 10) | (1 << 17) | (1 << 19);
    LCDIF->CTRL1 = 0;
    LCDIF->CTRL1 |= (7 << 16);
    LCDIF->TRANSFER_COUNT = lcd.width | (lcd.height << 16);
    LCDIF->VDCTRL0 = 0;
    LCDIF->VDCTRL0 |= lcd.vspw | (1 << 20) | (1 << 21) | (1 << 24) | (1 << 28);
    LCDIF->VDCTRL1 = lcd.vspw + lcd.vbpd + lcd.height + lcd.vfpd;
    LCDIF->VDCTRL2 = (lcd.hspw + lcd.hbpd + lcd.width + lcd.hfpd) | (lcd.hspw << 18);
    LCDIF->VDCTRL3 = (lcd.vspw + lcd.vbpd) | ((lcd.hspw + lcd.hbpd) << 16);
    LCDIF->VDCTRL4 = (lcd.width) | (1 << 18);
    LCDIF->CUR_BUF = lcd.framebuffer;
    LCDIF->NEXT_BUF = lcd.framebuffer;

    lcd_enable();
    delay_ms(20);
    lcd_clear(lcd.background);
}

inline void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color)
{
    *((uint32_t*)(lcd.framebuffer + (y * lcd.width + x) * lcd.pixsize)) = color;
}

inline uint32_t lcd_read_pixel(uint16_t x, uint16_t y)
{
    return *((uint32_t*)(lcd.framebuffer + (y * lcd.width + x) * lcd.pixsize));
}

void lcd_fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
    if (x0 < 0)
        x0 = 0;
    if (y0 < 0)
        y0 = 0;
    if (x1 >= lcd.width)
        x1 = lcd.width - 1;
    if (y1 >= lcd.height)
        y1 = lcd.height - 1;

    for (uint16_t y = y0; y <= y1; y++) {
        for (uint16_t x = x0; x <= x1; x++)
            lcd_draw_pixel(x, y, color);
    }
}

void lcd_clear(uint32_t color)
{
    uint32_t* p = (uint32_t*)lcd.framebuffer;
    for (int i = 0; i < lcd.width * lcd.height; i++)
        p[i] = color;
}

static LCD_ID lcd_read_id(void)
{
    gpio_pin_config_t config;

    /* 打开模拟开关 */
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10B0);
    config.direction = kGPIO_DigitalOutput;
    config.outputLogic = 1;
    gpio_init(GPIO3, 3, &config);

    /* 初始化ID GPIO */
    config.direction = kGPIO_DigitalInput;
    /* R7 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xF080);
    gpio_init(GPIO3, 28, &config);
    /* G7 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xF080);
    gpio_init(GPIO3, 20, &config);
    /* B7 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0); // B7
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xF080);
    gpio_init(GPIO3, 12, &config);

    /* 读取ID */
    uint8_t idx = 0;
    idx = gpio_readPin(GPIO3, 28); // M0
    idx |= (gpio_readPin(GPIO3, 20) << 1); // M1
    idx |= (gpio_readPin(GPIO3, 12) << 2); // M2

    switch (idx) {
    case 0:
        return LCD_ID_4342;
    case 1:
        return LCD_ID_7084;
    case 2:
        return LCD_ID_7016;
    default:
        return NC;
    }
}

static void lcd_gpio_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);

    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);

    /* IO属性
       bit 16:0 HYS关闭
       bit [15:14]: 0 默认22K上拉
       bit [13]: 0 pull功能
       bit [12]: 0 pull/keeper使能
       bit [11]: 0 关闭开路输出
       bit [7:6]: 10 速度100Mhz
       bit [5:3]: 111 驱动能力为R0/7
       bit [0]: 1 高转换率 */
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0xB9);

    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0xB9);

    /* 点亮背光 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0x10B0);
    gpio_pin_config_t config = { .direction = kGPIO_DigitalOutput, .outputLogic = 1 };
    gpio_init(GPIO1, 8, &config);
}

static void lcd_reset(void)
{
    LCDIF->CTRL |= (1 << 31);
}

static void lcd_reset_stop(void)
{
    LCDIF->CTRL &= ~(1 << 31);
}

static void lcd_enable(void)
{
    LCDIF->CTRL |= (1 << 0);
}

/// @brief LCD时钟初始化, LCDCLK = 24 * loopDiv / prediv / div
/// @param loopDiv DIV_SELECT, 范围为27 ~ 54
/// @param prediv 预分频器分频值, 范围为1 ~ 8
/// @param div 分频器分频值, 范围为1 ~ 8
static void lcd_clk_init(const uint8_t loopDiv, uint8_t prediv, uint8_t div)
{
    /* 选择时钟 */
    CCM->CSCDR2 &= ~(7 << 15);
    CCM->CSCDR2 |= (2 << 15);
    CCM->CSCDR2 &= ~(7 << 9);

    /* 设置分频 */
    CCM_ANALOG->PLL_VIDEO_NUM = 0; // 不使用小数分频器
    CCM_ANALOG->PLL_VIDEO_DENOM = 0;
    CCM_ANALOG->PLL_VIDEO = (1 << 13) | (2 << 19) | (loopDiv << 0);
    CCM_ANALOG->MISC2 &= ~(3 << 30);
    CCM->CSCDR2 &= ~(7 << 12);
    CCM->CSCDR2 |= ((prediv - 1) << 12);
    CCM->CBCMR &= ~(7 << 23);
    CCM->CBCMR |= ((div - 1) << 23);
}