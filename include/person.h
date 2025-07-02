#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include "../include/stock.h"
#include "../include/cer.h"

class person
{
private:
    std::string name;
    std::vector<stock> stocks;
    static std::vector<cer> cers;

public:
    person(std::string p_name);
    void createStock(char p_category, std::string p_currency, double p_amount, double p_price);
    std::string getName();
    double queryPerson(std::string p_currency);
    void pushCer(std::string p_src, std::string p_des, double p_rate);
};

#endif