#include "FileDataReader.h"

FileDataReader::FileDataReader(FileData fileData)
{
    _fileData = fileData;
    _pointer = 0;
    _bufferLength = 0;
}

bool FileDataReader::Read(uint32_t n)
{
    _pointer += _bufferLength;

    if (_pointer + n > _fileData.Size)
    {
        _bufferLength = _fileData.Size - _pointer;
        return false;
    }

    _bufferLength = n;
    return true;
}

void FileDataReader::Reset()
{
    _pointer = 0;
    _bufferLength = 0;
}

const char* FileDataReader::Buffer() const
{
    return _fileData.Data + _pointer;
}

bool FileDataReader::IsEnd() const
{
    return _pointer == _fileData.Size;
}
