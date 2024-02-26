#include "Evaluator.hpp"

Evaluator::Evaluator()
{
}

Evaluator::~Evaluator()
{
}

int Evaluator::GetEvaluation(const PieceArray &pieces, PieceColor turn)
{
    int evaluation = 0;
    int perspective = turn == PieceColor::WHITE ? 1 : -1;

    int material = CountMaterial(pieces,turn);

    evaluation += material;
    return evaluation * perspective;

}

int Evaluator::CountMaterial(const PieceArray &pieces, PieceColor turn) const
{
    int material = 0;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (pieces[row][col].type == PieceType::EMPTY)
                continue;

            if (pieces[row][col].color == turn)
            {
                material += GetPieceValue(pieces[row][col].type);
            }
            else
            {
                material -= GetPieceValue(pieces[row][col].type);
            }
        }
    }
    return material;
}

int Evaluator::GetPieceValue(PieceType piece) const
{
    switch (piece)
    {
    case PieceType::PAWN:
        return 1;
        break;
    case PieceType::KNIGHT:
        return 3;
        break;
    case PieceType::BISHOP:
        return 3;
        break;
    case PieceType::ROOK:
        return 5;
        break;
    case PieceType::QUEEN:
        return 9;
        break;
    default:
        return 0;
        break;
    }
}