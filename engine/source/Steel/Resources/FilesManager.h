#pragma once

#include "FileData.h"

#include <filesystem>
#include <vector>

class FilesManager
{
public:
    FilesManager() = default;
    virtual ~FilesManager() = default;

    virtual void Init() = 0;
    virtual void Terminate() = 0;

    virtual bool FileExists(const std::string& filePath) = 0;
    virtual FileData ReadFile(const std::string& filePath) = 0;
    virtual bool WriteFile(const std::string& filePath, FileData fileData) = 0;
    virtual bool GetFilesInDirRecursive(const std::string& dirPath, std::vector<std::filesystem::path>& outFilesPath) = 0;
};
