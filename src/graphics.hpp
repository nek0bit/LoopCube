#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

struct Graphics
{
    Graphics(SDL_Window* window);
    ~Graphics();

    void init();
    void createBuffers();
    void bindBuffer() const;
    void loadShaders(const std::string& vertShaderFilename,
                     const std::string& fragShaderFilename);
    void useShader() const;

    SDL_Window* window;
    SDL_GLContext context;
    GLuint shader;
    GLuint vbo;
};
