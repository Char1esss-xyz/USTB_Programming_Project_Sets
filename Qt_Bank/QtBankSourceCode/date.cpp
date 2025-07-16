#include "date.h"

Date::Date(int year, int month, int day):year(year), month(month), day(day)
{
    int days_in_month[13] = {0, 31, (28 + ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(year < 1900 || month > 12 || month < 1 || day < 1 || day > days_in_month[month]){
        throw DateException();
    }
    totalDays = distance();
}

Date::~Date()
= default;

int Date::distance() const
{
    int the_days_form_the_begin = 0;
    int days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // 每月天数表

    // 计算从 2000 年 1 月 1 日到目标年份的前一年结束的总天数
    int years = year - 2000;

    // 普通年份天数
    the_days_form_the_begin += years * 365;

    // 闰年天数：从 2000 年到目标年份前一年之间的闰年数量
    int leap_years = ((year - 1) / 4 - 1999 / 4)
                     - ((year - 1) / 100 - 1999 / 100)
                     + ((year - 1) / 400 - 1999 / 400);
    the_days_form_the_begin += leap_years;

    // 加上当前年份中月份的天数
    for(int i = 1; i < month; ++i) {
        the_days_form_the_begin += days_in_month[i];
    }

    // 如果是闰年且月份大于2，加1天
    if(month > 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        the_days_form_the_begin += 1;
    }

    // 加上当前月份的天数（减去1，因为1号已经算在1月1日）
    the_days_form_the_begin += day - 1;

    return the_days_form_the_begin;
}

int Date::operator - (const Date &d) const {
    return this->totalDays - d.totalDays;
}

int Date::getYear() const {
    return this->year;
}

int Date::getMonth() const{
    return this->month;
}

int Date::getDay() const{
    return this->day;
}

bool Date::isLeapYear() const{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int Date::getTotalDays() const{
    return this->totalDays;
}

ostream &operator <<(ostream &out,const Date &date) {
    return out << date.year << "-" << date.month << "-" << date.day;
}

void Date::show() const {
    cout << year << "-" << month << "-" << day;
}

int Date::getMaxDay() const {
    int days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(isLeapYear())days_in_month[2]=29;
    return days_in_month[month];
}

bool Date::operator < (const Date &d) const {
    return this->totalDays < d.totalDays;
}

bool Date::operator == (const Date &d) const {
    return this->totalDays == d.totalDays;
}

Date Date::read() {
    int year = 0, month = 0, day = 0;
    char ch;
    cin >> year >> ch >> month >> ch >> day;
    return {year, month, day};
}


