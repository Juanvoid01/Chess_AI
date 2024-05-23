#pragma once
#include "Action.hpp"

class StartGameAction : public Action
{
public:
    StartGameAction(const std::string &&FEN)
        : Action(Type::START_GAME), FEN(FEN)
    {
    }
    void execute(ActionController& controller) override
    {
        controller.StartGame(FEN);
    }
    const std::string FEN;
};