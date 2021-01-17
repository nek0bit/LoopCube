#include "timer.hpp"

Timer::Timer(int FPS)
    : FPS{0},
      deltaTime{0, 0},
      lastFrame{0},
      currFrame{0}
{
    setFPS(FPS);
}

void Timer::setFPS(int FPS)
{
    this->FPS = 1000 / FPS;
}

void Timer::setTime()
{
    lastFrame = currFrame;
    currFrame = SDL_GetPerformanceCounter();
    
    deltaTime.ms = (double)((currFrame - lastFrame)*1000
                           / (double)SDL_GetPerformanceFrequency());
    deltaTime.s = deltaTime.ms / 1000;
}

int Timer::calcSleep()
{
    return floor(FPS - deltaTime.s);
}
