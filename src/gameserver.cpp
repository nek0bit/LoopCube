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

std::vector<unsigned char> GameServer::checkChunkAt(const long int x, const long int y)
{
    chunkLock.lock();
    std::shared_ptr<Chunk> cget;
    std::vector<unsigned char> get_s;
    if ((cget = chunks.getChunkAt(x, y)) == nullptr)
    {
        chunks.generateChunkAt(x, y);
        cget = chunks.getChunkAt(x, y);
        std::cout << "Chunk at " << x << " and " << y << " generated..." << std::endl;
    }
    if (cget)
    {
        // Call a function that stringifies the chunk
        get_s = cget->serialize();
    }
    chunkLock.unlock();

    std::cout << "Deserialize!" << std::endl;
    cget->deserialize(get_s, false);

    return get_s;
}
