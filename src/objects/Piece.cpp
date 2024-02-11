#include "Piece.hpp"

Piece::Piece(float posX, float posY, float width, float height, PieceType pieceType, PieceColor color, const Renderer &r)
    : Object(r, GetTextureName(pieceType, color), posX, posY, width, height), type(pieceType), color(color)
{
}

Piece::~Piece()
{
}

void Piece::Render()
{
    Object::Render();
}

void Piece::ChangePieceType(PieceType newType, PieceColor newColor)
{
    type = newType;
    color = newColor;
    SetTexture(GetTextureName(newType, newColor));
}

TextureName Piece::GetTextureName(PieceType pieceType, PieceColor color)
{
    switch (pieceType)
    {
    case PieceType::PAWN:
        return color == PieceColor::WHITE ? TextureName::WPAWN : TextureName::BPAWN;
        break;
    case PieceType::KING:
        return color == PieceColor::WHITE ? TextureName::WKING : TextureName::BKING;
        break;
    case PieceType::QUEEN:
        return color == PieceColor::WHITE ? TextureName::WQUEEN : TextureName::BQUEEN;
        break;
    case PieceType::KNIGHT:
        return color == PieceColor::WHITE ? TextureName::WKNIGHT : TextureName::BKNIGHT;
        break;
    case PieceType::BISHOP:
        return color == PieceColor::WHITE ? TextureName::WBISHOP : TextureName::BBISHOP;
        break;
    case PieceType::ROOK:
        return color == PieceColor::WHITE ? TextureName::WROOK : TextureName::BROOK;
        break;
    default:
        return TextureName::EMPTY;
        break;
    }
}
