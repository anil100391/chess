#include <string>
#include <array>
#include <iostream>
#include <algorithm>
#include "board.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
using std::string;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
char cpiece::toString() const noexcept
{
    switch ( _p )
    {
    case cpiece::bpawn: return 'p';
    case cpiece::bknight: return 'n';
    case cpiece::bbishop: return 'b';
    case cpiece::brook: return 'r';
    case cpiece::bqueen: return 'q';
    case cpiece::bking: return 'k';
    case cpiece::wpawn: return 'P';
    case cpiece::wknight: return 'N';
    case cpiece::wbishop: return 'B';
    case cpiece::wrook: return 'R';
    case cpiece::wqueen: return 'Q';
    case cpiece::wking: return 'K';
    default: break;
    }
    return '.';
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
czobrist cboard::_posHasher;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::setBoard( const char* fenstr )
{
    // reset
    _castlePerm  = 0;
    _empassantSq = -1;
    _currentPly  = -1;

    // sq 0 corresponds to A1 and sq 64 corresponds to H8
    string fen( fenstr );

    auto sq = []( int r, int f )->int
    {
        return static_cast<int>(r) * 8 + static_cast<int>(f);
    };

    auto it = fen.begin();
    for ( int r = static_cast<int>(BOARD_RANK::EIGHT); r >= static_cast<int>(BOARD_RANK::ONE) && it !=fen.end(); --r )
    {
        int f = static_cast<int>(BOARD_FILE::A);
        while ( f <= static_cast<int>(BOARD_FILE::H) && it != fen.end() )
        {
            char c = *it;
            ++it;

            switch ( c )
            {
            case 'p': _sq[sq(r, f++)] = cpiece( cpiece::bpawn ); break;
            case 'b': _sq[sq(r, f++)] = cpiece( cpiece::bbishop ); break;
            case 'n': _sq[sq(r, f++)] = cpiece( cpiece::bknight ); break;
            case 'r': _sq[sq(r, f++)] = cpiece( cpiece::brook ); break;
            case 'q': _sq[sq(r, f++)] = cpiece( cpiece::bqueen ); break;
            case 'k': _sq[sq(r, f++)] = cpiece( cpiece::bking ); break;
            case 'P': _sq[sq(r, f++)] = cpiece( cpiece::wpawn ); break;
            case 'B': _sq[sq(r, f++)] = cpiece( cpiece::wbishop ); break;
            case 'N': _sq[sq(r, f++)] = cpiece( cpiece::wknight ); break;
            case 'R': _sq[sq(r, f++)] = cpiece( cpiece::wrook ); break;
            case 'Q': _sq[sq(r, f++)] = cpiece( cpiece::wqueen ); break;
            case 'K': _sq[sq(r, f++)] = cpiece( cpiece::wking ); break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            {
                for ( int i = 1; i <= c - '0'; i++ )
                {
                    _sq[sq(r, f++)] = cpiece( cpiece::none );
                }
            }
            break;
            default: break;
            }
        }
    }
    while (it!=fen.end())
    {
        char c = *it;
        switch(c)
        {
            case 'w': _sideToMove = 1; break;
            case 'b': _sideToMove = 0; break;
            case 'K': _castlePerm |= whiteK; break;
            case 'Q': _castlePerm |= whiteQ; break;
            case 'k': _castlePerm |= blackK; break;
            case 'q': _castlePerm |= blackQ; break;
            default: break;
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string cboard::toFen() const
{
    std::string fen;

    auto sq = []( int r, int f )->int
    {
        return static_cast<int>(r) * 8 + static_cast<int>(f);
    };

    for ( int r = static_cast<int>(BOARD_RANK::EIGHT); r >= static_cast<int>(BOARD_RANK::ONE); --r )
    {
        int emptyCounter = 0;
        for ( int f = static_cast<int>(BOARD_FILE::A); f <= static_cast<int>(BOARD_FILE::H); ++f )
        {
            char c = _sq[sq( r, f )].toString();
            if ( c == '.' )
                ++emptyCounter;
            else
            {
                if ( emptyCounter != 0 )
                {
                    fen += std::to_string( emptyCounter );
                    emptyCounter = 0;
                }
                fen += c;
            }
        }

        if ( emptyCounter != 0 )
            fen += std::to_string( emptyCounter );

        if ( r != static_cast<int>(BOARD_RANK::ONE) )
            fen += "/";
    }

    if ( sideToMove() == light )
        fen += " w";
    else
        fen += " b";

    if ( _castlePerm )
    {
        fen += " ";
        if (_castlePerm & whiteK) fen += 'K';
        if (_castlePerm & whiteQ) fen += 'Q';
        if (_castlePerm & blackK) fen += 'k';
        if (_castlePerm & blackQ) fen += 'q';
    }

    return fen;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateMoves() const
{
    vector<cmove> moves;
    for ( int ii = 0; ii < 64; ++ii )
    {
        const cpiece &p = _sq[ii];
        if ( p.getType() == cpiece::none || p.getColor() != sideToMove() )
            continue;
        auto piecemoves = generateMoveforPiece( p, ii );
        moves.insert( moves.end(), piecemoves.begin(), piecemoves.end() );
    }

    // remove illegal moves
    {
        cboard copy = *this;
        auto it = std::partition( moves.begin(), moves.end(),
                                  [&copy](const cmove& m)
                                  {
                                      copy.makeMove( m );
                                      bool ret = !copy.isInCheck( opposite( copy.sideToMove() ) );
                                      copy.takeMove();
                                      return ret;
                                  } );
        if ( it != moves.end() )
        {
            moves.erase( it, moves.end() );
        }
    }

    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateMoveforPiece( const cpiece &p, int fromSq ) const
{
    switch ( p.getType() )
    {
    case cpiece::bpawn:
    case cpiece::wpawn:
        return generatePawnMoves( fromSq );
    case cpiece::brook:
    case cpiece::wrook:
        return generateRookMoves( fromSq );
    case cpiece::bknight:
    case cpiece::wknight:
        return generateKnightMoves( fromSq );
    case cpiece::bbishop:
    case cpiece::wbishop:
        return generateBishopMoves( fromSq );
    case cpiece::bqueen:
    case cpiece::wqueen:
        return generateQueenMoves( fromSq );
    case cpiece::bking:
    case cpiece::wking:
        return generateKingMoves( fromSq );
    default: break;
    }
    return vector<cmove>();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generatePawnMoves( int atSq ) const
{
    assert( isPawn( _sq[atSq].getType() ) );

    const color &col = _sq[atSq].getColor();

    vector <cmove> moves;
    int dir = (col == light) ? 1 : -1;

    auto isPromotion = []( int toSq )
    {
        return rank( toSq ) == BOARD_RANK::ONE || rank( toSq ) == BOARD_RANK::EIGHT;
    };

    int fromSq = atSq;
    int toSq   = fromSq + 8 * dir;
    if ( isSquareEmpty( toSq ) )
    {
        if ( !isPromotion( toSq ) )
            moves.emplace_back( fromSq, toSq, nopieceType(), nopieceType() );
        else
        {
            moves.emplace_back( fromSq, toSq, nopieceType(), queenType ( col ) );
            moves.emplace_back( fromSq, toSq, nopieceType(), rookType  ( col ) );
            moves.emplace_back( fromSq, toSq, nopieceType(), bishopType( col ) );
            moves.emplace_back( fromSq, toSq, nopieceType(), knightType( col ) );
        }

        bool firstMove = (col == light) ? rank( atSq ) == BOARD_RANK::TWO
                                        : rank( atSq ) == BOARD_RANK::SEVEN;

        toSq = fromSq + 16 * dir;
        if ( firstMove && isSquareEmpty( toSq ) )
        {
            moves.emplace_back( fromSq, toSq );
        }
    }

    // capture moves
    if ( file( atSq ) == BOARD_FILE::A )
    {
        int delta = col == light ? 9 : 7;
        toSq = fromSq + delta * dir;
        if ( isOccupiedBy( opposite( col ), toSq ) && !isOccupiedByKing( toSq ) )
        {
            if ( !isPromotion( toSq ) )
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), nopieceType() );
            else
            {
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), queenType ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), rookType  ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), bishopType( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), knightType( col ) );
            }
        }
        if (empassantSq() == toSq && isSquareEmpty(toSq))
            moves.emplace_back(fromSq, toSq, col == light ? cpiece::bpawn : cpiece::wpawn, nopieceType(), false, true);
    }
    else if ( file( atSq ) == BOARD_FILE::H )
    {
        int delta = col == light ? 7 : 9;
        toSq = fromSq + delta * dir;
        if ( isOccupiedBy( opposite( col ), toSq ) && !isOccupiedByKing( toSq ) )
        {
            if ( !isPromotion( toSq ) )
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), nopieceType() );
            else
            {
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), queenType ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), rookType  ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), bishopType( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), knightType( col ) );
            }
        }
        if (empassantSq() == toSq && isSquareEmpty(toSq))
            moves.emplace_back(fromSq, toSq, col == light ? cpiece::bpawn : cpiece::wpawn, nopieceType(), false, true);
    }
    else
    {
        toSq = fromSq + 9 * dir;
        if ( isOccupiedBy( opposite( col ), toSq ) && !isOccupiedByKing( toSq ) )
        {
            if ( !isPromotion( toSq ) )
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), nopieceType() );
            else
            {
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), queenType ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), rookType  ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), bishopType( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), knightType( col ) );
            }
        }
        if (empassantSq() == toSq && isSquareEmpty(toSq))
            moves.emplace_back(fromSq, toSq, col == light ? cpiece::bpawn : cpiece::wpawn, nopieceType(), false, true);

        toSq = fromSq + 7 * dir;
        if ( isOccupiedBy( opposite( col ), toSq ) && !isOccupiedByKing( toSq ) )
        {
            if ( !isPromotion( toSq ) )
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), nopieceType() );
            else
            {
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), queenType ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), rookType  ( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), bishopType( col ) );
                moves.emplace_back( fromSq, toSq, _sq[toSq].getType(), knightType( col ) );
            }
        }
        if (empassantSq() == toSq && isSquareEmpty(toSq))
            moves.emplace_back(fromSq, toSq, col == light ? cpiece::bpawn : cpiece::wpawn, nopieceType(), false, true);
    }

    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector <cmove> cboard::generateRookMoves( int atSq ) const
{
    assert( isRook( _sq[atSq].getType() ) );
    return generateRookLikeMoves( atSq );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::pair<bool, bool> cboard::addMove( int fromSq, int toSq, vector<cmove> &list ) const
{
    if ( !isSquareEmpty( toSq ) )
    {
        // check capture
        if ( _sq[toSq].getColor() != _sq[fromSq].getColor() && !isOccupiedByKing( toSq ) )
        {
            list.emplace_back( fromSq, toSq, _sq[toSq].getType() );
            return std::make_pair( true, true );
        }

        return std::make_pair( false, false );
    }

    list.emplace_back( fromSq, toSq );
    return std::make_pair( true, false );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector <cmove> cboard::generateRookLikeMoves( int atSq ) const
{
    vector <cmove> moves;

    int currRank = static_cast<int>(rank( atSq ));
    int currFile = static_cast<int>(file( atSq ));

    static constexpr int directions[4][2] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
    for ( int ii = 0; ii < 4; ++ii )
    {
        const int *dir = directions[ii];
        int i = 1;
        while ( true )
        {
            int toSqRank = currRank + i * dir[0];
            int toSqFile = currFile + i * dir[1];
            if ( toSqRank < static_cast<int>(BOARD_RANK::ONE)   ||
                 toSqRank > static_cast<int>(BOARD_RANK::EIGHT) ||
                 toSqFile < static_cast<int>(BOARD_FILE::A)     ||
                 toSqFile > static_cast<int>(BOARD_FILE::H) )
            {
                break;
            }

            int toSq = toSqRank * 8 + toSqFile;
            auto [added, capture] = addMove( atSq, toSq, moves );
            if ( !added || capture )
                break;
            i++;
        }
    }
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateQueenMoves( int atSq ) const
{
    assert( isQueen( _sq[atSq].getType() ) );
    vector <cmove> moves = generateRookLikeMoves( atSq );
    vector <cmove> bishopmoves = generateBishopLikeMoves( atSq );
    moves.insert( moves.end(), bishopmoves.begin(), bishopmoves.end() );
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateKingMoves( int atSq ) const
{
    assert( isKing( _sq[atSq] ) );

    int atSqRank = static_cast<int>(rank( atSq ));
    int atSqFile = static_cast<int>(file( atSq ));

    vector<cmove> moves;
    static constexpr int directions[8][2] = { {1, 0},  {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
    for ( int ii = 0; ii < 8; ++ii )
    {
        const int *dir = directions[ii];
        int toSqRank = atSqRank + dir[0];
        int toSqFile = atSqFile + dir[1];

        if ( toSqRank < static_cast<int>(BOARD_RANK::ONE)   ||
             toSqRank > static_cast<int>(BOARD_RANK::EIGHT) ||
             toSqFile < static_cast<int>(BOARD_FILE::A)     ||
             toSqFile > static_cast<int>(BOARD_FILE::H) )
        {
            continue;
        }

        int toSq = toSqRank * 8 + toSqFile;
        if ( isSquareAttacked( opposite( sideToMove() ), toSq ) )
            continue;

        auto [added, captured] = addMove( atSq, toSq, moves );
        if ( !added || !captured )
            continue;
    }

    vector<cmove> castleMoves = generateCastleMoves(atSq);
    moves.insert(moves.end(), castleMoves.begin(), castleMoves.end());
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateCastleMoves(int atSq) const
{
    assert(isKing(_sq[atSq]));
    int canBlackCastle = (blackQ | blackK);
    int canWhiteCastle = (whiteQ | whiteK);

    vector<cmove> moves;
    color col = _sq[atSq].getColor();
    if ((col == dark && !(canBlackCastle & _castlePerm)) ||
        (col == light && !(canWhiteCastle & _castlePerm)) ||
        isInCheck(col))
    {
        return moves;
    }

    bool kingSideCastle = ( (col == dark  && _castlePerm & blackK) ||
                            (col == light && _castlePerm & whiteK) )    &&
                          isSquareEmpty(atSq + 1)                       &&
                          isSquareEmpty(atSq + 2)                       &&
                          !isSquareAttacked(opposite(col), atSq + 1)    &&
                          !isSquareAttacked(opposite(col), atSq + 2);
    if (kingSideCastle)
    {
        moves.emplace_back(atSq, atSq + 2, cpiece::none, cpiece::none, true);
    }

    bool queenSideCastle = ( (col == dark  && _castlePerm & blackQ) ||
                             (col == light && _castlePerm & whiteQ) )   &&
                           isSquareEmpty(atSq - 1)                      &&
                           isSquareEmpty(atSq - 2)                      &&
                           isSquareEmpty(atSq - 3)                      &&
                           !isSquareAttacked(opposite(col), atSq - 1)   &&
                           !isSquareAttacked(opposite(col), atSq - 2);
    if (queenSideCastle)
    {
        moves.emplace_back(atSq, atSq - 2, cpiece::none, cpiece::none, true);
    }

    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector <cmove> cboard::generateBishopMoves( int atSq ) const
{
    assert( isBishop( _sq[atSq].getType() ) );
    return generateBishopLikeMoves( atSq );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateBishopLikeMoves( int atSq ) const
{
    vector <cmove> moves;

    int currRank = static_cast<int>(rank( atSq ));
    int currFile = static_cast<int>(file( atSq ));
    static constexpr int directions[4][2] = { { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 } };

    for ( int ii = 0; ii < 4; ++ii )
    {
        const int *dir = directions[ii];
        int i = 1;
        while ( true )
        {
            int toSqRank = currRank + i * dir[0];
            int toSqFile = currFile + i * dir[1];
            if ( toSqRank < static_cast<int>(BOARD_RANK::ONE)   ||
                 toSqRank > static_cast<int>(BOARD_RANK::EIGHT) ||
                 toSqFile < static_cast<int>(BOARD_FILE::A)     ||
                 toSqFile > static_cast<int>(BOARD_FILE::H) )
            {
                break;
            }

            int toSq = toSqRank * 8 + toSqFile;
            auto [added, capture] = addMove( atSq, toSq, moves );
            if ( !added || capture )
                break;
            i++;
        }
    }

    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateKnightMoves( int atSq ) const
{
    assert( isKnight( _sq[atSq].getType() ) );

    vector <cmove> moves;
    int currSqPadded = toPadded( atSq );
    static vector<int> toSquares{23, 25, 14, 10, -10, -14, -25, -23};
    for (int toSquareDelta : toSquares)
    {
        int toSquarePadded = currSqPadded + toSquareDelta;
        int toSq           = fromPadded(toSquarePadded);

        if (toSq == -1)
            continue;

        addMove( atSq, toSq, moves );
    }
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool cboard::isSquareAttacked( const color& attacker, int sq ) const
{
    int paddedSq = toPadded( sq );

    // attacked by pawn
    int pawnMoveDir = (attacker == light) ? 1 : -1;
    static constexpr int pdelta[2] = { 11, 13 };
    for ( int ii = 0; ii < 2; ++ii )
    {
        int toSq = fromPadded( paddedSq - pawnMoveDir * pdelta[ii] );
        if ( toSq == -1 || isSquareEmpty( toSq ) )
            continue;
        const cpiece &p = _sq[toSq];
        if ( p.getColor() == attacker && isPawn( p ) )
            return true;
    }

    // attacked by knight
    static vector<int> toSquares{23, 25, 14, 10, -10, -14, -25, -23};
    for (int toSquareDelta : toSquares)
    {
        int toSquarePadded = paddedSq + toSquareDelta;
        int toSq           = fromPadded(toSquarePadded);

        if ( toSq == -1 || isSquareEmpty( toSq ) )
            continue;

        const cpiece &p = _sq[toSq];
        if ( p.getColor() == attacker && isKnight( p ) )
            return true;
    }

    int sqRank = static_cast<int>(rank( sq ));
    int sqFile = static_cast<int>(file( sq ));

    // attacked diagonally (bishop, queen, king)
    static constexpr int directionsd[4][2] = { { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 } };
    for ( int ii = 0; ii < 4; ++ii )
    {
        const int *dir = directionsd[ii];
        int i = 1;
        while ( true )
        {
            int curSqRank = sqRank + i * dir[0];
            int curSqFile = sqFile + i * dir[1];
            if ( curSqRank < static_cast<int>(BOARD_RANK::ONE)   ||
                 curSqRank > static_cast<int>(BOARD_RANK::EIGHT) ||
                 curSqFile < static_cast<int>(BOARD_FILE::A)     ||
                 curSqFile > static_cast<int>(BOARD_FILE::H) )
            {
                break;
            }

            int curSq = curSqRank * 8 + curSqFile;
            bool empty = isSquareEmpty( curSq );
            if ( empty )
            {
                ++i;
                continue;
            }
            else
            {
                const cpiece &p = _sq[curSq];
                if ( p.getColor() != attacker )
                    break;

                if ( isBishop( p ) || isQueen( p ) || (isKing( p ) && i == 1) )
                    return true;

                break;
            }
        }
    }

    // attacked straight (rook, queen, king)
    static constexpr int directionss[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
    for ( int ii = 0; ii < 4; ++ii )
    {
        const int *dir = directionss[ii];
        int i = 1;
        while ( true )
        {
            int curSqRank = sqRank + i * dir[0];
            int curSqFile = sqFile + i * dir[1];
            if ( curSqRank < static_cast<int>(BOARD_RANK::ONE)   ||
                 curSqRank > static_cast<int>(BOARD_RANK::EIGHT) ||
                 curSqFile < static_cast<int>(BOARD_FILE::A)     ||
                 curSqFile > static_cast<int>(BOARD_FILE::H) )
            {
                break;
            }

            int curSq = curSqRank * 8 + curSqFile;
            bool empty = isSquareEmpty( curSq );
            if ( empty )
            {
                ++i;
                continue;
            }
            else
            {
                const cpiece &p = _sq[curSq];

                if ( p.getColor() != attacker )
                    break;

                if ( isRook( p ) || isQueen( p ) || (isKing( p ) && i == 1) )
                    return true;

                break;
            }
        }
    }

    return false;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool cboard::isInCheck( const color &col ) const
{
    int ks = kingSq( col );
    return isSquareAttacked( opposite( col ), ks );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool cboard::isCheckmate() const
{
    return isInCheck( sideToMove() ) && generateMoves().empty();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool cboard::makeMove( int fromSq, int toSq ) noexcept
{
    if ( !isValidSq( fromSq ) || !isValidSq( toSq ) || isSquareEmpty( fromSq ) )
        return false;

    const cpiece &p = _sq[fromSq];
    if ( p.getColor() != sideToMove() )
        return false;

    auto moves = generateMoveforPiece( p, fromSq );
    auto it = std::find_if( moves.begin(), moves.end(),
                            [&fromSq, &toSq](const cmove& m)
                            {
                                return m.getfromSq() == fromSq && m.gettoSq() == toSq;
                            } );

    if ( it == moves.end() )
        return false;

    const cmove &move = *it;
    // remove illegal moves
    if ( isInCheck( sideToMove() ) )
    {
        makeMove( move );
        if ( !isInCheck( opposite( sideToMove() ) ) )
            return true;

        takeMove();
        return false;
    }

    makeMove( move );
    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::updateEmpassantSq(const cmove &move)
{
    int fromSq = move.getfromSq();
    int toSq = move.gettoSq();

    if (isPawn(_sq[fromSq]))
    {
        int delta = toSq - fromSq;
        if (std::abs(delta) == 16)
        {
            _empassantSq = delta > 0 ? fromSq + 8 : fromSq - 8;
            return;
        }
    }
    _empassantSq = -1;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::updateCastlePermission(const cmove &move)
{
    int fromSq = move.getfromSq();

    if (isKing(_sq[fromSq]))
    {
        if (_sq[fromSq].getColor() == light)
        {
            _castlePerm &= (blackK | blackQ);
        }
        else
        {
            _castlePerm &= (whiteK | whiteQ);
        }
    }
    else if (isRook(_sq[fromSq]))
    {
        if (_sq[fromSq].getColor() == light && fromSq == 0)
        {
            _castlePerm &= (whiteK | blackK | blackQ);
        }
        else if (_sq[fromSq].getColor() == light && fromSq == 7)
        {
            _castlePerm &= (whiteQ | blackK | blackQ);
        }
        else if (_sq[fromSq].getColor() == dark && fromSq == 56)
        {
            _castlePerm &= (blackK | whiteK | whiteQ);
        }
        else if (_sq[fromSq].getColor() == dark && fromSq == 63)
        {
            _castlePerm &= (blackQ | whiteK | whiteQ);
        }
    }
    else if (move.isCapture() && isRook(move.capturedPiece()))
    {
        int toSq = move.gettoSq();
        if (move.capturedPiece() == cpiece::brook) 
        {
            if (toSq == 56)
                _castlePerm &= (whiteK | whiteQ | blackK);
            else if (toSq == 63)
                _castlePerm &= (whiteK | whiteQ | blackQ);
        }
        else if (move.capturedPiece() == cpiece::wrook) 
        {
            if (toSq == 0)
                _castlePerm &= (blackK | blackQ | whiteK);
            else if (toSq == 7)
                _castlePerm &= (blackK | blackQ | whiteQ);
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::makeMove( const cmove& move ) noexcept
{
    ++_currentPly;
    addHistoryState( move );

    int fromSq = move.getfromSq();
    int toSq   = move.gettoSq();

    // target square should be empty or occupied by other side
    assert( _sq[toSq].getType() == cpiece::none ||
            _sq[toSq].getColor() != sideToMove() );

    updateCastlePermission(move);
    updateEmpassantSq(move);

    _sq[toSq]   = move.isPromotion() ? cpiece( move.promotedPiece() ) : _sq[fromSq];
    _sq[fromSq] = cpiece( cpiece::none );

    if ( move.isCastle() )
    {
        bool kingSide = (toSq - fromSq > 0);
        if ( kingSide )
        {
            _sq[toSq+1]= cpiece(cpiece::none);
            _sq[toSq-1] = (sideToMove() == dark) ? cpiece(cpiece::brook) : cpiece(cpiece::wrook);
        }
        else
        {
            _sq[toSq-2]= cpiece(cpiece::none);
            _sq[toSq+1] = (sideToMove() == dark) ? cpiece(cpiece::brook) : cpiece(cpiece::wrook);
        }
    }

    if (move.isEmpassant())
    {
        int dir = (toSq - fromSq > 0) ? 1 : -1;
        _sq[toSq - 8 * dir] = cpiece(cpiece::none);
    }

    _sideToMove ^= 1;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::takeMove() noexcept
{
    _sq               = _history[_currentPly]._pieces;
    _castlePerm       = _history[_currentPly]._castlePerm;
    _empassantSq      = _history[_currentPly]._empassantSq;

    _sideToMove ^= 1;

    --_currentPly;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
uint64_t cboard::hash() const
{
    uint64_t key = 0;

    for ( int ii = 0; ii < 64; ++ii )
    {
        const auto& p = _sq[ii];
        if ( p.getType() == cpiece::none )
            continue;

        key ^= _posHasher._pieceKeys[p.getType()][ii];
    }

    if ( _empassantSq != -1 )
        key ^= _posHasher._pieceKeys[cpiece::none][_empassantSq];

    if ( sideToMove() == light )
        key ^= _posHasher._sideKey;

    key ^= _posHasher._castleKeys[_castlePerm];

    return key;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::display() const noexcept
{
    auto sq = []( int r, int f )->int
    {
        return static_cast<int>(r) * 8 + static_cast<int>(f);
    };

    for ( int r = static_cast<int>(BOARD_RANK::EIGHT); r >= static_cast<int>(BOARD_RANK::ONE); --r )
    {
        for ( int f = static_cast<int>(BOARD_FILE::A); f <= static_cast<int>(BOARD_FILE::H); ++f )
        {
            std::cout << _sq[sq(r, f)].toString() << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::goToPly( int ply )
{
    _currentPly  = ply;
    _sideToMove  = ply % 2 == 0 ? 1 : 0;
    _sq          = _history[ply]._pieces;
    _castlePerm  = _history[_currentPly]._castlePerm;
    _empassantSq = _history[_currentPly]._empassantSq;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::addHistoryState( const cmove &moveAboutToMake )
{
    _history[_currentPly]._pieces       = _sq;
    _history[_currentPly]._castlePerm   = _castlePerm;
    _history[_currentPly]._empassantSq  = _empassantSq;
    _history[_currentPly]._movePlayed   = moveAboutToMake;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string cmove::toString() const
{
    int fromSq = getfromSq();
    int toSq   = gettoSq();

    std::string str;
    unsigned char fr = '1' + static_cast<unsigned char>(rank( fromSq ));
    unsigned char ff = 'a' + static_cast<unsigned char>(file( fromSq ));
    str += ff;
    str += fr;

    unsigned char tr = '1' + static_cast<unsigned char>(rank( toSq ));
    unsigned char tf = 'a' + static_cast<unsigned char>(file( toSq ));

    str += tf;
    str += tr;
    return str;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::string cgamehistorydata::toString() const
{
    int fromSq = _movePlayed.getfromSq();
    int toSq   = _movePlayed.gettoSq();

    std::string str;
    if ( !isPawn( _pieces[fromSq] )  )
    {
        str += _pieces[fromSq].toString();
    }

    if ( _movePlayed.isCapture() )
    {
        if ( isPawn( _pieces[fromSq] ) )
        {
            unsigned char f = 'a' + static_cast<unsigned char>(file( fromSq ));
            str += f;
        }

        str += "x";
    }

    unsigned char r = '1' + static_cast<unsigned char>(rank( toSq ));
    unsigned char f = 'a' + static_cast<unsigned char>(file( toSq ));

    str += f;
    str += r;
    return str;
}

