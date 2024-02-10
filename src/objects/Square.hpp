#pragma once
#include "Piece.hpp"

class Square
{
public:
    Square(int row, int col, float posX, float posY, float width, float height, const Renderer &r);
    ~Square();

    void Render();

    inline PieceType GetPiece() { return piece.type; }
    inline PieceColor GetPieceColor() { return piece.color; }
    inline int GetRow() const { return row; }
    inline int GetCol() const { return col; }
    void PutPiece(PieceType newPiece, PieceColor color);
    inline void Clear() { piece.ChangePieceType(PieceType::EMPTY, PieceColor::WHITE); }
    inline bool IsEmpty() const{return piece.type == PieceType::EMPTY;}

    inline void Translate(float x, float y) { piece.Translate(x, y); }
    inline void SetScale(float x, float y) { piece.SetScale(x, y); }
    inline void SetPosition(float x, float y) { piece.SetPosition(x, y); }
    inline void SetCenter(float x, float y) { piece.SetCenter(x, y); }

private:
    Piece piece;
    int row;
    int col;
};