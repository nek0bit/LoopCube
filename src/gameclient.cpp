#include "gameclient.hpp"

GameClient::GameClient(std::string address, uint16_t port, Timer& timer, WinSize& winSize)
    : winSize{winSize},
      serverChunks{nullptr},
      camera{&winSize},
      time{8600, 28500, 8600, 22000, 1700, 1700},
      timer{timer}
{
    camera.x = 0;
    camera.y = 0;
}

GameClient::~GameClient()
{}

void GameClient::update(EventWrapper& events)
{}

void GameClient::render(SDL_Renderer* renderer, TextureHandler& textures)
{}
