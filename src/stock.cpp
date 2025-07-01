#include "../include/stock.h"

stock::stock(char p_category, std::string p_currency, double p_amount, double p_price)
{
    category = p_category;
    currency = p_currency;
    amount = p_amount;
    price = p_price;
}

char stock::getCategory()
{
    return category;
}

std::string stock::getCurrency()
{
    return currency;
}

double stock::getAmount()
{
    return amount;
}

double stock::getPrice()
{
    return price;
}