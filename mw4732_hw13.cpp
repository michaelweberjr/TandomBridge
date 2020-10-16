/*
    CH16# 9:
    The goal for this Project is to create a simple two-dimensional predator-prey simulation. In this simulation the prey are ants and the pred-ators are doodlebugs. These critters live in a world composed of a 20 x 20 grid of cells. Only one critter may occupy a cell at a time. The grid is  enclosed, so a critter is not allowed to move off the edges of the world. Time is simulated in time steps. Each critter performs some action every time step.

    The ants behave according to the following model:
    Move. Every time step, randomly try to move up, down, left, or right. If the neighboring cell in the selected direction is occupied or would move the ant off the grid, then the ant stays in the current cell.

    Breed. If an ant survives for three time steps, then at the end of the time step (that is; after moving) the ant will breed. This is simulated by creat-ing a new ant in an adjacent (up, down, left, or right) cell that is empty. If there is no empty cell available, then no breeding occurs. Once an off-spring is produced, an ant cannot produce an offspring until three more time steps have elapsed.

    The doodlebugs behave according to the following model:
    Move. Every time step, if there is an adjacent ant (up, down, left, or right), then the doodlebug will move to that cell and eat the ant. Otherwise, the doodlebug moves according to the same rules as the ant. Note that a doodlebug cannot eat other doodlebugs.

    Breed. If a doodlebug survives for eight time steps, then at the end of the time step it will spawn off a new doodlebug in the same manner as the ant.

    Starve. If a doodlebug has not eaten an ant within the last three time steps, then at the end of the third time step it will starve and die. The doodlebug should then be removed from the grid of cells.

    During one turn, all the doodlebugs should move before the ants do.

    Write a program to implement this simulation and draw the world using ASCII characters of "o" for an ant and "X" for a doodlebug or "-" for an empty space. Create a class named Organism that encapsulates basic data common to both ants and doodlebugs. This class should have a virtual function named move that is defined in the derived classes of Ant and Doodlebug. You may need additional data structures to keep track of which critters have moved.

    Initialize the world with 5 doodlebugs and 100 ants. After each time step, prompt the user to press Enter to move to the next time step. You should see a cyclical pattern between the population of predators and prey, although random perturbations may lead to the elimination of one or both species.
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const int ANT_TURNS_TO_BREED = 3;
const int DOO_TURNS_TO_BREED = 8;
const int DOO_TURNS_TO_STARVE = 3;

const int DEGREES_OF_FREEDOM = 4;
const int DIR_UP = 0;
const int DIR_RIGHT = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;

class Board;

class Organism
{
private:
    int x, y;
    bool needsToMove;

protected:
    void setMoved() { needsToMove = false; }

public:
    Organism(int startX, int startY) : x(startX), y(startY), needsToMove(true) {}
    Organism(int startX, int startY, bool fromBreeding) : x(startX), y(startY), needsToMove(fromBreeding) {}
    virtual ~Organism() { }

    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    bool status() const { return needsToMove;  }
    void reset() { needsToMove = true;  }

    virtual void move(Board& board);
    virtual char toChar() const = 0;
    virtual bool isAnt() const = 0;
};

class Ant : public Organism
{
private:
    int turnsToBreeding;

public:
    Ant(int startX, int startY) : Organism(startX, startY), turnsToBreeding(0) {}
    Ant(int startX, int startY, bool fromBreeding) : Organism(startX, startY, fromBreeding), turnsToBreeding(0) {}

    virtual void move(Board& board);
    virtual char toChar() const { return 'o'; }
    virtual bool isAnt() const { return true;  }
};

class DoodleBug : public Organism
{
private:
    int turnsToBreeding;
    int turnsToStarving;

public:
    DoodleBug(int startX, int startY) : Organism(startX, startY), turnsToBreeding(0), turnsToStarving(0) {}
    DoodleBug(int startX, int startY, bool fromBreeding) : Organism(startX, startY, fromBreeding), turnsToBreeding(0), turnsToStarving(0) {}

    virtual void move(Board& board);
    virtual char toChar() const { return 'X'; }
    virtual bool isAnt() const { return false; }
};

class Board
{
private:
    int sizeX, sizeY;
    int turnCount, antBreedCount, antAteCount, doodBreedCount, doodStarveCount;
    int antTurnBreedCount, antTurnAteCount, doodTurnBreedCount, doodTurnStarveCount;

    // 1d board array of Organism*
    Organism** cells;

    vector<Organism*> garbage;
    void cleanup();

public:
    Board(int newSizeX, int newSizeY, int antCount, int doodleBugCount);
    ~Board();

    void step();
    void print();
    bool isEmpty(int x, int y);
    bool hasAnt(int x, int y);
    void setCell(int x, int y, Organism* creature);
    void eraseCell(int x, int y);
    int doodlebugCount();
    void getRandomDir(int startX, int startY, bool findFirst, int& outX, int& outY);

    void upAntBreedCount() { antBreedCount++; antTurnBreedCount++; }
    void upAntAteCount() { antAteCount++; antTurnAteCount++; }
    void upDoodBreedCount() { doodBreedCount++; doodTurnBreedCount++; }
    void upDoodStarveCount() { doodStarveCount++; doodTurnStarveCount++; }
};

int main()
{
    srand(time(0));
    Board board = Board(20, 20, 100, 5);

    while (true)
    {
        board.print();

        cout << "Press enter to go to the next turn..." << endl;
        cin.ignore(1);

        board.step();
    }
}


void Organism::move(Board& board)
{
    needsToMove = false;
     
    int tempX, tempY;
    board.getRandomDir(x, y, true, tempX, tempY);

    if (tempX != -1)
    {
        board.setCell(tempX, tempY, this);
        board.setCell(x, y, nullptr);
        x = tempX;
        y = tempY;
    }
}

void Ant::move(Board& board)
{
    Organism::move(board);

    if (++turnsToBreeding == ANT_TURNS_TO_BREED)
    {
        int tempX, tempY;
        board.getRandomDir(getX(), getY(), false, tempX, tempY);

        if (tempX != -1)
        {
            Ant* newAnt = new Ant(tempX, tempY, true);
            board.setCell(tempX, tempY, newAnt);
            board.upAntBreedCount();
        }

        turnsToBreeding = 0;
    }
}

void DoodleBug::move(Board& board)
{
    bool triedDir[DEGREES_OF_FREEDOM] = { false, false, false, false };
    bool ateAnAnt = false;

    for (int i = 0; i < DEGREES_OF_FREEDOM; i++)
    {
        int dir = rand() % (DEGREES_OF_FREEDOM - i);
        int dirCount = 0;
        for (int j = 0; j < DEGREES_OF_FREEDOM; j++)
        {
            if (!triedDir[j])
            {
                if (dir == dirCount++)
                {
                    dir = j;
                    break;
                }
            }
        }

        int tempX = getX();
        int tempY = getY();
        switch (dir)
        {
        case DIR_UP:
            tempY--;
            break;
        case DIR_RIGHT:
            tempX++;
            break;
        case DIR_DOWN:
            tempY++;
            break;
        case DIR_LEFT:
            tempX--;
            break;
        }

        if (board.hasAnt(tempX, tempY))
        {
            ateAnAnt = true;
            board.eraseCell(tempX, tempY);
            board.setCell(tempX, tempY, this);
            board.setCell(getX(), getY(), nullptr);
            setX(tempX);
            setY(tempY);
            turnsToStarving = 0;
            break;
        }
        else
        {
            triedDir[dir] = true;
        }
    }

    if (ateAnAnt)
    {
        board.upAntAteCount();
        setMoved();
    }
    else
    {
        if (++turnsToStarving == DOO_TURNS_TO_STARVE)
        {
            board.eraseCell(getX(), getY());
            board.upDoodStarveCount();
            return;
        }
        else
        {
            Organism::move(board);
        }
    }

    if (++turnsToBreeding == DOO_TURNS_TO_BREED)
    {
        int tempX, tempY;
        board.getRandomDir(getX(), getY(), false, tempX, tempY);

        if (tempX != -1)
        {
            DoodleBug* newDood = new DoodleBug(tempX, tempY, true);
            board.setCell(tempX, tempY, newDood);
            board.upDoodBreedCount();
        }

        turnsToBreeding = 0;
    }
}

Board::Board(int newSizeX, int newSizeY, int antCount, int doodleBugCount)
    : sizeX(newSizeX), sizeY(newSizeY), turnCount(1), antBreedCount(0), antAteCount(0), doodBreedCount(0), doodStarveCount(0),
      antTurnBreedCount(0), antTurnAteCount(0), doodTurnBreedCount(0), doodTurnStarveCount(0)
{
    int totalSpaces = sizeX * sizeY;

    cells = new Organism*[totalSpaces];
    for (int i = 0; i < totalSpaces; i++)
    {
        cells[i] = nullptr;
    }

    int remainingSpaces = totalSpaces;
    for (int i = 0; i < antCount; i++)
    {
        int space = rand() % remainingSpaces--;
        int spaceCount = 0;
        for (int j = 0; j < totalSpaces; j++)
        {
            if (cells[j] == nullptr)
            {
                if (++spaceCount == space)
                {
                    int x = j % sizeY;
                    int y = j / sizeY;
                    cells[j] = new Ant(x, y);
                    break;
                }
            }
        }
    }

    for (int i = 0; i < doodleBugCount; i++)
    {
        int space = rand() % remainingSpaces--;
        int spaceCount = 0;
        for (int j = 0; j < totalSpaces; j++)
        {
            if (cells[j] == nullptr)
            {
                if (++spaceCount == space)
                {
                    int x = j % sizeY;
                    int y = j / sizeY;
                    cells[j] = new DoodleBug(x, y);
                    break;
                }
            }
        }
    }
}


void Board::step()
{
    antTurnBreedCount = 0;
    antTurnAteCount = 0;
    doodTurnBreedCount = 0;
    doodTurnStarveCount = 0;
    turnCount++;

    // run doodlebugs first
    for (int i = 0; i < sizeX * sizeY; i++)
    {
        if (cells[i] != nullptr && !cells[i]->isAnt() && cells[i]->status())
        {
            cells[i]->move(*this);
        }
    }

    // run ants second
    for (int i = 0; i < sizeX * sizeY; i++)
    {
        if (cells[i] != nullptr && cells[i]->status())
        {
            cells[i]->move(*this);
        }
    }

    // reset all of the organisms
    for (int i = 0; i < sizeX * sizeY; i++)
    {
        if (cells[i] != nullptr)
        {
            cells[i]->reset();
        }
    }
}

void Board::setCell(int x, int y, Organism* creature)
{
    cells[y * sizeX + x] = creature;
}

void Board::eraseCell(int x, int y)
{
    int cell = y * sizeX + x;
    garbage.push_back(cells[cell]);
    cells[cell] = nullptr;
}

void Board::getRandomDir(int startX, int startY, bool findFirst, int& outX, int& outY)
{
    bool triedDir[DEGREES_OF_FREEDOM] = { false, false, false, false };

    for (int i = 0; i < DEGREES_OF_FREEDOM; i++)
    {
        int dir = rand() % (DEGREES_OF_FREEDOM - i);
        int dirCount = 0;
        for (int j = 0; j < DEGREES_OF_FREEDOM; j++)
        {
            if (!triedDir[j])
            {
                if (dir == dirCount++)
                {
                    dir = j;
                    break;
                }
            }
        }

        int tempX = startX;
        int tempY = startY;
        switch (dir)
        {
        case DIR_UP:
            tempY--;
            break;
        case DIR_RIGHT:
            tempX++;
            break;
        case DIR_DOWN:
            tempY++;
            break;
        case DIR_LEFT:
            tempX--;
            break;
        }

        if (isEmpty(tempX, tempY))
        {
            outX = tempX;
            outY = tempY;
            return;
        }
        else
        {
            if (findFirst)
            {
                outX = -1;
                outY = -1;
                return;
            }

            triedDir[dir] = true;
        }
    }

    outX = -1;
    outY = -1;
}

bool Board::isEmpty(int x, int y)
{
    if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
    {
        return false;
    }

    int cell = y * sizeX + x;
    if (cells[cell] == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Board::hasAnt(int x, int y)
{
    if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
    {
        return false;
    }

    int cell = y * sizeX + x;
    if (cells[cell] == nullptr)
    {
        return false;
    }
    else
    {
        return cells[cell]->isAnt();
    }
}

void Board::print()
{
    cout << "Turn[" << turnCount << "]: Total ants breed = " << antBreedCount << ", Total ants eaten = " << antAteCount << ", Total doodlebugs breed = " << doodBreedCount
        << ", Total doodlebugs starved = " << doodStarveCount << endl;
    cout << "This turn: ants breed = " << antTurnBreedCount << ", ants eaten = " << antTurnAteCount << ", doodlebugs breed = " << doodTurnBreedCount
        << ", doodlebugs starved = " << doodTurnStarveCount << endl;

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            int cell = i * sizeY + j;
            if (cells[cell] == nullptr)
            {
                cout << "-";
            }
            else
            {
                cout << cells[cell]->toChar();
            }
        }
        cout << endl;
    }
}

void Board::cleanup()
{
    for (int i = 0; i < garbage.size(); i++)
    {
        delete garbage[i];
    }

    garbage.clear();
}

Board::~Board() 
{ 
    for (int i = 0; i < sizeX * sizeY; i++)
    {
        if (cells[i] != nullptr)
        {
            delete cells[i];
        }
    }

    cleanup();
    delete[] cells; 
}
