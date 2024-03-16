//
// Created by Piotr on 15.03.2024.
//

#ifndef KITTY_OS_CPP_RTC_H
#define KITTY_OS_CPP_RTC_H

#include <stdint.h>
#include <types.h>

#define RTC_SECONDS_REGISTER 0x00
#define RTC_MINUTES_REGISTER 0x02
#define RTC_HOURS_REGISTER 0x04
#define RTC_WEEKDAY_REGISTER 0x06
#define RTC_DAY_OF_MONTH_REGISTER 0x07
#define RTC_MONTH_REGISTER 0x08
#define RTC_YEAR_REGISTER 0x9
#define RTC_CENTURY_REGISTER 0x32

struct rtc_date_t
{
    uint seconds;
    uint minutes;
    uint hours;
    uint day;
    uint month;
    uint year;
};

uint8_t rtc_get_cmos_data(uint rtc_register);
void rtc_get_date(rtc_date_t* out);

#endif //KITTY_OS_CPP_RTC_H
