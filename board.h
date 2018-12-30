#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <cassert>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
using std::vector;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class cpiece
{
public:

    enum class PIECE
    {
        bpawn,
        bknight,
        bbishop,
        brook,
        bqueen,
        bking,
        wpawn,
        wknight,
        wbishop,
        wrook,
        wqueen,
        wking,
        none
    };

    cpiece() noexcept = default;

    cpiece(PIECE pi, int square=-1) noexcept : _p(pi), _sq(square)
    {}

    void setSquare(int square) noexcept
    {
        _sq = square;
    }

    int getSquare() const noexcept
    {
        return _sq;
    }

    PIECE getType() const noexcept
    {
        return _p;
    }

    int pieceColor() const noexcept
    {
        assert( _p != PIECE::none );
        return static_cast<int>(_p) <= 5 ? 0 : 1;
    }

    char toString() const noexcept;

private:

    PIECE _p  = PIECE::none;
    int   _sq = -1;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class cmove
{

public:

    cmove() = delete;

    cmove( cpiece::PIECE pt, int fromSq, int toSq ) noexcept
        : _pt( pt ),
          _fromSq( fromSq ),
          _toSq( toSq )
    {}

    cpiece::PIECE getPieceType() const noexcept
    {
        return _pt;
    }

    int getfromSq() const noexcept
    {
        return _fromSq;
    }

    int gettoSq() const noexcept
    {
        return _toSq;
    }

private:

    cpiece::PIECE _pt;
    int           _fromSq;
    int           _toSq;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class cboard
{

public:

    cboard()
        : _sq(64)
    {}

    void setSideToMove( int side ) noexcept
    {
        _sideToMove = side;
    }

    int sideToMove() const noexcept
    {
        return _sideToMove;
    }

    void setBoard( const char* fenstr );

    vector<cmove> generateMoves() const;

    void makeMove( cmove move ) noexcept;
    void takeMove( cmove move ) noexcept;

    void display() const noexcept;

private:

    int toPadded(int sq) const noexcept
    {
        return ((sq / 8) + 2) * 12 + ((sq % 8) + 2);
    }

    int fromPadded( int sq ) const noexcept
    {
        int sqRow = sq / 12;
        int sqCol = sq % 12;
        if ( sqRow < 2 || sqRow > 9 || sqCol < 2 || sqCol > 9 )
            return -1;
        return ((sq / 12) - 2) * 8 + ((sq % 12) - 2);
    }

    vector<cmove> generateMoveforPiece( const cpiece &p ) const;
    vector<cmove> generatePawnMoves( int color, int fromSq ) const;
    vector<cmove> generateKnightMoves( int color, int atSq ) const;
    vector<cmove> generateBishopMoves( int color, int atSq ) const;
    vector<cmove> generateRookMoves( int color, int atSq ) const;
    vector<cmove> generateQueenMoves( int color, int atSq ) const;
    vector<cmove> generateKingMoves( int color, int atSq ) const;
    vector<cmove> generateBishopLikeMoves( const cpiece::PIECE pt, int atSq ) const;
    vector<cmove> generateRookLikeMoves( const cpiece::PIECE pt, int atSq ) const;

    bool isSquareEmpty( int sq ) const noexcept
    {
        return _sq[sq].getType() == cpiece::PIECE::none;
    }

    vector<cpiece> _sq;
    int            _sideToMove = 1;
};

#endif // _BOARD_H_
