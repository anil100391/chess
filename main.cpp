#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include "board.h"
#include "perft.h"
using namespace std;


int main(int argc, char * argv[])
{
    cboard b;
    const char *pos="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const char *curr_pos="rnb1kbnr/pp1ppppp/1qp5/8/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 2 3";
    b.setBoard( pos );
    std::cout << b.toFen() << "\n";
    b.display();
    /*
    b.setBoard( pos );
    auto moves = b.generateMoves();
    for ( auto m : moves )
    {
        b.makeMove( m );
        b.display();
        b.takeMove( m );
    }
    */
    int depth = 0;
    if (argc == 2)
        depth = std::atoi(argv[1]);
    cin >> depth;
    PerftResult res;
    uint64_t perftNodes = Perft( b, depth, res );
    std::cout << "perft nodes: " << res.nodes << std::endl;
    std::cout << "perft captures: " << res.nd[1].captures << std::endl;
    std::cout << "perft enpassant: " << res.nd[1].enpassant << std::endl;
    std::cout << "perft checks: " << res.nd[1].checks << std::endl;
    std::cout << "perft checkmates: " << res.nd[1].checkmate << std::endl;
    /*
    std::cout << "perft captures pawn: " << res.nd[1].pawncaptures << std::endl;
    std::cout << "perft captures rook: " << res.nd[1].rookcaptures << std::endl;
    std::cout << "perft captures knight: " << res.nd[1].knightcaptures << std::endl;
    std::cout << "perft captures bishop: " << res.nd[1].bishopcaptures << std::endl;
    std::cout << "perft captures queen: " << res.nd[1].queencaptures << std::endl;
    std::cout << "perft captures king: " << res.nd[1].kingcaptures << std::endl;
    */
    cin.get();
    cin.get();
    return 0;
}
