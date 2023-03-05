#pragma once

#include "AsepriteData.h"
#include "FileData.h"
#include "FileDataReader.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/Resources/FileLoader.h"

class AsepriteLoader : public FileLoader
{
public:
    static AsepriteData* LoadAsepriteData(const FileData& fileData);

private:
    static Sprite* ReadCelChunk(FileDataReader& reader, uint32_t& chunkSizeLeft, uint32_t width, uint32_t height);
    static bool ReadTagsChunk(FileDataReader& reader, std::vector<Sprite*>& inSprites, std::vector<uint32_t>& inDurations,
                              std::vector<Animation*>& outAnimations);
};
