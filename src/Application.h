#pragma once

#include "Renderer.h"
#include "Window.h"

#include <string>

class Application
{
    Window *window;
    Renderer *renderer;

public:
    int Init(std::string appName, int width, int height)
    {
        window = new Window(appName, width, height);

        if (window == nullptr)
        {
            return -1;
        }

        renderer = new Renderer();

        if (!renderer->Init())
        {
            return -1;
        }

        return 0;
    }
    bool Running()
    {
        return window->ShouldNotClose();
    }
    void Update()
    {
        window->PollEvents();

        renderer->Clear(0.0f, 0.0f, 0.0f, 1.0f);

        window->Update();
    }
    ~Application()
    {
    }
};