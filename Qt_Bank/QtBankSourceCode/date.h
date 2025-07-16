#ifndef __Date__
#define __Date__

#include "iostream"
using namespace std;
class Date;

class Date
{
private:
    int year, month, day;
    int totalDays; // 从2000年1月1日开始
public:
    Date() = default;
    Date(int year, int month, int day);
    ~Date();
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    bool isLeapYear() const;
    int getTotalDays() const;
    int getMaxDay() const;
    int distance() const; //获得从2000年1月1日距今多少天
    void show() const;

    //重载运算符
    int operator - (const Date &d) const;
    bool operator < (const Date &d) const;
    bool operator == (const Date &d) const;
    friend ostream & operator<<(ostream &out, const Date &date);

    static Date read();
};

class DateException : public exception{
public:
    const char *what() const noexcept override{
        return "Not a right Date. Please input again.";
    }
};


#endif
