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

#ifndef NEXUS_EXT_CORE_ASSET_MANAGER_HPP
#define NEXUS_EXT_CORE_ASSET_MANAGER_HPP

#include "../../platform/nxPlatform.hpp"

#include <unordered_map>
#include <typeinfo>
#include <utility>
#include <memory>
#include <string>

namespace nexus { namespace core {

    /**
     * @brief The Asset class represents a movable container for any type of data.
     */
    class NEXUS_API Asset
    {
      private:
        std::unique_ptr<void, void(*)(void*)> data{nullptr, [](void*){}};
        std::reference_wrapper<const std::type_info> type; ///< Type information of the stored data.

      public:
        /**
         * @brief Default constructor for an empty Asset.
         */
        Asset() : type(typeid(void)) {}

        /**
         * @brief Constructor that takes a value of any type and moves it into the Asset.
         * @tparam T The type of the value to be stored.
         * @param value The value to be stored.
         */
        template <typename T>
        Asset(T&& value) : data(new typename std::decay<T>::type(std::move(value)),
            [](void* ptr) { delete static_cast<typename std::decay<T>::type*>(ptr); })
        , type(typeid(T)) { }

        /**
         * @brief Move constructor for Asset.
         * @param other The rvalue reference to another Asset.
         */
        Asset(Asset&& other) noexcept
        : data(std::exchange(other.data, nullptr))
        , type(std::exchange(other.type, std::ref(typeid(void))))
        { }

        /**
         * @brief Move assignment operator for Asset.
         * @param other The rvalue reference to another Asset.
         * @return A reference to the current Asset after the move.
         */
        Asset& operator=(Asset&& other) noexcept
        {
            if (this != &other)
            {
                // Move the data and type from the other Asset
                data = std::exchange(other.data, nullptr);
                type = std::exchange(other.type, std::ref(typeid(void)));
            }
            return *this;
        }

        /**
         * @brief Static function to create an Asset with a given type and constructor arguments.
         * @tparam T The type of the value to be stored.
         * @tparam Args Variadic template for constructor arguments.
         * @param args The constructor arguments.
         * @return An Asset containing the created value.
         */
        template <typename T, typename... Args>
        static Asset Make(Args&&... args)
        {
            return Asset(T(args...));
        }

        /**
         * @brief Gets a pointer to the stored data of type T.
         * @tparam T The type of the data to retrieve.
         * @return A pointer to the stored data.
         * @throws std::bad_cast if the stored type does not match the requested type.
         */
        template <typename T> T* Get()
        {
            if (type.get() != typeid(T)) throw std::bad_cast();
            return static_cast<T*>(data.get());
        }

        /**
         * @brief Gets a pointer to the stored data of type T.
         * @tparam T The type of the data to retrieve.
         * @return A pointer to the stored data.
         * @throws std::bad_cast if the stored type does not match the requested type.
         */
        template <typename T> const T* Get() const
        {
            if (type.get() != typeid(T)) throw std::bad_cast();
            return static_cast<T*>(data.get());
        }

        /**
         * @brief Gets the type information of the stored data.
         * @return The type information.
         */
        const std::type_info& Type() const
        {
            return type;
        }
    };

    /**
     * @brief The AssetManager class manages assets/resources of various types using a map.
     */
    class NEXUS_API AssetManager
    {
      private:
        std::unordered_map<std::string, Asset> map; ///< Map of assets with their associated names.

      public:
        /**
         * @brief Adds a new asset to the manager with a given name and constructor arguments.
         * @tparam T_Asset The type of the asset to add.
         * @tparam Args Variadic template for constructor arguments.
         * @param name The name to associate with the asset.
         * @param args The constructor arguments.
         * @return A pair containing an iterator to the inserted asset and a boolean indicating success.
         */
        template<typename T_Asset, typename... Args>
        auto Add(const std::string& name, Args... args)
        {
            return map.emplace(name, Asset::Make<T_Asset>(args...));
        }

        /**
         * @brief Adds a new asset to the manager with a given name and a pre-constructed asset.
         * @tparam T_Asset The type of the asset to add.
         * @param name The name to associate with the asset.
         * @param asset The pre-constructed asset.
         * @return A pair containing an iterator to the inserted asset and a boolean indicating success.
         */
        template<typename T_Asset>
        auto Add(const std::string& name, T_Asset&& asset)
        {
            return map.emplace(name, Asset(asset));
        }

        /**
         * @brief Adds a new asset to the manager or replaces an existing one with the given name and constructor arguments.
         * @tparam T_Asset The type of the asset to add.
         * @tparam Args Variadic template for constructor arguments.
         * @param name The name to associate with the asset.
         * @param args The constructor arguments.
         * @return A pair containing an iterator to the inserted/replaced asset and a boolean indicating success.
         */
        template<typename T_Asset, typename... Args>
        auto AddOrReplace(const std::string& name, Args... args)
        {
            return map.insert_or_assign(name, Asset::Make<T_Asset>(args...));
        }

        /**
         * @brief Adds a new asset to the manager or replaces an existing one with the given name and a pre-constructed asset.
         * @tparam T_Asset The type of the asset to add.
         * @param name The name to associate with the asset.
         * @param asset The pre-constructed asset.
         * @return A pair containing an iterator to the inserted/replaced asset and a boolean indicating success.
         */
        template<typename T_Asset>
        auto AddOrReplace(const std::string& name, T_Asset&& asset)
        {
            return map.insert_or_assign(name, Asset(asset));
        }

        /**
         * @brief Removes an asset from the manager with the given name.
         * @param name The name of the asset to remove.
         */
        void Remove(const std::string& name)
        {
            auto it = map.find(name);
            if (it != map.end()) map.erase(it);
        }

        /**
         * @brief Reserves space for a specified number of assets in the map.
         * @param size The number of assets to reserve space for.
         */
        void Reserve(std::size_t size)
        {
            map.reserve(size);
        }

        /**
         * @brief Gets the number of assets in the manager.
         * @return The number of assets.
         */
        std::size_t GetSize() const
        {
            return map.size();
        }

        /**
         * @brief Gets the type information of the asset with the given name.
         * @param name The name of the asset.
         * @return A pointer to the type information or nullptr if the asset does not exist.
         */
        const std::type_info* GetType(const std::string& name) const
        {
            auto it = map.find(name);
            if (it != map.end()) return &it->second.Type();
            return nullptr;
        }

        /**
         * @brief Gets a pointer to the stored data of the asset with the given name and type.
         * @tparam T_Asset The type of the data to retrieve.
         * @param name The name of the asset.
         * @return A pointer to the stored data or nullptr if the asset does not exist or has a different type.
         */
        template<typename T_Asset>
        T_Asset* Get(const std::string& name)
        {
            auto it = map.find(name);
            if (it != map.end()) return it->second.Get<T_Asset>();
            return nullptr;
        }

        /**
         * @brief Gets a pointer to the stored data of the asset with the given name and type.
         * @tparam T_Asset The type of the data to retrieve.
         * @param name The name of the asset.
         * @return A pointer to the stored data or nullptr if the asset does not exist or has a different type.
         */
        template<typename T_Asset>
        const T_Asset* Get(const std::string& name) const
        {
            auto it = map.find(name);
            if (it != map.end()) return it->second.Get<T_Asset>();
            return nullptr;
        }

        /**
         * @brief Gets a pointer to the asset with the given name.
         * @param name The name of the asset.
         * @return A pointer to the asset or nullptr if the asset does not exist.
         */
        Asset* Get(const std::string& name)
        {
            auto it = map.find(name);
            if (it != map.end()) return &it->second;
            return nullptr;
        }

        /**
         * @brief Gets a pointer to the asset with the given name.
         * @param name The name of the asset.
         * @return A pointer to the asset or nullptr if the asset does not exist.
         */
        const Asset* Get(const std::string& name) const
        {
            auto it = map.find(name);
            if (it != map.end()) return &it->second;
            return nullptr;
        }

        /**
         * @brief Gets a reference to the asset with the given name.
         * If the asset does not exist, a new empty asset is created and returned.
         * @param name The name of the asset.
         * @return A reference to the asset.
         */
        Asset& operator[](const std::string& name)
        {
            auto it = map.find(name);
            if (it != map.end()) return it->second;
            return map.emplace(name, Asset()).first->second;
        }

        /**
         * @brief Returns an iterator pointing to the beginning of the asset manager's map.
         * @return An iterator pointing to the beginning.
         */
        auto begin() { return map.begin(); }

        /**
         * @brief Returns an iterator pointing to the end of the asset manager's map.
         * @return An iterator pointing to the end.
         */
        auto end() { return map.end(); }
    };

}}

#endif //NEXUS_EXT_CORE_ASSET_MANAGER_HPP