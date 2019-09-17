// Main.cpp

#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    HumanPlayer t("Niko");
    SmartPlayer s("roboNiko");
    Board b(3, 2);
    Game g(b, &t, &s);
    g.play();
    
//    // testing Board...
//    // testing sow...
//    //  2 2 2
//    // 0     0
//    //  2 2 2
//    Board b(3, 2);
//    Side endS = SOUTH;
//    int endH = 1;
//    // normal case
//    assert(b.sow(SOUTH, 1, endS, endH));
//    assert(endS == SOUTH && endH == 3 && b.beans(SOUTH, 3) == 3);
//    //  2 2 2
//    // 0     0
//    //  0 3 3
//    // lands in a hole
//    assert(b.sow(NORTH, 2, endS, endH));
//    assert(endS == NORTH && endH == 0 && b.beans(NORTH, 0 == 1));
//    //  3 0 2
//    // 1     0
//    //  0 3 3
//    // long sow that wraps around
//    b.setBeans(SOUTH, 3, 10);
//    //  3 0 2
//    // 1      0
//    //  0 3 10
//    assert(b.sow(SOUTH, 3, endS, endH));
//    assert(endS == NORTH && endH == 2 && b.beans(SOUTH, 3) == 1);
//    //  4 2 4
//    // 1     2
//    //  1 4 1
//    // testing moveToPot...
//    // normal case
//    assert(b.moveToPot(SOUTH, 2, SOUTH));
//    assert(b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, 0) == 6);
//    //  4 2 4
//    // 1     6
//    //  1 0 1
//    // counter case, should be false
//    assert(b.moveToPot(NORTH, 0, SOUTH) == false);
//
//    // testing Player...
//    // testing HumanPlayer...
//    Board c(3, 0);
//    c.setBeans(SOUTH, 1, 1);
//    c.setBeans(NORTH, 2, 1);
//    //  0 1 0
//    // 1     6
//    //  1 0 0
//    HumanPlayer h("morty");
//    // regular case
//    cerr << "Tester: choose hole 1" << endl;
//    assert(h.chooseMove(c, SOUTH) == 1);
//    // case where chooseMove must reprompt for valid move
//    c.setBeans(SOUTH, 1, 0);
//    c.setBeans(SOUTH, 2, 1);
//    cerr << "Tester: choose hole 1" << endl;
//    assert(h.chooseMove(c, SOUTH) == 2);
//
//    // testing BadPlayer...
//    BadPlayer bad("charlie");
//    // regular case
//    assert(bad.chooseMove(c, NORTH) == 2);
//
//    // testing SmartPlayer...
//    SmartPlayer s("dolores");
//    // regular case
//    assert(s.chooseMove(c, NORTH) == 2);
//    //  0 1 0
//    // 0     0
//    //  3 2 1
//    Board b1(3,0);
//    b1.setBeans(SOUTH, 1, 3);
//    b1.setBeans(SOUTH, 2, 2);
//    b1.setBeans(SOUTH, 3, 1);
//    b1.setBeans(NORTH, 2, 1);
//    // win-in-one-turn case
//    assert(s.chooseMove(b1, SOUTH) == 3);
//
//    // testing Game...
//
//    //  2 2 2
//    // 0     0
//    //  2 2 2
//    Board d(3, 2);
//    BadPlayer p1("ren");
//    BadPlayer p2("stimpy");
//    Game g(d, &p1, &p2);
//    // p1 goes first
//    // testing move...
//    // normal cases
//    assert(g.move());
//    //  2 2 2
//    // 0     0
//    //  0 3 3
//    assert(g.beans(SOUTH, 3) == 3);
//    assert(g.move());
//    //  0 2 2
//    // 1     0
//    //  1 3 3
//    assert(g.beans(SOUTH, 1) == 1);
//    assert(g.move());
//    //  0 2 2
//    // 1     0
//    //  0 4 3
//    // go-again case
//    assert(g.move());
//    //  1 1 0
//    // 3     0
//    //  0 4 3
//    assert(g.beans(NORTH, 0) == 3 && g.beans(NORTH, 3) == 0);
//    // capture case
//    d.setBeans(SOUTH, 3, 0);
//    //  2 2 2
//    // 0     0
//    //  2 2 0
//    Game g2(d, &p1, &p2);
//    assert(g2.move());
//    //  2 2 0
//    // 0     3
//    //  0 3 0
//    assert(g2.beans(SOUTH, 0) == 3 && g2.beans(SOUTH, 3) == 0 && g2.beans(NORTH, 3) == 0);
//    // testing play...
//    // checking that a game between two computer players will eventually end
//    Board e(3, 2);
//    SmartPlayer br("bret");
//    SmartPlayer je("jemaine");
//    Game g3(e, &br, &je);
//    g3.play();
//    bool isO, hasW = false;
//    Side winner = SOUTH;
//    g3.status(isO, hasW, winner);
//    assert(isO);
//
//    cerr << endl << "woohoo" << endl;
}

int eval(Board b)
{
    // evaluates the current state of a board from SOUTH's perspective
    int sPot = b.beans(SOUTH, 0);
    int nPot = b.beans(NORTH, 0);
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
        // game is over... who won?
    {
        int sMinusN = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
        if(sMinusN == 0)
            return 0;
        if(sMinusN > 0)
            return INFINITY;
        if(sMinusN < 0)
            return -INFINITY;
    }
    // game is in progress... what's the score differential?
    int allBeans = b.totalBeans();
    if(sPot > allBeans/2)
        return INFINITY;
    if(nPot > allBeans/2)
        return -INFINITY;
    return sPot - nPot;
}
