#include "Perft.hpp"
#include "ChessEngine.hpp"

#include <iostream>
#include <chrono>

static ChessEngine chessEngine;

unsigned long long int Perft(unsigned int depth);

void PerftTest(const std::string &FEN, unsigned int depth)
{
    chessEngine.LoadFEN(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long long int n_moves = Perft(depth);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Perft : " << n_moves << "  execution time: " << duration.count() << " ms\n";
}

unsigned long long int Perft(unsigned int depth)
{
    if (depth == 0)
        return 1;

    unsigned long long int nodes = 0;

    int n_moves = chessEngine.GetLegalMoves().size();
    PosStateInfo posInfoState;

    for (Move move : chessEngine.GetLegalMoves())
    {
        posInfoState = chessEngine.GetStateInfo();
        chessEngine.MakeMove(move);
        nodes += Perft(depth - 1);
        chessEngine.UnMakeMove(move, posInfoState);
    }
    return nodes;
}
