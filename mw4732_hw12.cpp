/*
    CH12 #7: Write a checkbook balancing program. The program will read in, from the console, the following for all checks that were not cashed as of the last time you balanced your checkbook: the number of each check (int), the amount of the check (double), and whether or not it has been cashed (1 or 0, boolean in the array). Use an array with the class as the type. The class should be a class for a check. There should be three member variables to record the check number, the check amount, and whether or not the check was cashed. The class for a check will have a member variable of type Money (as defined on page 662 in the book; Display 11.9) to record the check amount. So, you will have a class used within a class. The class for a check should have accessor and mutator functions as well as constructors and functions for both input and output of a check.  In addition to the checks, the program also reads all the deposits (from the console; cin), the old and the new account balance (read this in from the user at the console; cin). You may want another array to hold the deposits. The new account balance should be the old balance plus all deposits, minus all checks that have been cashed.

    The program outputs the total of the checks cashed, the total of the deposits, what the new balance should be, and how much this figure differs from what the bank says the new balance is. It also outputs two lists of checks: the checks cashed since the last time you balanced your checkbook and the checks still not cashed. [ edit: if you can, Display both lists of checks in sorted order from lowest to highest check number.]
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

class Money
{
public:
    friend Money operator+(const Money&, const Money&);
    Money& operator+=(const Money&);
    friend Money operator-(const Money&, const Money&);
    Money& operator-=(const Money&);
    friend Money operator-(const Money&);
    friend Money operator==(const Money&, const Money&);
    friend Money operator<(const Money&, const Money&);
    Money(long dollars, int cents) : allCents(dollars*100 + cents) { }
    Money(long dollars) : allCents(dollars*100) { }
    Money() : allCents(0) { }
    double getValue() const { return (double)allCents / 100.0; }
    string toString() const;
    friend istream& operator>>(istream& ins, Money& a);
    friend ostream& operator<<(ostream& outs, Money& a);

private:
    long allCents;
};

class Check
{
private:
    int id;
    Money amount;
    bool cashed;

public:
    int getID() const { return id; };
    void setID(int newID) { id = newID; }
    Money getAmount() const { return amount; };
    void setAmount(Money newAmount) { amount = newAmount; }
    bool hasBeenCashed() const { return cashed; };
    bool updateCashed(bool newCashed) { cashed = newCashed; }
    string toString() const;

    Check(int newID, Money newAmount, bool isCashed) : id(newID), amount(newAmount), cashed(isCashed) { }
    friend ostream& operator<<(ostream& outs, Check& check);
};

class Account
{
private:
    Money startingBalance;
    Money estimatedBalance;
    vector<Money> deposits;
    vector<Check> checks;

public:
    Money getFinalBalance();
    Money getCashedCheckAmount() const;
    Money getEstimatedBalance() const { return estimatedBalance; }
    Money getStartingBalance() const { return startingBalance; }
    vector<Money>& getDeposits() { return deposits; }
    vector<Check>& getChecks() { return checks; }

    Account(Money balance, Money endBalance) : startingBalance(balance), estimatedBalance(endBalance) { }
    void addCheck(Check);
    void addDeposit(Money);
    void printStatement();
    void sortChecksByID();
};

void readChecks(Account&);
void readDeposits(Account&);
string numberToString(long);
void quickSort(vector<Check>&, int, int);
int partition(vector<Check>&, int, int);

int main()
{
    cout << "Check Account Balancer" << endl;
    cout << "Please enter all dollar amounts in the following format: $X.XX or -$X.XX\n" << endl;
    Money balance;
    cout << "Please enter starting balance: ";
    cin >> balance;
    
    Money endBalance;
    cout << "Please enter estimated end balance: ";
    cin >> endBalance;
    cout << endl;

    Account acc = Account(balance, endBalance);

    readChecks(acc);
    cout << endl;
    readDeposits(acc);
    cout << endl;
    
    acc.sortChecksByID();
    acc.printStatement();
    Money final = acc.getFinalBalance();
    cout << "\nYour new balance is: " << final << endl;
    Money estimatedDiff = final - acc.getEstimatedBalance();
    cout << "Your estimated blance was off by " << estimatedDiff << endl;
}

void readChecks(Account& acc)
{
    bool stillRunning = true;
    string response;
    cout << "Do you have any checks to enter (y/n)? ";
    cin >> response;
    cout << endl;

    while (stillRunning)
    {
        if (response[0] == 'n' || response[0] == 'N')
        {
            stillRunning = false;
        }
        else
        {
            int ID;
            Money m;
            bool cashed;
            string cashedResponse;
            cout << "Please enter the ID of the check: ";
            cin >> ID; 
            cout << "Please enter the check amount: ";
            cin >> m;
            cout << "Has the check been cashed (y/n)? ";
            cin >> cashedResponse;
            if (cashedResponse[0] == 'y' || cashedResponse[0] == 'Y')
            {
                cashed = true;
            }
            else
            {
                cashed = false;
            }
            
            acc.addCheck(Check(ID, m, cashed));

            cout << "\nDo you have more checks to enter (y/n)? ";
            cin >> response;
            cout << endl;
        }
    }
}

void readDeposits(Account& acc)
{
    bool stillRunning = true;
    string response;
    cout << "Do you have any deposits to enter (y/n)? ";
    cin >> response;
    cout << endl;

    while (stillRunning)
    {
        if (response[0] == 'n' || response[0] == 'N')
        {
            stillRunning = false;
        }
        else
        {
            Money m;
            cout << "Please enter the amount of the deposit: ";
            cin >> m;
            acc.addDeposit(m);

            cout << "\nDo you have more deposits to enter (y/n)? ";
            cin >> response;
            cout << endl;
        }
    }
}

string numberToString(long num)
{
    string result;
    if (num < 10)
    {
        result.push_back(num + '0');
        return result;
    }
    else
    {
        result.push_back((num % 10) + '0');
        return numberToString(num / 10) + result;
    } 
}

string Money::toString() const
{
    long tempCents = allCents;
    string neg;
    if (allCents < 0)
    {
        neg = "-";
        tempCents = -tempCents;
    }

    long dollars = tempCents / 100;
    int cents = tempCents - dollars * 100;

    string trailingZero;
    if (cents == 0) trailingZero = "0";
    return neg + "$" + numberToString(dollars) + "." + numberToString(cents) + trailingZero;
}

Money operator+(const Money& a, const Money& b)
{
    long allCents = a.allCents + b.allCents;
    long dollars = allCents / 100;
    int cents = allCents - dollars * 100;
    return Money(dollars, cents);
}

Money& Money::operator+=(const Money& rhs)
{
    allCents += rhs.allCents;
    return *this;
}

Money operator-(const Money& a, const Money& b)
{
    long allCents = a.allCents - b.allCents;
    long dollars = allCents / 100;
    int cents = allCents - dollars * 100;
    return Money(dollars, cents);
}

Money& Money::operator-=(const Money& rhs)
{
    allCents -= rhs.allCents;
    return *this;
}

Money operator-(const Money& a)
{
    long allCents = a.allCents;
    long dollars = allCents / 100;
    int cents = allCents - dollars * 100;
    return Money(-dollars, cents);
}

Money operator==(const Money& a, const Money& b)
{
    return a.allCents == b.allCents;
}

Money operator<(const Money& a, const Money& b)
{
    return a.allCents < b.allCents;
}

istream& operator>>(istream& ins, Money& a)
{
    char symbol, decimalPoint, digit1, digit2;
    long dollars;
    int cents;
    bool negative;
    
    ins >> symbol;
    if (symbol == '-')
    {
        negative = true;
        ins >> symbol;
    }
    else
    {
        negative = false;
    }

    ins >> dollars >> decimalPoint >> digit1 >> digit2;

    if (symbol != '$' || decimalPoint != '.' || !isdigit(digit1) || !isdigit(digit2))
    {
        cout << "Error illegal form for muney input\n";
        exit(1);
    }

    cents = (digit1 - '0') * 10 + (digit2 - '0');
    a.allCents = dollars * 100 + cents;

    if (negative)
    {
        a.allCents = -a.allCents;
    }

    return ins;
}

ostream& operator<<(ostream& outs, Money& a)
{
    outs << a.toString();
    return outs;
}

string Check::toString() const
{
    string checkStr = "Check: ID = " + numberToString(id) + ", Amount = " + amount.toString() + ", ";
    if (cashed)
    {
        checkStr += "check has been cashed";
    }
    else
    {
        checkStr += "check has not been chashed";
    }
    return checkStr;
}

ostream& operator<<(ostream& outs, Check& check)
{
    cout << check.toString();
    return outs;
}

void Account::addCheck(Check c)
{
    checks.push_back(c);
}

void Account::addDeposit(Money m)
{
    deposits.push_back(m);
}

Money Account::getFinalBalance()
{
    Money finalBalance = startingBalance;
    for (int i = 0; i < deposits.size(); i++)
    {
        finalBalance += deposits[i];
    }

    for (int i = 0; i < checks.size(); i++)
    {
        if (checks[i].hasBeenCashed())
        {
            finalBalance -= checks[i].getAmount();
        }
    }

    return finalBalance;
}

Money Account::getCashedCheckAmount() const
{
    Money m;
    for (int i = 0; i < checks.size(); i++)
    {
        if (checks[i].hasBeenCashed())
        {
            m += checks[i].getAmount();
        }
    }

    return m;
}

void Account::printStatement()
{
    cout << "Account Starting Balance: " << startingBalance << endl;
    Money checksCashed = getCashedCheckAmount();
    cout << "Total value of cashed checks: " << checksCashed << endl << endl;

    cout << "Cashed Checks: " << endl;
    for (int i = 0; i < checks.size(); i++)
    {
        if (checks[i].hasBeenCashed())
        {
            cout << checks[i] << endl;
        }
    }

    cout << "\nUncashed Checks: " << endl;
    for (int i = 0; i < checks.size(); i++)
    {
        if (!checks[i].hasBeenCashed())
        {
            cout << checks[i] << endl;
        }
    }

    cout << "\nDeposits:" << endl;
    for (int i = 0; i < deposits.size(); i++)
    {
        cout << "[" << i + 1 << "]: " << deposits[i] << endl;
    }
}

void Account::sortChecksByID()
{
    quickSort(checks, 0, checks.size() - 1);
}

// sorts checks by id number in ascending order
void quickSort(vector<Check>& list, int start, int end)
{
    if (start < end)
    {
        int p = partition(list, start, end);
        quickSort(list, start, p - 1);
        quickSort(list, p + 1, end);
    }
}

int partition(vector<Check>& list, int start, int end)
{
    int pivot = list[end].getID();
    int pIndex = start;

    for (int i = start; i < end; i++)
    {
        if (list[i].getID() < pivot)
        {
            Check temp = list[i];
            list[i] = list[pIndex];
            list[pIndex++] = temp;
        }
    }

    Check temp = list[end];
    list[end] = list[pIndex];
    list[pIndex] = temp;

    return pIndex;
}