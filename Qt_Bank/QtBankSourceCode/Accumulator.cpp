#include "Accumulator.h"
using namespace std;

Accumulator::Accumulator(Date date, double value): lastDate(date), value(value), sum(0){}
// 获取实际存款
double Accumulator::getSum(Date date) {
    double a = sum + value * (date - this->lastDate);
    return a;
}
// 存取款
void Accumulator::change(Date date, double value) {
    sum = getSum(date);
    lastDate = date;
    this->value = value;
}
// 每月重置
void Accumulator::reset(Date date, double value) {
    lastDate = date;
    this->value = value;
    sum = 0;
}

Accumulator::~Accumulator() = default;
