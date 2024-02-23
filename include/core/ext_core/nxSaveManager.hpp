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

#ifndef NEXUS_EXT_CORE_SAVE_MANAGER_HPP
#define NEXUS_EXT_CORE_SAVE_MANAGER_HPP

#include "../../platform/nxPlatform.hpp"

#include <functional>
#include <fstream>
#include <cstring>
#include <string>

namespace nexus { namespace core {

    /**
     * @brief A class for managing save data, providing simplified handling for various data structures and version compatibility.
     */
    class NEXUS_API SaveManager
    {
      private:
        enum RetCode {
            SUCCESS = 0,
            LOAD_FAILURE = 1,
            WRITE_FAILURE = 2,
            FILE_NOT_FOUND = 3,
            INCOMPATIBLE_VERSION = 4,
        };

        int version = -1;
        std::string directory;
        void* origin = nullptr, *data = nullptr;

        using IncompatibleVersionCallback = std::function<bool(const std::ifstream& file, const int version)>;
        IncompatibleVersionCallback onIncompatibleVersion;

      public:
        /**
         * @brief Constructor for the SaveManager class.
         * @tparam _Ts The type of the save data.
         * @param origin The original data to be saved.
         * @param version The version number of the save data format.
         * @param directory The directory path for saving and loading files.
         */
        template <typename _Ts>
        SaveManager(const _Ts& origin, int version = 1, const std::string& directory = "")
        : version(version), directory(directory)
        {
            this->origin = operator new(sizeof(_Ts));
            std::memcpy(this->origin, &origin, sizeof(_Ts));

            data = operator new(sizeof(_Ts));
            std::memcpy(data, &origin, sizeof(_Ts));
        }

        /**
         * @brief Destructor for the SaveManager class.
         * Cleans up allocated memory and resources.
         */
        ~SaveManager()
        {
            operator delete(origin);
            operator delete(data);
        }

        /**
         * @brief Sets the directory path for saving and loading files.
         * @param directory The directory path to set.
         */
        inline void SetDirectory(const std::string& directory)
        {
            this->directory = directory;
        }

        /**
         * @brief Sets a callback function to handle incompatible version scenarios during loading.
         * @param callback The callback function to set.
         */
        inline void SetOnIncompatibleVersion(const IncompatibleVersionCallback& callback)
        {
            onIncompatibleVersion = callback;
        }

        /**
         * @brief Gets a pointer to the save data of the specified type.
         * @tparam _Ts The type of the save data.
         * @return A pointer to the save data.
         */
        template <typename _Ts>
        inline _Ts* Get() { return reinterpret_cast<_Ts*>(data); }

        /**
         * @brief Loads save data from a file.
         * @tparam _Ts The type of the save data.
         * @param fileName The path to the file to load.
         * @return An error code indicating the result of the operation.
         */
        template <typename _Ts>
        int Load(const std::string& fileName)
        {
            // Open file in reading mode
            std::ifstream file(directory + fileName, std::ios::binary);
            if (!file.is_open()) return FILE_NOT_FOUND;

            // Read version from file
            int fileVersion;
            file.read(reinterpret_cast<char*>(&fileVersion), sizeof(int));

            // Check if the version read matches the expected version
            if (fileVersion != version)
            {
                RetCode ret = INCOMPATIBLE_VERSION;

                if (onIncompatibleVersion && onIncompatibleVersion(file, fileVersion))
                {
                    ret = SUCCESS;
                }

                file.close();
                return ret;
            }

            // Read '_Ts' data from file
            file.read(reinterpret_cast<char*>(data), sizeof(_Ts));

            // Check if reading was successful
            if (file.fail())
            {
                file.close();
                data = origin;
                return LOAD_FAILURE;
            }

            // Close file
            file.close();

            return SUCCESS;
        }

        /**
         * @brief Writes save data to a file.
         * @tparam _Ts The type of the save data.
         * @param fileName The path to the file to write.
         * @return An error code indicating the result of the operation.
         */
        template <typename _Ts>
        int Write(const std::string& fileName)
        {
            // Open file in write mode
            std::ofstream file(directory + fileName, std::ios::binary);
            if (!file.is_open()) return FILE_NOT_FOUND;

            // Write version first to file then data
            file.write(reinterpret_cast<char*>(&version), sizeof(int));
            file.write(reinterpret_cast<char*>(data), sizeof(_Ts));

            // Check if the write was successful
            if (file.fail()) return WRITE_FAILURE;

            // Close file
            file.close();

            return SUCCESS;
        }
    };

}}

#endif //NEXUS_EXT_CORE_SAVE_MANAGER_HPP