#pragma once

#include "Square.hpp"
#include "Move.hpp"
#include "Object.hpp"
#include <vector>

class Board : public Object
{
public:
    Board(float posX, float posY, float width, float height, const Renderer& r);
    ~Board();
    void Render(const Renderer &renderer);
    const std::vector<Move> &GetMoves(int col, int row);

private:
    /*float positions[16] = {
    100.0f, 100.0f, 0.0f, 0.0f,
    200.0f, 100.0f, 1.0f, 0.0f,
    200.0f, 200.0f, 1.0f, 1.0f,
    100.0f, 200.0f, 0.0f, 1.0f};*/
    std::array<std::array<Square, 8>, 8> squares;
};