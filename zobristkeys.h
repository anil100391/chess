#ifndef _ZOBRIST_KEYS_H_
#define _ZOBRIST_KEYS_H_

#include <random>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class czobrist
{
public:

    czobrist();

    uint64_t _pieceKeys[13][64];
    uint64_t _sideKey;
    uint64_t _castleKeys[16];

private:

    void initialize();

    std::random_device _dev;     // for seeding the engine
    std::mt19937_64    _engine;
};

#endif // _ZOBRIST_KEYS_H_
