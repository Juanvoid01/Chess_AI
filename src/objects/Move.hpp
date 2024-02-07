#pragma once
#include "Piece.hpp"
#include <string>

struct Move
{

    int iniRow;
    int iniCol;
    int endRow;
    int endCol;
    PieceType piece;
    bool capture = false;
    bool check = false;
    bool checkMate = false;
    bool shortCastle = false;
    bool longCastle = false;
    bool promotion = false;
    bool specifyIniRow = false;
    bool specifyIniCol = false;
    PieceType promotionPiece = PieceType::QUEEN;

    std::string ToString()
    {
        std::string result;
        result.reserve(8);

        if (shortCastle)
        {
            result.append("0-0");
        }
        else if (longCastle)
        {
            result.append("0-0-0");
        }
        else
        {
            if (piece == PieceType::PAWN)
            {
                result.append("" + colToChar(iniCol));
            }
            else
            {
                result.append("" + pieceTypeToChar(piece));
            }

            if (specifyIniCol)
            {
                result.append("" + colToChar(iniCol));
            }

            if (specifyIniRow)
            {
                result.append("" + ('0' + iniRow));
            }

            if (capture)
            {
                result.append("" + 'x');
            }

            result.append("" + colToChar(endCol) + ('0' + endRow));


            if(promotion)
            {
                result.append("=" + pieceTypeToChar(piece)); 
            }
        }

        if (checkMate)
        {
            result.append("#");
        }
        else if (check)
        {
            result.append("+");
        }

        return result;
    }

private:
    char colToChar(int col)
    {
        if (col < 1 || col > 8)
            return '0'; // error

        return 'a' + col - 1;
    }

    char pieceTypeToChar(PieceType type)
    {
        switch (type)
        {
        case PieceType::KING:
            return 'K';
            break;
        case PieceType::QUEEN:
            return 'Q';
            break;
        case PieceType::KNIGHT:
            return 'N';
            break;
        case PieceType::BISHOP:
            return 'B';
            break;
        case PieceType::ROOK:
            return 'R';
            break;
        default:
            return '0';
            break;
        }
    }
};