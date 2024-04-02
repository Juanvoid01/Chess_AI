#pragma once

#include "ChessAI.hpp"
#include "Board.hpp"
#include "Perft.hpp"
#include "InformationText.hpp"

#include "Actions.hpp"

#include <chrono>
#include <thread>

class Controller
{
public:
    enum class State
    {
        NONE,
        EXECUTING_IA_MOVE,
        WAITING_AI
    };

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

    inline State GetState() const { return state; }

private:
    State state = State::NONE;

    const Renderer &renderer;

    std::shared_ptr<MoveGenerator> moveGenerator;
    std::unique_ptr<ChessAI> chessAI;
    std::unique_ptr<InformationText> informationText;
    std::unique_ptr<Board> board;
    std::unique_ptr<Actions> actions;
    std::unique_ptr<ActionController> actionController;

    std::chrono::time_point<std::chrono::steady_clock> startTime;

    bool iaMoveRequest = false;
    int max_depth_search = 100;
    bool infiniteSearch = false;
    int thinkingAItime = 1000; // in miliseconds
};