#include "MoveOrder.hpp"

MoveOrder::MoveOrder()
{
}

void MoveOrder::OrderMoves(MoveArray &moves, int nMoves, const PieceArray &pieces)
{
    this->pieces = &pieces;

    for (int i = 0; i < nMoves - 1; ++i)
    {
        PieceType iniPiece = pieces[moves[i].iniRow][moves[i].iniCol].type;
        PieceType endPiece = pieces[moves[i].endRow][moves[i].endCol].type;

        int iniPieceValue = Evaluator::GetPieceValue(iniPiece);
        int endPieceValue = Evaluator::GetPieceValue(endPiece);

        moves[i].evaluation = iniPieceValue + endPieceValue;
    }
}

void MoveOrder::Sort(MoveArray &moves, int nMoves)
{

    bool swapped;
    for (int i = 0; i < nMoves - 1; ++i)
    {
        swapped = false;
        for (int j = 0; j < nMoves - i - 1; ++j)
        {
            if (moves[j].evaluation < moves[j + 1].evaluation)
            {
                std::swap(moves[j], moves[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}