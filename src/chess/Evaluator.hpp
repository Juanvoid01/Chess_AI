#pragma once

#include "Move.hpp"

class Evaluator
{
public:
    Evaluator();
    ~Evaluator();

    int GetEvaluation(const PieceArray &pieces, PieceColor turn);

    inline static int GetPieceValue(PieceType piece);

private:
    const PieceArray *pieces;

    // white pieces value - black pieces value
    int CountMaterial() const;

    int GetPieceRelativeValue(int row, int col) const;

    bool IsEndgameFase();

    int GetKingSeparation(short rowWK, short colWK, short rowBK, short colBK) const;

    bool inEndgame;

    short pawnRelativeValues[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {5, 10, 10, -20, -20, 10, 10, 5},
        {5, -5, -10, 0, 0, -10, -5, 5},
        {0, 0, 0, 20, 20, 0, 0, 0},
        {5, 5, 10, 25, 25, 10, 5, 5},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {0, 0, 0, 0, 0, 0, 0, 0}};

    short knightRelativeValues[8][8] = {
        {-50, -40, -30, -30, -30, -30, -40, -50},
        {-40, -20, 0, 5, 5, 0, -20, -40},
        {-30, 5, 10, 15, 15, 10, 5, -30},
        {-30, 0, 15, 20, 20, 15, 0, -30},
        {-30, 5, 15, 20, 20, 15, 5, -30},
        {-30, 0, 10, 15, 15, 10, 0, -30},
        {-40, -20, 0, 0, 0, 0, -20, -40},
        {-50, -40, -30, -30, -30, -30, -40, -50}};

    short bishopRelativeValues[8][8] = {
        {-20, -10, -10, -10, -10, -10, -10, -20},
        {-10, 5, 0, 0, 0, 0, 5, -10},
        {-10, 10, 10, 10, 10, 10, 10, -10},
        {-10, 0, 10, 10, 10, 10, 0, -10},
        {-10, 5, 5, 10, 10, 5, 5, -10},
        {-10, 0, 5, 10, 10, 5, 0, -10},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-20, -10, -10, -10, -10, -10, -10, -20}};

    short rookRelativeValues[8][8] = {
        {0, 0, 0, 5, 5, 0, 0, 0},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {5, 10, 10, 10, 10, 10, 10, 5},
        {0, 0, 0, 0, 0, 0, 0, 0}};

    short queenRelativeValues[8][8] = {
        {-20, -10, -10, -5, -5, -10, -10, -20},
        {-10, 0, 5, 0, 0, 0, 0, -10},
        {-10, 5, 5, 5, 5, 5, 0, -10},
        {0, 0, 5, 5, 5, 5, 0, -5},
        {-5, 0, 5, 5, 5, 5, 0, -5},
        {-10, 0, 5, 5, 5, 5, 0, -10},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-20, -10, -10, -5, -5, -10, -10, -20}};

    short kingRelativeValues[8][8] = {
        {20, 30, 10, 0, 0, 10, 30, 20},
        {20, 20, 0, 0, 0, 0, 20, 20},
        {-10, -20, -20, -20, -20, -20, -20, -10},
        {-20, -30, -30, -40, -40, -30, -30, -20},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30}};

    short kingEndgameRelativeValues[8][8] = {
        {-50, -45, -40, -35, -35, -40, -45, -50},
        {-45, -40, 10, 10, 10, 10, -40, -45},
        {-40, 10, 20, 30, 30, 20, 10, -40},
        {-35, 10, 30, 40, 40, 30, 10, -35},
        {-35, 10, 30, 40, 40, 30, 10, -35},
        {-40, 10, 20, 30, 30, 20, 10, -40},
        {-45, -40, 10, 10, 10, 10, -40, -45},
        {-50, -45, -40, -35, -35, -40, -45, -50}};
};


inline int Evaluator::GetPieceValue(PieceType piece)
{
    switch (piece)
    {
    case PieceType::PAWN:
        return 100;
        break;
    case PieceType::KNIGHT:
        return 320;
        break;
    case PieceType::BISHOP:
        return 330;
        break;
    case PieceType::ROOK:
        return 500;
        break;
    case PieceType::QUEEN:
        return 900;
        break;
    default:
        return 0;
        break;
    }
}