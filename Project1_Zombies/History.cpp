// History.cpp

#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_nR = nRows;
    m_nC = nCols;
    for(int i = 0; i < nRows; i++)
    {
        for(int j = 0; j < nCols; j++)
            m_mem[i][j] = 0;
    }
}

bool History::record(int r, int c)
{
    if (r < 1 || r > m_nR || c < 1 || c > m_nC)
        return false;
    m_mem[r - 1][c - 1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    
    for(int i = 0; i < m_nR; i++)
    {
        for(int j = 0; j < m_nC; j++)
        {
            cout << History::decode(m_mem[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}

char History::decode(int n) const
{
    switch(n)
    {
        case 0: return '.';
        case 1: return 'A';
        case 2: return 'B';
        case 3: return 'C';
        case 4: return 'D';
        case 5: return 'E';
        case 6: return 'F';
        case 7: return 'G';
        case 8: return 'H';
        case 9: return 'I';
        case 10: return 'J';
        case 11: return 'K';
        case 12: return 'L';
        case 13: return 'M';
        case 14: return 'N';
        case 15: return 'O';
        case 16: return 'P';
        case 17: return 'Q';
        case 18: return 'R';
        case 19: return 'S';
        case 20: return 'T';
        case 21: return 'U';
        case 22: return 'V';
        case 23: return 'W';
        case 24: return 'X';
        case 25: return 'Y';
    }
    return 'Z';
}
