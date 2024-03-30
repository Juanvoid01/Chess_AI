#pragma once

#include "Object.hpp"
#include "PieceType.hpp"

class Piece : public Object
{
public:
    enum class State
    {
        QUIET,
        MOVING
    };

    Piece(float posX, float posY, float width, float height, PieceType pieceType, PieceColor pieceColor, const Renderer &r);
    Piece();
    ~Piece();

    Piece(const Piece& other);

    void Render();
    void Update();

    void ChangePieceType(PieceType type, PieceColor color);

    PieceType type;
    PieceColor color;
    State state;

    void MoveTo(float x, float y);

    float moveVelocity = 20.f;

    bool invisible = false;
private:
    float posToMoveX;
    float posToMoveY;

    TextureName GetTextureName(PieceType pieceType, PieceColor color);

};