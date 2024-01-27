#pragma once

enum class PieceType
{
    EMPTY = 0,
    PAWN,
    KING,
    QUEEN,
    KNIGHT,
    BISHOP,
    ROCK
};

class Piece
{
public:
    Piece(PieceType PieceType);
    ~Piece();

    void Render();

    PieceType type;

private:

};