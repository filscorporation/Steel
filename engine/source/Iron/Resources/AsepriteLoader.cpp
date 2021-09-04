#include <fstream>
#include <zlib.h>

#include "AsepriteLoader.h"
#include "ResourcesManager.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Rendering/OpenGLAPI.h"

int Decompress(const void* src, uint32_t srcLen, void* dst, uint32_t dstLen)
{
    z_stream strm  = { nullptr };
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    int err, ret;

    err = inflateInit2(&strm, (15 + 32));
    if (err == Z_OK)
    {
        err = inflate(&strm, Z_FINISH);
        if (err == Z_STREAM_END)
        {
            ret = strm.total_out;
        }
        else
        {
            inflateEnd(&strm);
            return err;
        }
    }
    else
    {
        inflateEnd(&strm);
        return err;
    }

    inflateEnd(&strm);
    return ret;
}

void Fill(unsigned char* target, uint32_t targetWidth, uint32_t targetHeight,
          const unsigned char* source, uint32_t sourceWidth, uint32_t sourceHeight, uint32_t sourceX, uint32_t sourceY)
{
    for (uint32_t i = 0; i < sourceWidth; ++i)
    {
        for (uint32_t j = 0; j < sourceHeight; ++j)
        {
            if (i + sourceX >= 0 && i + sourceX < targetWidth && j + sourceY >=0 && j + sourceY < targetHeight)
            {
                for (int k = 0; k < 4; ++k)
                {
                    target[((j + sourceY) * targetWidth + (i + sourceX)) * 4 + k] = source[(j * sourceWidth + i) * 4 + k];
                }
            }
        }
    }
}

void GetCelData(std::ifstream& file, uint32_t celType, char* imageData, uint32_t celWidth, uint32_t celHeight, uint32_t dataSize)
{
    switch (celType)
    {
        case 0: // Raw Cell
        {
            if (!file.read(imageData, celWidth * celHeight * 4))
            {
                Log::LogError("Could not read raw image data");
                return;
            }
            break;
        }
        case 2: // Compressed Image
        {
            auto compressedData = new char[dataSize];
            if (!file.read(compressedData, dataSize))
            {
                Log::LogError("Could not read compressed image data");
                delete[] compressedData;
                return;
            }

            int err = Decompress(compressedData, dataSize, imageData, celWidth * celHeight * 4);
            delete[] compressedData;
            if (err < 0)
            {
                Log::LogError("Error decompressing cel data: {0}", err);
                return;
            }

            break;
        }
        default:
            Log::LogError("Aseprite unknown cel chunk type: {0}", celType);
            break;
    }
}

Sprite* AsepriteLoader::ReadCelChunk(std::ifstream& file, uint32_t& chunkSizeLeft, uint32_t width, uint32_t height)
{
    char buffer[4];

    file.ignore(2); // Ignore layer index
    chunkSizeLeft -= 2;

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read cel x position");
        return nullptr;
    }
    int16_t celX = (int16_t)ConvertToInt(buffer, 2);
    chunkSizeLeft -= 2;

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read cel y position");
        return nullptr;
    }
    int16_t celY = (int16_t)ConvertToInt(buffer, 2);
    chunkSizeLeft -= 2;

    file.ignore(1); // Ignore opacity level
    chunkSizeLeft -= 1;

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read cel type");
        return nullptr;
    }
    uint32_t celType = ConvertToInt(buffer, 2);
    chunkSizeLeft -= 2;

    if (celType == 1)
    {
        Log::LogError("Could not process linked cel");
        return nullptr;
    }

    file.ignore(7); // Ignore empty data
    chunkSizeLeft -= 7;

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read cel width");
        return nullptr;
    }
    chunkSizeLeft -= 2;
    uint32_t celWidth = ConvertToInt(buffer, 2);
    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read cel height");
        return nullptr;
    }
    chunkSizeLeft -= 2;
    uint32_t celHeight = ConvertToInt(buffer, 2);

    auto fullImageData = new unsigned char[width * height * 4]();
    auto imageData = new char[celWidth * celHeight * 4];

    GetCelData(file, celType, imageData, celWidth, celHeight, chunkSizeLeft);
    chunkSizeLeft = 0;

    Fill(fullImageData, width, height, (unsigned char*)imageData, celWidth, celHeight, celX, celY);
    Texture* texture = Texture::CreateImageTexture(fullImageData, (uint32_t)width, (uint32_t)height);

    auto image = new Sprite(texture);
    image->IsTransparent = IsImageTransparent(fullImageData, width, height);

    Application::Instance->GetResourcesManager()->AddImage(image);

    delete[] fullImageData;
    delete[] imageData;

    return image;
}

bool AsepriteLoader::ReadTagsChunk(std::ifstream& file, std::vector<Sprite*>& inSprites, std::vector<uint32_t>& inDurations, bool loopAll, std::vector<Animation*>& outAnimations)
{
    char buffer[4];

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read tags count");
        return false;
    }
    uint32_t tagsCount = ConvertToInt(buffer, 2);
    if (tagsCount == 0)
    {
        Log::LogError("Tags count is 0");
        return false;
    }

    file.ignore(8); // Ignore unused bytes

    for (uint32_t i = 0; i < tagsCount; ++i)
    {
        if (!file.read(buffer, 2))
        {
            Log::LogError("Could not read tag from frame index");
            return false;
        }
        uint32_t from = ConvertToInt(buffer, 2);

        if (!file.read(buffer, 2))
        {
            Log::LogError("Could not read tag to frame index");
            return false;
        }
        uint32_t to = ConvertToInt(buffer, 2);

        std::vector<Sprite*> sprites;
        sprites.reserve(to - from + 1);
        std::vector<uint32_t> durations(to - from + 1);
        for (uint32_t j = from; j <= to; ++j)
        {
            sprites.push_back(inSprites[j]);
            durations[j - from] = inDurations[j];
        }
        auto animation = new Animation(sprites, durations);

        file.ignore(13); // Ignore unimportant tag data

        if (!file.read(buffer, 2))
        {
            Log::LogError("Could not read tag name length");
            return false;
        }
        uint32_t nameLength = ConvertToInt(buffer, 2);

        if (nameLength == 0)
            animation->Name = "Animation" + std::to_string(i + 1);
        else
        {
            auto name = new char[nameLength + 1];
            if (!file.read(name, nameLength))
            {
                Log::LogError("Could not read tag name");
                delete[] name;
                return false;
            }
            name[nameLength] = '\0';
            animation->Name = std::string(name);
            delete[] name;
        }

        animation->Loop = loopAll;
        Application::Instance->GetResourcesManager()->AddAnimation(animation);
        outAnimations.push_back(animation);
    }

    return true;
}

bool AsepriteLoader::LoadAsepriteData(const char* filePath, bool loopAll, AsepriteData& outData)
{
    char buffer[4];
    uint32_t bytesBeforeTags = 0;
    bool tagsChunkFound = false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        Log::LogError("Could not open {0}", filePath);
        return false;
    }

    file.ignore(6); // Ignore file size and magic number

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read frames count");
        return false;
    }
    uint32_t framesCount = ConvertToInt(buffer, 2);
    if (framesCount == 0)
    {
        Log::LogError("Aseprite file contains zero frames");
        return false;
    }

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read image width");
        return false;
    }
    uint32_t width = ConvertToInt(buffer, 2);

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read image height");
        return false;
    }
    uint32_t height = ConvertToInt(buffer, 2);

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read color depth");
        return false;
    }
    uint32_t colorDepth = ConvertToInt(buffer, 2);
    if (colorDepth != 32)
    {
        Log::LogError("Unexpected color depth: " + std::to_string(colorDepth));
        return false;
    }

    file.ignore(128 - 14); // Ignore 128 bit header
    bytesBeforeTags += 128;
    std::vector<uint32_t> framesDurations;
    framesDurations.reserve(framesCount);

    for (uint32_t i = 0; i < framesCount; ++i)
    {
        file.ignore(6); // Ignore frame size and magic number

        if (!file.read(buffer, 2))
        {
            Log::LogError("Could not read old frame number of chunks");
            return false;
        }
        uint32_t oldChunksCount = ConvertToInt(buffer, 2);

        if (!file.read(buffer, 2))
        {
            Log::LogError("Could not read frame duration");
            return false;
        }
        framesDurations.push_back(ConvertToInt(buffer, 2));

        file.ignore(2); // Ignore two bytes

        if (!file.read(buffer, 4))
        {
            Log::LogError("Could not read new frame number of chunks");
            return false;
        }
        uint32_t chunksCount = ConvertToInt(buffer, 4);

        if (chunksCount == 0)
            chunksCount = oldChunksCount;
        if (!tagsChunkFound)
            bytesBeforeTags += 16;

        for (uint32_t j = 0; j < chunksCount; ++j)
        {
            if (!file.read(buffer, 4))
            {
                Log::LogError("Could not read chunk size");
                return false;
            }
            uint32_t chunkSize = ConvertToInt(buffer, 4);
            uint32_t chunkSizeLeft = chunkSize - 4;

            if (!file.read(buffer, 2))
            {
                Log::LogError("Could not read chunk type");
                return false;
            }
            uint32_t chunkType = ConvertToInt(buffer, 2);
            chunkSizeLeft -= 2;

            if (chunkSizeLeft == 0)
            {
                Log::LogError("Chunk data is empty");
                return false;
            }

            switch (chunkType)
            {
                case 0x2005: // Cel Chunk
                {
                    auto image = ReadCelChunk(file, chunkSizeLeft, width, height);
                    if (image == nullptr)
                        return false;

                    image->Path = filePath;
                    outData.Sprites.push_back(image);

                    break;
                }
                case 0x2018: // Tags Chunk
                {
                    // Skip tags for now and read it again after all sprites loaded
                    tagsChunkFound = true;
                    bytesBeforeTags += 6;

                    break;
                }
                default:
                    // Ignoring other chunks
                    break;
            }

            if (chunkSizeLeft != 0)
                file.ignore(chunkSizeLeft); // Ignore unread data from chunk
            if (!tagsChunkFound)
                bytesBeforeTags += chunkSize;
        }
    }

    if (tagsChunkFound)
    {
        // Restart reading file to position where tags were found
        file.clear();
        file.seekg(bytesBeforeTags);

        if (!ReadTagsChunk(file, outData.Sprites, framesDurations, loopAll, outData.Animations))
        {
            return false;
        }
    }
    else if (outData.Sprites.size() > 1)
    {
        // If there is no tags but multiple sprites - create animation from all of them
        auto animation = new Animation(outData.Sprites, framesDurations);

        animation->Name = "Animation";
        animation->Loop = loopAll;
        Application::Instance->GetResourcesManager()->AddAnimation(animation);
        outData.Animations.push_back(animation);
    }

    return true;
}
