#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

#include <string>
#include "Account.h"
#include "Accumulator.h"

class SavingsAccount :public Account{
private:
    Accumulator acc;
    double rate;
public:
    SavingsAccount(Date date, std::string Log_on_User, std::string id, double rate);
    double getRate() const;
    void deposit(Date date, double amount, std::string desc) override;
    void withdraw(Date date, double amount, std::string desc) override;
    void settle(Date date) override;
    double getDebt() override{
        return 0;
    }
    int isCreditAccount() const override {
        return 0;
    }
};


#endif //SAVINGSACCOUNT_H
