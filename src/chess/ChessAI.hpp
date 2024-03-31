#pragma once

#include "Evaluator.hpp"
#include "MoveGenerator.hpp"
#include "TranspositionTable.hpp"
#include "MoveOrder.hpp"

#include <atomic>

class ChessAI
{
public:
    ChessAI();
    ~ChessAI();

    void StartSearch(MoveGenerator &moveGen);

    inline Move GetBestMove() const { return bestMoveFound; };

    inline void AbortSearch() { abortSearch = true; }

    inline int GetCurrentDepthInSearch() const { return currentDepthInSearch; }

    inline bool IsSearching() const { return searching; }

    std::atomic<int> depthReached = 0;
    std::atomic<int> nodesVisited = 0;

private:
    Evaluator evaluator;
    MoveOrder moveOrder;
    MoveGenerator *moveGenerator;

    Move bestMoveInIteration;
    int bestEvalInIteration;
    Move bestMoveFound;
    int bestEvalFound;

    PieceColor turnToMove;

    Zobrist zobrist;
    TranspositionTable ttTable;

    const int immediateMateScore = 100000;
    const int positiveInfinity = 9999999;
    const int negativeInfinity = -9999999;

    bool searching;
    std::atomic<bool> abortSearch;
    int currentDepthInSearch;

    int Search(int depth, int ply, int alpha, int beta);

    int SearchCaptures(int alpha, int beta);

    void RunIterativeDeepening(int maxDepth);

    inline bool IsMateScore(int score) const { return abs(score) > 90000; }

    inline int NumPlyToMateFromScore(int score) const { return immediateMateScore - abs(score); }
};