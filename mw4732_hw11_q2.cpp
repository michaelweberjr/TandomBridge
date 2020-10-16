#include <iostream>
using namespace std;

const int ARRAY_1_SIZE = 6;
const int ARRAY_2_SIZE = 20;

int sumOfSquares(int[], int);
bool isSorted(int[], int);

int main()
{
    int arr[ARRAY_1_SIZE] = { 2, -1, 3, 10, 5, -8 };
    cout << "Result: " << sumOfSquares(arr, ARRAY_1_SIZE) << endl;
    cout << "Is sorted: " << (isSorted(arr, ARRAY_1_SIZE) ? "true" : "false") << endl;
    int sorted[ARRAY_2_SIZE] = { 1, 2, 6, 20, 21, 23, 24, 30, 40, 40, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
    cout << "Is sorted: " << (isSorted(sorted, ARRAY_2_SIZE) ? "true" : "false") << endl;
    int sorted2[ARRAY_2_SIZE] = { 1, 2, 6, 20, 21, 23, 24, 30, 39, -40, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
    cout << "Is sorted: " << (isSorted(sorted2, ARRAY_2_SIZE) ? "true" : "false") << endl;
}

int sumOfSquares(int arr[], int arrSize)
{
    if (arrSize == 1)
    {
        return arr[0] * arr[0];
    }

    return sumOfSquares(arr, arrSize - 1) + arr[arrSize - 1] * arr[arrSize - 1];
}

bool isSorted(int arr[], int arrSize)
{
    if (arrSize == 1)
    {
        return true;
    }

    if (!isSorted(arr, arrSize - 1))
    {
        return false;
    }
    else
    {
        return arr[arrSize - 1] >= arr[arrSize - 2];
    }
}
