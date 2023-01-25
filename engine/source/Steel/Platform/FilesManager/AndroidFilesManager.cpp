#include "AndroidFilesManager.h"
#include "Steel/Core/Log.h"

#include <iostream>

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

std::unordered_map<std::string, int> filesPath;
std::vector<FileData> filesData;

void AndroidFilesManager::Init()
{

}

void AndroidFilesManager::Terminate()
{
    for (auto& data : filesData)
        data.Delete();
    filesData.clear();
    filesPath.clear();
}

FileData AndroidFilesManager::ReadFile(const std::string& filePath)
{
    if (filesPath.find(filePath) == filesPath.end())
        return FileData::Empty();

    int index = filesPath[filePath];
    if (index < 0 || index >= filesData.size())
    {
        Log::LogError("Could not read file, cached data index is wrong {0}", index);
        return FileData::Empty();
    }

    return filesData[filesPath[filePath]];
}

bool AndroidFilesManager::WriteFile(const std::string& filePath, FileData fileData)
{
    Log::LogError("Writing and editing resources is not supported on android platform");

    return false;
}

bool AndroidFilesManager::GetFilesInDirRecursive(const std::string& dirPath, std::vector<std::filesystem::path>& outFilesPath)
{
    for (auto& pair : filesPath)
        outFilesPath.push_back(pair.first);

    return true;
}

void CacheAllFilesRecursive(AAssetManager* assetManager, AAssetDir* assetDir, const std::string& dirPath)
{
    const char* filename = (const char*)NULL;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != nullptr)
    {
        std::string filenameString = dirPath + std::string(filename);
        AAsset* asset = AAssetManager_open(assetManager, filenameString.c_str(), AASSET_MODE_UNKNOWN);
        if (asset == nullptr)
            continue;

        FileData result {};

        result.Size = AAsset_getLength(asset) + 1;

        char* buffer = new char[result.Size];
        AAsset_read(asset, buffer, result.Size - 1);
        buffer[result.Size - 1] = '\0';
        result.Data = buffer;

        AAsset_close(asset);

        filesPath["/" + filenameString] = filesData.size(); // cache index in filesData
        filesData.push_back(result);
    }
}

void CacheAllFiles(JNIEnv* env, jobject obj, jobject java_asset_manager)
{
    // Load all assets in memory

    AAssetManager* assetManager = AAssetManager_fromJava(env, java_asset_manager);
    AAssetDir* assetDir = AAssetManager_openDir(assetManager, "");
    if (assetDir == nullptr)
        return;

    CacheAllFilesRecursive(assetManager, assetDir, "");
    AAssetDir_close(assetDir);

    assetDir = AAssetManager_openDir(assetManager, "Resources");
    if (assetDir == nullptr)
    {
        return;
    }
    CacheAllFilesRecursive(assetManager, assetDir, "Resources/");
    AAssetDir_close(assetDir);
}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_initManager(JNIEnv* env, jobject obj, jobject java_asset_manager);
}

JNIEXPORT void Java_com_android_appsteel_AppSteelLib_initManager(JNIEnv* env, jobject obj, jobject java_asset_manager)
{
    CacheAllFiles(env, obj, java_asset_manager);
}
