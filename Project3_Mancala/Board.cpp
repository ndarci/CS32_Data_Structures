// Board.cpp

#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if(nHoles < 1)
        nHoles = 1;
    if(nInitialBeansPerHole < 0)
        nInitialBeansPerHole = 0;
    m_nHoles = nHoles;
    m_nPot = 0;
    m_sPot = 0;
    for(int i = 0; i < m_nHoles; i++)
    {
        m_north.push_back(nInitialBeansPerHole);
        m_south.push_back(nInitialBeansPerHole);
    }
}

int Board::holes() const
{
    // number of holes on each side
    return m_nHoles;
}

int Board::beans(Side s, int hole) const
{
    // number of beans in a hole
    if(hole < 0 || hole > m_nHoles)
        return -1;
    switch(s)
    {
        case NORTH:
            if(hole == 0)
                return m_nPot;
            return m_north[hole-1];
            break;
        case SOUTH:
            if(hole == 0)
                return m_sPot;
            return m_south[hole-1];
            break;
    }
    return -1;
}

int Board::beansInPlay(Side s) const
{
    // beans in all holes of one side (not pot)
    int count = 0;
    switch(s)
    {
        case NORTH:
            for(int i = 0; i < m_north.size(); i++)
                count += m_north[i];
            return count;
            break;
        case SOUTH:
            for(int i = 0; i < m_south.size(); i++)
                count += m_south[i];
            return count;
            break;
    }
    return -1;
}

int Board::totalBeans() const
{
    // beans in all holes and both pots
    int count = beansInPlay(NORTH) + beansInPlay(SOUTH) + m_nPot + m_sPot;
    return count;
}

int Board::m_sowSouth(int hole, int& bTS)
{
    // called to sow from a hole on SOUTH's side
    // performs the sow & nothing more... more complex behavior delegated elsewhere
    int i = hole-1;
    while(i < m_south.size() && bTS > 0)
    {
        bTS--;
        m_south[i]++;
        i++;
    }
    if(bTS == 1)
        return 0; // landed in the pot after this side
    if(bTS > 1)
        return -1; // more beans to sow, continue...
    return i; // landed somewhere on this side
}

int Board::m_sowNorth(int hole, int& bTS)
{
    // does the same as m_sowSouth but NORTH version
    int i = hole-1;
    while(i >= 0 && bTS > 0)
    {
        bTS--;
        m_north[i]++;
        i--;
    }
    if(bTS == 1)
        return 0; // landed in the pot after this side
    if(bTS > 1)
        return -1; // more beans to sow, continue...
    return i+2; // landed somewhere on this side
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    // sow beans from one hole... NO extra turns/captures
    if(hole < 1 || hole > m_nHoles)
        return false;
    int beansToSow = 0;
    int pos = 0;
    switch(s)
    {
        case NORTH:
            beansToSow = m_north[hole-1];
            if(beansToSow == 0)
                return false;
            m_north[hole-1] = 0;
            // sow the beans on the NORTH side,
            // starting at the next hole (NORTH goes in reverse)
            pos = m_sowNorth(hole-1, beansToSow);
            if(pos >= 0)    // if we're done sowing...
                // here we've used all the beans on NORTH or the last is in nPot
            {
                endSide = NORTH;
                endHole = pos;
            }
            // continue while there are still beans to sow
            while(beansToSow > 0)
            {
                // currently we're at the end of the NORTH holes
                beansToSow--;   // add one to the NORTH pot
                m_nPot++;
                if(beansToSow <= 0)     // if we're done, get out of the loop
                    break;
                // otherwise, start sowing the SOUTH side
                // the SOUTH side works normally. Start at the front
                pos = m_sowSouth(1, beansToSow);
                if(pos < 1)     // if we still have more beans...
                {
                    pos = m_sowNorth(m_nHoles, beansToSow);    // sow the NORTH side again, starting at the back
                    // skips sPot because it's NORTH's turn
                }
                else    // we ended sowing somewhere on the SOUTH side
                {
                    endSide = SOUTH;
                    endHole = pos;
                    break;
                }
                if(pos > 0)     // get here if we ended sowing on the NORTH side,
                    // the second time around (when we started at the back)
                {
                    endSide = NORTH;
                    endHole = pos;
                    break;
                }
            }   // by the end of the loop, we've finished sowing,
            // or we're back where we started (the front end of NORTH)
            break;
        case SOUTH:
            beansToSow = m_south[hole-1];
            if(beansToSow == 0)
                return false;
            m_south[hole-1] = 0;
            // sow the beans on the SOUTH side,
            // starting at our current hole
            pos = m_sowSouth(hole+1, beansToSow);
            if(pos >= 0)    // if we're done sowing...
                            // here we've used all the beans on SOUTH or the last is in sPot
            {
                endSide = SOUTH;
                endHole = pos;
            }
            // continue while there are still beans to sow
            while(beansToSow > 0)
            {
                // currently we're at the end of the SOUTH holes
                beansToSow--;   // add one to the SOUTH pot
                m_sPot++;
                if(beansToSow <= 0)     // if we're done, get out of the loop
                    break;
                // otherwise, start sowing the NORTH side
                // the NORTH side works in reverse. Start at the back
                pos = m_sowNorth(m_nHoles, beansToSow);
                if(pos < 1)     // if we still have more beans...
                {
                    pos = m_sowSouth(1, beansToSow);    // sow the SOUTH side again, starting at the front
                                                        // skips nPot because it's SOUTH's turn
                }
                else    // we ended sowing somewhere on the NORTH side
                {
                    endSide = NORTH;
                    endHole = pos;
                    break;
                }
                if(pos > 0)     // get here if we ended sowing on the SOUTH side,
                                // the second time around (when we started at the front)
                {
                    endSide = SOUTH;
                    endHole = pos;
                    break;
                }
            }   // by the end of the loop, we've finished sowing,
                // or we're back where we started (the end of SOUTH)
            break;
    }
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    // move all beans in this hole to the pot of the potOwner
    if(hole < 1 || hole > m_nHoles)
        return false;
    int dumpMe = beans(s, hole);
    if(dumpMe == 0)
        return false;
    // add these beans to the right pot
    switch(potOwner)
    {
        case NORTH:
            m_nPot += dumpMe;
            break;
        case SOUTH:
            m_sPot += dumpMe;
            break;
    }
    // clear the hole the beans came from
    switch(s)
    {
        case NORTH:
            m_north[hole-1] = 0;
            break;
        case SOUTH:
            m_south[hole-1] = 0;
            break;
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
    // FOR TESTING... set that hole's beans to whatever we want
    if(hole < 0 || hole > m_nHoles)
        return false;
    if(beans < 0)
        return false;
    switch(s)
    {
        case NORTH:
            if(hole == 0)
            {
                m_nPot += beans;
                break;
            }
            m_north[hole-1] = beans;
            break;
        case SOUTH:
            if(hole == 0)
            {
                m_sPot += beans;
                break;
            }
            m_south[hole-1] = beans;
            break;
    }
    return true;
}
