#pragma once

#include "Evaluator.hpp"
#include "MoveGenerator.hpp"
#include "TranspositionTable.hpp"
#include "MoveOrder.hpp"

#include <memory>

class ChessAI
{
public:
    ChessAI(std::shared_ptr<MoveGenerator> moveGen);
    ~ChessAI();

    void StartSearch(int maxDepth = 100);

    inline Move GetBestMove() const { return bestMoveFound; };

    inline void AbortSearch() { abortSearch = true; }

    inline int GetCurrentDepthInSearch() const { return currentDepthInSearch; }

    volatile bool IsSearching() const { return searching; }

    volatile int depthReached = 0;
    volatile int nodesVisited = 0;

    std::string infoSearch;

    bool useTT = false;
    
private:
    Evaluator evaluator;
    MoveOrder moveOrder;
    std::shared_ptr<MoveGenerator> moveGenerator;

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

    volatile bool searching;
    volatile bool abortSearch;
    int currentDepthInSearch;
    bool hasSearchedAtLeastOneMove;

    int Search(int depth, int ply, int alpha, int beta);

    int SearchCaptures(int alpha, int beta);

    void RunIterativeDeepening(int maxDepth);

    inline bool IsMateScore(int score) const { return abs(score) > 90000; }

    inline int NumPlyToMateFromScore(int score) const { return immediateMateScore - abs(score); }
};