#include "bankusers.h"

BankUsers::BankUsers(const string& id, const string& password) {
    this->id = id;
    this->password = password;
}

string BankUsers::getID() const {
    return id;
}

string BankUsers::getPassword() const {
    return password;
}
