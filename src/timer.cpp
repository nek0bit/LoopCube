#include "timer.hpp"

Timer::Timer(const int FPS)
    : fps{0},
      deltaTime{},
      lastFrame{},
      currFrame{},
      FPS{0}
{
    setFPS(FPS);
    lastFrame = std::chrono::high_resolution_clock::now().time_since_epoch();
}

void Timer::setFPS(const int FPS) noexcept
{
    this->FPS = 1000 / FPS;
}

void Timer::calcFPS() noexcept
{
    fps = 1.0f / deltaTime;
}

void Timer::setTime()
{
    currFrame = std::chrono::high_resolution_clock::now().time_since_epoch();
    
    std::chrono::duration<double> deltaTimeTmp = currFrame - lastFrame;
    
    deltaTime = deltaTimeTmp.count();
        
    lastFrame = currFrame;
}
