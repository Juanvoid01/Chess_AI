#include "Perft.hpp"
#include "ChessEngine.hpp"

#include <iostream>
#include <chrono>

static ChessEngine chessEngine;

unsigned long long int Perft(unsigned int depth);
unsigned long long int PerftMoveInfo(unsigned int depth);

void PerftTest(const std::string &FEN, unsigned int depth)
{
    chessEngine.LoadFEN(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long long int n_moves = Perft(depth);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Perft " << depth << " : " << n_moves << "  execution time: " << duration.count() << " ms\n";
}

void PerftTestMoveInfo(const std::string &FEN)
{
    chessEngine.LoadFEN(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long long int n_moves = PerftMoveInfo(2);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "PerftMoveInfo : " << n_moves << "  execution time: " << duration.count() << " ms\n";
}

unsigned long long int Perft(unsigned int depth)
{
    if (depth == 0)
        return 1;

    unsigned long long int nodes = 0;
    MoveArray moves;
    int n_moves = 0;
    chessEngine.GetLegalMoves(moves, n_moves);
    PosStateInfo posInfoState = chessEngine.GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        chessEngine.MakeMove(moves[i]);
        nodes += Perft(depth - 1);
        chessEngine.UnMakeMove(moves[i], posInfoState);
    }
    return nodes;
}

unsigned long long int PerftMoveInfo(unsigned int depth)
{
    if (depth == 0)
        return 1;

    unsigned long long int nodes = 0;

    MoveArray moves;
    int n_moves = 0;
    chessEngine.GetLegalMoves(moves, n_moves);

    PosStateInfo posInfoState = chessEngine.GetStateInfo();

    for (int i = 0; i < n_moves; i++)
    {
        chessEngine.MakeMove(moves[i]);
        unsigned long long int newNodes = Perft(depth - 1);
        nodes += newNodes;
        chessEngine.UnMakeMove(moves[i], posInfoState);

        if (depth == 2)
        {
            std::cout << moves[i].ToBasicString() << ": " << newNodes << '\n';
        }
    }
    return nodes;
}
