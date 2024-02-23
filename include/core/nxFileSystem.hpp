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

#ifndef NEXUS_CORE_FILE_SYSTEM_HPP
#define NEXUS_CORE_FILE_SYSTEM_HPP

#include "../platform/nxPlatform.hpp"

#include <SDL_stdinc.h>
#include <string>
#include <vector>

namespace nexus { namespace core {

    /**
     * @brief Get the size of a file.
     * @param filePath The path to the file.
     * @return The size of the file in bytes.
     */
    NEXUS_API size_t GetFileSize(const std::string& filePath);

    /**
     * @brief Get the extension of a file.
     * @param filePath The path to the file.
     * @return The extension of the file.
     */
    NEXUS_API std::string GetFileExtension(const std::string& filePath);

    /**
     * @brief Get the name of a file.
     * @param filePath The path to the file.
     * @param getExt Whether to include the extension in the returned name.
     * @return The name of the file.
     */
    NEXUS_API std::string GetFileName(const std::string& filePath, bool getExt = true);

    /**
     * @brief Get the directory path of a file.
     * @param filePath The path to the file.
     * @return The directory path of the file.
     */
    NEXUS_API std::string GetDirectoryPath(const std::string& filePath);

    /**
     * @brief Get the base path of the program.
     * @return The base path of the program with '/' appended at the end.
     */
    NEXUS_API const std::string& GetBasePath();

    /**
     * @brief Check if a file exists.
     * @param fileName The name of the file.
     * @return True if the file exists, false otherwise.
     */
    NEXUS_API bool FileExists(const std::string& fileName);

    /**
     * @brief Check if a directory exists.
     * @param dirPath The path to the directory.
     * @return True if the directory exists, false otherwise.
     */
    NEXUS_API bool DirectoryExists(const std::string& dirPath);

    /**
     * @brief Get the list of files in a directory.
     * @param dirPath The path to the directory.
     * @return A vector containing the names of files in the directory.
     */
    NEXUS_API std::vector<std::string> GetDirectoryFiles(const std::string& dirPath);

    /**
     * @brief Get the list of files in a directory that match a filter.
     * @param basePath The base path of the directory.
     * @param filter The filter to apply to file names.
     * @param scanSubdirs Whether to scan subdirectories.
     * @return A vector containing the names of files that match the filter.
     */
    NEXUS_API std::vector<std::string> GetDirectoryFiles(const std::string& basePath, const std::string& filter, bool scanSubdirs = false);

    /**
     * @brief Get the working directory.
     * @return The current working directory.
     */
    NEXUS_API std::string GetWorkingDirectory();

    /**
     * @brief Set the working directory.
     * @param path The path to set as the working directory.
     */
    NEXUS_API void SetWorkingDirectory(const std::string& path);

    /**
     * @brief Load a file as a raw binary.
     * @param filePath The path to the file.
     * @return A vector containing the raw binary data of the file.
     */
    NEXUS_API std::vector<Uint8> LoadRawFile(const std::string& filePath);

    /**
     * @brief Load a file as text.
     * @param filePath The path to the file.
     * @return The contents of the file as a string.
     */
    NEXUS_API std::string LoadTextFile(const std::string& filePath);


    /* Object-Oriented FileSystem */

    /**
     * @brief Class for working with a specific directory.
     */
    class NEXUS_API FileSystem
    {
      private:
        std::string workingDir;

      public:
        /**
         * @brief Constructs a FileSystem object.
         * @param workingDir The working directory path.
         */
        FileSystem(const std::string& workingDir = "./");

        /**
         * @brief Set the working directory.
         * @param path The path to set as the working directory.
         */
        void SetWorkingDirectory(const std::string &path);

        /**
         * @brief Get the working directory.
         * @return The current working directory.
         */
        const std::string& GetWorkingDirectory() const;

        /**
         * @brief Get the list of files in a directory.
         * @param dirPath The path to the directory.
         * @return A vector containing the names of files in the directory.
         */
        std::vector<std::string> GetDirectoryFiles(const std::string& dirPath);

        /**
         * @brief Get the list of files in a directory that match a filter.
         * @param basePath The base path of the directory.
         * @param filter The filter to apply to file names.
         * @param scanSubdirs Whether to scan subdirectories.
         * @return A vector containing the names of files that match the filter.
         */
        std::vector<std::string> GetDirectoryFiles(const std::string& basePath, const std::string& filter, bool scanSubdirs = false);

        /**
         * @brief Check if a file exists.
         * @param fileName The name of the file.
         * @return True if the file exists, false otherwise.
         */
        bool FileExists(const std::string& fileName);

        /**
         * @brief Check if a directory exists.
         * @param dirPath The path to the directory.
         * @return True if the directory exists, false otherwise.
         */
        bool DirectoryExists(const std::string& dirPath);

        /**
         * @brief Get the size of a file.
         * @param filePath The path to the file.
         * @return The size of the file in bytes.
         */
        size_t GetFileSize(const std::string& filePath);

        /**
         * @brief Get the extension of a file.
         * @param filePath The path to the file.
         * @return The extension of the file.
         */
        std::string GetFileExtension(const std::string& filePath);

        /**
         * @brief Get the name of a file.
         * @param filePath The path to the file.
         * @param getExt Whether to include the extension in the returned name.
         * @return The name of the file.
         */
        std::string GetFileName(const std::string& filePath, bool getExt = true);

        /**
         * @brief Load a file as a raw binary.
         * @param filePath The path to the file.
         * @return A vector containing the raw binary data of the file.
         */
        std::vector<Uint8> LoadRawFile(const std::string& filePath);

        /**
         * @brief Load a file as text.
         * @param filePath The path to the file.
         * @return The contents of the file as a string.
         */
        std::string LoadTextFile(const std::string& filePath);
    };

}}

#endif //NEXUS_CORE_FILE_SYSTEM_HPP
