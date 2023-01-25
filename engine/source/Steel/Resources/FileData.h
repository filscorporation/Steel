#pragma once

struct FileData
{
    const char* Data;
    long Size;

    void Delete()
    {
        delete[] Data;
        Data = nullptr;
        Size = 0;
    }

    bool IsEmpty() const
    {
        return Data == nullptr;
    }

    static FileData Empty()
    {
        return FileData { nullptr, 0 };
    }
};
