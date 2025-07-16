#include "CreditAccount.h"
#include <sstream>
#include <iomanip>
using namespace std;

// 构造函数
CreditAccount::CreditAccount(Date date, std::string Log_on_User, std::string id,
                             double credit, double rate, double fee)
    : Account(date, Log_on_User, id), acc(date, 0), credit(credit), rate(rate), fee(fee) {}

// 返回利率
double CreditAccount::getRate() const{
    return this->rate;
}

// 返回信用额
double CreditAccount::getCredit() const{
    return this->credit;
}

// 返回年费
double CreditAccount::getFee() const {
    return this->fee;
}

// 返回额度
double CreditAccount::getAvailableCredit() const {
    return credit;
}

// 存款
void CreditAccount::deposit(Date date, double amount, std::string desc) {
    if (amount <= 0) {
        throw std::runtime_error("Amount should be positive");
    }

    balance += amount;
    acc.change(date, getDebt());

    record(date, amount, desc);
}

// 取款
void CreditAccount::withdraw(Date date, double amount, std::string desc) {
    if (amount <= 0) {
        throw std::runtime_error("Amount should be positive");
    }

    if (amount > getAvailableCredit() + balance) {
        std::cout << "overcredit" << std::endl;
        throw AccountException(this);
    }

    balance -= amount;

    record(date, -amount, desc);
}

// 结算利息、年费
void CreditAccount::settle(Date date) {
    double interest = acc.getSum(date) * rate;
    if(interest != 0){
        record(date, interest, "interest");
    }
    if(date.getMonth() == 1){
        record(date, -fee, "annual fee");
    }
    acc.reset(date, getDebt());
}

// 获取负债
double CreditAccount::getDebt() {
    if(balance < 0) return balance;
    else return 0;
}

std::string CreditAccount::show() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    if (balance < 0) {
        oss << id << " Balance: 0.00"
            << " Available credit:" << (getAvailableCredit() + balance);
    } else {
        oss << Account::show()
            << " Available credit:" << getAvailableCredit();
    }

    return oss.str();
}

CreditAccount::~CreditAccount() = default;;
