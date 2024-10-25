#include "bsp_beep.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_epit.h"
#include "bsp_exti.h"
#include "bsp_int.h"
#include "bsp_key.h"
#include "bsp_key_filter.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "imx6u.h"
#include "stdio.h"

int main(void)
{
    int_init();
    clk_init_imx6ull();
    clk_enable();
    beep_init();
    led_init();
    delay_init();
    uart1_init();
    lcd_init();

    led_on();

    lcd.background = LCD_BLACK;
    lcd.foreground = LCD_RED;
    lcd_show_string(10, 10, 240, 16, 24, "IMX6ULL LCD TEST");
    lcd_show_string(10, 40, 240, 16, 24, "LCD TEST");

    // uint8_t i = 0;

    while (1) {
        led_toggle();

        // switch (i) {
        // case 0:
        //     lcd_clear(LCD_RED);
        //     break;
        // case 1:
        //     lcd_clear(LCD_GREEN);
        //     break;
        // case 2:
        //     lcd_clear(LCD_BLUE);
        //     break;
        // default:
        //     break;
        // }

        // if (++i > 2)
        //     i = 0;

        delay_ms(500);
    }

    return 0;
}
