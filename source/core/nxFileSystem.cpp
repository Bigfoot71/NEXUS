/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#include "core/nxFileSystem.hpp"
#include "core/nxException.hpp"
#include "core/nxLog.hpp"

#include <SDL_filesystem.h>
#include <filesystem>
#include <algorithm>
#include <fstream>

using namespace nexus;

size_t core::GetFileSize(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    return (file.is_open() ? static_cast<size_t>(file.tellg()) : 0);
}

std::string core::GetFileExtension(const std::string& filePath)
{
    size_t lastDotPos = filePath.find_last_of('.');

    if (lastDotPos != std::string::npos)
    {
        std::string extension = filePath.substr(lastDotPos + 1);

        // Convert extension to lowercase
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        return extension;
    }

    return "";
}

std::string core::GetFileName(const std::string& filePath, bool getExt)
{
    std::filesystem::path path(filePath);
    if (getExt) return path.filename().string();
    return path.stem().string();
}

std::string core::GetDirectoryPath(const std::string& filePath)
{
    std::filesystem::path path(filePath);
    return path.parent_path().string();
}

const std::string& core::GetBasePath()
{
    static std::string basePath;

    if (basePath.empty())
    {
        const char* str = SDL_GetBasePath();
        basePath = str;
        SDL_free((void*)str);
    }

    return basePath;
}

bool core::FileExists(const std::string& fileName)
{
    std::ifstream file(fileName);
    return file.good();
}

bool core::DirectoryExists(const std::string& dirPath)
{
    return std::filesystem::is_directory(dirPath);
}

std::vector<std::string> core::GetDirectoryFiles(const std::string& dirPath)
{
    std::vector<std::string> files;

    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                files.push_back(entry.path().string());
            }
        }
    }
    catch (const std::exception& e)
    {
        NEXUS_LOG(Error) << e.what() << "\n";
    }

    return files;
}

std::vector<std::string> core::GetDirectoryFiles(const std::string& basePath, const std::string& filter, bool scanSubdirs)
{
    std::vector<std::string> files;

    try
    {
        if (scanSubdirs)
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(basePath))
            {
                if (std::filesystem::is_regular_file(entry) && entry.path().filename().string().find(filter) != std::string::npos)
                {
                    files.push_back(entry.path().string());
                }
            }
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(basePath))
            {
                if (std::filesystem::is_regular_file(entry) && entry.path().filename().string().find(filter) != std::string::npos)
                {
                    files.push_back(entry.path().string());
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        NEXUS_LOG(Error) << e.what() << "\n";
    }

    return files;
}

std::string core::GetWorkingDirectory()
{
    return std::filesystem::current_path().string();
}

void core::SetWorkingDirectory(const std::string& path)
{
    std::filesystem::current_path(path);
}

std::vector<Uint8> core::LoadRawFile(const std::string& filePath)
{
    std::vector<Uint8> data;
    std::ifstream file(filePath, std::ios::binary);

    if (file)
    {
        // Determines the file size
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Memory reserve for vector depending on file size
        data.resize(static_cast<size_t>(fileSize));

        // Read raw data from file into vector
        file.read(reinterpret_cast<char*>(data.data()), fileSize);

        file.close();
    }
    else
    {
        throw core::NexusException("Filesystem", "Error opening file [" + filePath + "]");
    }

    return data;
}

std::string core::LoadTextFile(const std::string& filePath)
{
    std::string text;
    std::ifstream file(filePath);

    if (file)
    {
        text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }
    else
    {
        throw core::NexusException("Filesystem", "Error opening file [" + filePath + "]");
    }

    return text;
}


/* Oriented Object FileSystem */

core::FileSystem::FileSystem(const std::string& workingDir)
: workingDir(workingDir)
{
    if (this->workingDir.empty())
    {
        this->workingDir = GetBasePath();
    }
    else if (this->workingDir.back() != '/')
    {
        this->workingDir.back() += '/';
    }
}

void core::FileSystem::SetWorkingDirectory(const std::string &path)
{
    workingDir = path;
}

const std::string& core::FileSystem::GetWorkingDirectory() const
{
    return workingDir;
}

std::vector<std::string> core::FileSystem::GetDirectoryFiles(const std::string& dirPath)
{
    return core::GetDirectoryFiles(workingDir + dirPath);
}

std::vector<std::string> core::FileSystem::GetDirectoryFiles(const std::string& basePath, const std::string& filter, bool scanSubdirs)
{
    return core::GetDirectoryFiles(workingDir + basePath);
}

bool core::FileSystem::FileExists(const std::string& fileName)
{
    return core::FileExists(workingDir + fileName);
}

bool core::FileSystem::DirectoryExists(const std::string& dirPath)
{
    return core::DirectoryExists(workingDir + dirPath);
}

size_t core::FileSystem::GetFileSize(const std::string& filePath)
{
    return core::GetFileSize(workingDir + filePath);
}

std::string core::FileSystem::GetFileExtension(const std::string& filePath)
{
    return core::GetFileExtension(workingDir + filePath);
}

std::string core::FileSystem::GetFileName(const std::string& filePath, bool getExt)
{
    return core::GetFileName(workingDir, getExt);
}

std::vector<Uint8> core::FileSystem::LoadRawFile(const std::string& filePath)
{
    return core::LoadRawFile(workingDir + filePath);
}

std::string core::FileSystem::LoadTextFile(const std::string& filePath)
{
    return core::LoadTextFile(workingDir + filePath);
}
