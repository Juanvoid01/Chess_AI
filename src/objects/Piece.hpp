#pragma once

#include "Object.hpp"

enum class PieceType
{
    EMPTY = 0,
    PAWN,
    KING,
    QUEEN,
    KNIGHT,
    BISHOP,
    ROOK
};

enum class PieceColor
{
    WHITE,
    BLACK
};

class Piece : public Object
{
public:
    Piece(float posX, float posY, float width, float height, PieceType pieceType, PieceColor color, const Renderer &r);
    ~Piece();

    void Render();

    void ChangePieceType(PieceType type, PieceColor color);
    
    PieceType type;
    PieceColor color;

private:
    TextureName GetTextureName(PieceType pieceType, PieceColor color);
};