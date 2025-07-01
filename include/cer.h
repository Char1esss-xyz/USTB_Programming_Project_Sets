#ifndef CER_H
#define CER_H

#include <string>

class cer
{
private:
    std::string src;
    std::string des;
    double rate;

public:
    cer(std::string p_src, std::string p_des, double p_rate);
    double exchange(std::string p_src, std::string p_des, int value);
    std::string getSrc();
    std::string getDes();
    double getRate();
};

#endif