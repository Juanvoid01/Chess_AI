#include "Square.hpp"

Square::Square(int row, int col, float posX, float posY, float width, float height, const Renderer &r)
    : Object(r, selectedColor, posX, posY, width, height),
      row(row), col(col), piece(posX, posY, width, height, PieceType::EMPTY, PieceColor::WHITE, r)
{
    Object::SetColor(selectedColor[0], selectedColor[1], selectedColor[2], selectedColor[3]);
}

Square::~Square()
{
}

void Square::Render()
{
    if (selected)
        Object::Render();
    piece.Render();
}

void Square::PutPiece(PieceType newPiece, PieceColor color)
{
    piece.ChangePieceType(newPiece, color);
}
