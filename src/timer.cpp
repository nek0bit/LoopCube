#include "timer.hpp"

Timer::Timer(int FPS)
    : FPS{0},
      deltaTime{},
      lastFrame{},
      currFrame{}
{
    setFPS(FPS);
    lastFrame = std::chrono::high_resolution_clock::now().time_since_epoch();
}

void Timer::setFPS(int FPS)
{
    this->FPS = 1000 / FPS;
}

void Timer::setTime()
{
    currFrame = std::chrono::high_resolution_clock::now().time_since_epoch();
    
    std::chrono::duration<double> deltaTimeTmp = currFrame - lastFrame;
    
    deltaTime = deltaTimeTmp.count();
        
    lastFrame = currFrame;
}

int Timer::calcSleep()
{
    return floor(FPS - deltaTime);
}
