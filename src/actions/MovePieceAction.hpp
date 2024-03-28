#pragma once
#include "Action.hpp"

#include "Move.hpp"

class MovePieceAction : public Action
{
public:
    MovePieceAction(Move move)
                : Action(Type::MOVEPIECE), move(move)
    {
    }
    void execute(ActionController& controller) override
    {
    }

private:
    Move move;
};