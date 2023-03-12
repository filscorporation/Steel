#pragma once

#include "Steel/Resources/FilesManager.h"

#include <vector>
#include <unordered_map>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class AndroidFilesManager : public FilesManager
{
public:
    AndroidFilesManager() = default;
    ~AndroidFilesManager() override = default;

    void Init() override;
    void Terminate() override;

    static void CacheAllFiles(AAssetManager* assetManager);

    FileData ReadFile(const std::string& filePath) override;
    bool WriteFile(const std::string& filePath, FileData fileData) override;
    bool GetFilesInDirRecursive(const std::string& dirPath, std::vector<std::filesystem::path>& outFilesPath) override;

private:
    static void CacheAllFilesRecursive(AAssetManager* assetManager, AAssetDir* assetDir, const std::string& dirPath);

    static std::unordered_map<std::string, int> filesPath;
    static std::vector<FileData> filesData;
};
