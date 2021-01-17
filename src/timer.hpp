#pragma once

#include <SDL2/SDL.h>

struct DeltaTime
{
    float ms;
    float s;
};

struct Timer
{
    Timer(int FPS);
    ~Timer() = default;

    void setFPS(int FPS);
    void setTime();
    int calcSleep();

    float FPS;
    DeltaTime deltaTime;
    Uint64 lastFrame;
    Uint64 currFrame;
};
