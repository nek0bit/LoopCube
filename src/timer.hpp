#pragma once

#include <chrono>
#include <iostream>
#include <cmath>

struct Timer
{
    Timer(const int FPS);
    ~Timer() = default;

    void setFPS(const int FPS) noexcept;
    void setTime();
    inline int calcSleep() const noexcept
        {
            return std::floor(FPS - deltaTime);
        }

    void calcFPS() noexcept;

    double fps;
    
    double deltaTime;
    std::chrono::high_resolution_clock::duration lastFrame;
    std::chrono::high_resolution_clock::duration currFrame;
private:
    double FPS;
};
