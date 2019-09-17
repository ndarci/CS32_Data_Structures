// Player.cpp

#include "Side.h"
#include "Player.h"
#include "Board.h"
#include <stack>
#include <iostream>
using namespace std;

Player::Player(string name)
: m_name(name)
{}

string Player::name() const
{
    // get the name
    return m_name;
}

bool Player::isInteractive() const
{
    // false if player is a computer
    return false;
}

HumanPlayer::HumanPlayer(string name)
: Player(name)
{}

HumanPlayer::~HumanPlayer()
{}

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    cout << this->name() << ", choose a move..." << endl;
    int move;
    cin >> move;
    // move is not on the board
    if(move <= 0 || move > b.holes())
    {
        cout << "Invalid move! Valid moves are between 1 and " << b.holes() << "." << endl;
        // try again (with recursion!)
        return chooseMove(b, s);
    }
    // no beans to move
    if(b.beans(s, move) == 0)
    {
        cout << "Invalid move! You must choose a hole with beans in it." << endl;
        // try again
        return chooseMove(b, s);
    }
    return move;
}

BadPlayer::BadPlayer(string name)
: Player(name)
{}

BadPlayer::~BadPlayer()
{}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    switch(s)
    {
        // find the first hole with any beans in it, that's the first valid move
        // just chooses the first valid move (left to right)
        case NORTH:
            for(int i = 1; i <= b.holes(); i++)
                if(b.beans(NORTH, i) > 0)
                    return i;
            break;
        case SOUTH:
            for(int i = 1; i <= b.holes(); i++)
                if(b.beans(SOUTH, i) > 0)
                    return i;
            break;
    }
    return -1; // something's very wrong
}

//==========================================================================
// AlarmClock ac(numMilliseconds);  // Set an alarm clock that will time out
//                                  // after the indicated number of ms
// if (ac.timedOut())  // Will be false until the alarm clock times out; after
//                     // that, always returns true.
//==========================================================================

#include <chrono>
#include <future>
#include <atomic>

class AlarmClock
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

SmartPlayer::SmartPlayer(string name)
: Player(name)
{}

SmartPlayer::~SmartPlayer()
{}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    int returnMe = 1;
    int value = 0;
    // start the clock: we have 5 seconds to perform this move.
    AlarmClock ac(TIMELIMIT);
    m_chooseMove(ac, s, b, returnMe, value);
    return returnMe;
}

void SmartPlayer::m_doNextMove(AlarmClock& ac, Side s, Board& b, int m) const
{
    // similar to Game::move but without writing to cout
    // performs a complete move for m_chooseMove to analyze
    Side endS = SOUTH; // temp value
    int endH = 1;
    b.sow(s, m, endS, endH);
    if(endH == 0 && endS == s)
    {
        // get another move, need to call m_chooseMove again to see where to go next
        int next, v;
        m_chooseMove(ac, s, b, next, v);
        // now next contains the next move to make after landing in our own pot
        m_doNextMove(ac, s, b, next);
    }
    else if(endS == s && endH > 0 && b.beans(s, endH) == 1 && b.beans(opponent(s), endH) > 0)
    {
        // got a capture, move beans to pot
        if(!b.moveToPot(opponent(s), endH, s) || !b.moveToPot(s, endH, s))
            return; // should never be executed
    }
    if(b.beansInPlay(SOUTH) == 0)
        // game's over, NORTH gets the rest of the beans
    {
        for(int i = 1; i <= b.holes(); i++)
        {
            if(b.moveToPot(NORTH, i, NORTH))
                continue;
        }
        return;
    }
    else if (b.beansInPlay(NORTH) == 0)
        // game's over, SOUTH gets the rest of the beans
    {
        for(int i = 1; i <= b.holes(); i++)
        {
            if(b.moveToPot(SOUTH, i, SOUTH))
                continue;
        }
        return;
    }
    return;
}

void SmartPlayer::m_chooseMove(AlarmClock& ac, Side s, const Board& b, int& bestHole, int& value) const
{
    // put all our possible moves in a stack to compare them
    stack<int> validMoves;
    for(int i = 1; i <= b.holes(); i++)
    {
        if(b.beans(s, i) > 0)
            validMoves.push(i);
    }
    // base case: either the game is over or we're out of time
    if(ac.timedOut() || validMoves.empty())
    {
        bestHole = -1;
        value = eval(b);
        return;
    }
    // need to keep track of what the best move so far was so we can output it
    int currentBestVal = eval(b);
    int currentBestMove = validMoves.top();
    // iterate through every possible move on this board configuration
    while(validMoves.size() > 0)
    {
        if(ac.timedOut())
        {
            value = currentBestVal;
            break;
        }
        int thisMove = validMoves.top();
        validMoves.pop();
        Board b2(b);
        // simulate this move
        m_doNextMove(ac, s, b2, thisMove);
        int newBH = 1;
        int newVal = 0;
        // see the effects of the opponent's best countermove
        m_chooseMove(ac, opponent(s), b2, newBH, newVal);
        switch(s)
        {
            // check if it's a good idea to go in this direction down the game tree
            case SOUTH:
                if(newVal > currentBestVal)
                {
                    value = newVal;
                    currentBestVal = newVal;
                    currentBestMove = thisMove;
                }
                else
                {
                    value = eval(b);
                }
                break;
            case NORTH:
                if(newVal < currentBestVal)
                {
                    value = newVal;
                    currentBestVal = newVal;
                    currentBestMove = thisMove;
                }
                else
                {
                    value = eval(b);
                }
                break;
        }
    }
    // output the best move we came up with from all our options after the loop
    bestHole = currentBestMove;
    return;
}
