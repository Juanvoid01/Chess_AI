#pragma once

#include "Evaluator.hpp"
#include "MoveGenerator.hpp"
#include "TranspositionTable.hpp"

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

    void OrderMoves(MoveArray &moves, int n_moves);

    // returns true if move1 is most promising than move2
    bool MoveComparison(const Move &move1, const Move &move2);

    int SearchCaptures(int alpha, int beta);

    void RunIterativeDeepening(int maxDepth);

    inline bool IsMateScore(int score) const { return abs(score) > 90000; }
};