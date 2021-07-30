#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <ctime>
#include "board.h"
#include "perft.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static std::vector<const char*> positions =
{
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static const PerftResult* GetPerftResults( const char* fen, int depth )
{
    static std::map<std::string, std::array<PerftResult, 6>> results;

    if ( results.empty() )
    {
        // fill results
        {
            auto& posres = results["r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"];
            posres[0].nodes = 1;
            posres[1].nodes = 46;
            posres[2].nodes = 2079;
            posres[3].nodes = 89890;
            posres[4].nodes = 3894594;
            posres[5].nodes = 164075551;
        }

        {
            auto& posres = results["rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"];
            posres[1].nodes = 44;
            posres[2].nodes = 1486;
            posres[3].nodes = 62379;
            posres[4].nodes = 2103487;
            posres[5].nodes = 89941194;
        }

        {
            auto& posres = results["r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"];
            posres[1].nodes = 6;

            posres[2].nodes = 264;
            posres[2].nd[1].captures = 87;
            posres[2].nd[1].enpassant = 0;
            posres[2].nd[1].checks = 10;
            posres[2].nd[1].checkmate = 0;

            posres[3].nodes = 9467;
            posres[3].nd[1].captures = 1021;
            posres[3].nd[1].enpassant = 4;
            posres[3].nd[1].checks = 38;
            posres[3].nd[1].checkmate = 22;

            posres[4].nodes = 422333;
            posres[4].nd[1].captures = 131393;
            posres[4].nd[1].enpassant = 0;
            posres[4].nd[1].checks = 15492;
            posres[4].nd[1].checkmate = 5;

            posres[5].nodes = 15833292;
            posres[5].nd[1].captures = 2046173;
            posres[5].nd[1].enpassant = 6512;
            posres[5].nd[1].checks = 200568;
            posres[5].nd[1].checkmate = 50562;

            // posres[6].nodes = 706045033;
            // posres[6].nd[1].captures = 210369132;
            // posres[6].nd[1].enpassant = 212;
            // posres[6].nd[1].checks = 26973664;
            // posres[6].nd[1].checkmate = 81076;
        }

        {
            auto& posres = results["8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"];
            posres[1].nodes = 14;
            posres[1].nd[1].captures = 1;
            posres[1].nd[1].enpassant = 0;
            posres[1].nd[1].checks = 2;
            posres[1].nd[1].checkmate = 0;

            posres[2].nodes = 191;
            posres[2].nd[1].captures = 14;
            posres[2].nd[1].enpassant = 0;
            posres[2].nd[1].checks = 10;
            posres[2].nd[1].checkmate = 0;

            posres[3].nodes = 2812;
            posres[3].nd[1].captures = 209;
            posres[3].nd[1].enpassant = 2;
            posres[3].nd[1].checks = 267;
            posres[3].nd[1].checkmate = 0;

            posres[4].nodes = 43238;
            posres[4].nd[1].captures = 3348;
            posres[4].nd[1].enpassant = 123;
            posres[4].nd[1].checks = 1680;
            posres[4].nd[1].checkmate = 17;

            posres[5].nodes = 674624;
            posres[5].nd[1].captures = 52051;
            posres[5].nd[1].enpassant = 1165;
            posres[5].nd[1].checks = 52950;
            posres[5].nd[1].checkmate = 17;

            // posres[6].nodes = 11030083;
            // posres[6].nd[1].captures = 940350;
            // posres[6].nd[1].enpassant = 33325;
            // posres[6].nd[1].checks = 452473;
            // posres[6].nd[1].checkmate = 2733;
        }

        {
            auto& posres = results["r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"];
            posres[1].nodes = 48;
            posres[1].nd[1].captures = 8;
            posres[1].nd[1].enpassant = 0;
            posres[1].nd[1].checks = 0;
            posres[1].nd[1].checkmate = 0;

            posres[2].nodes = 2039;
            posres[2].nd[1].captures = 351;
            posres[2].nd[1].enpassant = 1;
            posres[2].nd[1].checks = 3;
            posres[2].nd[1].checkmate = 0;

            posres[3].nodes = 97862;
            posres[3].nd[1].captures = 17102;
            posres[3].nd[1].enpassant = 45;
            posres[3].nd[1].checks = 993;
            posres[3].nd[1].checkmate = 1;

            posres[4].nodes = 4085603;
            posres[4].nd[1].captures = 757163;
            posres[4].nd[1].enpassant = 1929;
            posres[4].nd[1].checks = 25523;
            posres[4].nd[1].checkmate = 43;

            posres[5].nodes = 193690690;
            posres[5].nd[1].captures = 35043416;
            posres[5].nd[1].enpassant = 73365;
            posres[5].nd[1].checks = 3309887;
            posres[5].nd[1].checkmate = 30171;

            // posres[6].nodes = 8031647685;
            // posres[6].nd[1].captures = 1558445089;
            // posres[6].nd[1].enpassant = 3577504;
            // posres[6].nd[1].checks = 92238050;
            // posres[6].nd[1].checkmate = 360003;
        }

        {
            auto& posres = results["rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"];
            posres[0].nodes = 1;
            posres[1].nodes = 20;
            posres[2].nodes = 400;
            
            posres[3].nodes = 8902;
            posres[3].nd[1].captures = 34;
            posres[3].nd[1].checks = 12;
          
            posres[4].nodes = 197281;
            posres[4].nd[1].captures = 1576;
            posres[4].nd[1].checks = 469;
            posres[4].nd[1].checkmate = 8;

            posres[5].nodes = 4865609;
            posres[5].nd[1].captures = 82719;
            posres[5].nd[1].enpassant = 258;
            posres[5].nd[1].checks = 27351;
            posres[5].nd[1].checkmate = 347;

            // posres[6].nodes = 119060324;
            // posres[6].nd[1].captures = 2812008;
            // posres[5].nd[1].enpassant = 5248;
            // posres[6].nd[1].checks = 809099;
            // posres[6].nd[1].checkmate = 10828;
        }
    }

    auto it = results.find( fen );
    return (it != results.end() ) ? &(it->second[depth]) : nullptr;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main()
{
    auto begin = clock();
    for ( auto fen : positions )
    {
        for ( int ii = 1; ii < 5; ++ii )
        {
            unsigned int depth = ii;
            const PerftResult* knownRes = GetPerftResults( fen, depth );
            if ( !knownRes )
            {
                // std::cout << "results for passed position not found\n";
                continue;
            }

            cboard b;
            b.setBoard( fen );
            if ( ii == 1 )
                std::cout << "Testing position: " << fen << "\n";

            PerftResult res;
            Perft( b, depth, res );

            if ( res.nodes != knownRes->nodes )
            {
                std::cout << "Failed at depth: " << depth << " \n";
            }
            else
            {
                auto nodeIt = knownRes->nd.find(1);
                auto deepcheck =  nodeIt != knownRes->nd.end();

                if (deepcheck &&
                     (res.nd[1].captures  != nodeIt->second.captures   ||
                      res.nd[1].enpassant != nodeIt->second.enpassant  ||
                      res.nd[1].checks    != nodeIt->second.checks     ||
                      res.nd[1].checkmate != nodeIt->second.checkmate))
                {
					std::cout << "Failed at depth: " << depth << " \n";
                }
                else
					std::cout << "Passed at depth: " << depth << " \n";
            }
        }
    }

    auto elapsed = clock() - begin;

    std::cout << "ran in " << (1.0f * elapsed) / CLOCKS_PER_SEC << "s\n";
    return 0;
}