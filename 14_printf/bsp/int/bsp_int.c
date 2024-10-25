#include "bsp_int.h"

static sys_irq_handle_t sysIrqHandles[NUMBER_OF_INT_VECTORS];
static uint32_t irqNesting = 0;

void system_irqtable_init(void)
{
    for (int i = 0; i < NUMBER_OF_INT_VECTORS; i++) {
        sysIrqHandles[i].irqHandler = default_irqhandler;
        sysIrqHandles[i].userParam = NULL;
    }
}

void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* param)
{
    sysIrqHandles[irq].irqHandler = handler;
    sysIrqHandles[irq].userParam = param;
}

/// @brief 中断初始化
void int_init(void)
{
    GIC_Init();
    system_irqtable_init();
    __set_VBAR(0x87800000);
}

void default_irqhandler(unsigned int gicc_iar, void* param)
{
    while (1) { }
}

void system_irqhandler(unsigned int gicc_iar)
{
    if ((gicc_iar & 0x3ff) < NUMBER_OF_INT_VECTORS) {
        irqNesting++;
        sysIrqHandles[gicc_iar].irqHandler(gicc_iar, sysIrqHandles[gicc_iar].userParam);
        irqNesting--;
    }
}