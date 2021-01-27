#pragma once
#include <iostream>

#ifdef __SWITCH__
#include <switch.h>
#endif

#include <SDL2/SDL.h>

#include "game.hpp"
#include "timer.hpp"
#include <thread>
#include <chrono>

int main();
void gameLoop();
