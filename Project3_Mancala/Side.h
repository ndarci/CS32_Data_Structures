// Side.h

#ifndef SIDE_INCLUDED
#define SIDE_INCLUDED

class Board;
class Game;

enum Side { NORTH, SOUTH };

const int INFINITY = 999;
const int TIMELIMIT = 4500;

const int NSIDES = 2;
const int POT = 0;

int eval(Board b);

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif // SIDE_INCLUDED
