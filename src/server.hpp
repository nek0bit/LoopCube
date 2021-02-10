#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <exception>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

struct Server
{
    Server(const std::string address, const uint32_t port, bool doInit = true);
    ~Server();

    void startServer(); // Note: Blocking
};
