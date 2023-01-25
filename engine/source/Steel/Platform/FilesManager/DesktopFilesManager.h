#pragma once

#include "Steel/Resources/FilesManager.h"

#include <string>

class DesktopFilesManager : public FilesManager
{
public:
    DesktopFilesManager() = default;
    ~DesktopFilesManager() override = default;

    void Init() override;
    void Terminate() override;

    FileData ReadFile(const std::string& filePath) override;
    bool WriteFile(const std::string& filePath, FileData fileData) override;
    bool GetFilesInDirRecursive(const std::string& dirPath, std::vector<std::filesystem::path>& outFilesPath) override;
};
