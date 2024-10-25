#include "bsp_delay.h"

void delay_init(void)
{
    GPT1->CR = 0;
    GPT1->CR |= 1 << 15; // 软复位
    while ((GPT1->CR >> 15) & 0x01) { } // 等待复位完成
    GPT1->CR = (1 << 1) | (1 << 6); // 设置时钟源位ipg_clk=66MHz, restart模式
    GPT1->PR = 66 - 1; // 设置分频系数, 频率为1MHz
    GPT1->OCR[0] = 0xffffffff; // 4294967295us ≈ 71.58min
    GPT1->CR |= 1 << 0; // 使能GPT1
}

/// @brief 延时
void delay_short(volatile unsigned int n)
{
    while (n--) { }
}

void delay_us(uint32_t n)
{
    uint32_t old = GPT1->CNT;
    uint32_t new = 0;
    uint32_t value = 0;

    while (1) {
        new = GPT1->CNT;

        if (new > old) {
            value += (new - old);
        } else if (new < old) {
            value += (0xffffffff - old + new);
        }

        if (value >= n) {
            return;
        }

        old = new;
    }
}

/// @brief 延时
/// @param n 毫秒数
void delay_ms(uint32_t n)
{
    while (n--) {
        delay_us(1000);
    }
}