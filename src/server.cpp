#include "server.hpp"

Server::Server(const std::string address, const uint32_t port, bool doInit)
{
    if (doInit) // Needed for the default server
    {
        if (SDL_Init(0) == -1)
        {
            const char* error = SDL_GetError();
            std::cerr << "[error] Couldn't initialize SDL2: " << error << std::endl;
            throw std::runtime_error(error);
        }

        if (SDLNet_Init() == -1)
        {
            const char* error = SDLNet_GetError();
            std::cerr << "[error] Couldn't initialize SDL2_net: " << error << std::endl;
            throw std::runtime_error(error);
        }
    }
}

Server::~Server()
{

}

void Server::startServer()
{

}
