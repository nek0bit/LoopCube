#include "api.hpp"


void Api::sendPlayerPos(const int fd, const int playerX, const int playerY)
{
    uint8_t playerXSize = sizeof(playerX);
    uint8_t playerYSize = sizeof(playerY);
    std::vector<uint8_t> result{};
    std::function<void(uint8_t)> gen = [&result](uint8_t back)->void { result.push_back(back); };

    // Store as int for now
    // I don't really need the precision :)
    
    result.push_back(static_cast<uint8_t>(COMMAND_PLAYER_POS));

    result.push_back(playerXSize);
    Generic::serializeSigned(playerX, playerXSize, gen);

    result.push_back(playerYSize);
    Generic::serializeSigned(playerY, playerYSize, gen);

    uint8_t totalSize = playerXSize + playerYSize + 5; // 2 + 3

    result.insert(result.begin(), totalSize);

    result.push_back(0xff);
    
    send(fd, &result[0], totalSize+1, 0);
}

void Api::sendRecvChunk(const int fd, const long chunkX, const long chunkY)
{
    uint8_t chunkXSize = sizeof(chunkX);
    uint8_t chunkYSize = sizeof(chunkY);
    
    std::vector<uint8_t> result{};
    std::function<void(uint8_t)> gen = [&result](uint8_t back)->void { result.push_back(back); };

    // The command
    result.push_back(static_cast<uint8_t>(COMMAND_GET_CHUNK));

    result.push_back(chunkXSize);
    Generic::serializeSigned(chunkX, chunkXSize, gen);

    result.push_back(chunkYSize);
    Generic::serializeSigned(chunkY, chunkYSize, gen);
    
    // Finally, concat the size of everything
    // 2 = total, 3 for rest of data
    uint8_t totalSize = chunkXSize + chunkYSize + 5; // 2 + 3

    // Put at beginning
    result.insert(result.begin(), totalSize);

    result.push_back(0xff); // End
    
    send(fd, &result[0], totalSize+1, 0);
}

void Api::sendPlaceBlock(const int fd, const uint32_t id,
                         const int64_t chunkX, const int64_t chunkY,
                         const uint16_t x, const uint16_t y)
{
    uint8_t idSize = sizeof(id);
    uint8_t chunkXSize = sizeof(chunkX);
    uint8_t chunkYSize = sizeof(chunkY);
    uint8_t xSize = sizeof(x);
    uint8_t ySize = sizeof(y);
    
    std::vector<uint8_t> result{};
    std::function<void(uint8_t)> gen = [&result](uint8_t back)->void { result.push_back(back); };

    // The command
    result.push_back(static_cast<uint8_t>(COMMAND_PLACE_BLOCK_ABSOLUTE));

    result.push_back(idSize);
    Generic::serializeUnsigned(id, idSize, gen);

    result.push_back(chunkXSize);
    Generic::serializeSigned(chunkX, chunkXSize, gen);

    result.push_back(chunkYSize);
    Generic::serializeSigned(chunkY, chunkYSize, gen);

    result.push_back(xSize);
    Generic::serializeSigned(x, xSize, gen);

    result.push_back(ySize);
    Generic::serializeSigned(y, ySize, gen);
    
    // Finally, append the size of everything
    // 5 = total, 3 for rest of data
    uint8_t totalSize = idSize + chunkXSize + chunkYSize + xSize + ySize + 8; // 5 + 3

    // Size of all data
    result.insert(result.begin(), totalSize);

    result.push_back(0xff); // End
    
    send(fd, &result[0], totalSize+1, 0);
}

void Api::sendDestroyBlock(const int fd,
                           const int64_t chunkX, const int64_t chunkY,
                           const uint16_t x, const uint16_t y)
{
    uint8_t chunkXSize = sizeof(chunkX);
    uint8_t chunkYSize = sizeof(chunkY);
    uint8_t xSize = sizeof(x);
    uint8_t ySize = sizeof(y);
    
    std::vector<uint8_t> result{};
    std::function<void(uint8_t)> gen = [&result](uint8_t back)->void { result.push_back(back); };


    // The command
    result.push_back(static_cast<uint8_t>(COMMAND_DESTROY_BLOCK_ABSOLUTE));

    result.push_back(chunkXSize);
    Generic::serializeSigned(chunkX, chunkXSize, gen);

    result.push_back(chunkYSize);
    Generic::serializeSigned(chunkY, chunkYSize, gen);

    result.push_back(xSize);
    Generic::serializeSigned(x, xSize, gen);

    result.push_back(ySize);
    Generic::serializeSigned(y, ySize, gen);
    
    // Finally, append the size of everything
    // 4 = total, 3 for rest of data
    uint8_t totalSize = chunkXSize + chunkYSize + xSize + ySize + 7;

    // Put at beginning
    result.insert(result.begin(), totalSize);

    result.push_back(0xff); // End
    
    send(fd, &result[0], totalSize+1, 0);
}
