#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int BOARD_SIZE = 6;
int* createBoard(int);
void printBoard(int*, int);
int solveBoard(int*, int);

int main()
{
    int* board = createBoard(BOARD_SIZE);
    //int board[BOARD_SIZE] = { 0, 4, 25, 76, 30, 12, 18, 3, 42 };
    printBoard(board, BOARD_SIZE);
    cout << solveBoard(board, BOARD_SIZE) << endl;
}

int* createBoard(int size)
{
    int* board = new int[size];
    board[0] = 0;
    srand(time(0));
    for (int i = 1; i < size; i++)
    {
        board[i] = rand() % 100 + 1;
    }

    return board;
}

void printBoard(int* board, int size)
{
    cout << "| ";
    for (int i = 0; i < size; i++)
    {
        cout << board[i] << " | ";
    }
    cout << endl;
}

int solveBoard(int* board, int size)
{
    if (size == 1) return board[0];

    int jump;
    int step;
    if (size > 2)
    {
        jump = board[0] + solveBoard(board + 2, size - 2);
        step = board[0] + solveBoard(board + 1, size - 1);
        if (jump < step)
        {
            return jump;
        }
        else
        {
            return step;
        }
    }
    else
    {
        return board[0] + solveBoard(board + 1, size - 1);
    }
}
