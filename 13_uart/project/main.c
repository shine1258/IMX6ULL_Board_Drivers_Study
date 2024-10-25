#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_epit.h"
#include "bsp_exti.h"
#include "bsp_int.h"
#include "bsp_key.h"
#include "bsp_key_filter.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "imx6u.h"

int main(void)
{
    int_init();
    clk_init_imx6ull();
    clk_enable();
    // key_init();
    // key_filter_init();
    beep_init();
    led_init();
    // exti_init();
    // epit_init(1, 66000000 / 20);
    delay_init();
    uart1_init();

    led_on();

    char a;

    while (1) {
        uart1_puts("请输入1个字符: ");
        a = uart1_getc();
        uart1_putc(a);
        uart1_puts("\r\n");
        uart1_puts("你输入的字符是: ");
        uart1_putc(a);
        uart1_puts("\r\n");
    }

    return 0;
}
