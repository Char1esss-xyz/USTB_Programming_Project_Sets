#ifndef STOCK_H
#define STOCK_H

#include <string>
class stock
{
private:
    char category;
    double amount;
    double price;
    std::string currency;

public:
    stock(char p_category, std::string p_currency, double p_amount, double p_price);
    void addAmount(double p_amount);
    char getCategory();
    std::string getCurrency();
    double getAmount();
    double getPrice();
};

#endif