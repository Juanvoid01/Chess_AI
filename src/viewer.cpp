#include <iostream>

#include "viewer.hpp"
#include "Board.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Viewer::run()
{
    int result = 0;
    GLFWwindow *window;

    if (!glfwInit())
    {
        result = -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Chess AI", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        result = -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "Error! glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer(windowWidth, windowHeight);
        Board board(0.0f, 0.0f, 200.0f, 200.0f, renderer);
        board.PutInitialPosition();
        board.SetScale(2.0f, 2.0f);
        board.Translate(50.0f, 50.0f);
        // Loop until the user closes the window

        double mouseX, mouseY;
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            board.Render(renderer);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();

            glfwGetCursorPos(window, &mouseX, &mouseY);
            mouseY = windowHeight - mouseY;
            board.SetCenter(mouseX,mouseY);
            std::cout << "MouseX: " << mouseX << "  "
                      << "MouseY: " << mouseY << '\n';
        }
    }
    // Cleanup
    glfwTerminate();
}

void Viewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Your drawing functions go here

    glfwSwapBuffers(glfwGetCurrentContext());
}
