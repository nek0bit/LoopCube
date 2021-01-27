#include "chunkgroup.hpp"

//***************************************
// _ChunkDataSplit: For vertical chunks
//***************************************
_ChunkDataSplit::_ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      x{x}
{}

// Generates a chunk if it isn't loaded
std::unordered_map<long int, ChunkData>::iterator _ChunkDataSplit::checkGenerate(long int y)
{
    auto ind = data.find(y);
    // Check if element doesn't exists
    if (ind == data.end())
    {
        // Generate the chunk
        data.insert({y, ChunkData{true, std::make_shared<Chunk>(Chunk{x, y})}});
        return data.find(y);
    }
    return ind;
}

// Clears and reupdates the loadedChunks vector
void _ChunkDataSplit::updateLoaded()
{
    prepareLoaded();

    for (size_t i = 0; i < loadedChunks.size(); ++i)
    {
        const int chunkPos = loadPtr.y + i;
        std::shared_ptr<Chunk> dataReceived = getData(chunkPos);
        
        // Generate if nullptr (might want to remove this later)
        if (dataReceived == nullptr)
        {            
            dataReceived = checkGenerate(chunkPos)->second.data;
        }
        
        loadedChunks[i] = dataReceived;
    }
}

// Update all loaded chunks in the split
void _ChunkDataSplit::updateSplit(Camera& camera)
{
    for (auto& chunk: loadedChunks)
    {
        chunk->updateAll(camera);
    }
}

// Renders all loaded chunks in the split if visible
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

// Clears and resizes loadedChunks
void _ChunkDataSplit::prepareLoaded()
{
    loadedChunks.clear();
    loadedChunks.resize(loadDistance.y);
}

//***************************************************
// _ChunkData: For horizontal vertical chunks
//***************************************************

_ChunkData::_ChunkData(LoadPtr& loadPtr, LoadDistance& loadDistance)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      loadedSplits{},
      data{}
{}

