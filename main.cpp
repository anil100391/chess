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
std::vector<std::string> parseCommand(const std::string &cmd)
{
    std::vector<std::string> cmdArgs;
    if ( cmd.empty() )
    {
        return cmdArgs;
    }

    auto it = std::find(cmd.begin(), cmd.end(), ' ');
    if ( it == cmd.end())
    {
        cmdArgs.push_back(cmd);
        return cmdArgs;
    }

    size_t argStartPos = it-cmd.begin();
    cmdArgs.push_back(cmd.substr(0, argStartPos));
    cmdArgs.push_back(cmd.substr(argStartPos + 1));
    return cmdArgs;
}

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

    std::string command;
    std::string commandArg;
    while ( true )
    {
        std::getline(std::cin, command);
        auto args = parseCommand(command);

        if (args.empty())
            continue;

        command = args[0];
        if (args.size() > 1)
            commandArg = args[1];

        if ( command == "move" )
        {
            auto moves = b.generateMoves();
            auto it = std::find_if(moves.begin(), moves.end(),
                                   [&commandArg](const cmove& m)
                                   {
                                       return (m.toString() == commandArg);
                                   });

            const cmove& move = *it;
            if ( it != moves.end() )
            {
                b.makeMove(move);
                b.display();
            }
            else
            {
                std::cout << "illegal move\n";
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
        else if ( command == "fen" )
        {
            b.setBoard(commandArg.c_str());
            b.display();
        }
        else if (command == "quit" || command == "exit")
        {
            return 0;
        }
    }

    cin.get();
    return 0;
}

