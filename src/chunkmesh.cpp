#include "chunkmesh.hpp"

void ChunkMesh::mutableGenerateChunkMesh(Model& modifyMesh, const std::vector<std::shared_ptr<Block>>& data,
                                         const long chunkX, const long chunkY)
{
    constexpr uint16_t BLOCK_W = constants::blockW;
    constexpr uint16_t BLOCK_H = constants::blockH;
    constexpr uint16_t CHUNK_W = constants::chunkWidth;
    constexpr uint16_t CHUNK_H = constants::chunkHeight;

    // The vertices for the chunkMesh
    std::vector<Vertex> mesh;

    // Loop through each block
    for (auto& block: data)
    {
        if (block != nullptr)
        {
            const TextureInfo tInfo = constants::textureInfo[block->textureId];
            const texcoord_t tCoord = Texture::getTilemapCoord({
                    static_cast<unsigned>(tInfo.sizeX),
                    static_cast<unsigned>(tInfo.sizeY),
                    static_cast<unsigned>(tInfo.tilemapX),
                    static_cast<unsigned>(tInfo.tilemapY)
                }, block->typeX, 0);
            const float begX = block->position.x - (chunkX * CHUNK_W * BLOCK_W);
            const float begY = block->position.y - (chunkY * CHUNK_H * BLOCK_H);
            const float endX = begX + BLOCK_W,
                endY = begY + BLOCK_H;

            mesh.emplace_back(glm::vec3(begX, endY, 0.0f), glm::vec2(tCoord.begX, tCoord.begY));
            mesh.emplace_back(glm::vec3(endX, endY, 0.0f), glm::vec2(tCoord.endX, tCoord.begY));
            mesh.emplace_back(glm::vec3(endX, begY, 0.0f), glm::vec2(tCoord.endX, tCoord.endY));
            
            mesh.emplace_back(glm::vec3(endX, begY, 0.0f), glm::vec2(tCoord.endX, tCoord.endY));
            mesh.emplace_back(glm::vec3(begX, begY, 0.0f), glm::vec2(tCoord.begX, tCoord.endY));
            mesh.emplace_back(glm::vec3(begX, endY, 0.0f), glm::vec2(tCoord.begX, tCoord.begY));
        }
    }

    modifyMesh.setBufferData(mesh);
}
