#include "../include/person.h"
#include "../include/stock.h"
#include "../include/cer.h"
#include <string>
#include <vector>

cer person::cers;

person::person(std::string p_name)
{
    name = p_name;
}

void person::createStock(char p_category, std::string p_currency, double p_amount, double p_price)
{
    if (p_amount < 0 || p_price < 0)
        throw std::runtime_error("error: amount and price must be positive");
    for (auto &i : stocks)
    {
        if (i.getCategory() == p_category && i.getCurrency() == p_currency)
        {
            i.addAmount(p_amount);
            return;
        }
    }
    stocks.push_back(stock(p_category, p_currency, p_amount, p_price));
}

std::string person::getName()
{
    return name;
}

double person::queryPerson(std::string p_currency)
{
    double res = 0;
    for (auto i : stocks)
    {
        double totalPrice = i.getAmount() * i.getPrice();
        if (i.getCurrency() == p_currency)
        {
            res += totalPrice;
        }
        else
        {
            res += cers.convert(i.getCurrency(), p_currency, totalPrice);
        }
    }
    return res;
}

void person::pushCer(std::string p_src, std::string p_des, double p_rate)
{
    cers.add_rate(p_src, p_des, p_rate);
}

double person::queryStock(char p_category, std::string p_currency)
{
    double res = 0;
    for (auto i : stocks)
    {
        if (i.getCategory() == p_category)
        {
            double totalPrice = i.getAmount() * i.getPrice();
            if (i.getCurrency() == p_currency)
            {
                res += totalPrice;
            }
            else
            {

                res += cers.convert(i.getCurrency(), p_currency, totalPrice);
            }
        }
    }
    return res;
}
