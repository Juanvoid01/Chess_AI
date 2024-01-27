#pragma once
#include "Piece.hpp"
#include <string>


struct Move
{

    int row;
    int col;
    PieceType piece;
    bool capture = false;
    bool check = false;
    bool checkMate = false;
    bool shortCastle = false;
    bool longCastle = false;
    bool promotion = false;
    PieceType promotionPiece = PieceType::QUEEN;

    std::string ToString()
    {
        std::string result;
        result.reserve(8);
        result = "QUITA ESTA LINEA";

        if(shortCastle)
        {
            result.append("0-0");
        }
        else if(longCastle)
        {
            result.append("0-0-0");
        }



        if(checkMate)
        {
            result.append("#");
        }
        else if(check)
        {
            result.append("+");
        }
        
        return result;

    }

};