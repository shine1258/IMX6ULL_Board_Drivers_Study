#include "bsp_epit.h"
#include "bsp_int.h"
#include "bsp_led.h"

void epit1_irqhandler(unsigned int giccIar, void* param);

void epit_init(uint32_t frac, uint32_t value)
{
    if (frac > 4096)
        frac = 4096;

    EPIT1->CR = 0;
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | ((frac - 1) << 4) | (1 << 24);
    EPIT1->LR = value; // 加载寄存器
    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, epit1_irqhandler, NULL);

    EPIT1->CR |= 1 << 0; // 使能EPIT1
}

void epit1_irqhandler(unsigned int giccIar, void* param)
{
    if (EPIT1->SR & (1 << 0)) {
        led_toggle();
    }

    EPIT1->SR |= 1 << 0; // 写1清除中断标志位
}