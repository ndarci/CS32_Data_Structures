// Board.h

#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include "Side.h"
#include <vector>
using namespace std;

class Board
{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    int m_nHoles;
    int m_nPot;
    int m_sPot;
    int m_sowNorth(int hole, int& bTS);
    int m_sowSouth(int hole, int& bTS);
    vector<int> m_north;
    vector<int> m_south;
};

#endif // BOARD_INCLUDED
