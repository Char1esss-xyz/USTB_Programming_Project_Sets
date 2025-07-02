#include "../include/person.h"
#include <string>
#include <vector>

std::vector<cer> person::cers;

person::person(std::string p_name)
{
    name = p_name;
}

void person::createStock(char p_category, std::string p_currency, double p_amount, double p_price)
{
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
            for (auto j : cers)
            {
                if (j.getSrc() == p_currency || j.getDes() == p_currency)
                {
                    res += j.exchange(i.getCurrency(), p_currency, totalPrice);
                }
            }
        }
    }
    return res;
}

void person::pushCer(std::string p_src, std::string p_des, double p_rate)
{
    cer p_cer(p_src, p_des, p_rate);
    cers.push_back(p_cer);
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
                res += i.getAmount() * i.getPrice();
            else
            {
                for (auto j : cers)
                {
                    if (j.getSrc() == p_currency || j.getDes() == p_currency)
                    {
                        res += j.exchange(i.getCurrency(), p_currency, totalPrice);
                    }
                }
            }
        }
    }
    return res;
}
