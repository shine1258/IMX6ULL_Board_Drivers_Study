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
#include "bsp_rtc.h"
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
    rtc_init();

    led_on();

    lcd.background = LCD_BLACK;
    lcd.foreground = LCD_RED;
    lcd_show_string(10, 10, 240, 16, 24, "IMX6ULL");
    lcd_show_string(10, 40, 240, 16, 24, "RTC TEST");

    rtc_datetime_t rtc_date_time;

    while (1) {
        led_toggle();

        rtc_get_datetime(&rtc_date_time);
        printf("%04d-%02d-%02d %02d:%02d:%02d\r\n", rtc_date_time.year, rtc_date_time.month, rtc_date_time.day, rtc_date_time.hour, rtc_date_time.minute, rtc_date_time.second);

        delay_ms(1000);
    }

    return 0;
}
