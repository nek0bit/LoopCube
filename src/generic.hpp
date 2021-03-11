#pragma once

#ifndef __HEADLESS
#include <SDL2/SDL.h>
#include "texturehandler.hpp"

#include "../include/glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "graphics.hpp"
#endif

#ifdef __HEADLESS
struct SDL_Rect
{
    int x;
    int y;
    int w;
    int h;
};
#endif

#include <iostream>
#include <cmath>
#include <cstdint>
#include <functional>
#include <glm/glm.hpp>

struct GridCollision_t
{
    unsigned int width;
    unsigned int height;
};

namespace Generic
{
    template <typename CMP>
    bool collision(CMP x1, CMP y1, CMP w1, CMP h1,
                   CMP x2, CMP y2, CMP w2, CMP h2)
    {
        return (x1 < x2 + w2 &&
                x1 + w1 > x2 &&
                y1 < y2 + h2 &&
                y1 + h1 > y2);
    }

    bool collision(SDL_Rect r1, SDL_Rect r2);

    GridCollision_t gridCollision(const unsigned width, const unsigned height,
                                  glm::vec2 box, const glm::vec2& size);

    void serializeUnsigned(const unsigned value, const unsigned length,
                           std::function<void(uint8_t)> appendData);

    template <typename T>
    T topToBottomFlip(const T& val, const T& height) noexcept
    {
        return std::abs(height) - val;
    }

    template <typename Container, typename Type>
    Type deserializeUnsigned(const Container& value, const size_t begIndex, const uint8_t length)
    {
        constexpr uint8_t BYTE_SIZE = 8;
        Type result = 0;

        for (uint8_t i = 0; i < length; ++i)
        {
            uint8_t val = value.at(i+begIndex);
            result |= val<<(i*BYTE_SIZE);
        }

        return result;
    }

    void serializeSigned(const int value, const unsigned length, std::function<void(uint8_t)> appendData);

    template <typename Container, typename Type>
    Type deserializeSigned(const Container& value, const size_t begIndex, const uint8_t length)
    {
        constexpr uint8_t BYTE_SIZE = 8;
        Type result = 0;

        bool isNegative = (value.at(begIndex + length - 1) & 128) == 128;
        for (uint8_t i = 0; i < length; ++i)
        {
            uint8_t val = value.at(begIndex + i);
            result |= (i+1 == length ? val & 127 : val) << (i * BYTE_SIZE);
        }
        
        return isNegative ? -result : result;
    }

    void serializeDouble(const double val, std::function<void(uint8_t)> appendData);

    double lerp(double v1, double v2, double t);


#ifndef __HEADLESS
    
    namespace Render
    {
        void renderRepeating(const Graphics& graphics,
                             const int texture, const int clipWidth,
                             const int clipHeight, const int offsetX, const int offsetY,
                             const int width, const int height, const int gap, const int top,
                             const bool verticle = false, const int srcX = 0, const int srcY = 0,
                             int srcW = -1, int srcH = -1) noexcept;
        void generateSquare(std::vector<Vertex>& mesh, float, float, float, float,
                            float, float, float, float);
    }
#endif
}
