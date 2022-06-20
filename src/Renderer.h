#pragma once

#include <glad/glad.h>

#include <iostream>

#include "Texture.h"

class Renderer
{
    unsigned int screenTexture;
    unsigned int width, height;
    bool shouldUpdate = false;

    unsigned int fbo, vao;

    const float screenVertices[24]{
        //   positions     texture coordinates
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    const char *vertexShader = R"VERTEX(
        #version 450 core
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 texCoords;

        out vec2 TexCoords;

        void main()
        {
            gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
            TexCoords = texCoords;
        } 
    )VERTEX";

    const char *fragmentShader = R"FRAGMENT(
        #version 450 core
        in vec2 TexCoords;
        out vec4 color;

        uniform sampler2D screenTexture;

        void main()
        { 
            color = texture(screenTexture, TexCoords);
        }
    )FRAGMENT";

    void initTexture()
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &screenTexture);
        glTextureParameteri(screenTexture, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(screenTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(screenTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(screenTexture, 1, GL_RGB8, width, height);
        glBindTexture(GL_TEXTURE_2D, screenTexture);

        DrawTexture(*Texture::BlackTexture(width, height));
    }
    void initFBO()
    {
        glCreateFramebuffers(1, &fbo);
        glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, screenTexture, 0);

        auto status = glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER_NOT_COMPLETE\n";

            exit(-1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
    void initBuffers()
    {
        unsigned int vbo;
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, sizeof(float) * 4 * 24, screenVertices, 0);

        glCreateVertexArrays(1, &vao);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 4);

        glEnableVertexArrayAttrib(vao, 0);
        glEnableVertexArrayAttrib(vao, 1);

        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);

        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribBinding(vao, 1, 0);
    }
    unsigned int compileShader(unsigned int type, const char *source)
    {
        unsigned int id;
        id = glCreateShader(type);

        glShaderSource(id, 1, &source, 0);
        glCompileShader(id);

        int success;
        char infoLog[8192];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            auto typeStr = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
            glGetShaderInfoLog(id, 8192, 0, infoLog);
            std::cout << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n"
                      << infoLog << std::endl;

            exit(-1);
        }

        return id;
    }
    unsigned int linkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID)
    {
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShaderID);
        glAttachShader(shaderProgram, fragmentShaderID);
        glLinkProgram(shaderProgram);

        int success;
        char infoLog[8192];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 8192, 0, infoLog);
            std::cout << "ERROR::SHADER::LINKING_FAILED\n"
                      << infoLog << std::endl;

            exit(-1);
        }

        return shaderProgram;
    }

    void createShaderProgram()
    {
        auto vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShader);
        auto fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        auto shaderProgram = linkProgram(vertexShaderID, fragmentShaderID);

        glUseProgram(shaderProgram);
    }

public:
    bool Init(int width, int height)
    {
        if (!gladLoadGL())
        {
            return false;
        }

        this->width = width;
        this->height = height;

        this->initBuffers();
        this->initTexture();
        this->initFBO();
        this->createShaderProgram();

        shouldUpdate = true;

        return true;
    }
    void Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClearNamedFramebufferfi(fbo, GL_COLOR, 0, 0, 0);

        shouldUpdate = true;
    }
    void DrawTexture(const Texture &texture)
    {
        glTextureSubImage2D(screenTexture, 0, 0, 0, this->width, this->height,
                            GL_RGB, GL_UNSIGNED_BYTE, texture.pixels);

        shouldUpdate = true;
    }
    void Update()
    {
        if (shouldUpdate)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 2);

            shouldUpdate = false;
        }
    }
};