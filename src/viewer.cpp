#include <iostream>
#include "viewer.hpp"

Viewer::Viewer()
{
}
void DrawText(GLTtext *gltText, const glm::mat4 &mvp);

void Viewer::Run()
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

    float windowWidth = 1366.f, windowHeight = 1000.f;
    window = glfwCreateWindow(1366, 1000, "Chess AI", NULL, NULL);
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

    if (!gltInit())
    {
        std::cout << "Failed to initialize glText" << std::endl;
        glfwTerminate();
        result = -1;
    }

    {

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // glfwGetWindowSize(window, &windowWidth, &windowHeight);

        renderer = std::make_unique<Renderer>(windowWidth, windowHeight, &DrawText);

        controller = std::make_unique<Controller>(*renderer);

        eventManager = std::make_unique<EventManager>(window, renderer.get(), controller.get());

        // Loop until the user closes the window

        while (!glfwWindowShouldClose(window))
        {

            controller->Update();

            renderer->Clear();

            controller->Render();

            glfwSwapBuffers(glfwGetCurrentContext());

            // Poll for and process events
            glfwPollEvents();
        }
    }
    // Cleanup
    gltTerminate();
    glfwTerminate();
}

void DrawText(GLTtext *gltText, const glm::mat4 &mvp)
{
    // GLTtext *t = gltCreateText();
    // gltSetText(t, text.GetText().c_str());

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Begin text drawing (this for instance calls glUseProgram)
    gltBeginDraw();

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);

    // auto mvp = glm::scale(text.GetMVP(), glm::vec3(1.0f, -1.0f, 1.0f));

    gltDrawText(gltText, (GLfloat *)&mvp);

    // Finish drawing text
    gltEndDraw();

    // Deleting text
    // gltDeleteText(t);
}
