#pragma once
#include "Object.hpp"
#include "PieceType.hpp"

class PromotionSelector : public Object
{
public:
    PromotionSelector(const Renderer &r, float boardx, float boardY, float squareWidth, float squareHeight)
        : Object(r, TextureName::PROMO_WHITE_TOP, boardx, boardY, squareWidth, 4 * squareHeight),
          squareHeight(squareHeight), squareWidth(squareWidth), boardX(boardX), boardY(boardY), inSelection(false){

                                                                                                };

    ~PromotionSelector(){};

    inline bool IsInSelection() const { return inSelection; }

    void StartSelection(short promoRow, short promoCol, PieceColor pieceColor, bool rotated)
    {
        this->rotated = rotated;
        this->pieceColor = pieceColor;
        this->promoRow = promoRow;
        this->promoCol = promoCol;

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
            if (InTop())
            {
                return PieceType::BISHOP;
            }
            else
            {
                return PieceType::QUEEN;
            }
        }
        else if (mouseY < GetY() + 2 * squareHeight)
        {
            if (InTop())
            {
                return PieceType::ROOK;
            }
            else
            {
                return PieceType::KNIGHT;
            }
        }
        else if (mouseY < GetY() + 3 * squareHeight)
        {
            if (InTop())
            {
                return PieceType::KNIGHT;
            }
            else
            {
                return PieceType::ROOK;
            }
        }
        else
        {
            if (InTop())
            {
                return PieceType::QUEEN;
            }
            else
            {
                return PieceType::BISHOP;
            }
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

    inline short GetPromoRow() const { return promoRow; }
    inline short GetPromoCol() const { return promoCol; }

private:
    float squareHeight;
    float squareWidth;
    float boardX;
    float boardY;
    bool inSelection;

    short promoRow;
    short promoCol;
    bool rotated;
    PieceColor pieceColor;

    inline bool InTop() const { return (promoRow == 7 && !rotated) || (promoRow == 0 && rotated); }
    inline void AdjustPosition()
    {
        if (rotated)
        {
            if (promoRow == 7)
            {
                if (pieceColor == PieceColor::WHITE)
                {
                    SetTexture(TextureName::PROMO_WHITE_DOWN);
                }
                else
                {
                    SetTexture(TextureName::PROMO_BLACK_DOWN);
                }

                Object::SetPosition(boardX + (7 - promoCol) * squareWidth, boardY);
            }
            else
            {
                if (pieceColor == PieceColor::WHITE)
                {
                    SetTexture(TextureName::PROMO_WHITE_TOP);
                }
                else
                {
                    SetTexture(TextureName::PROMO_BLACK_TOP);
                }

                Object::SetPosition(boardX + (7 - promoCol) * squareWidth, boardY + 4 * squareHeight);
            }
        }
        else
        {
            if (promoRow == 7)
            {
                if (pieceColor == PieceColor::WHITE)
                {
                    SetTexture(TextureName::PROMO_WHITE_TOP);
                }
                else
                {
                    SetTexture(TextureName::PROMO_BLACK_TOP);
                }
                Object::SetPosition(boardX + promoCol * squareWidth, boardY + 4 * squareHeight);
            }
            else
            {
                if (pieceColor == PieceColor::WHITE)
                {
                    SetTexture(TextureName::PROMO_WHITE_DOWN);
                }
                else
                {
                    SetTexture(TextureName::PROMO_BLACK_DOWN);
                }
                Object::SetPosition(boardX + promoCol * squareWidth, boardY);
            }
        }
    }
};