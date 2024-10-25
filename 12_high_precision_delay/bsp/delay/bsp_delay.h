#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "imx6u.h"

void delay_init(void);
void delay_short(volatile unsigned int n);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);

#endif // __BSP_DELAY_H