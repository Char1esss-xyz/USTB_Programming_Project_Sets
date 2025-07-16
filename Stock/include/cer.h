#ifndef CER_H
#define CER_H

#include <string>
#include <map>
#include <stdexcept>

class cer
{
public:
    cer(){};
    void add_rate(std::string src, std::string des, double rate);
    double convert(std::string src, std::string des, double value);

private:
    std::map<std::string, std::map<std::string, double>> cer_list;
};
#endif