#include "zobristkeys.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
czobrist::czobrist()
    : _dev{},
      _engine{_dev()}
{
    initialize();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void czobrist::initialize()
{
    // piece keys for 13 pieces (cpiece::none to cpiece::wking)
    for ( int ii = 0; ii < 13; ++ii )
    {
        for ( int jj = 0; jj < 64; ++jj )
        {
            _pieceKeys[ii][jj] = _engine();
        }
    }

    // side to move key... will be hashed in only if white to move
    _sideKey = _engine();

    // castle permissions
    for ( int ii = 0; ii < 16; ++ ii )
    {
        _castleKeys[ii] = _engine();
    }
}

