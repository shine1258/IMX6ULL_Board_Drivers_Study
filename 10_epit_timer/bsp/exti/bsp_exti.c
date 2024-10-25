#include "bsp_exti.h"
#include "bsp_beep.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

void gpio1_io18_irqhandler(unsigned int gicc_iar, void* param);

/// @brief 初始化GPIO1_IO18外部中断
/// @param
void exti_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    gpio_pin_config_t config = {
        .direction = kGPIO_DigitalInput,
        .interruptMode = kGPIO_IntFallingEdge
    };
    gpio_init(GPIO1, 18, &config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_io18_irqhandler, NULL);
    gpio_interrupt_enable(GPIO1, 18);
}

void gpio1_io18_irqhandler(unsigned int gicc_iar, void* param)
{
    delay_ms(10); // 在实际的开发中, 禁止在中断服务函数中调用延时函数!!!

    if (gpio_readPin(GPIO1, 18) == 0) {
        beep_toggle();
    }

    gpio_interrupt_clear_flag(GPIO1, 18); // 清除中断标志位
}