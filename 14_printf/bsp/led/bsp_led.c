#include "bsp_led.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"

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