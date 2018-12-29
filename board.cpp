#include <map>
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
    case PIECE::bpawn: return 'p';
    case PIECE::bknight: return 'n';
    case PIECE::bbishop: return 'b';
    case PIECE::brook: return 'r';
    case PIECE::bqueen: return 'q';
    case PIECE::bking: return 'k';
    case PIECE::wpawn: return 'P';
    case PIECE::wknight: return 'N';
    case PIECE::wbishop: return 'B';
    case PIECE::wrook: return 'R';
    case PIECE::wqueen: return 'Q';
    case PIECE::wking: return 'K';
    }
    return '.';
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::setBoard( const char* fenstr )
{
    string fen( fenstr );
    int counter = 0;
    for ( char c : fen )
    {
        if ( counter >= 64 )
            break;

        std::map<char, cpiece::PIECE> nameToenumTable{ std::make_pair( 'p', cpiece::PIECE::bpawn ),
                                                       std::make_pair( 'b', cpiece::PIECE::bbishop ),
                                                       std::make_pair( 'n', cpiece::PIECE::bknight ),
                                                       std::make_pair( 'r', cpiece::PIECE::brook ),
                                                       std::make_pair( 'q', cpiece::PIECE::bqueen ),
                                                       std::make_pair( 'k', cpiece::PIECE::bking ),
                                                       std::make_pair( 'P', cpiece::PIECE::wpawn ),
                                                       std::make_pair( 'B', cpiece::PIECE::wbishop ),
                                                       std::make_pair( 'N', cpiece::PIECE::wknight ),
                                                       std::make_pair( 'R', cpiece::PIECE::wrook ),
                                                       std::make_pair( 'Q', cpiece::PIECE::wqueen ),
                                                       std::make_pair( 'K', cpiece::PIECE::wking ),
        };

        switch ( c )
        {
        case 'p':
        case 'b':
        case 'n':
        case 'r':
        case 'q':
        case 'k':
        case 'P':
        case 'B':
        case 'N':
        case 'R':
        case 'Q':
        case 'K':
            _sq[counter] = cpiece( nameToenumTable[c], counter );
            counter++;
            break;
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
                _sq[counter] = cpiece( cpiece::PIECE::none, counter );
                counter++;
            }
        }
        break;
        default: break;
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateMoves() const
{
    vector<cmove> moves;
    for ( const cpiece &p : _sq )
    {
        if ( p.toString() == '.' || p.pieceColor() != sideToMove() )
            continue;
        auto piecemoves = generateMoveforPiece( p );
        moves.insert( moves.end(), piecemoves.begin(), piecemoves.end() );
    }
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateMoveforPiece( const cpiece &p ) const
{
    switch ( p.getType() )
    {
    case cpiece::PIECE::bpawn:
    case cpiece::PIECE::wpawn:
        return generatePawnMoves( p.pieceColor(), p.getSquare() );
    case cpiece::PIECE::brook:
    case cpiece::PIECE::wrook:
        return generateRookMoves( p.pieceColor(), p.getSquare() );
    case cpiece::PIECE::bbishop:
    case cpiece::PIECE::wbishop:
        return generateBishopMoves( p.pieceColor(), p.getSquare() );
    case cpiece::PIECE::bqueen:
    case cpiece::PIECE::wqueen:
        return generateQueenMoves( p.pieceColor(), p.getSquare() );
    case cpiece::PIECE::bking:
    case cpiece::PIECE::wking:
        return generateKingMoves( p.pieceColor(), p.getSquare() );
    case cpiece::PIECE::bknight:
    case cpiece::PIECE::wknight:
        return generateKnightMoves( p.pieceColor(), p.getSquare() );
    default: break;
    }
    return vector<cmove>();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generatePawnMoves( int color, int atSq ) const
{
    vector <cmove> moves;
    if ( color == 0 )
    {
        if ( atSq / 8 == 1 )
        {
            moves.emplace_back( cpiece( cpiece::PIECE::bpawn, atSq ), atSq, atSq + 16 );
        }
        moves.emplace_back( cpiece( cpiece::PIECE::bpawn, atSq ), atSq, atSq + 8 );
    }
    else if ( color == 1 )
    {
        if ( atSq / 8 == 6 )
        {
            moves.emplace_back( cpiece( cpiece::PIECE::wpawn, atSq ), atSq, atSq - 16 );
        }
        moves.emplace_back( cpiece( cpiece::PIECE::wpawn, atSq ), atSq, atSq - 8 );
    }

    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateQueenMoves( int color, int atSq ) const
{
    cpiece::PIECE pt = color == 0 ? cpiece::PIECE::bqueen : cpiece::PIECE::wqueen;
    vector <cmove> moves = generateRookLikeMoves( pt, atSq );
    auto bishopmoves = generateBishopLikeMoves( pt, atSq );
    moves.insert( moves.end(), bishopmoves.begin(), bishopmoves.end() );
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector <cmove> cboard::generateRookMoves( int color, int atSq ) const
{
    cpiece::PIECE pt = color == 0 ? cpiece::PIECE::brook : cpiece::PIECE::wrook;
    return generateRookLikeMoves( pt, atSq );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector <cmove> cboard::generateRookLikeMoves( const cpiece::PIECE pt, int atSq ) const
{
    assert( pt == cpiece::PIECE::wking  || pt == cpiece::PIECE::bking ||
            pt == cpiece::PIECE::wrook  || pt == cpiece::PIECE::brook ||
            pt == cpiece::PIECE::wqueen || pt == cpiece::PIECE::bqueen );

    vector <cmove> moves;
    int currRow = atSq / 8;
    int currCol = atSq % 8;
    bool isKing = (pt == cpiece::PIECE::wking || pt == cpiece::PIECE::bking);
    for ( int i = currRow + 1; i <= (isKing ? std::min(currRow + 1, 7) : 7); i++ )
    {
        if ( _sq[8 * i + currCol].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, 8 * i + currCol );
    }
    for ( int i = currRow - 1; i >= (isKing ? std::max(currRow - 1, 0) : 0); i-- )
    {
        if ( _sq[8 * i + currCol].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, 8 * i + currCol );
    }
    for ( int i = currCol + 1; i <= (isKing ? std::min(currCol + 1, 7) : 7); i++ )
    {
        if ( _sq[8 * currRow + i].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, 8 * currRow + i );
    }
    for ( int i = currCol - 1; i >= (isKing ? std::max(currCol - 1, 0) : 0); i-- )
    {
        if ( _sq[8 * currRow + i].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, 8 * currRow + i );
    }

    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector <cmove> cboard::generateBishopMoves( int color, int atSq ) const
{
    cpiece::PIECE pt = color == 0 ? cpiece::PIECE::bbishop
                                  : cpiece::PIECE::wbishop;
    return generateBishopLikeMoves( pt, atSq );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateBishopLikeMoves( const cpiece::PIECE pt, int atSq ) const
{
    assert( pt == cpiece::PIECE::wking   || pt == cpiece::PIECE::bking   ||
            pt == cpiece::PIECE::wbishop || pt == cpiece::PIECE::bbishop ||
            pt == cpiece::PIECE::wqueen  || pt == cpiece::PIECE::bqueen );

    vector <cmove> moves;
    int currRow = atSq / 8;
    int currCol = atSq % 8;
    bool isKing = (pt == cpiece::PIECE::wking || pt == cpiece::PIECE::bking);
    int i = 1;
    while ( true )
    {
        int toSquareRow = atSq / 8 + i;
        int toSquareCol = atSq % 8 + i;
        int toSquare = toSquareRow * 8 + toSquareCol;
        if ( toSquareCol > 7 || toSquareRow > 7 )
            break;
        if ( _sq[toSquare].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, toSquare );
        if ( isKing )
            break;
        i++;
    }
    i = 1;
    while ( true )
    {
        int toSquareRow = atSq / 8 - i;
        int toSquareCol = atSq % 8 - i;
        int toSquare = toSquareRow * 8 + toSquareCol;
        if ( toSquareCol < 0 || toSquareRow < 0 )
            break;
        if ( _sq[toSquare].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, toSquare );
        if ( isKing )
            break;
        i++;
    }
    i = 1;
    while ( true )
    {
        int toSquareRow = atSq / 8 + i;
        int toSquareCol = atSq % 8 - i;
        int toSquare = toSquareRow * 8 + toSquareCol;
        if ( toSquareCol < 0 || toSquareRow>7 )
            break;
        if ( _sq[toSquare].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, toSquare );
        if ( isKing )
            break;
        i++;
    }
    i = 1;
    while ( true )
    {
        int toSquareRow = atSq / 8 - i;
        int toSquareCol = atSq % 8 + i;
        int toSquare = toSquareRow * 8 + toSquareCol;
        if ( toSquareCol > 7 || toSquareRow < 0 )
            break;
        if ( _sq[toSquare].getType() != cpiece::PIECE::none )
            break;
        moves.emplace_back( cpiece( pt, atSq ), atSq, toSquare );
        if ( isKing )
            break;
        i++;
    }
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateKingMoves( int color, int atSq ) const
{
    const cpiece::PIECE pt = color == 0 ? cpiece::PIECE::bking
                                        : cpiece::PIECE::wking;
    vector<cmove> moves = generateRookLikeMoves( pt, atSq );
    vector<cmove> diagmoves = generateBishopLikeMoves( pt, atSq );
    moves.insert( moves.end(), diagmoves.begin(), diagmoves.end() );
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vector<cmove> cboard::generateKnightMoves( int color, int atSq ) const
{
    const cpiece::PIECE pt = color == 0 ? cpiece::PIECE::bknight
                                        : cpiece::PIECE::wknight;
    vector <cmove> moves;
    int currSqPadded = toPadded( atSq );
    static vector<int> toSquares{23, 25, 14, 10, -10, -14, -25, -23};
    for ( int toSquareDelta : toSquares )
    {
        int toSquarePadded = currSqPadded + toSquareDelta;
        int toSquare = fromPadded( toSquarePadded );
        if ( toSquare != -1 && _sq[toSquare].getType() == cpiece::PIECE::none )
            moves.emplace_back( cpiece( pt, atSq ), atSq, toSquare );
    }
    return moves;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::makeMove( cmove move ) noexcept
{
    _sq[move.getfromSq()] = cpiece( cpiece::PIECE::none, move.getfromSq() );
    _sq[move.gettoSq()] = move.getPiece();
    _sq[move.gettoSq()].setSquare( move.gettoSq() );
    _sideToMove ^= 1;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::takeMove( cmove move ) noexcept
{
    _sq[move.gettoSq()] = cpiece( cpiece::PIECE::none, move.gettoSq() );
    _sq[move.getfromSq()] = move.getPiece();
    _sq[move.getfromSq()].setSquare( move.getfromSq() );
    _sideToMove ^= 1;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void cboard::display() const noexcept
{
    for ( int i = 0; i < 64; i++ )
    {
        std::cout << _sq[i].toString() << " ";
        if ( (i + 1) % 8 == 0 )
            std::cout << "\n";
    }
    std::cout << "\n";
}
