#include "api.hpp"


void Api::sendPlayerPos(const int fd, const double x, const double y)
{
    ;
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
    // 2 = total, 1 is command
    uint8_t totalSize = chunkXSize + chunkYSize + 5; // 2 + 1 

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
    // 5 = total, 1 is command
    uint8_t totalSize = idSize + chunkXSize + chunkYSize + xSize + ySize + (sizeof(uint8_t)*6); 

    result.insert(result.begin(), totalSize);
    
    send(fd, &result[0], result.size(), 0);
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
    // 4 = total, 1 is command
    uint8_t totalSize = chunkXSize + chunkYSize + xSize + ySize + (sizeof(uint8_t)*5);

    result.insert(result.begin(), totalSize);
    
    send(fd, &result[0], totalSize, 0);
}
