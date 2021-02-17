#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include "board.h"
#include "perft.h"
using namespace std;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    cboard b;

    const char *positions[] =
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "
    };

    b.setBoard( positions[0] );
    std::cout << b.toFen() << "\n";
    b.display();
    /*
    auto moves = b.generateMoves();
    for ( const auto& move : moves )
    {
        if ( move.toString() == "g2g4" )
        {
            b.makeMove(move);
            std::cout << "generating moves\n";
            b.display();
            auto newmoves = b.generateMoves();
            std::cout << "done generating moves\n";
            b.display();
            std::cout << "num moves: " << newmoves.size() << std::endl;
            for ( const auto& nm : newmoves )
            {
                std::cout << nm.toString() << std::endl;
            }
            break;
        }
    }

    return 0;
    */
    /*
    for ( const auto& move : moves )
    {
        cboard copy = b;
        copy.makeMove(move);
        PerftResult res;
        uint64_t perftNodes = Perft( copy, 1, res );
        std::cout << move.toString() << ": " << perftNodes << std::endl;
        copy.takeMove(move);
    }

    return 0;
    */

    std::string command;
    std::string commandArg;
    while ( true )
    {
        std::cin >> command >> commandArg; // e.g. move e2e4 OR perft 2
        if ( command == "move" )
        {
            auto moves = b.generateMoves();
            for ( const auto& move : moves )
            {
                std::cout << move.toString() << std::endl;
                if ( move.toString() == commandArg )
                {
                    b.makeMove(move);
                    b.display();
                    break;
                }
            }
        }
        else if ( command == "perft" )
        {
            int depth = std::stoi(commandArg);

            PerftResult res;
            uint64_t perftNodes = Perft( b, depth, res );
            std::cout << "perft nodes: " << res.nodes << std::endl;
            std::cout << "perft captures: " << res.nd[1].captures << std::endl;
            std::cout << "perft enpassant: " << res.nd[1].enpassant << std::endl;
            std::cout << "perft checks: " << res.nd[1].checks << std::endl;
            std::cout << "perft checkmates: " << res.nd[1].checkmate << std::endl;
        }
    }

    cin.get();
    return 0;
}

