#include "Account.h"
#include <cmath>
#include "map"
#include <sstream>
#include <iomanip>

double Account::total = 0;
multimap<Date, AccountRecord> Account::recordMap;

Account::Account(const Date& date, std::string Log_on_User, std::string id ) : id(id), Log_on_User(Log_on_User), balance(0){
}

Account::~Account() = default;

void Account::record(Date date, double amount, const string& desc) {
    amount = floor(amount * 100 + 0.5) / 100; // 保留两位小数
    recordMap.insert(make_pair(date, AccountRecord(date, this, amount, balance, desc)));
}

std::string Account::getId() const{
    return this->id;
}

std::string Account::getUser() const{
    return this->Log_on_User;
}

double Account::getTotal() {
    return total;
}

std::string Account::show() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2); // 固定小数点，保留两位
    oss << id << " Balance: " << balance;
    return oss.str();
}

string Account::query(Date &day1, Date &day2, string user) {
    string temp = "";
    for(auto & it : recordMap){
        if((day1 < it.first || day1 == it.first) && (it.first < day2 || it.first == day2) && it.second.account->getUser() == user){
            string line = to_string(it.first.getYear()) + "-" +
                          to_string(it.first.getMonth()) + "-" +
                          to_string(it.first.getDay()) + " " +
                          it.second.account->getId() + " " +
                          (ostringstream() << fixed << setprecision(2) << it.second.amount).str() + " " +
                          (ostringstream() << fixed << setprecision(2) << it.second.balance).str() + " " +
                          it.second.desc + "\n";
            temp += line;
        }
    }
    return temp;
}

double Account::calculateTotalIncomeBetweenDates(Date day1, Date day2, string user){
    double Income = 0;
    for(auto & it : recordMap){
        if((day1 < it.first || day1 == it.first) && (it.first < day2 || it.first == day2) && it.second.account->getUser() == user){
            const AccountRecord& record = it.second;
            if (record.account->getUser() == user && record.amount > 0) {
                    Income += record.amount;
            }
        }
    }
    return Income;
}

double Account::calculateTotalExpenseBetweenDates(Date day1, Date day2, string user){
    double Expense = 0;
    for(auto & it : recordMap){
        if((day1 < it.first || day1 == it.first) && (it.first < day2 || it.first == day2) && it.second.account->getUser() == user){
            const AccountRecord& record = it.second;
            if (record.account->getUser() == user && record.amount < 0) {
                Expense += record.amount;
            }
        }
    }
    return Expense;
}

AccountRecord::AccountRecord(const Date& date, const Account *account, double amount, double balance, std::string desc)
        :date(date), account(account), amount(amount), balance(balance), desc(desc){}
