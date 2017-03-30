#include "stdafx.h"

#include "stdlib.h"

#include "FastSet.h"

static void
Die( const char *StringOfDeath )
{
    printf( "%s.\n", StringOfDeath );
    exit( 0xDEADBEEF );
}

//
// FastSet
//

FastSet::FastSet( int CreationSize ) : Size( CreationSize ), StackTop( 0 )
{
    Stack = new int[Size];
    Member = new int[Size];
    for ( int i = 0; i<Size; i++ ) Member[i] = Stack[i] = -1; // purify hack
};

FastSet &
Union( FastSet &LeftOp, FastSet &RightOp )
{
    FastSetIterator i;
    int member;
    FastSet *Result;
    if ( LeftOp.Size == RightOp.Size )
    {
        Result = new FastSet( LeftOp );
        for ( member = i.Init( RightOp ); i.MoreLeft(); member = i.Next() )
        {
            Result->Add( member );
        }
        return *Result;
    }
    else
    {
        Die( "INTERNAL : FastSet Union called with different sized arguments" );
        exit( 0xDEADBEEF ); // dead code but the compiler wants what the compiler wants
    }
}

FastSet &
Intersection( FastSet &LeftOp, FastSet &RightOp )
{
    FastSetIterator i;
    int member;
    FastSet *Result;
    if ( LeftOp.Size == RightOp.Size )
    {
        Result = new FastSet( LeftOp.Size );
        for ( member = i.Init( RightOp ); i.MoreLeft(); member = i.Next() )
        {
            if ( LeftOp.IsMember( member ) == true )
                Result->Add( member );
        }
        return *Result;
    }
    else
    {
        Die( "INTERNAL : FastSet Intersection called with different sized arguments" );
        exit( 0xDEADBEEF ); // dead code but the compiler wants what the compiler wants
    }
}

// Result = LeftOp - RightOp

FastSet &
Subtract( FastSet &LeftOp, FastSet &RightOp )
{
    FastSetIterator i;
    int member;
    FastSet *Result;
    if ( LeftOp.Size == RightOp.Size )
    {
        Result = new FastSet( LeftOp.Size );
        for ( member = i.Init( LeftOp ); i.MoreLeft(); member = i.Next() )
        {
            if ( RightOp.IsMember( member ) != true )
                Result->Add( member );
        }
        return *Result;
    }
    else
    {
        Die( "INTERNAL : FastSet Subtraction called with different sized arguments" );
        exit( 0xDEADBEEF ); // dead code but the compiler wants what the compiler wants
    }
}

bool
AreEqual( FastSet &LeftOp, FastSet &RightOp )
{
    FastSetIterator i;
    int member;
    if ( LeftOp.Size == RightOp.Size && LeftOp.StackTop == RightOp.StackTop )
    {
        for ( member = i.Init( LeftOp ); i.MoreLeft(); member = i.Next() )
        {
            if ( RightOp.IsMember( member ) == false )
                return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool
AreDisjoint( FastSet &LeftOp, FastSet &RightOp )
{
    FastSetIterator i;
    int member;
    for ( member = i.Init( LeftOp ); i.MoreLeft(); member = i.Next() )
    {
        if ( RightOp.IsMember( member ) == true )
            return false;
    }
    return true;
}

void
Swap( FastSet &LeftOp, FastSet &RightOp )
{
    FastSet Temp( LeftOp.Size );
    if ( LeftOp.Size == RightOp.Size )
    {
        // explicit shallow copies all around
        delete Temp.Stack;
        delete Temp.Member;
        Temp.StackTop = LeftOp.StackTop;
        Temp.Stack = LeftOp.Stack;
        Temp.Member = LeftOp.Member;
        LeftOp.StackTop = RightOp.StackTop;
        LeftOp.Stack = RightOp.Stack;
        LeftOp.Member = RightOp.Member;
        RightOp.StackTop = Temp.StackTop;
        RightOp.Stack = Temp.Stack;
        RightOp.Member = Temp.Member;
        Temp.Stack = Temp.Member = 0;
    }
    else
    {
        Die( "INTERNAL : FastSet Swap called with different sized arguments" );
    }
}

FastSet::FastSet( FastSet &FS ) : Size( FS.Size )  // deep copy constructer
{
    FastSetIterator i;
    int member;
    StackTop = 0;
    Stack = new int[Size];
    Member = new int[Size];
    for ( int j = 0; j<Size; j++ ) Member[j] = Stack[j] = -1;  // purify hack
    for ( member = i.Init( FS ); i.MoreLeft(); member = i.Next() )
    {
        Add( member );
    }
}

FastSet &
FastSet::operator=( FastSet &RightOp )  // deep assignment
{
    FastSetIterator i;
    int member;
    if ( Size == RightOp.Size )
    {
        StackTop = 0;
        for ( member = i.Init( RightOp ); i.MoreLeft(); member = i.Next() )
        {
            Add( member );
        }
        return *this;
    }
    else
    {
        Die( "INTERNAL : FastSet operator= called with different sized arguments" );
        return *this;  // dead code but the compiler wants what the compiler wants
    }
}

void
FastSet::Add( unsigned int i )
{
    if ( 0 <= i && i < Size )
    {
        if ( IsMember(i) )
        {
            // value is already in set
        }
        else
        {
            Member[i] = StackTop;
            Stack[StackTop] = i; // or Stack[Member[i]] = i;
            StackTop++;
        }
    }
    else
    {
        Die( "INTERNAL : FastSet::Add was called with a value out of range.\n" );
    }
}

void
FastSet::Remove( unsigned int i )
{
    int Switch;
    if ( 0 <= i && i < Size )
    {
        if ( IsMember(i) )
        {
            Switch = StackTop - 1;
            Stack[Member[i]] = Stack[Switch];
            Member[Stack[Switch]] = Member[i];
            Member[i] = -1;
            StackTop--;
        }
        else
        {
            // value is not in set
        }
    }
    else
    {
        Die( "INTERNAL : FastSet::Remove was called with a value out of range.\n" );
    }
}

void
FastSet::Subtract( FastSet *FastSet )
{
    FastSetIterator i;
    int member;
    for ( member = i.Init( *FastSet ); i.MoreLeft(); member = i.Next() )
    {
        if ( IsMember( member ) == true ) Remove( member );
    }
}

void
FastSet::Union( FastSet *FS )
{
    FastSetIterator i;
    int member;

    if ( Size == FS->Size )
    {
        for ( member = i.Init( *FS ); i.MoreLeft(); member = i.Next() )
        {
            Add( member );
        }
    }
    else
    {
        Die( "INTERNAL : FastSet Union called with different sized arguments" );
    }
}

void
FastSet::Dump()
{
    int i;
    printf( "StackTop is %d\n", StackTop );
    for ( i = 0; i < Size; i++ )
        printf( "Member[%d]  %4d  Stack[%d] %4d \n", i, Member[i], i, Stack[i] );
}

//
// FastSetIterator
//

FastSetIterator::FastSetIterator()
{
    FS = 0;
    StackIndex = 0;
}

int
FastSetIterator::Init( FastSet &FastSet )
{
    FS = &FastSet;
    StackIndex = 0;
    if ( FS->Size == 0 )
    {
        return -1;
    }
    else
    {
        return FS->Stack[StackIndex];
    }
}

int
FastSetIterator::MoreLeft()
{
    return ( 0 < FS->StackTop && StackIndex < FS->StackTop );
}

int
FastSetIterator::Next()
{
    StackIndex++;
    if ( StackIndex < FS->StackTop )
    {
        return FS->Stack[StackIndex];
    }
    else
    {
        return -1;
    }
}
