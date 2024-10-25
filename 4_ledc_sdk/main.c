#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"

void delay(volatile unsigned int n);
void delay_ms(volatile unsigned int n);
void clk_enable(void);
void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);

int main(void)
{
    clk_enable();
    led_init();

    while (1) {
        led_toggle();
        delay_ms(500);
    }

    return 0;
}

/// @brief 延时
void delay(volatile unsigned int n)
{
    while (n--) { }
}

/// @brief 延时
/// @param n 毫秒数
void delay_ms(volatile unsigned int n)
{
    while (n--) {
        delay(0x7FF);
    }
}

/// @brief 使能外设时钟
void clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}

/// @brief 初始化LED
void led_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);

    GPIO1->GDIR = 0x8; // GPIO1_IO03设置为输出
    GPIO1->DR = 0x0; // 默认开启LED
}

void led_on(void)
{
    GPIO1->DR &= ~(1 << 3);
}

void led_off(void)
{
    GPIO1->DR |= 1 << 3;
}

void led_toggle(void)
{
    GPIO1->DR ^= 1 << 3;
}