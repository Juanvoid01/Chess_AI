#pragma once
#include "Piece.hpp"

class Square : public Object
{
public:
    Square(int row, int col, float posX, float posY, float width, float height, const Renderer &r);
    ~Square();

    void Render();

    inline const Piece &GetPieceObject() const { return piece; }
    inline PieceType GetPiece() const { return piece.type; }
    inline PieceColor GetPieceColor() const { return piece.color; }
    inline int GetRow() const { return row; }
    inline int GetCol() const { return col; }
    void PutPiece(PieceType newPiece, PieceColor color);
    inline void Clear() { piece.ChangePieceType(PieceType::EMPTY, PieceColor::WHITE); }
    inline bool IsEmpty() const { return piece.type == PieceType::EMPTY; }

    inline void Translate(float x, float y)
    {
        Object::Translate(x, y);
        piece.Translate(x, y);
    }
    inline void SetScale(float x, float y)
    {
        Object::SetScale(x, y);
        piece.SetScale(x, y);
    }
    inline void SetPosition(float x, float y)
    {
        Object::SetPosition(x, y);
        piece.SetPosition(x, y);
    }
    inline void SetCenter(float x, float y)
    {
        Object::SetCenter(x, y);
        piece.SetCenter(x, y);
    }

    inline void ReCalculateMVP()
    {
        Object::ReCalculateMVP();
        piece.ReCalculateMVP();
    }

    inline void SetVisibility(bool value) { piece.invisible = !value; }

    inline bool IsSelected() const { return selected; }

    inline void Select()
    {
        selected = true;
        if (isLastMove)
        {
            isLastMove = false;
            SetColor(selectedColor);
        }
    }

    inline void SelectAsLastMove()
    {
        selected = true;
        SetColor(lastMoveColor);
        isLastMove = true;
    }

    inline void UnSelect()
    {
        selected = false;
        if (isLastMove)
        {
            isLastMove = false;
            SetColor(selectedColor);
        }
    }

private:
    Piece piece;
    int row;
    int col;
    glm::vec4 selectedColor = glm::vec4(0.f, 0.5f, 0.f, 0.5f);
    glm::vec4 lastMoveColor = glm::vec4(0.8f, 0.8f, 0.f, 0.5f);
    bool selected = false;
    bool isLastMove = false;
};