#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"

int main(void)
{
    clk_enable();
    led_init();
    beep_init();

    while (1) {
        led_toggle();
        beep_toggle();
        delay_ms(1000);
    }

    return 0;
}
