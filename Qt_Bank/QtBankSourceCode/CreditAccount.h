#ifndef CREDITACCOUNT_H
#define CREDITACCOUNT_H
#include "string"
#include "Account.h"
#include "Accumulator.h"

class CreditAccount : public Account{
private:
    Accumulator acc;
    double credit;
    double rate;
    double fee;
public:
    CreditAccount(Date date, std::string Log_on_User, std::string id, double credit, double rate, double fee);
    ~CreditAccount() override;
    double getDebt() override;
    double getCredit() const;
    double getRate() const;
    double getFee() const;
    double getAvailableCredit() const;
    void deposit(Date date, double amount, std::string desc) override;
    void withdraw(Date date, double amount, std::string desc) override;
    void settle(Date date) override;
    int isCreditAccount() const override {
        return 1;
    }
    std::string show() const override;
};


#endif //CREDITACCOUNT_H
