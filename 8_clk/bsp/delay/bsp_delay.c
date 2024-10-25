#include "bsp_delay.h"

/// @brief 延时
void delay_short(volatile unsigned int n)
{
    while (n--) { }
}

/// @brief 延时
/// @param n 毫秒数
void delay_ms(volatile unsigned int n)
{
    while (n--) {
        delay_short(0x7FF);
    }
}