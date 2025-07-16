#ifndef BANKUSERS_H
#define BANKUSERS_H

#include <string>
using namespace std;

class BankUsers {
public:
    BankUsers(const string& id, const string& password);
    string getID() const;
    string getPassword() const;

private:
    string id;
    string password;
};

#endif // BANKUSERS_H
