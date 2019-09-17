//  testSequence.cpp

#include <iostream>
#include <cassert>
using namespace std;

#include "Sequence.h"

//check:
//typical situation (action in middle)
//at the head
//at the tail
//empty list
//one-element list


int main()
{
    ItemType box;
    
    cerr << "testing copy constructor ..." << endl;
    Sequence og;
    Sequence copy(og);
    // empty
    assert(copy.size() == 0);
    assert(!copy.get(0, box));
    // normal
    assert(og.insert(0, "foobar") == 0);
    Sequence copy2(og);
    assert(copy2.get(0, box) && box == "foobar");
    assert(copy2.size() == 1);
    //og.dump();
    //copy2.dump();
    assert(og.insert(1, "unicorn") == 1);
    Sequence copy3(og);
    assert(copy3.get(1, box) && box == "unicorn");
    assert(copy3.size() == 2);
    //og.dump();
    //copy3.dump();
    cerr << "Passed all tests." << endl << endl;
    
    cerr << "testing assignment operator ..." << endl;
    // empty
    Sequence f;
    Sequence g = f;
    assert(!f.get(0, box) && !g.get(0, box));
    // normal
    assert(f.insert(0, "foobar") == 0);
    Sequence h = f;
    assert(h.get(0, box) && box == "foobar");
    cerr << "Passed all tests." << endl << endl;
    
    cerr << "testing insert(int pos, const ItemType& value) ..." << endl;
    Sequence s;
    // out of bounds
    assert(s.insert(1, "1") == -1);
    assert(s.insert(-1, "1") == -1);
    // empty
    assert(s.insert(0, "1") == 0);
    // one element
    assert(s.insert(1, "2") == 1);
    // tail
    assert(s.insert(2, "3") == 2);
    assert(s.insert(3, "10") == 3);
    // typical
    assert(s.insert(2, "0") == 2);
    // head
    assert(s.insert(0, "13") == 0);
    cerr << "Passed all tests." << endl << endl;
    //s.dump();
    
    cerr << "testing insert(const ItemType& value) ..." << endl;
    Sequence a;
    // empty
    assert(a.insert("b") == 0);
    // one element
    assert(a.insert("b") == 0);
    assert(a.insert(0, "c") == 0);
    assert(a.insert(1, "d") == 1);
    assert(a.insert(2, "e") == 2);
    assert(a.insert(3, "x") == 3);
    // typical
    assert(a.insert("f") == 3);
    // tail
    assert(a.insert("z") == 7);
    // head
    assert(a.insert("a") == 0);
    cerr << "Passed all tests." << endl << endl;
    //a.dump();
    
    cerr << "testing erase(int pos) ... " << endl;
    Sequence c;
    // empty
    assert(!c.erase(0));
    assert(c.insert(0, "deleteme") == 0);
    // one element
    assert(c.erase(0) && c.size() == 0);
    // typical
    assert(a.erase(1));
    // head
    assert(a.erase(0));
    // tail
    assert(a.erase(4));
    // out of bounds
    assert(!a.erase(-1));
    assert(!a.erase(6));
    cerr << "Passed all tests." << endl << endl;
    //a.dump();
    
    cerr << "testing remove(const ItemType& value) ..." << endl;
    // empty
    assert(c.remove("foobar") == 0);
    assert(c.insert(0, "foobar") == 0);
    // one element
    assert(c.remove("foobar") == 1);
    assert(a.insert(0, "10") == 0);
    assert(a.insert(3, "10") == 3);
    // head and tail
    assert(a.remove("10") == 2);
    // out of bounds
    assert(a.remove("1000") == 0);
    assert(a.remove("10") == 0);
    cerr << "Passed all tests." << endl << endl;
    //a.dump();
    
    cerr << "testing get(int pos, ItemType& value) ..." << endl;
    // empty
    assert(!c.get(0, box));
    assert(c.insert(0, "foobar") == 0);
    // one element
    assert(c.get(0, box) && box == "foobar");
    assert(c.remove("foobar") == 1);
    Sequence b;
    assert(b.insert(0, "1") == 0);
    assert(b.insert(1, "2") == 1);
    assert(b.insert(2, "3") == 2);
    assert(b.insert(3, "10") == 3);
    // typical
    assert(b.get(1, box) && box == "2");
    // head
    assert(b.get(0, box) && box == "1");
    // tail
    assert(b.get(3, box) && box == "10");
    // out of bounds
    assert(!b.get(4, box));
    cerr << "Passed all tests." << endl << endl;
    //b.dump();
    
    cerr << "testing set(int pos, const ItemType& value ..." << endl;
    // empty
    assert(!c.set(0, "foobar"));
    assert(c.insert(0, "foobar") == 0);
    // one element
    assert(c.set(0, "FUBAR") && c.get(0, box) && box == "FUBAR");
    assert(c.remove("FUBAR") == 1);
    // typical
    assert(b.set(2, "33") && b.get(2, box) && box == "33");
    // head
    assert(b.set(0, "11") && b.get(0, box) && box == "11");
    // tail
    assert(b.set(3, "1000") && b.get(3, box) && box == "1000");
    cerr << "Passed all tests." << endl << endl;
    //b.dump();
    
    cerr << "testing find(const ItemType& value) ..." << endl;
    // empty
    assert(c.find("") == -1);
    assert(c.insert(0, "foobar") == 0);
    // one element
    assert(c.find("foobar") == 0);
    assert(c.remove("foobar") == 1);
    // out of bounds
    assert(b.find("69") == -1);
    // typical
    assert(b.find("2") == 1);
    // head
    assert(b.find("11") == 0);
    // tail
    assert(b.find("1000") == 3);
    cerr << "Passed all tests." << endl << endl;
    //b.dump();
    
    cerr << "testing swap(Sequence& other) ..." << endl;
    // typical
    Sequence x;
    Sequence y;
    assert(x.insert(0, "x0") == 0);
    assert(x.insert(1, "x1") == 1);
    assert(x.insert(2, "x2") == 2);
    assert(y.insert(0, "y0") == 0);
    assert(y.insert(1, "y1") == 1);
    x.swap(y);
    assert(x.get(0, box) && box == "y0");
    assert(y.get(0, box) && box == "x0");
    assert(x.size() == 2 && y.size() == 3);
    // empty (in all combinations)
    // c is empty...
    x.swap(c);
    assert(!x.get(0, box) && x.size() == 0);
    assert(c.get(0, box) && box == "y0");
    // x is empty...
    x.swap(y);
    assert(!y.get(0, box) && y.size() == 0);
    assert(x.get(0, box) && box == "x0");
    assert(c.erase(1) && c.erase(0));
    // c and y are empty
    c.swap(y);
    assert(c.size() == 0 && y.size() == 0);
    // one element
    assert(c.insert(0, "foo") == 0);
    assert(y.insert(0, "bar") == 0);
    c.swap(y);
    assert(c.get(0, box) && box == "bar");
    cerr << "Passed all tests." << endl << endl;

    cerr << "testing subsequence(const Sequence& seq1, const Sequence& seq2) ..." << endl;
    Sequence foo;
    Sequence bar;
    // normal case
    assert(foo.insert(0, "A") == 0);
    assert(foo.insert(1, "B") == 1);
    assert(foo.insert(2, "C") == 2);
    assert(foo.insert(3, "D") == 3);
    assert(foo.insert(4, "E") == 4);
    assert(foo.insert(5, "F") == 5);
    assert(bar.insert(0, "C") == 0);
    assert(bar.insert(1, "D") == 1);
    assert(bar.insert(2, "E") == 2);
    assert(subsequence(foo, bar) == 2);
    assert(subsequence(foo, a) == -1);
    assert(subsequence(bar, foo) == -1);
    assert(subsequence(foo, foo) == 0);
    Sequence emp;
    // empty case
    assert(subsequence(emp, emp) == -1);
    assert(subsequence(foo, emp) == -1);
    Sequence one;
    // one element
    assert(one.insert(0, "666") == 0);
    assert(subsequence(foo, one) == -1);
    assert(one.set(0, "E"));
    assert(subsequence(foo, one) == 4);
    assert(one.set(0, "A"));
    assert(subsequence(foo, one) == 0);
    assert(one.set(0, "F"));
    assert(subsequence(foo, one) == 5);
    assert(bar.insert(3, "F") == 3);
    assert(bar.insert(4, "G") == 4);
    assert(bar.insert(5, "H") == 5);
    assert(subsequence(foo, bar) == -1);
    cerr << "Passed all tests." << endl << endl;
    
    cerr << "testing interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) ..." << endl;
    Sequence nums;
    // normal case
    assert(nums.insert(0, "0") == 0);
    assert(nums.insert(1, "1") == 1);
    assert(nums.insert(2, "2") == 2);
    interleave(foo, nums, emp);
    assert(emp.get(5, box) && box == "2");
    assert(emp.get(6, box) && box == "D");
    // alias case
    interleave(emp, emp, emp);
    assert(emp.size() == 18);
    // empty cases
    Sequence e1;
    Sequence e2;
    interleave(e1, e2, emp);
    assert(emp.size() == 0);
    assert(e1.insert(0, "hello") == 0);
    interleave(e1, e2, emp);
    assert(emp.get(0, box) && box == "hello");
    cerr << "Passed all tests." << endl << endl;

}
