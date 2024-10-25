#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_key.h"
#include "bsp_led.h"

int main(void)
{
    clk_init_imx6ull();
    clk_enable();
    key_init();
    beep_init();
    led_init();

    led_on();

    int i = 0;
    KeyValue kv;

    while (1) {
        kv = key_getValue();

        if (kv == KEY0_PRES) {
            beep_toggle();
        }

        i++;

        if (i == 500) {
            i = 0;
            led_toggle();
        }

        delay_ms(1);
    }

    return 0;
}
