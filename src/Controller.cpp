#include "Controller.hpp"

#include <thread>

Controller::Controller(const Renderer &renderer)
    : renderer(renderer)
{
    float boardWidth = renderer.GetWindowHeight() * 8.f / 12.f;

    informationText = std::make_unique<InformationText>(800.f, 150.f, renderer);
    board = std::make_unique<Board>(0.0f, 0.0f, boardWidth, boardWidth, renderer, chessAI, moveGenerator);
    actionController = std::make_unique<ActionController>(*this);
    actions = std::make_unique<Actions>(*actionController);
    // informationText->SetScale(1.3f, 1.3f);
    //  board.LoadFEN(FEN_QUEEN_VS_PAWN_ENDGAME);

    board->SetCenter(renderer.GetWindowWidth() / 2.f, renderer.GetWindowHeight() / 2.f);
}

void Controller::Update()
{

    actions->ExecuteActions();

    if (waitingIA)
    {
        informationText->SetState(chessAI.IsSearching() ? "Search started" : "Search finished");
        informationText->SetDepth("Depth : " + std::to_string(chessAI.depthReached));
        informationText->SetNodes("Nodes : " + std::to_string(chessAI.nodesVisited));

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        if (elapsed >= thinkingAItime)
        {
            waitingIA = false;
            chessAI.AbortSearch();
            Move moveAI = chessAI.GetBestMove();

            board->MoveIA(moveAI);
        }
    }
    else
    {
        board->Update();
    }
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
    searchWorker.detach();
    waitingIA = true;
    startTime = std::chrono::steady_clock::now();
}

void Controller::Resize(float newWidth, float newHeight)
{
    float newScale = newHeight / renderer.GetOriginalWindowHeight();
    // board->ReCalculateMVP();
    // informationText->ReCalculateMVP();
    board->SetScale(newScale, newScale);
    board->SetCenter(newWidth / 2.f, newHeight / 2.f);

    informationText->SetScale(newScale, newScale);
}