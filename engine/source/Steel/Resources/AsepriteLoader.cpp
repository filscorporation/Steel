#include <fstream>
#include <zlib.h>

#include "AsepriteLoader.h"
#include "ResourcesManager.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"

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

void GetCelData(FileDataReader& reader, uint32_t celType, char* imageData, uint32_t celWidth, uint32_t celHeight, uint32_t dataSize)
{
    switch (celType)
    {
        case 0: // Raw Cell
        {
            if (!reader.Read(celWidth * celHeight * 4))
            {
                Log::LogError("Could not read raw image data");
                return;
            }
            std::copy(reader.Buffer(), reader.Buffer() + celWidth * celHeight * 4, imageData);
            break;
        }
        case 2: // Compressed Image
        {
            if (!reader.Read(dataSize))
            {
                Log::LogError("Could not read compressed image data");
                return;
            }

            int err = Decompress(reader.Buffer(), dataSize, imageData, celWidth * celHeight * 4);
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

Sprite* AsepriteLoader::ReadCelChunk(FileDataReader& reader, uint32_t& chunkSizeLeft, uint32_t width, uint32_t height)
{
    reader.Read(2); // Ignore layer index
    chunkSizeLeft -= 2;

    if (!reader.Read(2))
    {
        Log::LogError("Could not read cel x position");
        return nullptr;
    }
    int16_t celX = (int16_t)ConvertToInt(reader.Buffer(), 2);
    chunkSizeLeft -= 2;

    if (!reader.Read(2))
    {
        Log::LogError("Could not read cel y position");
        return nullptr;
    }
    int16_t celY = (int16_t)ConvertToInt(reader.Buffer(), 2);
    chunkSizeLeft -= 2;

    reader.Read(1); // Ignore opacity level
    chunkSizeLeft -= 1;

    if (!reader.Read(2))
    {
        Log::LogError("Could not read cel type");
        return nullptr;
    }
    uint32_t celType = ConvertToInt(reader.Buffer(), 2);
    chunkSizeLeft -= 2;

    if (celType == 1)
    {
        Log::LogError("Could not process linked cel");
        return nullptr;
    }

    reader.Read(7); // Ignore empty data
    chunkSizeLeft -= 7;

    if (!reader.Read(2))
    {
        Log::LogError("Could not read cel width");
        return nullptr;
    }
    chunkSizeLeft -= 2;
    uint32_t celWidth = ConvertToInt(reader.Buffer(), 2);
    if (!reader.Read(2))
    {
        Log::LogError("Could not read cel height");
        return nullptr;
    }
    chunkSizeLeft -= 2;
    uint32_t celHeight = ConvertToInt(reader.Buffer(), 2);

    auto fullImageData = new unsigned char[width * height * 4]();
    auto imageData = new char[celWidth * celHeight * 4];

    GetCelData(reader, celType, imageData, celWidth, celHeight, chunkSizeLeft);
    chunkSizeLeft = 0;

    Fill(fullImageData, width, height, (unsigned char*)imageData, celWidth, celHeight, celX, celY);
    Texture* texture = Texture::CreateImageTexture(fullImageData, (uint32_t)width, (uint32_t)height);
    Application::Context()->Resources->AddResource(texture);

    auto image = new Sprite(texture);
    image->SetIsTransparent(IsImageTransparent(fullImageData, width, height));

    Application::Context()->Resources->AddResource(image);

    delete[] fullImageData;
    delete[] imageData;

    return image;
}

bool AsepriteLoader::ReadTagsChunk(FileDataReader& reader, std::vector<Sprite*>& inSprites, std::vector<uint32_t>& inDurations,
                                   std::vector<Animation*>& outAnimations)
{
    if (!reader.Read(2))
    {
        Log::LogError("Could not read tags count");
        return false;
    }
    uint32_t tagsCount = ConvertToInt(reader.Buffer(), 2);
    if (tagsCount == 0)
    {
        Log::LogError("Tags count is 0");
        return false;
    }

    reader.Read(8); // Ignore unused bytes

    for (uint32_t i = 0; i < tagsCount; ++i)
    {
        if (!reader.Read(2))
        {
            Log::LogError("Could not read tag from frame index");
            return false;
        }
        uint32_t from = ConvertToInt(reader.Buffer(), 2);

        if (!reader.Read(2))
        {
            Log::LogError("Could not read tag to frame index");
            return false;
        }
        uint32_t to = ConvertToInt(reader.Buffer(), 2);

        std::vector<Sprite*> sprites;
        sprites.reserve(to - from + 1);
        std::vector<uint32_t> durations(to - from + 1);
        for (uint32_t j = from; j <= to; ++j)
        {
            sprites.push_back(j >= 0 && j < inSprites.size() ? inSprites[j] : nullptr);
            durations[j - from] = inDurations[j];
        }
        auto animation = new Animation(sprites, durations);

        reader.Read(13); // Ignore unimportant tag data

        if (!reader.Read(2))
        {
            Log::LogError("Could not read tag name length");
            return false;
        }
        uint32_t nameLength = ConvertToInt(reader.Buffer(), 2);

        if (nameLength == 0)
            animation->Name = "Animation" + std::to_string(i + 1);
        else
        {
            if (!reader.Read(nameLength))
            {
                Log::LogError("Could not read tag name");
                return false;
            }
            animation->Name = std::string(reader.Buffer(), nameLength);
        }

        animation->Loop = false;
        Application::Instance->GetResourcesManager()->AddResource(animation);
        outAnimations.push_back(animation);
    }

    return true;
}

AsepriteData* AsepriteLoader::LoadAsepriteData(const FileData& fileData)
{
    FileDataReader reader(fileData);
    uint32_t bytesBeforeTags = 0;
    bool tagsChunkFound = false;

    reader.Read(6); // Ignore file size and magic number

    if (!reader.Read(2))
    {
        Log::LogError("Could not read frames count");
        return nullptr;
    }
    uint32_t framesCount = ConvertToInt(reader.Buffer(), 2);
    if (framesCount == 0)
    {
        Log::LogError("Aseprite file contains zero frames");
        return nullptr;
    }

    if (!reader.Read(2))
    {
        Log::LogError("Could not read image width");
        return nullptr;
    }
    uint32_t width = ConvertToInt(reader.Buffer(), 2);

    if (!reader.Read(2))
    {
        Log::LogError("Could not read image height");
        return nullptr;
    }
    uint32_t height = ConvertToInt(reader.Buffer(), 2);

    if (!reader.Read(2))
    {
        Log::LogError("Could not read color depth");
        return nullptr;
    }
    uint32_t colorDepth = ConvertToInt(reader.Buffer(), 2);
    if (colorDepth != 32)
    {
        Log::LogError("Unexpected color depth: " + std::to_string(colorDepth));
        return nullptr;
    }

    reader.Read(128 - 14); // Ignore 128 bit header
    bytesBeforeTags += 128;
    std::vector<uint32_t> framesDurations;
    framesDurations.reserve(framesCount);

    auto outData = new AsepriteData();

    int imagesCount = 0;
    for (uint32_t i = 0; i < framesCount; ++i)
    {
        reader.Read(6); // Ignore frame size and magic number

        if (!reader.Read(2))
        {
            Log::LogError("Could not read old frame number of chunks");
            return nullptr;
        }
        uint32_t oldChunksCount = ConvertToInt(reader.Buffer(), 2);

        if (!reader.Read(2))
        {
            Log::LogError("Could not read frame duration");
            return nullptr;
        }
        framesDurations.push_back(ConvertToInt(reader.Buffer(), 2));

        reader.Read(2); // Ignore two bytes

        if (!reader.Read(4))
        {
            Log::LogError("Could not read new frame number of chunks");
            return nullptr;
        }
        uint32_t chunksCount = ConvertToInt(reader.Buffer(), 4);

        if (chunksCount == 0)
            chunksCount = oldChunksCount;
        if (!tagsChunkFound)
            bytesBeforeTags += 16;

        for (uint32_t j = 0; j < chunksCount; ++j)
        {
            if (!reader.Read(4))
            {
                Log::LogError("Could not read chunk size");
                return nullptr;
            }
            uint32_t chunkSize = ConvertToInt(reader.Buffer(), 4);
            uint32_t chunkSizeLeft = chunkSize - 4;

            if (!reader.Read(2))
            {
                Log::LogError("Could not read chunk type");
                return nullptr;
            }
            uint32_t chunkType = ConvertToInt(reader.Buffer(), 2);
            chunkSizeLeft -= 2;

            if (chunkSizeLeft == 0)
            {
                Log::LogError("Chunk data is empty");
                return nullptr;
            }

            switch (chunkType)
            {
                case 0x2005: // Cel Chunk
                {
                    auto image = ReadCelChunk(reader, chunkSizeLeft, width, height);
                    if (image == nullptr)
                        return nullptr;

                    imagesCount++;
                    outData->Sprites.push_back(image);

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
                reader.Read(chunkSizeLeft); // Ignore unread data from chunk
            if (!tagsChunkFound)
                bytesBeforeTags += chunkSize;
        }
    }

    if (tagsChunkFound)
    {
        // Restart reading file to position where tags were found
        reader.Reset();
        reader.Read(bytesBeforeTags);

        if (!ReadTagsChunk(reader, outData->Sprites, framesDurations, outData->Animations))
        {
            return nullptr;
        }
    }
    else if (outData->Sprites.size() > 1)
    {
        // If there is no tags but multiple sprites - create animation from all of them
        auto animation = new Animation(outData->Sprites, framesDurations);

        animation->Name = "Animation";
        animation->Loop = false;
        Application::Instance->GetResourcesManager()->AddResource(animation);
        outData->Animations.push_back(animation);
    }

    return outData;
}
