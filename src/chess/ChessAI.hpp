#pragma once

#include "Evaluator.hpp"
#include "MoveGenerator.hpp"

#include <limits>

/*const int POSITIVE_INFINITY = std::numeric_limits<int>::max();
const int NEGATIVE_INFINITY = std::numeric_limits<int>::min();
*/
class ChessAI
{
public:
    ChessAI();
    ~ChessAI();

    Move GetBestMove(MoveGenerator &moveGen);

private:
    Evaluator evaluator;
    MoveGenerator *moveGenerator;

    Move bestMoveInIteration;
    int bestEvalInIteration;
    PieceColor turnToMove;

    const int immediateMateScore = 100000;
    const int positiveInfinity = 9999999;
    const int negativeInfinity = -9999999;

    int Search(int depth, int ply, int alpha, int beta);

    void OrderMoves(MoveArray &moves, int n_moves);

    // returns true if move1 is most promising than move2
    bool MoveComparison(const Move &move1, const Move &move2);

    int SearchCaptures(int alpha, int beta);
};