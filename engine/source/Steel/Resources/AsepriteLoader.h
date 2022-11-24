#pragma once

#include "AsepriteData.h"
#include "../Rendering/Sprite.h"
#include "../Resources/FileLoader.h"

class AsepriteLoader : public FileLoader
{
public:
    static AsepriteData* LoadAsepriteData(const char* filePath);

private:
    static Sprite* ReadCelChunk(std::ifstream& file, uint32_t& chunkSizeLeft, uint32_t width, uint32_t height);
    static bool ReadTagsChunk(std::ifstream& file, std::vector<Sprite*>& inSprites, std::vector<uint32_t>& inDurations,
                              std::vector<Animation*>& outAnimations);
};
