//  History.h

#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_nR;
    int m_nC;
    int m_mem[MAXROWS][MAXCOLS];
    char decode(int n) const;
};

#endif
