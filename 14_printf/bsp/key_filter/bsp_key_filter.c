#include "bsp_key_filter.h"
#include "bsp_beep.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

void gpio1_io16_io31_irqhandler(unsigned int gicc_iar, void* param);
static void timer_init(uint32_t value);
static void timer_stop(void);
static void timer_restart(uint32_t value);
static void timer_irqhandler(unsigned int giccIar, void* param);

void key_filter_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    gpio_pin_config_t config = {
        .direction = kGPIO_DigitalInput,
        .interruptMode = kGPIO_IntFallingEdge
    };
    gpio_init(GPIO1, 18, &config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_io16_io31_irqhandler, NULL);
    gpio_interrupt_enable(GPIO1, 18);

    timer_init(66000000 / 50); // 10ms
}

void gpio1_io16_io31_irqhandler(unsigned int gicc_iar, void* param)
{
    /* 开启定时器 */
    timer_restart(66000000 / 50); // 10ms

    /* 清除中断标志位 */
    gpio_interrupt_clear_flag(GPIO1, 18);
}

static void timer_init(uint32_t value)
{
    EPIT1->CR = 0;
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);
    EPIT1->LR = value; // 加载寄存器
    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, timer_irqhandler, NULL);
}

static void timer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

static void timer_restart(uint32_t value)
{
    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = value;
    EPIT1->CR |= 1 << 0;
}

static void timer_irqhandler(unsigned int giccIar, void* param)
{
    if (EPIT1->SR & (1 << 0)) {
        timer_stop();
        beep_toggle();
    }

    EPIT1->SR |= 1 << 0; // 写1清除中断标志位
}