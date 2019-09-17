// Game.cpp

#include "Game.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north)
: m_board(b), m_Splayer(south), m_Nplayer(north), m_whoseTurn(SOUTH)
{
    // create a game with a copy of board b, South goes first
}

void Game::display() const
{
    // NORTH label
    cout << "  ";
    cout << m_Nplayer->name() << endl;
    // NORTH side of board
    cout << "  ";
    for(int i = 1; i <= m_board.holes(); i++)
        cout << m_board.beans(NORTH, i) << " ";
    cout << endl;
    // NORTH & SOUTH pots
    cout << m_board.beans(NORTH, 0);
    for(int i = 0; i < m_board.holes(); i++)
        cout << "  ";
    cout << " " << m_board.beans(SOUTH, 0);
    cout << endl;
    // SOUTH side of board
    cout << "  ";
    for(int i = 1; i <= m_board.holes(); i++)
        cout << m_board.beans(SOUTH, i) << " ";
    cout << endl;
    // SOUTH label
    cout << "  ";
    cout << m_Splayer->name() << endl;
    cout << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    // check if game is over, set the winner if it is
    // if NORTH is empty, the game's over
    over = false;
    if(m_board.beansInPlay(NORTH) == 0)
        over = true;
    // if SOUTH is empty, the game's over
    if(m_board.beansInPlay(SOUTH) == 0)
        over = true;
    // there's a winner if one pot is bigger
    hasWinner = (m_board.beans(NORTH, 0) != m_board.beans(SOUTH, 0));
    if(m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0))
        winner = NORTH;
    else
        winner = SOUTH;
}

void Game::m_moveNorth()
{
    Side endS;
    int endH;
    int m = m_Nplayer->chooseMove(m_board, NORTH);
    m_board.sow(NORTH, m, endS, endH);
    // landed in a pot, take another turn
    if(endS == NORTH && endH == 0)
    {
        status(m_isOver, m_hasW, m_whoWon);
        if(m_isOver)
            return;
        display();
        cerr << m_Nplayer->name() << " gets another turn." << endl << endl;
        display();
        // call this same function recursively
        m_moveNorth();
    }
    // did a capture, perform that capture
    if(endS == NORTH && endH > 0
       && m_board.beans(NORTH, endH) == 1
       && m_board.beans(SOUTH, endH) > 0)
    {
        cerr << m_Nplayer->name() << " is capturing " << m_board.beans(SOUTH, endH) + 1 << " beans." << endl << endl;
        if(m_board.moveToPot(SOUTH, endH, NORTH) && m_board.moveToPot(NORTH, endH, NORTH))
            return;
    }
}

void Game::m_moveSouth()
{
    Side endS;
    int endH;
    int m = m_Splayer->chooseMove(m_board, SOUTH);
    m_board.sow(SOUTH, m, endS, endH);
    // landed in a pot, take another turn
    if(endS == SOUTH && endH == 0)
    {
        status(m_isOver, m_hasW, m_whoWon);
        if(m_isOver)
            return;
        display();
        cerr << m_Splayer->name() << " gets another turn." << endl << endl;
        display();
        // call this same function recursively
        m_moveSouth();
    }
    // did a capture, perform that capture
    if(endS == SOUTH && endH > 0
       && m_board.beans(SOUTH, endH) == 1
       && m_board.beans(NORTH, endH) > 0)
    {
        cerr << m_Splayer->name() << " is capturing " << m_board.beans(NORTH, endH) + 1 << " beans." << endl << endl;
        if(m_board.moveToPot(NORTH, endH, SOUTH) && m_board.moveToPot(SOUTH, endH, SOUTH))
            return;
    }
}

bool Game::move()
{
    // have whoever's turn it is make a move (as long as the game isn't over)
    status(m_isOver, m_hasW, m_whoWon);
    if(m_isOver)
    {
        // game's over, sweep remaining beans
        display();
        cerr << "Game over!" << endl << endl;
        cerr << "Sweeping remaining beans into ";
        if(m_board.beansInPlay(SOUTH) == 0)
        {
            cerr << m_Nplayer->name() << "'s pot..." << endl << endl;
            for(int i = 1; i <= m_board.holes(); i++)
            {
                if(m_board.moveToPot(NORTH, i, NORTH))
                    continue;
            }
        }
        else if(m_board.beansInPlay(NORTH) == 0)
        {
            cerr << m_Splayer->name() << "'s pot..." << endl << endl;
            for(int i = 1; i <= m_board.holes(); i++)
            {
                if(m_board.moveToPot(SOUTH, i, SOUTH))
                    continue;
            }
        }
        return false;
    }
    // take the turn of whoever's turn it is
    switch(m_whoseTurn)
    {
        case NORTH:
            m_moveNorth();
            // when they're done, switch our turn tracker to the other player
            m_whoseTurn = SOUTH;
            break;
        case SOUTH:
            m_moveSouth();
            m_whoseTurn = NORTH;
            break;
    }
    // check if the game just ended
    status(m_isOver, m_hasW, m_whoWon);
    if(m_isOver)
        // call this function again if the game just ended so we can sweep the rest of the beans
        move();
    return true;
}

void Game::m_pressEnter()
{
    cout << "Press ENTER to continue.";
    cin.ignore();
}

void Game::play()
{
    // play the game... use display function so someone can follow what's happening
    cerr << "Starting a game between " << m_Nplayer->name() << " and " << m_Splayer->name() << "." << endl << endl;
    cerr << m_Splayer->name() << " goes first." << endl << endl;
    display();
    // keep making moves until the game is ended (move returns false)
    while(move())
    {
        status(m_isOver, m_hasW, m_whoWon);
        if(m_isOver)
            break;
        display();
        if(m_Nplayer->isInteractive() == false && m_Splayer->isInteractive() == false)
            m_pressEnter();
    }
    // game's over, beans have been swept, announce winner
    display();
    status(m_isOver, m_hasW, m_whoWon);
    if(m_hasW)
    {
        switch(m_whoWon)
        {
            case NORTH:
                cout << m_Nplayer->name();
                break;
            case SOUTH:
                cout << m_Splayer->name();
                break;
        }
        cout << " won!" << endl;
    }
    else
        cout << "It's a tie!" << endl;
    return;
}

int Game::beans(Side s, int hole) const
{
    // FOR TESTING... give # of beans in this hole
    return m_board.beans(s, hole);
}
