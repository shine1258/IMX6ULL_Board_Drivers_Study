#ifndef __BSP_INT_H
#define __BSP_INT_H

#include "imx6u.h"

typedef void (*system_irq_handler_t)(unsigned int gicc_iar, void* param);

typedef struct {
    system_irq_handler_t irqHandler;
    void* userParam;
} sys_irq_handle_t;

void int_init(void);
void default_irqhandler(unsigned int gicc_iar, void* param);
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* param);

#endif