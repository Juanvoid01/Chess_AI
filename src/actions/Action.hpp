#pragma once

#include "ActionController.hpp"

enum class ActionType;

// Abstract class Action
class Action
{
public:
    enum class Type;

    Action(Type type) : type(type) {}
    virtual void execute(ActionController& controller) = 0;
    virtual ~Action() {}

    Type GetType() const { return type; }

protected:
    Type type;
};

enum class Action::Type
{
    NONE = 0,
    IA = 1,
    ROTATEBOARD = 2,
    START_GAME = 3
};