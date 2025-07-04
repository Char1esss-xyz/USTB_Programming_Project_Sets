#include <iostream>
#include <vector>
#include "../include/cer.h"
#include "../include/person.h"
#include "../include/stock.h"
using namespace std;

int main()
{
    int trade_round = 0;
    int cer_round = 0;
    int query_round = 0;
    vector<person> persons;
    person cer_manager("cer_manager");

    cin >> trade_round;
    for (int i = 0; i < trade_round; i++)
    {
        char p_category;
        string p_name;
        string p_currency;
        double p_amount;
        double p_price;

        cin >> p_category >> p_name >> p_currency >> p_amount >> p_price;
        for (auto &people : persons)
        {
            if (p_name == people.getName())
            {
                people.createStock(p_category, p_currency, p_amount, p_price);
                break;
            }
        }
        person tmp_person(p_name);
        tmp_person.createStock(p_category, p_currency, p_amount, p_price);
        persons.push_back(tmp_person);        
    }

    cin >> cer_round;
    for(int i = 0; i < cer_round; i++)
    {
        string p_src;
        string p_des;
        double p_rate;

        cin >> p_src >> p_des >> p_rate;
        cer_manager.pushCer(p_src, p_des, p_rate);
    }

    cin >> query_round;
    for(int i = 0; i < query_round; i++)
    {
        string query_type;
        cin >> query_type;

        if(query_type == "PERSON")
        {
            string p_name;
            string p_currency;
            double res = 0;
            cin >> p_name >> p_currency;

            for(auto j : persons)
            {
                if(j.getName() == p_name)
                {
                    res += j.queryPerson(p_currency);
                }
            }
            cout << res << endl;
        }

        else if(query_type == "STOCK")
        {
            double res = 0;
            char p_category;
            string p_currency;
            cin >> p_category >> p_currency;

            for(auto j : persons)
            {
                res += j.queryStock(p_category, p_currency);
            }
            cout << res << endl;
        }
    }
    return 0;
}