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

#include "stdafx.h"

#include "FastSet.h"

int main()
{
    FastSet A(27), B(27), C(27);
    
    B.Add(7);B.Add(8);B.Add(9);B.Add(4);B.Add(5);B.Add(6);B.Add(26);
    C.Add(6);C.Add(0);C.Add(5);C.Add(1);C.Add(4);C.Add(2);C.Add(3);
    
    A = Union(B, C); 
    A.Dump();

    A = Intersection(B, C); 
    A.Dump();
    
    return 0;
}

