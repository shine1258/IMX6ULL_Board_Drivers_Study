#include "main.h"

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
    CCM_CCGR0 = 0xFFFFFFFF;
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

/// @brief 初始化LED
void led_init(void)
{
    IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 = 0x5; // 复用为GPIO1_IO03

    /* 配置GPIO1_IO03的的电气属性
     * bit 0(摇摆率): 0 低速率
     * bit 5:3(驱动能力): 110 R0/6
     * bit 7:6(速度): 10 100MHz
     * bit 11(开漏): 0 关闭
     * bit 12(保持器使能): 1 使能
     * bit 13(上拉或保持): 0 保持
     * bit 15:14(设置上下拉阻值): 00 100K下拉
     * bit 16(使能整形): 0 关闭 */
    IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 = 0x10B0;
    GPIO1_GDIR = 0x8; // GPIO1_IO03设置为输出
    GPIO1_DR = 0x0; // 默认开启LED
}

void led_on(void)
{
    GPIO1_DR &= ~(1 << 3);
}

void led_off(void)
{
    GPIO1_DR |= 1 << 3;
}

void led_toggle(void)
{
    GPIO1_DR ^= 1 << 3;
}