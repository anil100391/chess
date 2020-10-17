#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <cassert>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
using std::vector;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
struct color
{
    constexpr explicit color( bool v ) noexcept : _v( v ) {}
    bool _v;
    bool operator ==( const color &other ) const noexcept { return _v == other._v; }
    bool operator !=( const color &other ) const noexcept { return _v != other._v; }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static constexpr color dark { false };
static constexpr color light { true };

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static const color& opposite( const color &c )
{
    return c == light ? dark : light;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
enum class BOARD_RANK
{
    ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
enum class BOARD_FILE
{
    A, B, C, D, E, F, G, H
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static BOARD_RANK rank( int square )
{
    return static_cast<BOARD_RANK>(square / 8);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static BOARD_FILE file( int square )
{
    return static_cast<BOARD_FILE>(square % 8);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class cpiece
{
public:

    enum TYPE
    {
        none,
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
        wking
    };

    cpiece() noexcept = default;

    cpiece(TYPE pi) noexcept : _p(pi) {}

    TYPE getType() const noexcept { return _p; }

    const color& getColor() const noexcept
    {
        assert( _p != none );
        return static_cast<int>(_p) <= 6 ? dark : light;
    }

    char toString() const noexcept;

private:

    TYPE _p = none;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool isPawn( const cpiece &piece )
{
    return piece.getType() == cpiece::bpawn || piece.getType() == cpiece::wpawn;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool isKing( const cpiece &piece )
{
    return piece.getType() == cpiece::bking || piece.getType() == cpiece::wking;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool isRook( const cpiece &piece )
{
    return piece.getType() == cpiece::brook || piece.getType() == cpiece::wrook;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool isBishop( const cpiece &piece )
{
    return piece.getType() == cpiece::bbishop || piece.getType() == cpiece::wbishop;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool isKnight( const cpiece &piece )
{
    return piece.getType() == cpiece::bknight || piece.getType() == cpiece::wknight;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static bool isQueen( const cpiece &piece )
{
    return piece.getType() == cpiece::bqueen || piece.getType() == cpiece::wqueen;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static cpiece pawn( const color &col )
{
    return col == dark ? cpiece( cpiece::bpawn ) : cpiece( cpiece::wpawn );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static cpiece::TYPE queenType( const color &col )
{
    return col == dark ? cpiece::bqueen : cpiece::wqueen;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static cpiece::TYPE nopieceType()
{
    return cpiece::none;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class cmove
{

public:

    cmove() = delete;

    cmove( int fromSq, int toSq,
           cpiece::TYPE capturedPiece = cpiece::none,
           cpiece::TYPE promotedPiece = cpiece::none,
           bool isCastle = false,
           bool empassant = false ) noexcept
    {
        _movedata |= fromSq;
        _movedata |= (toSq << 6);
        if ( capturedPiece != cpiece::none )
            _movedata |= (static_cast<int>(capturedPiece) << 12);
        if ( promotedPiece != cpiece::none )
            _movedata |= (static_cast<int>(promotedPiece) << 16);
        if ( isCastle )
            _movedata |= castleMask;
        if ( empassant )
            _movedata |= epMask;
    }

    int getfromSq() const noexcept
    {
        return static_cast<int>(_movedata & fromSqMask);
    }

    int gettoSq() const noexcept
    {
        return (static_cast<int>(_movedata & toSqMask)) >> 6;
    }

    bool isCapture() const noexcept
    {
        return static_cast<int>(_movedata & captMask) != 0;
    }

    cpiece::TYPE capturedPiece() const noexcept
    {
        return static_cast<cpiece::TYPE>((_movedata & captMask) >> 12);
    }

    bool isCastle() const noexcept
    {
        return static_cast<int>(_movedata & castleMask) != 0;
    }

    bool isEmpassant() const noexcept
    {
        return static_cast<int>(_movedata & epMask) != 0;
    }

    bool isPromotion() const noexcept
    {
        return static_cast<int>(_movedata & promMask) != 0;
    }

    cpiece::TYPE promotedPiece() const noexcept
    {
        return static_cast<cpiece::TYPE>((_movedata & promMask) >> 16);
    }

private:

    //                                             ep c  prom capt to     from
    // 0000000000000000 0000000000000000 000000000|0 |0 |0000|0000|000000|000000
    //                                             1  1    4    4    6      6
    uint64_t    _movedata = 0u;

    static constexpr uint64_t fromSqMask   = 0x3F;
    static constexpr uint64_t toSqMask     = (fromSqMask << 6);
    static constexpr uint64_t captMask     = (0xF << 12);
    static constexpr uint64_t promMask     = (0xF << 16);
    static constexpr uint64_t castleMask   = (1 << 20);
    static constexpr uint64_t epMask       = (1 << 21);
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class cboard
{

public:

    cboard() : _sq(64)
    {}

    void setSideToMove( color side ) noexcept
    {
        if ( side == dark )
            _sideToMove = 0;
        else
            _sideToMove = 1;
    }

    color sideToMove() const noexcept { return _sideToMove == 0 ? dark : light; }

    void setBoard( const char* fenstr );

    vector<cmove> generateMoves() const;

    void makeMove( const cmove& move ) noexcept;
    void takeMove( const cmove& move ) noexcept;

    void display() const noexcept;

    const cpiece& operator[](int sq) const noexcept { return _sq[sq]; }

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

    // add a move from fromSq to toSq to moveList
    // returns a tuple of (true, true) if a move was added and it was a caputre move
    // returns a tuple of (true, false) if a move was added and it was not a caputre move
    // returns a tuple of (false, false) if no move was added
    std::pair<bool, bool> addMove( int fromSq, int toSq, vector<cmove> &list ) const;

    vector<cmove> generateMoveforPiece( const cpiece &p, int fromSq ) const;
    vector<cmove> generatePawnMoves( int fromSq ) const;
    vector<cmove> generateRookMoves( int atSq ) const;
    vector<cmove> generateRookLikeMoves( int atSq ) const;
    vector<cmove> generateKnightMoves( int atSq ) const;
    vector<cmove> generateBishopMoves( int atSq ) const;
    vector<cmove> generateBishopLikeMoves( int atSq ) const;
    vector<cmove> generateQueenMoves( int atSq ) const;
    vector<cmove> generateKingMoves( int atSq ) const;

    // find out if passed square is attacked by attacker
    // turn it is
    bool isSquareAttacked( const color& attacker, int sq ) const;

    bool isSquareEmpty( int sq ) const noexcept
    {
        return _sq[sq].getType() == cpiece::none;
    }

    bool isOccupiedByKing( int sq ) const noexcept
    {
        return _sq[sq].getType() == cpiece::wking || _sq[sq].getType() == cpiece::bking;
    }

    bool isOccupiedBy( color col, int sq ) const noexcept
    {
        return !isSquareEmpty( sq ) && _sq[sq].getColor() == col;
    }

    bool isOccupiedByBlack( int sq ) const noexcept
    {
        return isOccupiedBy( dark, sq );
    }

    bool isOccupiedByWhite( int sq ) const noexcept
    {
        return isOccupiedBy( light, sq );
    }

    vector<cpiece> _sq;

    int            _sideToMove = 1;
    int            _empassantSq = -1;
};

#endif // _BOARD_H_
