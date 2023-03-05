#pragma once

#include "FileData.h"

#include <cstdint>

class FileDataReader
{
public:
    explicit FileDataReader(FileData fileData);

    bool Read(uint32_t n);
    void Reset();
    const char* Buffer() const;

    bool IsEnd() const;

private:
    FileData _fileData;
    long _pointer;
    long _bufferLength;
};
