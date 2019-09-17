// Player.h

#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include "Side.h"
#include "Board.h"
#include <string>
#include <stack>
using namespace std;

class Board;
class AlarmClock;

class Player
{
public:
    Player(string name);
    virtual ~Player() {};
    string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
private:
    string m_name;
};

class BadPlayer : public Player
{
public:
    BadPlayer(string name);
    virtual ~BadPlayer();
    virtual int chooseMove(const Board& b, Side s) const;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(string name);
    virtual ~HumanPlayer();
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(string name);
    virtual ~SmartPlayer();
    virtual int chooseMove(const Board& b, Side s) const;
private:
    void m_chooseMove(AlarmClock& ac, Side s, const Board& b, int& bestHole, int& value) const;
    void m_doNextMove(AlarmClock& ac, Side s, Board& b, int m) const;
};

#endif // PLAYER_INCLUDED

