#pragma once

#include "MoveGenerator.hpp"
#include "Evaluator.hpp"

class MoveOrder
{
public:
    MoveOrder();
    ~MoveOrder() {}

    void OrderMoves(MoveArray &moves, int nMoves, const PieceArray &pieces);

private:
    const PieceArray *pieces;

    void Sort(MoveArray &moves, int nMoves);

};