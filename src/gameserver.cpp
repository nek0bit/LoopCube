#include "gameserver.hpp"

GameServer::GameServer()
    : gen{std::make_shared<ChunkGen>(ChunkGen{})},
      chunks{gen}
{}

GameServer::~GameServer()
{

}

void GameServer::update(Timer&)
{
    
}

std::vector<unsigned char> GameServer::checkChunkAt(const long int x, const long int y)
{
    std::lock_guard<std::mutex> lg(chunkLock);
    
    std::shared_ptr<Chunk> cget;
    
    std::vector<unsigned char> get_s;
    
    if ((cget = chunks.getChunkAt(x, y)) == nullptr)
    {
        chunks.generateChunkAt(x, y);
        cget = chunks.getChunkAt(x, y);
    }
    if (cget)
    {
        // Call a function that stringifies the chunk
        get_s = cget->serialize();
    }    
    
    return get_s;
}

void GameServer::modifyBlock(const int currFd, const std::vector<ServerThreadItem>& threads,
                             const uint32_t id,
                             int64_t chunkX, int64_t chunkY,
                             uint64_t x, uint64_t y,
                             bool createOrDestroy)
{
    std::lock_guard<std::mutex> lg(chunkLock);

    std::shared_ptr<Chunk> cget;

    command_actions action = ACTION_ZERO;

    if ((cget = chunks.getChunkAt(chunkX, chunkY)) != nullptr)
    {
        if (createOrDestroy == false) // Create
        {
            cget->placeBlock(id, x, y);
            action = ACTION_PLACE_BLOCK;
        }
        else // Destroy
        {
            cget->destroyBlock(x, y);
            action = ACTION_DESTROY_BLOCK;
        }
    }

    // Send the action to each player
    if (action != ACTION_ZERO)
    {
        std::vector<unsigned char> res;

        const uint16_t fullSize = sizeof(chunkX)+sizeof(chunkY)+8;

        // We don't need to push the size, because data can only be less than 65535
        // and there is no way anyone on earth should send that much data...
        Generic::serializeSigned(fullSize, 2, [&](uint8_t back)
            { res.push_back(back); });
        
        res.push_back(static_cast<unsigned char>(action) & 0xff);

        // Insert chunk X
        res.push_back(sizeof(chunkX));
        Generic::serializeSigned(chunkX, sizeof(chunkX), [&res](uint8_t back)->void {
            res.push_back(back);
        });

        // Insert chunk Y
        res.push_back(sizeof(chunkY));
        Generic::serializeSigned(chunkY, sizeof(chunkY), [&res](uint8_t back)->void {
            res.push_back(back);
        });

        // Send 2 bytes of chunk coordinate
        res.push_back(x & 0xff);
        res.push_back(y & 0xff);


        res.push_back(0xff);

        // Send the action to every connected player
        for (const ServerThreadItem& item: threads)
        {
            for (const pollfd& pfd: item.connections)
            {
                // Send to everyone excluding sender
                if (pfd.fd != currFd) send(pfd.fd, &res[0], fullSize, 0);
            }
        }
    }
}
