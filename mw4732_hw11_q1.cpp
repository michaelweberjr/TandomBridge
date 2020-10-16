#include <iostream>
using namespace std;

void printTriangle(int);
void printOpositeTriangles(int);
void printRuler(int);

int main()
{
    printTriangle(6);
    cout << "\n===================================================================================\n" << endl;
    printOpositeTriangles(4);
    cout << "\n===================================================================================\n" << endl;
    printRuler(5);
}

void printTriangle(int n)
{
    if (n != 1)
    {
        printTriangle(n - 1);
    }

    for (int i = 0; i < n; i++)
    {
        cout << "*";
    }
    cout << endl;
}

void printOpositeTriangles(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "*";
    }
    cout << endl;

    if (n != 1)
    {
        printOpositeTriangles(n - 1);
    }

    for (int i = 0; i < n; i++)
    {
        cout << "*";
    }
    cout << endl;
}

void printRuler(int n)
{
    if (n != 1)
    {
        printRuler(n - 1);
    }

    for (int i = 0; i < n; i++)
    {
        cout << "-";
    }
    cout << endl;

    if (n != 1)
    {
        printRuler(n - 1);
    }
}