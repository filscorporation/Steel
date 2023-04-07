#include "DesktopFilesManager.h"

#include <fstream>

void DesktopFilesManager::Init()
{

}

void DesktopFilesManager::Terminate()
{

}

bool DesktopFilesManager::FileExists(const std::string& filePath)
{
    std::ifstream infile(filePath, std::ifstream::binary);
    bool exists = infile.good();
    if (exists)
        infile.close();
    return exists;
}

FileData DesktopFilesManager::ReadFile(const std::string& filePath)
{
    std::ifstream infile(filePath, std::ifstream::binary);
    if (!infile.good())
    {
        return FileData::Empty();
    }

    FileData result {};

    infile.seekg(0, std::ifstream::end);
    result.Size = (long)infile.tellg() + 1;
    infile.seekg(0, std::ifstream::beg);

    char* buffer = new char[result.Size];
    infile.read(buffer, result.Size - 1);
    buffer[result.Size - 1] = '\0';
    result.Data = buffer;

    infile.close();

    return result;
}

bool DesktopFilesManager::WriteFile(const std::string& filePath, FileData fileData)
{
    std::ofstream outfile(filePath);
    if (!outfile.good())
    {
        return false;
    }
    outfile << fileData.Data;

    return true;
}

bool DesktopFilesManager::GetFilesInDirRecursive(const std::string& dirPath, std::vector<std::filesystem::path>& outFilesPath)
{
    if (!std::filesystem::exists(dirPath))
        return false;

    for (std::filesystem::recursive_directory_iterator i(dirPath), end; i != end; ++i)
        if (!is_directory(i->path()) && is_regular_file(i->path()))
            outFilesPath.push_back(i->path());

    return true;
}
