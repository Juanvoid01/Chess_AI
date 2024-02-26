#pragma once

#include "Evaluator.hpp"
#include "MoveGenerator.hpp"

#include <limits>

const int POSITIVE_INFINITY = std::numeric_limits<int>::max();
const int NEGATIVE_INFINITY = std::numeric_limits<int>::min();

class ChessAI
{
public:
    ChessAI();
    ~ChessAI();

    Move GetBestMove(MoveGenerator &moveGen);

private:
    Evaluator evaluator;
    MoveGenerator *moveGenerator;

    int Search(int depth, int alpha, int beta);
};