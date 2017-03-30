#pragma once

//
// An Efficient Representation for Sparse Sets
// Preston Briggs And Linda Torczon
// Rice UniversityACM Letters on Programming Languages and Systems, Vol. 2, Nos. 1-4, March-December 1993
//

// Why fast sets?  Mainly because sparse bit vectors (aren't they all)
// are expensive to iterate over.  Linked lists are cheap to iterate over
// but membership, adding, and removing are expensive.  FastSet gives a
// constant time for those three operations (like BitVectors) and gives
// an inexpensive iteration.  You pay for these execution properties with
// space
//
// for a set of size n with m members
//
//              Bit Vector   Fast Set     Linked Lists
// Membership   O(c)         O(c)         O(m)
// Add          O(c)         O(c)         O(m)
// Remove       O(c)         O(c)         O(m)
// Traverse     O(n)         O(m)         O(m)
//
// Space        O(n/32)      O(2n)        O(2m)

class FastSetIterator;

class FastSet 
{
public:
    friend class FastSetIterator;

    friend FastSet &Union( FastSet &LeftOp, FastSet &RightOp );
    friend FastSet &Intersection( FastSet &LeftOp, FastSet &RightOp );
    friend FastSet &Subtract( FastSet &LeftOp, FastSet &RightOp );

    friend bool AreEqual( FastSet &LeftOp, FastSet &RightOp );
    friend bool AreDisjoint( FastSet &LeftOp, FastSet &RightOp );
    friend void Swap( FastSet &LeftOp, FastSet &RightOp );

    FastSet( int CreationSize );
    FastSet( FastSet &FS ); // deep copy constructer
    FastSet &operator=( FastSet &RightOp );   // deep assignment
    ~FastSet() { delete Stack; delete Member; };

    bool IsMember( unsigned int i );
    void Add( unsigned int i );
    void Remove( unsigned int i );
    void Subtract( FastSet *FastSet );
    void Union( FastSet *FastSet );
    void Dump();  // debug

    const int Size;  // allocated size, maximum number of members
    int StackTop;    // number of members

private:
    int *Stack;
    int *Member;
};

inline bool
FastSet::IsMember( unsigned int i )
{
    return ( 0 <= i && i < Size &&
             0 <= Member[i] && Member[i] < StackTop &&
             Stack[Member[i]] == i);
}


//
// how to iterate :
// FastSetIterator i; int member;
// for ( member = i.Init(FS); i.MoreLeft(); member = i.Next() ) { }
//

class FastSetIterator 
{
public:
    FastSetIterator();
    int Init( FastSet &FastSet );
    int MoreLeft();
    int Next();
private:
    FastSet *FS;
    int StackIndex;
};
