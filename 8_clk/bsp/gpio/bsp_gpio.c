#include "bsp_gpio.h"
#include "cc.h"
#include "fsl_iomuxc.h"

void gpio_init(GPIO_Type* gpio, int pin, GPIO_PIN_CONFIG_t* config)
{
    if (config->direction == kGPIO_DigitalInput) {
        gpio->GDIR &= ~(1 << pin);
    } else {
        gpio->GDIR |= 1 << pin;
        gpio_writePin(gpio, pin, config->outputLogic);
    }
}

void gpio_writePin(GPIO_Type* gpio, int pin, int value)
{
    if (value == 0) {
        gpio->DR &= ~(1 << pin);
    } else {
        gpio->DR |= 1 << pin;
    }
}

int gpio_readPin(GPIO_Type* gpio, int pin)
{
    return (gpio->DR >> pin) & 0x1;
}