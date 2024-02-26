#pragma once

#include "Move.hpp"

class Evaluator
{
public:
    Evaluator();
    ~Evaluator();

    int GetEvaluation(const PieceArray &pieces, PieceColor turn);

private:
    PieceArray pieces;
    PieceType turn;

    int CountMaterial(const PieceArray &pieces, PieceColor turn) const;

    int GetPieceValue(PieceType piece) const;
};