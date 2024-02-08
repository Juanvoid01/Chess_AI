#pragma once

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Board.hpp"
#include "Renderer.hpp"

class EventManager
{
public:
    EventManager(GLFWwindow *window,std::shared_ptr<Renderer> renderer, std::shared_ptr<Board> board);
    ~EventManager();

private:
    static void Mouse_click_callback(GLFWwindow *window, int button, int action, int mods);
    static void Key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static std::shared_ptr<Renderer> renderer;
    static std::shared_ptr<Board> board;
};

