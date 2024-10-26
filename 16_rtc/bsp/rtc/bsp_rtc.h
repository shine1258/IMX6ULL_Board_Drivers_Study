#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "imx6u.h"

#define SECONDS_IN_A_DAY (86400)
#define SECONDS_IN_A_HOUR (3600)
#define SECONDS_IN_A_MINUTE (60)
#define DAYS_IN_A_YEAR (365)
#define DAYS_IN_A_LEAP_YEAR (366)
#define YEAR_RANGE_START (1970)
#define YEAR_RANGE_END (2099)

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_datetime_t;

void rtc_init(void);
void rtc_set_datetime(rtc_datetime_t* datetime);
uint32_t rtc_get_seconds(void);
void rtc_get_datetime(rtc_datetime_t* datetime);

#endif // !__BSP_RTC_H