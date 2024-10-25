#include "bsp_gpio.h"
#include "imx6u.h"

void gpio_init(GPIO_Type* gpio, uint32_t pin, gpio_pin_config_t* config)
{
    if (config->direction == kGPIO_DigitalInput) {
        gpio->GDIR &= ~(1 << pin);
    } else {
        gpio->GDIR |= 1 << pin;
        gpio_writePin(gpio, pin, config->outputLogic);
    }

    gpio_interrupt_init(gpio, pin, config->interruptMode);
}

void gpio_interrupt_enable(GPIO_Type* gpio, uint32_t pin)
{
    gpio->IMR |= 1 << pin;
}

void gpio_interrupt_disable(GPIO_Type* gpio, uint32_t pin)
{
    gpio->IMR &= ~(1 << pin);
}

void gpio_interrupt_clear_flag(GPIO_Type* gpio, uint32_t pin)
{
    gpio->ISR |= 1 << pin;
}

void gpio_interrupt_init(GPIO_Type* gpio, uint32_t pin, gpio_interrupt_mode_t mode)
{
    volatile uint32_t* icr = NULL;
    uint32_t icrShift;

    icrShift = pin;
    gpio->EDGE_SEL &= ~(1 << pin);

    if (pin < 16) { // 低16位
        icr = &(gpio->ICR1);
    } else { // 高16位
        icr = &(gpio->ICR2);
        icrShift -= 16;
    }

    *icr &= ~(0x3 << (icrShift * 2));

    switch (mode) {
    case kGPIO_IntLowLevel:
        break;
    case kGPIO_IntHighLevel:
        *icr |= 0x1 << (icrShift * 2);
        break;
    case kGPIO_IntRisingEdge:
        *icr |= 0x2 << (icrShift * 2);
        break;
    case kGPIO_IntFallingEdge:
        *icr |= 0x3 << (icrShift * 2);
        break;
    case kGPIO_IntRisingOrFallingEdge:
        gpio->EDGE_SEL |= 1 << pin;
        break;
    default:
        break;
    }
}

void gpio_writePin(GPIO_Type* gpio, uint32_t pin, int value)
{
    if (value == 0) {
        gpio->DR &= ~(1 << pin);
    } else {
        gpio->DR |= 1 << pin;
    }
}

int gpio_readPin(GPIO_Type* gpio, uint32_t pin)
{
    return (gpio->DR >> pin) & 0x1;
}