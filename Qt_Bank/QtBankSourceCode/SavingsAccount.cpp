#include "SavingsAccount.h"
#include "Accumulator.h"
#include "iostream"
using namespace std;

// 构造函数
SavingsAccount::SavingsAccount(Date date, std::string Log_on_User, std::string id, double rate) : Account(date, Log_on_User, id), acc(date, 0), rate(rate) {
    cout << date << "\t" << Log_on_User << "\t#" << id << " created" << endl;
}

// 存款
void SavingsAccount::deposit(Date date, double amount, std::string desc) {
    if (amount <= 0) {
        throw std::runtime_error("Amount should be positive");
    }
    balance += amount;
    record(date, amount, desc);
    acc.change(date, getBalance());
}

// 取款
void SavingsAccount::withdraw(Date date, double amount, std::string desc) {
    if (amount <= 0) {
        throw std::runtime_error("Amount should be positive");
    }

    if(amount <= getBalance()) {
        balance -= amount;
        record(date, -amount, desc);
        acc.change(date, getBalance());
    }
    else throw AccountException(this);
}

// 返回利率
double SavingsAccount::getRate() const{
    return this->rate;
}

// 结算利息
void SavingsAccount::settle(Date date) {
    if(date.getMonth() != 1) return;
    double a = acc.getSum(date);
    // 计算日期
    int days = (date - Date(date.getYear() - 1, 1, 1)) + 1;
    // 计算利息
    double interest = a * rate / days;
    record(date, interest, "interest");
    acc.change(date, interest);
    acc.reset(date, getBalance());
}

