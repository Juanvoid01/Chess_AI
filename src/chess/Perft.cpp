#include "Perft.hpp"
#include "ChessEngine.hpp"

#include <iostream>
#include <chrono>

#define RESETCOLOR "\033[0m"
#define REDCOLOR "\033[31m"
#define GREENCOLOR "\033[32m"
#define YELLOWCOLOR "\033[33m"
#define BLUECOLOR "\033[34m"
#define MAGENTACOLOR "\033[35m"
#define CYANCOLOR "\033[36m"
#define WHITECOLOR "\033[37m"

static ChessEngine chessEngine;

unsigned long long int Perft(unsigned int depth);
unsigned long long int PerftMoveInfo(unsigned int depth);

void runCorrectnessTest(const unsigned long long int correctResults[7], int MaxDepth = 6);

void PerftTest(const std::string &FEN, unsigned int depth)
{
    chessEngine.LoadFEN(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long long int n_moves = Perft(depth);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Perft " << depth << " : " << n_moves << "  execution time: " << duration.count() << " ms\n";
}

void PerftTestMoveInfo(const std::string &FEN, int depth)
{
    chessEngine.LoadFEN(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long long int n_moves = PerftMoveInfo(depth);

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

        std::cout << moves[i].ToBasicString() << ": " << newNodes << '\n';
    }
    return nodes;
}

void PerftTestIniPos()
{

    chessEngine.LoadFEN(FEN_START_POS);

    unsigned long long int results[7] = {1, 20, 400, 8902, 197281, 4865609, 119060324};

    runCorrectnessTest(results);
}

void PerftTestKiwipete()
{
    chessEngine.LoadFEN(FEN_KIWIPETE);

    unsigned long long int results[7] = {1, 48, 2039, 97862, 4085603, 193690690, 8031647685};

    runCorrectnessTest(results, 6);
}

void PerftTestTalkchess()
{
    chessEngine.LoadFEN(FEN_TALKCHESS);

    unsigned long long int results[7] = {1, 44, 1486, 62379, 2103487, 89941194, 0};

    runCorrectnessTest(results, 5);
}

void PerftTestEdwards2()
{
    chessEngine.LoadFEN(FEN_EDWARDS2);

    unsigned long long int results[7] = {1, 46, 2079, 89890, 3894594, 164075551, 0};

    runCorrectnessTest(results, 5);
}

void PerftTestPos4()
{
    chessEngine.LoadFEN(FEN_TEST4);

    unsigned long long int results[7] = {1, 6, 264, 9467, 422333, 15833292, 706045033};

    runCorrectnessTest(results, 6);
}

void PerftTestPos4Mirror()
{
    chessEngine.LoadFEN(FEN_TEST4_MIRROR);

    unsigned long long int results[7] = {1, 6, 264, 9467, 422333, 15833292, 706045033};

    runCorrectnessTest(results, 6);
}

void runCorrectnessTest(const unsigned long long int correctResults[7], int MaxDepth)
{
    std::cout << "\nRunning Perft Test :\n\n";

    for (int depth = 1; depth <= MaxDepth; depth++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        unsigned long long int n_moves = Perft(depth);

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Depth " << depth << " : ";
        std::cout << YELLOWCOLOR << n_moves << RESETCOLOR;
        std::cout << "  execution time: " << duration.count() << " ms  ";

        if (n_moves == correctResults[depth])
        {
            std::cout << GREENCOLOR << "correct" << RESETCOLOR << '\n';
        }
        else
        {
            std::cout << REDCOLOR << "incorrect " << RESETCOLOR << "should be: " << correctResults[depth] << '\n';
        }
    }
    std::cout << std::endl;
}
