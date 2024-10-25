#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_epit.h"
#include "bsp_exti.h"
#include "bsp_int.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "imx6u.h"

int main(void)
{
    int_init();
    clk_init_imx6ull();
    clk_enable();
    // key_init();
    beep_init();
    led_init();
    exti_init();
    epit_init(1, 66000000 / 20);

    while (1) {
        // led_toggle();
        delay_ms(500);
    }

    return 0;
}