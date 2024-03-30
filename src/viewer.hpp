#pragma once

#include "Controller.hpp"
#include "EventManager.hpp"

class Viewer
{
public:
    Viewer();
    ~Viewer() {}
    void Run();

private:

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Controller> controller;
    std::unique_ptr<EventManager> eventManager;

};
