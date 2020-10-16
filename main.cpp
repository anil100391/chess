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
    b.setBoard( curr_pos );
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
    uint64_t perftNodes = Perft( b, depth );
    std::cout << "perft: " << perftNodes << std::endl;
    cin.get();
    return 0;
}
