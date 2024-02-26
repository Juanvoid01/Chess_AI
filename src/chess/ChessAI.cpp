#include "ChessAI.hpp"
#include <algorithm>

bool MoveComparison(const Move &move1, const Move &move2);
int GetPieceValue(PieceType piece);

ChessAI::ChessAI()
{
}

ChessAI::~ChessAI()
{
}

Move ChessAI::GetBestMove(MoveGenerator &moveGen)
{
    moveGenerator = &moveGen;

    int bestEval = -1;
    Move bestMove = Move(-1, -1, -1, -1, MoveType::INVALID);

    MoveArray moves;
    int n_moves = 0;
    moveGenerator->GetLegalMoves(moves, n_moves);

    std::sort(moves.begin(), moves.begin() + n_moves, MoveComparison);

    PosStateInfo posInfoState = moveGenerator->GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        moveGenerator->MakeMove(moves[i]);

        int evaluation = -Search(6, NEGATIVE_INFINITY, POSITIVE_INFINITY);
        if (evaluation > bestEval || bestMove.type == MoveType::INVALID)
        {
            bestEval = evaluation;
            bestMove = moves[i];
        }

        moveGenerator->UnMakeMove(moves[i], posInfoState);
    }

    return bestMove;
}

int ChessAI::Search(int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return evaluator.GetEvaluation(moveGenerator->GetPieceArray(), moveGenerator->GetTurn());
    }

    MoveArray moves;
    int n_moves = 0;
    moveGenerator->GetLegalMoves(moves, n_moves);

    std::sort(moves.begin(), moves.begin() + n_moves, MoveComparison);

    if (moveGenerator->IsCheckMate())
    {
        return NEGATIVE_INFINITY;
    }
    else if (moveGenerator->IsStaleMate())
    {
        return 0;
    }

    int bestEvaluation = NEGATIVE_INFINITY;

    PosStateInfo posInfoState = moveGenerator->GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        moveGenerator->MakeMove(moves[i]);

        int evaluation = -Search(depth - 1, -beta, -alpha);
        bestEvaluation = std::max(evaluation, bestEvaluation);

        moveGenerator->UnMakeMove(moves[i], posInfoState);

        if (evaluation >= beta)
        {
            return beta; // move too good, opponent will avoid this position
        }
        alpha = std::max(alpha, evaluation);
    }
    return alpha;
}

int GetPieceValue(PieceType piece)
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

// returns true if move1 is most promising than move2
bool MoveComparison(const Move &move1, const Move &move2)
{
    int value1, value2;

    switch (move1.type)
    {
    case MoveType::CAPTURE:
        value1 = 5;
        break;
    case MoveType::QUIET:
        value1 = 1;
        break;
    case MoveType::DOUBLEPAWNPUSH:
        value1 = 1;
        break;
    default:
        value1 = 3;
        break;
    }

    switch (move2.type)
    {
    case MoveType::CAPTURE:
        value2 = 5;
        break;
    case MoveType::QUIET:
        value2 = 1;
        break;
    case MoveType::DOUBLEPAWNPUSH:
        value2 = 1;
        break;
    default:
        value2 = 3;
        break;
    }

     return value1 > value2;
}
