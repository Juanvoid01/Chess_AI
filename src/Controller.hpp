#pragma once

#include "ChessAI.hpp"
#include "Board.hpp"
#include "Perft.hpp"
#include "InformationText.hpp"


class Controller
{
public:
    Controller(const Renderer &renderer);
    ~Controller() {}

    void Update();
    void Render();

private:
    const Renderer &renderer;

    ChessAI chessAI;
    InformationText informationText;
    Board board;
};