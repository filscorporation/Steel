#pragma once

#include "AsepriteData.h"
#include "../Rendering/Sprite.h"
#include "../Resources/FileLoader.h"

class AsepriteLoader : public FileLoader
{
public:
    static bool LoadAsepriteData(const char* filePath, bool loopAll, AsepriteData& outData);

private:
    static Sprite* ReadCelChunk(std::ifstream& file, uint32_t& chunkSizeLeft, uint32_t width, uint32_t height,
                                const std::string& dataPath, int imageIndex);
    static bool ReadTagsChunk(std::ifstream& file, std::vector<Sprite*>& inSprites, std::vector<uint32_t>& inDurations,
                              bool loopAll, std::vector<Animation*>& outAnimations, const std::string& dataPath);
};
