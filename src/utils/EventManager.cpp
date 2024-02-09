#include "EventManager.hpp"

std::shared_ptr<Board> EventManager::board = nullptr;
std::shared_ptr<Renderer> EventManager::renderer = nullptr;

EventManager::EventManager(GLFWwindow *window, std::shared_ptr<Renderer> renderer, std::shared_ptr<Board> board)
{
    EventManager::renderer = renderer;
    EventManager::board = board;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetMouseButtonCallback(window, Mouse_click_callback);

    glfwSetKeyCallback(window, Key_pressed_callback);
}

EventManager::~EventManager()
{
}

void EventManager::Mouse_click_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = renderer->GetWindowHeight() - ypos;

        board->ClickEvent(xpos, ypos);
    }
}

void EventManager::Key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    float translationX = 0.0f;
    float translationY = 0.0f;
    bool translation = false;

    if (key == GLFW_KEY_A)
    {
        translationX = -10.f;
        translation = true;
    }
    if (key == GLFW_KEY_D)
    {
        translationX = 10.f;
        translation = true;
    }
    if (key == GLFW_KEY_W)
    {
        translationY = 10.f;
        translation = true;
    }
    if (key == GLFW_KEY_S)
    {
        translationY = -10.f;
        translation = true;
    }
    if (translation)
    {
        board->Translate(translationX, translationY);
    }
}

void EventManager::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    renderer->SetWindowWidth(width);
    renderer->SetWindowWidth(height);
}
