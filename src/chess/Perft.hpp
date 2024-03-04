#pragma once
#include <string>

#define FEN_START_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN_KIWIPETE "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FEN_TALKCHESS "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
#define FEN_EDWARDS2 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
#define FEN_TEST4 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define FEN_TEST4_MIRROR "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1"
#define FEN_STRANGEMOVES "r3k2r/p1pppp1p/N3Q3/qpP1KN2/6p1/8/Pp2p1pP/R6R w kq b6 0 2"
#define FEN_LASKER_ENDGAME "8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1"
#define FEN_QUEEN_ROOK_MATE "8/8/8/3k4/8/8/8/5RQK w - - 0 1"
#define FEN_QUEEN_VS_PAWN_ENDGAME "8/8/2KQ4/8/8/8/3p4/3k4 w - - 0 1"
#define FEN_ROOK_MATE "6kr/8/8/3K4/8/8/8/8 b - - 0 1"

void PerftTest(const std::string &FEN, unsigned int depth);

void PerftTestMoveInfo(const std::string &FEN, int depth = 2);

void PerftTestIniPos();

void PerftTestKiwipete();

void PerftTestTalkchess();

void PerftTestEdwards2();

void PerftTestPos4();

void PerftTestPos4Mirror();

void PerftTestStrangeMoves();