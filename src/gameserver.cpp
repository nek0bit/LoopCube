#include "gameserver.hpp"

GameServer::GameServer()
    : gen{std::make_shared<ChunkGen>(ChunkGen{})},
      chunks{gen}
{}

GameServer::~GameServer()
{

}

void GameServer::update(Timer& timer)
{
    
}

std::string GameServer::getChunkAt(const long int x, const long int y)
{
    chunkLock.lock();
    std::shared_ptr<Chunk> cget;
    std::string sget("");
    if ((cget = chunks.getChunkAt(x, y)) == nullptr)
    {
        chunks.generateChunkAt(x, y);
        cget = chunks.getChunkAt(x, y);
        std::cout << "Chunk at " << x << " and " << y << " generated..." << std::endl;
    }
    if (cget)
    {
        // Call a function that stringifies the chunk
        sget = "Some chunk data";
    }
    chunkLock.unlock();

    return sget;
}
