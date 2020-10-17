#ifndef _PERFT_H_
#define _PERFT_H_

#include <cstdint>
#include "board.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
struct PerftResult
{
    uint64_t nodes = 0;
    uint32_t captures = 0;
    uint32_t pawncaptures = 0;
    uint32_t rookcaptures = 0;
    uint32_t knightcaptures = 0;
    uint32_t bishopcaptures = 0;
    uint32_t queencaptures = 0;
    uint32_t kingcaptures = 0;
    uint32_t enpassant = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
uint64_t Perft( cboard &b, unsigned int depth, PerftResult &result )
{
    uint64_t nodes = 0;
    if ( depth == 0 )
    {
        result.nodes += 1;
        result.captures += 0;
        result.enpassant += 0;


        return 1;
    }

    auto moves = b.generateMoves(); 
    for ( auto move : moves )
    {
        if ( move.isCapture() )
        {
            result.captures += 1;
            if ( isPawn( b[move.getfromSq()].getType() ) )
                result.pawncaptures += 1;
            else if ( isRook( b[move.getfromSq()].getType() ) )
                result.rookcaptures += 1;
            else if ( isKnight( b[move.getfromSq()].getType() ) )
                result.knightcaptures += 1;
            else if ( isBishop( b[move.getfromSq()].getType() ) )
                result.bishopcaptures += 1;
            else if ( isQueen( b[move.getfromSq()].getType() ) )
                result.queencaptures += 1;
            else if ( isKing( b[move.getfromSq()].getType() ) )
                result.kingcaptures += 1;
        }
        if ( move.isEmpassant() )
            result.enpassant += 1;

        b.makeMove( move );
        nodes += Perft( b, depth - 1, result );
        b.takeMove( move );
    }

    return nodes;
}

#endif // _PERFT_H_
