#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "MCIMX6Y2.h"

typedef enum {
    kGPIO_NoIntMode = 0U,
    kGPIO_IntLowLevel = 1U,
    kGPIO_IntHighLevel = 2U,
    kGPIO_IntRisingEdge = 3U,
    kGPIO_IntFallingEdge = 4U,
    kGPIO_IntRisingOrFallingEdge = 5U,
} gpio_interrupt_mode_t;

typedef enum {
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput = 1U,
} gpio_pin_direction_t;

typedef struct
{
    gpio_pin_direction_t direction; // 方向
    uint8_t outputLogic; // 默认电平
    gpio_interrupt_mode_t interruptMode; // 中断模式
} gpio_pin_config_t;

void gpio_init(GPIO_Type* gpio, uint32_t pin, gpio_pin_config_t* config);
void gpio_writePin(GPIO_Type* gpio, uint32_t pin, int value);
int gpio_readPin(GPIO_Type* gpio, uint32_t pin);
void gpio_interrupt_enable(GPIO_Type* gpio, uint32_t pin);
void gpio_interrupt_disable(GPIO_Type* gpio, uint32_t pin);
void gpio_interrupt_clear_flag(GPIO_Type* gpio, uint32_t pin);
void gpio_interrupt_init(GPIO_Type* gpio, uint32_t pin, gpio_interrupt_mode_t mode);

#endif // __BSP_GPIO_H