#pragma once
#include "Object.hpp"
#include "PieceType.hpp"

class PromotionSelector : public Object
{
public:
    short endRow;
    short endCol;

    PromotionSelector(const Renderer &r, float boardx, float boardY, float squareWidth, float squareHeight)
        : Object(r, TextureName::PROMOTION, boardx, boardY, squareWidth, 4 * squareHeight),
          squareHeight(squareHeight), squareWidth(squareWidth), boardX(boardX), boardY(boardY), inSelection(false)
    {
        colSelection = 0;
    };

    ~PromotionSelector(){};

    inline bool IsInSelection() const { return inSelection; }

    void StartSelection(short endRow, short endCol, bool inTop)
    {
        this->inTop = inTop;

        this->endRow = endRow;
        this->endCol = endCol;

        colSelection = endCol;
        inSelection = true;
        AdjustPosition();
    }

    PieceType GetSelectedPiece(float mouseX, float mouseY)
    {
        if (!inSelection || !PosInside(mouseX, mouseY))
            return PieceType::EMPTY;

        inSelection = false;

        if (mouseY < GetY() + squareHeight)
        {
            return PieceType::BISHOP;
        }
        else if (mouseY < GetY() + 2 * squareHeight)
        {
            return PieceType::ROOK;
        }
        else if (mouseY < GetY() + 3 * squareHeight)
        {
            return PieceType::KNIGHT;
        }
        else
        {
            return PieceType::QUEEN;
        }
    }

    void Translate(float x, float y)
    {
        boardX += x;
        boardY += y;

        AdjustPosition();
    }

    void SetScale(float x, float y, float squareWidth, float squareHeight)
    {
        Object::SetScale(x, y);

        this->squareWidth = squareWidth;
        this->squareHeight = squareHeight;
    }

    void SetPosition(float boardX, float boardY)
    {
        this->boardX = boardX;
        this->boardY = boardY;

        AdjustPosition();
    }

    void SetCenter(float boardX, float boardY)
    {
        this->boardX = boardX;
        this->boardY = boardY;

        AdjustPosition();
    }

private:
    float squareHeight;
    float squareWidth;
    float boardX;
    float boardY;
    bool inSelection;
    short colSelection;
    bool inTop;

    inline void AdjustPosition()
    {
        if (inTop)
        {
            Object::SetPosition(boardX + colSelection * squareWidth, boardY + 4 * squareHeight);
        }
        else
        {
            Object::SetPosition(boardX + colSelection * squareWidth, boardY);
        }
    }
};