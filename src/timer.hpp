#pragma once

#include <chrono>
#include <iostream>
#include <cmath>

struct Timer
{
    Timer(int FPS);
    ~Timer() = default;

    void setFPS(int FPS);
    void setTime();
    int calcSleep();

    double FPS;
    double deltaTime;
    std::chrono::high_resolution_clock::duration lastFrame;
    std::chrono::high_resolution_clock::duration currFrame;
};
