#include "../include/cer.h"
#include <string>

cer::cer(std::string p_src, std::string p_des, double p_rate)
{
    src = p_src;
    des = p_des;
    rate = p_rate;
}

double cer::exchange(std::string p_src, std::string p_des, int value)
{
    if(src == p_src) return value * rate;
    if(src == p_des) return value / rate;
}

std::string cer::getSrc()
{
    return src;
}

std::string cer::getDes()
{
    return des;
}

double cer::getRate()
{
    return rate;
}