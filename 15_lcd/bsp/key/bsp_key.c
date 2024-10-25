#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "fsl_iomuxc.h"

void key_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    gpio_pin_config_t config = { .direction = kGPIO_DigitalInput };
    gpio_init(GPIO1, 18, &config);
}

static int key_read(void)
{
    return gpio_readPin(GPIO1, 18);
}

KeyValue key_getValue(void)
{
    KeyValue ret = 0;
    static uint8_t release = 1; // 1表示按键释放

    if (release && (key_read() == 0)) {
        delay_ms(10);
        release = 0;

        if (key_read() == 0) {
            ret = KEY0_PRES;
        }
    } else if (key_read() == 1) {
        ret = KEY_NONE;
        release = 1;
    }

    return ret;
}