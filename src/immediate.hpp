#pragma once
#include "../include/glad/glad.h"
#include <SDL2/SDL_ttf.h>

namespace UI
{
    namespace _ImmediateMode
    {
        extern TTF_Font* _FONT;
        extern GLuint _SHADER;
    }

    inline void setFont(TTF_Font* font = nullptr)
    {
        UI::_ImmediateMode::_FONT = font;
    }

    inline void setShader(const GLuint shader = 0)
    {
        UI::_ImmediateMode::_SHADER = shader;
    }
}
