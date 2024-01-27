#pragma once

#include "Square.hpp"
#include "Move.hpp"
#include <array>
#include <vector>

class Board
{
public:
    Board();
    ~Board();
    void render();

    const std::vector<Move>& GetMoves(int col, int row);
    
private:
    std::array<std::array<Square, 8>, 8> squares;
};