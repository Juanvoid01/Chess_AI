#include "Evaluator.hpp"

Evaluator::Evaluator()
{
}

Evaluator::~Evaluator()
{
}

int Evaluator::GetEvaluation(const PieceArray &pieces, PieceColor turn)
{
    this->pieces = &pieces;

    int evaluation = 0;
    int perspective = turn == PieceColor::WHITE ? 1 : -1;

    inEndgame = IsEndgameFase();

    int material = CountMaterial();

    evaluation += material;
    return evaluation * perspective;
}

// white pieces value - black pieces value
int Evaluator::CountMaterial() const
{
    int material = 0;

    PieceType pieceType;
    PieceColor pieceColor;

    short rowWK, colWK, rowBK, colBK;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            pieceType = (*pieces)[row][col].type;
            pieceColor = (*pieces)[row][col].color;

            if (pieceType == PieceType::EMPTY)
                continue;

            if (pieceColor == PieceColor::WHITE)
            {
                material += GetPieceRelativeValue(row, col);
            }
            else
            {
                material -= GetPieceRelativeValue(row, col);
            }

            if (pieceType == PieceType::KING)
            {
                if (pieceColor == PieceColor::WHITE)
                {
                    rowWK = row, colWK = col;
                }
                else
                {
                    rowBK = row, colBK = col;
                }
            }
        }
    }
    if (inEndgame)
        material +=(14 - GetKingSeparation(rowWK, colWK, rowBK, colBK));

    return material;
}

int Evaluator::GetPieceRelativeValue(int row, int col) const
{
    PieceColor pcolor = (*pieces)[row][col].color;
    PieceType ptype = (*pieces)[row][col].type;

    int value = GetPieceValue((*pieces)[row][col].type);

    int rowArray = pcolor == PieceColor::WHITE ? row : 7 - row;
    int colArray = pcolor == PieceColor::WHITE ? col : 7 - col;

    switch (ptype)
    {
    case PieceType::PAWN:
        value += pawnRelativeValues[rowArray][colArray];
        break;
    case PieceType::KNIGHT:
        value += knightRelativeValues[rowArray][colArray];
        break;
    case PieceType::BISHOP:
        value += bishopRelativeValues[rowArray][colArray];
        break;
    case PieceType::ROOK:
        value += rookRelativeValues[rowArray][colArray];
        break;
    case PieceType::QUEEN:
        value += queenRelativeValues[rowArray][colArray];
        break;
    case PieceType::KING:
        if (inEndgame)
        {
            value += kingEndgameRelativeValues[rowArray][colArray];
        }
        else
        {
            value += kingRelativeValues[rowArray][colArray];
        }

        break;
    default:
        value += 0;
        break;
    }
    return value;
}

int Evaluator::GetPieceValue(PieceType piece) const
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

bool Evaluator::IsEndgameFase()
{
    int numPieces = 0;
    int numQueens = 0;
    PieceType piece;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            piece = (*pieces)[row][col].type;
            if (piece != PieceType::EMPTY)
            {
                if (piece != PieceType::PAWN)
                {
                    numPieces++;
                }

                if (piece == PieceType::QUEEN)
                {
                    numQueens++;
                }
            }
        }
    }

    return numPieces <= 6 || numQueens <= 1 && numPieces <= 8;
}

int Evaluator::GetKingSeparation(short rowWK, short colWK, short rowBK, short colBK) const
{
    int separation = abs(rowWK - rowBK) + abs(colWK - colBK);
    return separation;
}