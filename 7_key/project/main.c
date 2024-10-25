#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_key.h"
#include "bsp_led.h"

int main(void)
{
    clk_enable();
    key_init();
    beep_init();
    led_init();

    led_off();

    KeyValue kv;

    while (1) {
        kv = key_getValue();

        if (kv == KEY0_PRES) {
            beep_toggle();
            led_toggle();
        }

        delay_ms(1);
    }

    return 0;
}
