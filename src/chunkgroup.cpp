#include "chunkgroup.hpp"

//***************************************
// _ChunkDataSplit: For vertical chunks
//***************************************
_ChunkDataSplit::_ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      x{x}
{}

// True = Chunk isn't loaded/Generate it
// False = Chunk Loaded/No need to generate
bool _ChunkDataSplit::checkGenerate(long int y)
{
    auto ind = data.find(y);
    // Check if element exists
    if (ind == data.end())
    {
        // Generate the chunk
        ind->second.data = std::make_shared<Chunk>(Chunk{x, y});
        ind->second.generated = true;
        return true;
    }
    return false;
}

void _ChunkDataSplit::updateLoaded()
{
    prepareLoaded();

    for (size_t i = 0; i < loadedChunks.size(); ++i)
    {
        std::shared_ptr<Chunk> dataReceived = getData(loadPtr.y + i);
        loadedChunks[i] = dataReceived;
    }
}

void _ChunkDataSplit::updateSplit(Camera& camera)
{
    for (auto& chunk: loadedChunks)
    {
        chunk->updateAll(camera);
    }
}

void _ChunkDataSplit::renderSplit(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera)
{
    for (auto& chunk: loadedChunks)
    {
        chunk->renderAllShadows(renderer, camera);
        chunk->renderAllBlocks(renderer, textures, camera);
    }
}

std::shared_ptr<Chunk> _ChunkDataSplit::getData(long int y)
{
    try { return data.at(y).data; }
    catch (std::out_of_range& err) { return nullptr; }
}

void _ChunkDataSplit::prepareLoaded()
{
    loadedChunks.clear();
    loadedChunks.resize(loadDistance.y);
}
