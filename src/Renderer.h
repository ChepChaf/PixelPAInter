#pragma once

#include <glad/glad.h>

class Renderer
{
public:
    bool Init()
    {
        return gladLoadGL() != 0;
    }
    void Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};