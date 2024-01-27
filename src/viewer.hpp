#pragma once

#include "controller.hpp"

class Viewer
{
public:
    Viewer(const Controller &c) : controller(c){};
    void run();

private:
    const Controller &controller;
    void render();
    void showBoard();
};
