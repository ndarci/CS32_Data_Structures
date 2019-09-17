//  Sequence.cpp

#include "Sequence.h"
#include <iostream>

using namespace std;

Sequence::Node::Node(const ItemType& val, Node* n, Node* l)
{
    // only Node constructor that gets used
    // initialize basic Node values
    m_data = val;
    m_next = n;
    m_last = l;
}

Sequence::~Sequence()
{
    // Sequence destructor
    // delete all Nodes in the Sequence
    Node* p = head;
    while(p != nullptr)
    {
        // nextOne avoids losing access to p->next() when we delete the Node p points to
        Node* nextOne = p->next();
        delete p;
        p = nextOne;
    }
}

Sequence::Sequence(const Sequence& other)
{
    // Sequence copy constructor
    // initialize basic Sequence values
    m_size = 0;
    head = nullptr;
    if( other.m_size > 0 )
    {
        head = new Node(other.head->data());
        m_size++;
        int i = 1;
        // fill this Sequence with every Node of the other Sequence
        for( Node* p = other.head->next(); p != nullptr; p = p->next() )
        {
            if( insert(i, p->data()) )
                i++;
        }
    }
}

Sequence& Sequence::operator=(const Sequence& other)
{
    // Sequence = operator function
    if (this != &other)
    {
        // use swap to transfer information, temp is destroyed after scope of the function
        Sequence temp(other);
        swap(temp);
    }
    return *this;
}

void Sequence::dump() const
{
    // print out the contents of a Sequence and their positions
    int i = 0;
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        cerr << "Position " << i << ": " << p->data() << endl;
        i++;
    }
    // print out the Sequence length
    cerr << "Sequence length: " << m_size << endl;
    cerr << endl;
}

int Sequence::insert(int pos, const ItemType& value)
{
    if( pos < 0 || pos > size() ) // check boundaries
        return -1;
    int i = 0; // i acts as a position counter
    Node* temp = nullptr; // temp is used when we're adding to the end
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        if(i == size() - 1)     // if this is the last element in the Sequence
            temp = p;           // keep a pointer to it before we point p to nullptr
        if(i == pos)
        {
            Node* newNode = new Node(value, p, p->last()); // insert a new Node, either in the middle or at the end
            p->setLast(newNode);
            if(newNode->last() != nullptr)
                newNode->last()->setNext(newNode);
            else
                head = newNode;
            m_size++;
            return pos;
        }
        i++;
    }
    Node* newNode = new Node(value);    // insert a new Node, if this is the beginning
    if(size() == 0)                     // check if the Sequence is empty
        head = newNode;
    else
    {
        newNode->setLast(temp);         // add to the beginning of a non-empty Sequence
        temp->setNext(newNode);
    }
    m_size++;
    return pos;
}

int Sequence::insert(const ItemType& value)
{
    int i = 0;
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        if( value <= p->data() )        // find the position where value is <= that Node's data
        {
            insert(i, value);           // insert the new Node in that position
            return i;
        }
        i++;
    }
    insert(i, value);
    return i;
}

bool Sequence::erase(int pos)
{
    if( pos < 0 || pos > size() )
        return false;
    int i = 0;
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        if( i == pos )          // find the Node that we want to delete
        {
            if(p->next() == nullptr && p->last() == nullptr)    // this is the only Node in the Sequence
                head = nullptr;                                 // make this the empty Sequence
            else if(p->next() == nullptr)       // this is the last Node
                p->last()->setNext(nullptr);    // disconnect it from the Sequence
            else if(p->last() == nullptr)       // this is the first Node
            {
                p->next()->setLast(nullptr);    // disconnect it from the Sequence
                head = p->next();
            }
            else                                // this is some Node in the middle
            {
                p->next()->setLast(p->last());  // disconnect it
                p->last()->setNext(p->next());
            }
            delete p; // get rid of the Node in question
            m_size--;
            return true;
        }
        i++;
    }
    return false;
}

int Sequence::remove(const ItemType& value)
{
    int i = 0;
    int numRemoved = 0;
    for( Node* p = head; p != nullptr; )
    {
        if( p->data() == value )        // find a Node that matches our value
        {
            p = p->next();              // keep iterating in case there are more Nodes to remove
            if( erase(i) == true )      // delete this Node
                numRemoved++;
        }
        else
        {
            p = p->next();
            i++;
        }
    }
    return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if( pos < 0 || pos >= size() )
        return false;
    int i = 0;
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        if( pos == i )          // find the Node at the position we want
        {
            value = p->data();  // copy its data into our value variable
            return true;
        }
        i++;
    }
    return false;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if( pos < 0 || pos >= size() )
        return false;
    int i = 0;
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        if( pos == i )          // find the Node at the position we want
        {
            p->setData(value);  // copy our value into its data variable
            return true;
        }
        i++;
    }
    return false;
}

int Sequence::find(const ItemType& value) const
{
    int i = 0;
    for( Node* p = head; p != nullptr; p = p->next() )
    {
        if( p->data() == value )    // find the first Node that matches our value
            return i;               // tell the calling function where that Node is
        i++;
    }
    i = -1;
    return i;
}

void Sequence::swap(Sequence& other)
{
    Node* temp = other.head;    // keep the address of the other Sequence's head so it doesn't get lost
    other.head = head;          // swap the head pointers
    head = temp;
    int t = other.m_size;       // swap the size variables
    other.m_size = m_size;
    m_size = t;
}

int subsequence( const Sequence& seq1, const Sequence& seq2 )
{
    ItemType box1;  // variables to store Node data values temporarily
    ItemType box2;
    int returnMe;   // variable to store position we eventually return
    if( seq1.size() == 0 || seq2.size() == 0 )  // check that we have non-empty Sequences
        return -1;
    else
        seq2.get(0, box2); // hold onto the first element of seq2
    for( int i = 0; i < seq1.size(); i++ )
    {
        if(seq1.get(i, box1) && box1 == box2)   // first element of seq2 matches with something in seq1
        {
            returnMe = i;
            i++;
            while( seq1.get(i, box1) && seq2.get(i-returnMe, box2) )    // make sure every other consecutive element in seq2 maps to seq1
            {
                if( box1 != box2 || (i == seq1.size() - 1 && i-returnMe != seq2.size() - 1) )
                    return -1;
                i++;
            }
            return returnMe;    // only gets here if seq2 has been fully iterated through...
                                // ...without terminating seq1 and every Node in seq2 was matched in seq1 (in order)
        }
    }
    return -1;      // something wasn't satisfied. Tell the caller seq2 is not a subsequence of seq1
}

void interleave( const Sequence& seq1, const Sequence& seq2, Sequence& result )
{
    Sequence temp;  // store interleaved sequence in here in case result is also one of the input Sequences
    ItemType box1;
    ItemType box2;
    int i = 0;
    int ri = 0;
    while( seq1.get(i, box1) && seq2.get(i, box2) )     // as long as both Sequences still have Nodes left...
    {
        temp.insert(ri, box2);      // insert them into result (with seq1's Nodes in front)
        temp.insert(ri, box1);
        i++;
        ri += 2;
    }
    if( !seq2.get(i, box2) )        // if it was seq2 that ran out...
        while(seq1.get(i, box1))    // insert the rest of seq1 into result
        {
            temp.insert(ri, box1);
            i++;
            ri++;
        }
    else if( !seq1.get(i, box1) )   // if it was seq1 that ran out...
        while(seq2.get(i, box2))    // insert the rest of seq2 into result
        {
            temp.insert(ri, box2);
            i++;
            ri++;
        }
    while( result.size() > 0 )      // delete all the Nodes in result
        result.erase(0);
    result = temp;                  // copy temp into result
}
