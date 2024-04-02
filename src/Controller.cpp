#include "Controller.hpp"

#include <thread>
#include <iostream>

Controller::Controller(const Renderer &renderer)
    : renderer(renderer)
{
    moveGenerator = std::make_unique<MoveGenerator>();
    chessAI = std::make_unique<ChessAI>(moveGenerator);

    float boardWidth = renderer.GetWindowHeight() * 8.f / 12.f;

    informationText = std::make_unique<InformationText>(800.f, 150.f, renderer);
    board = std::make_unique<Board>(0.0f, 0.0f, boardWidth, boardWidth, renderer, moveGenerator);
    actionController = std::make_unique<ActionController>(*this);
    actions = std::make_unique<Actions>(*actionController);
    informationText->SetScale(1.3f, 1.3f);
    // board->LoadFEN(FEN_QUEEN_VS_PAWN_ENDGAME);

    board->SetCenter(renderer.GetWindowWidth() / 2.f, renderer.GetWindowHeight() / 2.f);
}

void Controller::Update()
{

    if (state == State::NONE)
    {
        actions->ExecuteActions();
        board->Update();
        if (iaMoveRequest)
        {
            state = State::WAITING_AI;
            iaMoveRequest = false;
        }
    }
    else if (state == State::WAITING_AI)
    {
        informationText->SetState(chessAI->IsSearching() ? "Search started" : "Search finished");
        informationText->SetDepth("Depth : " + std::to_string(chessAI->depthReached));
        informationText->SetNodes("Nodes : " + std::to_string(chessAI->nodesVisited));

        if (infiniteSearch)
        {
            if (!chessAI->IsSearching())
            {
                state = State::EXECUTING_IA_MOVE;
            }
        }
        else
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
            if (elapsed >= thinkingAItime || !chessAI->IsSearching())
            {
                chessAI->AbortSearch();
                state = State::EXECUTING_IA_MOVE;
            }
        }
    }
    else if (state == State::EXECUTING_IA_MOVE)
    {
        if (!chessAI->IsSearching())
        {
            Move moveAI = chessAI->GetBestMove();

            board->MoveIA(moveAI);

            state = State::NONE;
        }
        else
        {
            actions->ExecuteActions();
            board->Update();
        }
    }
    else
    {
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
    if (state != State::NONE || !board->ReadyToMove())
        return;

    ChessAI *ai = chessAI.get();
    int max_depth = max_depth_search;

    auto startSearchFunc = [ai, max_depth]()
    {
        ai->StartSearch(max_depth);
    };

    std::thread searchWorker(startSearchFunc);
    searchWorker.detach();
    iaMoveRequest = true;
    if (!infiniteSearch)
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