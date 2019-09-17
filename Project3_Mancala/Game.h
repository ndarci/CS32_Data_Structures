// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Side.h"
#include "Board.h"

class Board;
class Player;

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
private:
    Board m_board;
    Player* m_Splayer;
    Player* m_Nplayer;
    Side m_whoseTurn;
    void m_moveNorth();
    void m_moveSouth();
    bool m_isOver;
    bool m_hasW;
    Side m_whoWon;
    void m_pressEnter();
};

#endif // GAME_INCLUDED
