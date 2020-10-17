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
    const char *curr_pos="rnbqkbnr/nppppppp/8/8/4K3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    b.setBoard( pos );
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
    std::cout << "perft captures: " << res.captures << std::endl;
    std::cout << "perft captures pawn: " << res.pawncaptures << std::endl;
    std::cout << "perft captures rook: " << res.rookcaptures << std::endl;
    std::cout << "perft captures knight: " << res.knightcaptures << std::endl;
    std::cout << "perft captures bishop: " << res.bishopcaptures << std::endl;
    std::cout << "perft captures queen: " << res.queencaptures << std::endl;
    std::cout << "perft captures king: " << res.kingcaptures << std::endl;
    std::cout << "perft enpassant: " << res.enpassant << std::endl;
    cin.get();
    cin.get();
    return 0;
}
