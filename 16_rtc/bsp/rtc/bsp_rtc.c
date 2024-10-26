#include "bsp_rtc.h"

static void rtc_enable(void);
static void rtc_disable(void);
static uint8_t rtc_isleapyear(uint16_t year);
static uint32_t rtc_datetime_to_seconds(rtc_datetime_t* datetime);
static void rtc_seconds_to_datetime(uint32_t seconds, rtc_datetime_t* datetime);

/// @brief 初始化RTC
void rtc_init(void)
{
    // rtc_datetime_t rtc_date_time = {
    //     .year = 2024,
    //     .month = 10,
    //     .day = 26,
    //     .hour = 15,
    //     .minute = 47,
    //     .second = 40
    // };

    SNVS->HPCOMR |= (1 << 31) | (1 << 8); // 允许访问SNVS寄存器
    rtc_enable();

    // rtc_set_datetime(&rtc_date_time);
}

/// @brief 设置日期和时间
/// @param datetime 要设置的日期和时间
void rtc_set_datetime(rtc_datetime_t* datetime)
{
    uint32_t tmp = SNVS->LPCR;
    rtc_disable();

    uint32_t seconds = rtc_datetime_to_seconds(datetime);
    SNVS->LPSRTCMR = seconds >> 17; // 高15位
    SNVS->LPSRTCLR = seconds << 15; // 低17位

    if (tmp & 0x1) // 如果以前已打开过RTC
        rtc_enable();
}

/// @brief 获取当前时间秒数
/// @return 当前时间秒数
uint32_t rtc_get_seconds(void)
{
    return (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);
}

/// @brief 获取当前日期和时间
/// @param datetime 接收日期和时间
void rtc_get_datetime(rtc_datetime_t* datetime)
{
    rtc_seconds_to_datetime(rtc_get_seconds(), datetime);
}

static void rtc_enable(void)
{
    SNVS->LPCR |= (1 << 0); // 使能RTC
    while ((SNVS->LPCR & 0x1) == 0) { }
}

static void rtc_disable(void)
{
    SNVS->LPCR &= ~(1 << 0); // 关闭RTC
    while ((SNVS->LPCR & 0x1) == 1) { }
}

uint8_t rtc_isleapyear(uint16_t year)
{
    uint8_t value = 0;

    if (year % 400 == 0)
        value = 1;
    else {
        if ((year % 4 == 0) && (year % 100 != 0))
            value = 1;
        else
            value = 0;
    }

    return value;
}

/// @brief 将时间转换为秒数
/// @param datetime 要转换日期和时间。
/// @return 转换后的秒数
uint32_t rtc_datetime_to_seconds(rtc_datetime_t* datetime)
{
    uint16_t i = 0;
    uint32_t seconds = 0;
    uint32_t days = 0;
    uint16_t monthdays[] = { 0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U };

    for (i = 1970; i < datetime->year; i++) {
        days += DAYS_IN_A_YEAR; /* 平年，每年365天 */
        if (rtc_isleapyear(i))
            days += 1; /* 闰年多加一天 		*/
    }

    days += monthdays[datetime->month];
    if (rtc_isleapyear(i) && (datetime->month >= 3))
        days += 1; /* 闰年，并且当前月份大于等于3月的话加一天 */

    days += datetime->day - 1;

    seconds = days * SECONDS_IN_A_DAY + datetime->hour * SECONDS_IN_A_HOUR + datetime->minute * SECONDS_IN_A_MINUTE + datetime->second;

    return seconds;
}

/// @brief 将秒数转换为时间
/// @param seconds 要转换的秒数
/// @param datetime 转换后的日期和时间
static void rtc_seconds_to_datetime(uint32_t seconds, rtc_datetime_t* datetime)
{
    uint32_t x;
    uint32_t secondsRemaining, days;
    uint16_t daysInYear;

    /* 每个月的天数       */
    uint8_t daysPerMonth[] = { 0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U };

    secondsRemaining = seconds; /* 剩余秒数初始化 */
    days = secondsRemaining / SECONDS_IN_A_DAY + 1; /* 根据秒数计算天数,加1是当前天数 */
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY; /*计算天数以后剩余的秒数 */

    /* 计算时、分、秒 */
    datetime->hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;
    datetime->minute = secondsRemaining / 60;
    datetime->second = secondsRemaining % SECONDS_IN_A_MINUTE;

    /* 计算年 */
    daysInYear = DAYS_IN_A_YEAR;
    datetime->year = YEAR_RANGE_START;
    while (days > daysInYear) {
        /* 根据天数计算年 */
        days -= daysInYear;
        datetime->year++;

        /* 处理闰年 */
        if (!rtc_isleapyear(datetime->year))
            daysInYear = DAYS_IN_A_YEAR;
        else /*闰年，天数加一 */
            daysInYear = DAYS_IN_A_YEAR + 1;
    }
    /*根据剩余的天数计算月份 */
    if (rtc_isleapyear(datetime->year)) /* 如果是闰年的话2月加一天 */
        daysPerMonth[2] = 29;

    for (x = 1; x <= 12; x++) {
        if (days <= daysPerMonth[x]) {
            datetime->month = x;
            break;
        } else {
            days -= daysPerMonth[x];
        }
    }

    datetime->day = days;
}
