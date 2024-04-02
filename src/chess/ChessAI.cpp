#include "ChessAI.hpp"
#include <iostream>

ChessAI::ChessAI(std::shared_ptr<MoveGenerator> moveGen)
    : moveGenerator(moveGen), searching(false)
{

}

ChessAI::~ChessAI()
{
}

void ChessAI::StartSearch(int maxDepth)
{
    depthReached = 0;
    nodesVisited = 0;
    searching = true;

    bestEvalInIteration = 0;
    bestMoveInIteration = InvalidMove;
    bestMoveFound = bestMoveInIteration;
    bestEvalFound = bestEvalInIteration;

    zobrist.InitializeHashWithPos(moveGenerator->GetPieceArray());

    turnToMove = moveGenerator->GetTurn();

    RunIterativeDeepening(maxDepth);

    if (!bestMoveFound.IsValid())
    {
        std::cout << "\nRandom move selected\n";

        MoveArray moves;
        int n_moves = 0;
        moveGenerator->GetLegalMoves(moves, n_moves);
        bestMoveFound = moves[0];
    }

    searching = false;
}

void ChessAI::RunIterativeDeepening(int maxDepth)
{

    abortSearch = false;

    std::cout << "Search started :\nDepths : ";

    for (int searchDepth = 1; searchDepth <= maxDepth; searchDepth++)
    {

        depthReached = searchDepth; // debug

        std::cout << searchDepth << ' ' << std::flush;

        Search(searchDepth, 0, negativeInfinity, positiveInfinity);

        if (abortSearch)
        {
            break;
        }
        else
        {
            currentDepthInSearch = searchDepth;

            bestMoveFound = bestMoveInIteration;
            bestEvalFound = bestEvalInIteration;

            if (IsMateScore(bestEvalFound) && NumPlyToMateFromScore(bestEvalFound) <= searchDepth)
            {
                // We found a checkmate
                break;
            }
        }
    }
    std::cout << "\nSearch Finished\n";
}

int ChessAI::Search(int depth, int ply, int alpha, int beta)
{

    if (abortSearch)
    {
        return 0;
    }

    nodesVisited++; // debug

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

    moveOrder.OrderMoves(moves, n_moves, moveGenerator->GetPieceArray());

    int evaluationBound = ttTable.upperBound;
    Move bestMoveInPosition = InvalidMove;

    PosStateInfo posInfoState = moveGenerator->GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        zobrist.AddMove(moves[i], moveGenerator->GetPieceArray());
        moveGenerator->MakeMove(moves[i]);

        int evaluation = -Search(depth - 1, ply + 1, -beta, -alpha);

        zobrist.PutHashValue(zobristOfThisPos);
        moveGenerator->UnMakeMove(moves[i], posInfoState);

        if (abortSearch)
        {
            return 0;
        }

        if (evaluation >= beta)
        {
            ttTable.StoreEvaluation(zobristOfThisPos, depth, ply, beta, ttTable.lowerBound, moves[i]);

            return beta;
        }

        if (evaluation > alpha)
        {
            // bestMoveInPosition = moves[i];

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
    /*if (abortSearch)
    {
        return 0;
    }*/

    nodesVisited++; // debug

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
