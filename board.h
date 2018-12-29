#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

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
        cpiece()
            :p(PIECE::none),sq(-1)
        {}
        cpiece(PIECE pi,int square=-1):p(pi),sq(square)
        {}

        void setSquare(int square)
        {
            sq=square;
        }
        int getSquare()
        {
            return sq;
        }

        PIECE getType() const
        {
            return p;
        }

        int pieceColor() const
        {
            if ( p != PIECE::none )
            {
                return static_cast<int>(p) <= 5 ? 0 : 1;
            }

            return -1;
        }

        char toString()
        {
            switch(p)
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
                default: return '.';
            }
        }
    
    private:
        PIECE p;
        int sq;
};
class cmove
{
    private:
        cpiece _p;
        int _fromSq;
        int _toSq;
    public:
        cmove(cpiece p,int fromSq,int toSq)
        {        
            _p=p;
            _fromSq=fromSq;
            _toSq=toSq;
        }
        cpiece getPiece()
        {
            return _p;
        }
        int getfromSq()
        {
                return _fromSq;
        }
        int gettoSq()
        {    
            return _toSq;
        }
};
class cboard
{
    private:
    vector <cpiece> sq;
    int _sideToMove = 1;

    public:
    cboard()
        : sq(64)
    {}

    void setSideToMove( int side )
    {
        _sideToMove = side;
    }

    int sideToMove() const
    {
        return _sideToMove;
    }

    void setBoard(string fen)
    {
        string row;
        int counter=0;
        for(char c: fen)
        {
            if(counter>=64)
                break;
            switch(c)
            {
                case 'p':
                    sq[counter]=cpiece(cpiece::PIECE::bpawn,counter);
                    counter++;
                    break;
                case 'b':
                    sq[counter]=cpiece(cpiece::PIECE::bbishop,counter);
                    counter++;
                    break;
                case 'n':
                    sq[counter]=cpiece(cpiece::PIECE::bknight,counter);
                    counter++;
                    break;
                case 'r':
                    sq[counter]=cpiece(cpiece::PIECE::brook,counter);
                    counter++;
                    break; 
                case 'q':
                    sq[counter]=cpiece(cpiece::PIECE::bqueen,counter);
                    counter++;
                    break;
                case 'k':
                    sq[counter]=cpiece(cpiece::PIECE::bking,counter);
                    counter++;
                    break;
                case 'P':
                    sq[counter]=cpiece(cpiece::PIECE::wpawn,counter);
                    counter++;
                    break;
                case 'B':
                    sq[counter]=cpiece(cpiece::PIECE::wbishop,counter);
                    counter++;
                    break;
                case 'N':
                    sq[counter]=cpiece(cpiece::PIECE::wknight,counter);
                    counter++;
                    break;
                case 'R':
                    sq[counter]=cpiece(cpiece::PIECE::wrook,counter);
                    counter++;
                    break; 
                case 'Q':
                    sq[counter]=cpiece(cpiece::PIECE::wqueen,counter);
                    counter++;
                    break;
                case 'K':
                    sq[counter]=cpiece(cpiece::PIECE::wking,counter);
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
                        for (int i=1; i<=c-'0';i++)
                        {
                            sq[counter]=cpiece(cpiece::PIECE::none,counter);
                            counter++;
                        }
                    }
                    break;
                default: break;
            }
       }
    }
    vector<cmove> generateMoves()
    {
        vector <cmove> moves;
        for(cpiece & p:sq)
        {
            if(p.toString()=='.' || p.pieceColor() != sideToMove() )
                continue;
            auto piecemoves = generateMoveforPiece(p, p.getSquare());
            moves.insert(moves.end(), piecemoves.begin(), piecemoves.end());
        }
        return moves;
    }
    vector<cmove> generateMoveforPiece(const cpiece &p,int currSq)
    {
        switch(p.getType())
        {
            case cpiece::PIECE::bpawn:
                return generateMoveforPawn(0, currSq);
            case cpiece::PIECE::wpawn:
                return generateMoveforPawn(1, currSq);
            case cpiece::PIECE::brook:
                return generateRookMoves(cpiece::PIECE::brook, currSq);
            case cpiece::PIECE::wrook:
                return generateRookMoves(cpiece::PIECE::wrook, currSq);
            case cpiece::PIECE::bbishop:
                return generateBishopMoves(cpiece::PIECE::bbishop, currSq);
            case cpiece::PIECE::wbishop:
                return generateBishopMoves(cpiece::PIECE::wbishop, currSq);
            case cpiece::PIECE::bqueen:
                return generateQueenMoves(cpiece::PIECE::bqueen, currSq);
            case cpiece::PIECE::wqueen:
                return generateQueenMoves(cpiece::PIECE::wqueen, currSq);
            
            case cpiece::PIECE::bking:
                return generateKingMoves(cpiece::PIECE::bking, currSq);
            case cpiece::PIECE::wking:
            
                return generateKingMoves(cpiece::PIECE::wking, currSq);
           case cpiece::PIECE::bknight:
                return generateKnightMoves(cpiece::PIECE::bknight, currSq);
            case cpiece::PIECE::wknight:
                return generateKnightMoves(cpiece::PIECE::wknight, currSq);
            default: break;
        }
        return vector<cmove> ();
    }
    int toPadded(int sq)
    {
        return ((sq/8)+2)*12+((sq%8)+2);
    }
    int fromPadded(int sq)
    {
        int sqRow=sq/12;
        int sqCol=sq%12;
        if(sqRow<2 || sqRow>9 || sqCol<2 || sqCol>9)
            return -1;
        return ((sq/12)-2)*8+((sq%12)-2);
    }
    vector <cmove> generateMoveforPawn(int color, int currSq)
    {
        vector <cmove> moves;
        if (color == 0)
        {
            if(currSq/8==1)
            {
                moves.push_back(cmove(cpiece(cpiece::PIECE::bpawn,currSq),currSq,currSq+16));
            }
            moves.push_back(cmove(cpiece(cpiece::PIECE::bpawn,currSq),currSq,currSq+8));
        }
        else if (color == 1)
        {
            if(currSq/8==6)
            {
                moves.push_back(cmove(cpiece(cpiece::PIECE::wpawn,currSq),currSq,currSq-16));
            }
            moves.push_back(cmove(cpiece(cpiece::PIECE::wpawn,currSq),currSq,currSq-8));
        }
        return moves;
    }
    vector <cmove> generateQueenMoves(cpiece::PIECE pt, int currSq)
    {
        vector <cmove> moves=generateRookMoves(pt,currSq);
        auto bishopmoves = generateBishopMoves(pt,currSq);
        moves.insert(moves.end(),bishopmoves.begin(), bishopmoves.end());
        return moves;
    }
    vector <cmove> generateRookMoves(cpiece::PIECE pt, int currSq)
    {
        vector <cmove> moves;
        int color=0;
        if(static_cast<int>(pt)>5)
            color=1;
        int currRow=currSq/8;
        int currCol=currSq%8;
        for (int i=currRow+1;i<8;i++)
        {
            if(sq[8*i+currCol].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,8*i+currCol));
        }
        for (int i=currRow-1;i>=0;i--)
        {
            if(sq[8*i+currCol].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,8*i+currCol));
        }
        for (int i=currCol+1;i<8;i++)
        {
            if(sq[8*currRow+i].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,8*currRow+i));
        }
        for (int i=currCol-1;i>=0;i--)
        {
            if(sq[8*currRow+i].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,8*currRow+i));
        }
        
        return moves;
    }
    vector <cmove> generateBishopMoves(cpiece::PIECE pt, int currSq)
    {
        vector <cmove> moves;
        int color=0;
        if(static_cast<int>(pt)>5)
            color=1;
        int currRow=currSq/8;
        int currCol=currSq%8;
        int i=1;
        while(true)
        {
            int toSquareRow=currSq/8+i;
            int toSquareCol=currSq%8+i;
            int toSquare=toSquareRow*8+toSquareCol;
            if(toSquareCol>7 || toSquareRow>7)
                break;
            if(sq[toSquare].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        i++;
        }
        i=1;
        while(true)
        {
            int toSquareRow=currSq/8-i;
            int toSquareCol=currSq%8-i;
            int toSquare=toSquareRow*8+toSquareCol;
            if(toSquareCol<0 || toSquareRow<0)
                break;
            if(sq[toSquare].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        i++;
        }
        i=1;
        while(true)
        {
            int toSquareRow=currSq/8+i;
            int toSquareCol=currSq%8-i;
            int toSquare=toSquareRow*8+toSquareCol;
            if(toSquareCol<0 || toSquareRow>7)
                break;
            if(sq[toSquare].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        i++;
        }
        i=1;
        while(true)
        {
            int toSquareRow=currSq/8-i;
            int toSquareCol=currSq%8+i;
            int toSquare=toSquareRow*8+toSquareCol;
            if(toSquareCol>7 || toSquareRow<0)
                break;
            if(sq[toSquare].getType()!=cpiece::PIECE::none)
                break;
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        i++;
        }
        return moves;
    }
    vector <cmove> generateKingMoves(cpiece::PIECE pt, int currSq)
    {
        vector <cmove> moves;
        int color=0;
        if(static_cast<int>(pt)>5)
            color=1;
        int currRow=currSq/8;
        int currCol=currSq%8;

        int toSquareRow=currSq/8+1;
        int toSquareCol=currSq%8+1;
        int toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareCol>7 || toSquareRow>7 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8-1;
        toSquareCol=currSq%8-1;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareCol<0 || toSquareRow<0 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8+1;
        toSquareCol=currSq%8-1;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareCol<0 || toSquareRow>7 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8-1;
        toSquareCol=currSq%8+1;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareCol>7 || toSquareRow<0 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8+1;
        toSquareCol=currSq%8;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareRow>7 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8-1;
        toSquareCol=currSq%8;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareRow<0 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8;
        toSquareCol=currSq%8+1;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareCol>7 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        toSquareRow=currSq/8;
        toSquareCol=currSq%8-1;
        toSquare=toSquareRow*8+toSquareCol;
        if(!(toSquareCol<0 || sq[toSquare].getType()!=cpiece::PIECE::none))
        {   
            moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));
        }
        return moves;
    }
    vector <cmove> generateKnightMoves(cpiece::PIECE pt, int currSq)
    {
        vector <cmove> moves;
        int color=0;
        if(static_cast<int>(pt)>5)
            color=1;
        int currSqPadded=toPadded(currSq);
        vector <int> toSquares{23,25,14,10,-10,-14,-25,-23};
        for (auto toSquareDelta: toSquares)
        {
            int toSquarePadded=currSqPadded+toSquareDelta;
            int toSquare=fromPadded(toSquarePadded);
            if(toSquare!=-1 && sq[toSquare].getType()==cpiece::PIECE::none)
                moves.push_back(cmove(cpiece(pt,currSq),currSq,toSquare));   
        }
        return moves;
    }

    void makeMove(cmove move)
    {
        sq[move.getfromSq()]=cpiece(cpiece::PIECE::none,move.getfromSq());
        sq[move.gettoSq()]=move.getPiece();
        sq[move.gettoSq()].setSquare(move.gettoSq());
        _sideToMove ^= 1;
    }

    void takeMove(cmove move)
    {
        sq[move.gettoSq()]=cpiece(cpiece::PIECE::none,move.gettoSq());
        sq[move.getfromSq()]=move.getPiece();
        sq[move.getfromSq()].setSquare(move.getfromSq());
        _sideToMove ^= 1;
    }

    void display()
    {
        for (int i=0;i<64;i++)
        {
            cout<<sq[i].toString()<<" ";
            if ((i+1)%8==0)
                cout<<endl;
        }
        cout<<endl;
    }

};

#endif // _BOARD_H_
