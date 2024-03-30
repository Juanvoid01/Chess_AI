#include "Controller.hpp"

#include <thread>


Controller::Controller(const Renderer &renderer)
    : renderer(renderer)
{

    informationText = std::make_unique<InformationText>(800.f, 150.f, renderer);
    board = std::make_unique<Board>(0.0f, 0.0f, 550.0f, 550.0f, renderer, chessAI, moveGenerator);
    actionController = std::make_unique<ActionController>(*this);
    actions = std::make_unique<Actions>(*actionController);
    informationText->SetScale(1.3f, 1.3f);
    // board.LoadFEN(FEN_QUEEN_VS_PAWN_ENDGAME);

    board->SetCenter(renderer.GetWindowWidth() / 2.f, renderer.GetWindowHeight() / 2.f);
}

void Controller::Update()
{
    actions->ExecuteActions();
    board->Update();
}
void Controller::Render()
{
    board->Render();
    informationText->Render();
}
void Controller::AddAction(Action *action) { actions->AddAction(action); }

void Controller::MakeMove(const Move &move)
{
    board->MakeMove(move);
}
void Controller::MakeIAmove()
{
    MoveGenerator *movegen = &moveGenerator;
    ChessAI *ai = &chessAI;

    auto startSearchFunc = [ai, movegen]()
    {
        ai->StartSearch(*movegen);
    };

    std::thread searchWorker(startSearchFunc);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    chessAI.AbortSearch();

    searchWorker.join();

    informationText->SetState(chessAI.IsSearching() ? "Search started" : "Search finished");
    informationText->SetDepth("Depth : " + std::to_string(chessAI.depthReached));
    informationText->SetNodes("Nodes : " + std::to_string(chessAI.nodesVisited));

    Move moveAI = chessAI.GetBestMove();

    board->MoveIA(moveAI);
}