#ifndef _PERFT_H_
#define _PERFT_H_

#include <cstdint>
#include "board.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
uint64_t Perft( cboard &b, unsigned int depth )
{
    uint64_t nodes = 0;
    if ( depth == 0 )
        return 1;

    auto moves = b.generateMoves(); 
    for ( auto move : moves )
    {
        b.makeMove( move );
        nodes += Perft( b, depth - 1 );
        b.takeMove( move );
    }
    return nodes;
}

#endif // _PERFT_H_
