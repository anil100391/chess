#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include "board.h"
#include "perft.h"
using namespace std;


int main()
{
    cboard b;
    const char *pos="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const char *curr_pos="rnbqkbnr/nppppppp/8/8/4N3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    b.setBoard( pos );
    uint64_t perftNodes = Perft( b, 3 );
    std::cout << "perft: " << perftNodes << std::endl;
    cin.get();
    return 0;
}