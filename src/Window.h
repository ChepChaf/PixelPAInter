#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

class Window
{
    GLFWwindow *window;

public:
    Window(std::string name, int &width, int &height)
    {
        if (!glfwInit())
        {
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, 1);

        auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        width = width > 0 ? width : mode->width;
        height = height > 0 ? height : mode->height;

        window = glfwCreateWindow(width, height, "Painter", NULL, NULL);

        if (!window)
        {
            return;
        }

        glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
                               if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
                               {
                                   glfwSetWindowShouldClose(window, GLFW_TRUE);
                                } });
    }
    bool ShouldNotClose()
    {
        return !glfwWindowShouldClose(window);
    }
    void PollEvents()
    {
        glfwPollEvents();
    }
    void Update()
    {
        glfwSwapBuffers(window);
    }

    ~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};
