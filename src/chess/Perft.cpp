#include "Perft.hpp"
#include "ChessEngine.hpp"

#include <iostream>
#include <chrono>

static ChessEngine chessEngine;

unsigned long long int Perft(unsigned int depth);

void PerftTest(const std::string &FEN, unsigned int depth)
{
    /*chessEngine.LoadFEN(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    Perft(depth);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Perft execution time: " << duration.count() << " milliseconds\n";*/
}

unsigned long long int Perft(int depth)
{
    /*int n_moves, i;
    unsigned long long int nodes = 0;

    if (depth == 0)
        return 1ULL;

    n_moves = chessEngine.GetLegalMoves().size();

    for (i = 0; i < n_moves; i++)
    {
        ChessEngine
        nodes += Perft(depth - 1);
        UndoMove(move_list[i]);
    }
    return nodes;*/
}
