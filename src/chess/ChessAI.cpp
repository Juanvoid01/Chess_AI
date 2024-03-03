#include "ChessAI.hpp"

ChessAI::ChessAI()
{
}

ChessAI::~ChessAI()
{
}

Move ChessAI::GetBestMove(MoveGenerator &moveGen)
{
    moveGenerator = &moveGen;

    bestEvalInIteration = 0;
    bestMoveInIteration = InvalidMove;

    zobrist.InitializeHashWithPos(moveGenerator->GetPieceArray());
    ttTable.Clear();

    turnToMove = moveGenerator->GetTurn();

    int alpha = negativeInfinity;
    int beta = positiveInfinity;
    int eval = Search(10, 0, alpha, beta);

    return bestMoveInIteration;
}

int ChessAI::Search(int depth, int ply, int alpha, int beta)
{

    if (ply > 0)
    {
        alpha = std::max(alpha, -immediateMateScore + ply);
        beta = std::min(beta, immediateMateScore - ply);
        if (alpha >= beta)
        {
            return alpha;
        }
    }
    uint64_t zobristOfThisPos = zobrist.GetHashValue();

    int ttVal = ttTable.GetEvaluation(zobristOfThisPos, depth, ply, alpha, beta);
    if (ttVal != ttTable.lookupFailed)
    {
        if (ply == 0)
        {
            bestMoveInIteration = ttTable.GetEntry(zobristOfThisPos).move;
            bestEvalInIteration = ttTable.GetEntry(zobristOfThisPos).value;
        }
        return ttVal;
    }

    if (depth == 0)
    {
        return SearchCaptures(alpha, beta);
    }

    MoveArray moves;
    int n_moves = 0;
    moveGenerator->GetLegalMoves(moves, n_moves);

    if (moveGenerator->IsCheckMate())
    {
        int mateScore = immediateMateScore - ply;
        return -mateScore;
    }
    else if (moveGenerator->IsStaleMate())
    {
        return 0;
    }

    OrderMoves(moves, n_moves);

    int evaluationBound = ttTable.upperBound;
    Move bestMoveInPosition = Move(-1, -1, -1, -1, MoveType::INVALID);

    PosStateInfo posInfoState = moveGenerator->GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        zobrist.AddMove(moves[i], moveGenerator->GetPieceArray());
        moveGenerator->MakeMove(moves[i]);

        int evaluation = -Search(depth - 1, ply + 1, -beta, -alpha);

        zobrist.PutHashValue(zobristOfThisPos);
        moveGenerator->UnMakeMove(moves[i], posInfoState);

        if (evaluation >= beta)
        {
            ttTable.StoreEvaluation(zobristOfThisPos, depth, ply, beta, ttTable.lowerBound, moves[i]);

            return beta;
        }

        if (evaluation > alpha)
        {
            evaluationBound = ttTable.exact;
            alpha = evaluation;
            if (ply == 0)
            {
                bestMoveInIteration = moves[i];
                bestEvalInIteration = evaluation;
            }
        }
    }
    ttTable.StoreEvaluation(zobrist.GetHashValue(), depth, ply, alpha, evaluationBound, bestMoveInPosition);
    return alpha;
}

int ChessAI::SearchCaptures(int alpha, int beta)
{
    int evaluation = evaluator.GetEvaluation(moveGenerator->GetPieceArray(), moveGenerator->GetTurn());

    if (evaluation >= beta)
    {
        return beta;
    }
    if (evaluation > alpha)
    {
        alpha = evaluation;
    }

    MoveArray moves;
    int n_moves = 0;
    moveGenerator->GetLegalMoves(moves, n_moves);

    PosStateInfo posInfoState = moveGenerator->GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        if (!moveGenerator->isCapture(moves[i].type))
            continue;

        moveGenerator->MakeMove(moves[i]);

        evaluation = -SearchCaptures(-beta, -alpha);

        moveGenerator->UnMakeMove(moves[i], posInfoState);

        if (evaluation >= beta)
        {
            return beta;
        }
        if (evaluation > alpha)
        {
            alpha = evaluation;
        }
    }

    return alpha;
}

void ChessAI::OrderMoves(MoveArray &moves, int n_moves)
{

    bool swapped;
    for (int i = 0; i < n_moves - 1; ++i)
    {
        swapped = false;
        for (int j = 0; j < n_moves - i - 1; ++j)
        {
            if (!MoveComparison(moves[j], moves[j + 1]))
            {
                std::swap(moves[j], moves[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// returns true if move1 is most promising than move2
bool ChessAI::MoveComparison(const Move &move1, const Move &move2)
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
        value1 = 2;
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
        value2 = 2;
        break;
    }

    const PieceArray &pieces = moveGenerator->GetPieceArray();

    value1 += evaluator.GetPieceValue(pieces[move1.iniRow][move1.iniCol].type);
    value2 += evaluator.GetPieceValue(pieces[move2.iniRow][move2.iniCol].type);

    return value1 > value2;
}
