#include "Square.hpp"

Square::Square(int row, int col, float posX, float posY, float width, float height, const Renderer &r)
    : row(row), col(col), piece(posX, posY, width, height, PieceType::EMPTY, PieceColor::WHITE, r)
{
}

Square::~Square()
{
}

void Square::Render(const Renderer &renderer)
{
    piece.Render(renderer);
}

void Square::PutPiece(PieceType newPiece, PieceColor color)
{
    piece.ChangePieceType(newPiece, color);
}

