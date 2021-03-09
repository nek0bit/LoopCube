#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

#include "../include/glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "texturehandler.hpp"
#include "camera.hpp"
#include "winsize.hpp"
#include "modelgroup.hpp"

struct Uniforms
{
    GLint proj;
    GLint view;
    GLint model;
    GLint tex;
    GLint zoom;
};

struct Graphics
{
    Graphics(SDL_Window* window, WinSize& winSize);
    ~Graphics();

    void init();
    // Prefer the first one commonly
    void loadShaders(const std::string& vertShaderFilename,
                     const std::string& fragShaderFilename);
    void postShader();
    void setupUniforms();
    
    inline void useShader() const
        {
            glUseProgram(shader);
        }
    
    inline GLint getUniformLocation(const char* val) const noexcept
        {
            return glGetUniformLocation(shader, val);
        }
    
    SDL_Window* window;
    SDL_GLContext context;
    Uniforms uniforms;
    GLuint shader;
    

    // Other components
    ModelGroup models;
    Camera camera;
    TextureHandler textures;
};
