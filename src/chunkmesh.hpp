#pragma once
#include <cstdint>
#include <memory>

#include "texture.hpp"
#include "block.hpp"
#include "model.hpp"
#include "constants.hpp"

namespace ChunkMesh
{
    void mutableGenerateChunkMesh(Model& modifyMesh, const std::vector<std::shared_ptr<Block>>& data,
                                  const long chunkX, const long chunkY);
}
