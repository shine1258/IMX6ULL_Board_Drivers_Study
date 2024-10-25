#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "MCIMX6Y2.h"

typedef enum {
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput = 1U,
} GPIO_PIN_DIRECTION;

typedef struct
{
    GPIO_PIN_DIRECTION direction; // 方向
    uint8_t outputLogic; // 默认电平
} GPIO_PIN_CONFIG_t;

void gpio_init(GPIO_Type* gpio, int pin, GPIO_PIN_CONFIG_t* config);
void gpio_writePin(GPIO_Type* gpio, int pin, int value);
int gpio_readPin(GPIO_Type* gpio, int pin);

#endif // __BSP_GPIO_H