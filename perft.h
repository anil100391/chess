#ifndef _PERFT_H_
#define _PERFT_H_

#include <cstdint>
#include <map>
#include "board.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
struct PerftResult
{
    uint64_t nodes = 0;
    struct nodeData
    {
        uint32_t checks = 0;
        uint32_t checkmate = 0;
        uint32_t captures = 0;
        uint32_t pawncaptures = 0;
        uint32_t rookcaptures = 0;
        uint32_t knightcaptures = 0;
        uint32_t bishopcaptures = 0;
        uint32_t queencaptures = 0;
        uint32_t kingcaptures = 0;
        uint32_t enpassant = 0;
    };

    std::map<int, nodeData> nd;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
uint64_t Perft( cboard &b, unsigned int depth, PerftResult &result )
{
    uint64_t nodes = 0;
    if ( depth == 0 )
    {
        result.nodes += 1;

        return 1;
    }

    auto moves = b.generateMoves(); 
    auto &nd = result.nd[depth];
    for ( auto move : moves )
    {
        if ( move.isCapture() )
        {
            nd.captures += 1;
            if ( isPawn( b[move.getfromSq()].getType() ) )
                nd.pawncaptures += 1;
            else if ( isRook( b[move.getfromSq()].getType() ) )
                nd.rookcaptures += 1;
            else if ( isKnight( b[move.getfromSq()].getType() ) )
                nd.knightcaptures += 1;
            else if ( isBishop( b[move.getfromSq()].getType() ) )
                nd.bishopcaptures += 1;
            else if ( isQueen( b[move.getfromSq()].getType() ) )
                nd.queencaptures += 1;
            else if ( isKing( b[move.getfromSq()].getType() ) )
                nd.kingcaptures += 1;
        }
        if ( move.isEmpassant() )
            nd.enpassant += 1;

        uint64_t beforeKey = b.hash();
        b.makeMove( move );
        if ( b.isInCheck( b.sideToMove() ) )
        {
            if ( b.isCheckmate() )
                nd.checkmate += 1;
            nd.checks += 1;
        }
        nodes += Perft( b, depth - 1, result );
        b.takeMove();
        uint64_t afterKey = b.hash();
        assert(beforeKey == afterKey);
    }

    return nodes;
}

#endif // _PERFT_H_
