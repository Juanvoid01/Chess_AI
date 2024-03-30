#include "Piece.hpp"

Piece::Piece(float posX, float posY, float width, float height, PieceType pieceType, PieceColor pieceColor, const Renderer &r)
    : Object(r, GetTextureName(pieceType, pieceColor), posX, posY, width, height), type(pieceType), color(pieceColor), state(State::QUIET)
{
}

Piece::Piece(const Piece &other)
    : Object(other.renderer, GetTextureName(other.type, color),
             other.GetX(), other.GetY(), other.GetWidth(), other.GetHeight()),
      type(other.type), color(other.color), state(other.state)
{
}

Piece::~Piece()
{
}

void Piece::Render()
{
    if (!invisible)
        Object::Render();
}

void Piece::Update()
{
    if (state == State::MOVING)
    {
        float dirX, dirY;

        Object::DirectionTo(posToMoveX, posToMoveY, dirX, dirY);
        float distance = Object::DistanceTo(posToMoveX, posToMoveY);
        dirX *= moveVelocity;
        dirY *= moveVelocity;

        if (distance <= moveVelocity)
        {
            Object::SetPosition(posToMoveX, posToMoveY);
            state = State::QUIET;
        }
        else
        {
            Object::Translate(dirX, dirY);
        }
    }
    else
    {
    }
}

void Piece::MoveTo(float x, float y)
{
    posToMoveX = x;
    posToMoveY = y;
    state = State::MOVING;
}

void Piece::ChangePieceType(PieceType newType, PieceColor newColor)
{
    type = newType;
    color = newColor;
    SetTexture(GetTextureName(newType, newColor));
}

TextureName Piece::GetTextureName(PieceType pieceType, PieceColor pieceColor)
{
    switch (pieceType)
    {
    case PieceType::PAWN:
        return pieceColor == PieceColor::WHITE ? TextureName::WPAWN : TextureName::BPAWN;
        break;
    case PieceType::KING:
        return pieceColor == PieceColor::WHITE ? TextureName::WKING : TextureName::BKING;
        break;
    case PieceType::QUEEN:
        return pieceColor == PieceColor::WHITE ? TextureName::WQUEEN : TextureName::BQUEEN;
        break;
    case PieceType::KNIGHT:
        return pieceColor == PieceColor::WHITE ? TextureName::WKNIGHT : TextureName::BKNIGHT;
        break;
    case PieceType::BISHOP:
        return pieceColor == PieceColor::WHITE ? TextureName::WBISHOP : TextureName::BBISHOP;
        break;
    case PieceType::ROOK:
        return pieceColor == PieceColor::WHITE ? TextureName::WROOK : TextureName::BROOK;
        break;
    default:
        return TextureName::EMPTY;
        break;
    }
}
