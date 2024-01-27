#pragma once
#include "Piece.hpp"

class Square
{
public:
    Square();
    ~Square();

    void Render();

    inline PieceType GetPiece() { return piece.type; }

    inline void PutPiece(PieceType newPiece) { piece.type = newPiece; }

private:
    Piece piece;
};