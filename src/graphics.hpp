#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

struct Graphics
{
    Graphics(SDL_Window* window);
    ~Graphics();

    void init();
    void createVAO();
    // Prefer the first one commonly
    void bindVAO() const;
    void loadShaders(const std::string& vertShaderFilename,
                     const std::string& fragShaderFilename);
    void useShader() const;
    void setupVertexLayout();

    SDL_Window* window;
    SDL_GLContext context;
    GLuint shader;
    GLuint vao;
};
