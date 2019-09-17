//  Sequence.h

#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

#include <string>

using ItemType = std::string;

class Sequence
{
public:
    // Default constructor
    Sequence() { m_size = 0; head = nullptr; }
    // My functions
    ~Sequence();
    Sequence(const Sequence& other);
    Sequence& operator=(const Sequence& other);
    void dump() const;
    // Required functions
    bool empty() const { return (head == nullptr); }
    int size() const { return m_size; }
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
private:
    class Node;
    Node* head;
    int m_size;
};

class Sequence::Node
{
public:
    Node(const ItemType& val, Node* n = nullptr, Node* l = nullptr);
    Node* next() { return m_next; }
    Node* last() { return m_last; }
    ItemType data() { return m_data; }
    void setNext(Node* p) { m_next = p; }
    void setLast(Node* p) { m_last = p; }
    void setData(const ItemType value) { m_data = value; }
private:
    Node* m_next;
    Node* m_last;
    ItemType m_data;
};

int subsequence( const Sequence& seq1, const Sequence& seq2 );
void interleave( const Sequence& seq1, const Sequence& seq2, Sequence& result );

#endif // SEQUENCE_INCLUDED
