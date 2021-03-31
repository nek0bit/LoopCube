#pragma once
#include "../include/glad/glad.h"
#include <SDL2/SDL_ttf.h>

namespace UI
{
    namespace _ImmediateMode
    {
        TTF_Font* _FONT;
        GLuint _SHADER;
    }

    inline void setFont(TTF_Font* font)
    {
        UI::_ImmediateMode::_FONT = font;
    }

    inline void setShader(const GLuint shader)
    {
        UI::_ImmediateMode::_SHADER = shader;
    }
}
