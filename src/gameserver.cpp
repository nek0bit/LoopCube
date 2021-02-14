#include "gameserver.hpp"

GameServer::GameServer()
{

}

GameServer::~GameServer()
{

}

void GameServer::update(Timer& timer)
{
    std::cout << timer.deltaTime << std::endl;
}
