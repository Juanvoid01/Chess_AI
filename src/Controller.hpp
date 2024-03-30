#pragma once

#include "ChessAI.hpp"
#include "Board.hpp"
#include "Perft.hpp"
#include "InformationText.hpp"

#include "Actions.hpp"

class Controller
{
public:
    Controller(const Renderer &renderer);
    ~Controller() {}

    void Update();
    void Render();

    void AddAction(Action *action);

    inline void RotateBoard() { board->Rotate(); }

    void MakeMove(const Move &move);
    void MakeIAmove();

    inline Board &GetBoard() { return *board; }

    void Resize(float newWidth, float newHeight);

private:
    const Renderer &renderer;

    MoveGenerator moveGenerator;
    ChessAI chessAI;
    std::unique_ptr<InformationText> informationText;
    std::unique_ptr<Board> board;
    std::unique_ptr<Actions> actions;
    std::unique_ptr<ActionController> actionController;
};