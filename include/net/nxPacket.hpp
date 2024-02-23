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

#ifndef RAYFLEX_NET_PACKET_HPP
#define RAYFLEX_NET_PACKET_HPP

#include "../platform/nxPlatform.hpp"

#include <SDL_stdinc.h>
#include <sodium.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <vector>

namespace nexus { namespace net {

    /**
     * @brief PacketHeader is sent at the beginning of all messages.
     *
     * The template allows us to use an "enum class" to ensure that the message
     * types are valid at compile time.
     *
     * @tparam T_Packet The packet type.
     */
    template <typename T_Packet>
    struct NEXUS_API PacketHeader
    {
        T_Packet id{};
        Uint32 size = 0;
        Uint8 nonce[crypto_secretbox_NONCEBYTES];   ///< Used during encryption, so that two identical packets do not give the same result
    };

    /**
     * @brief PacketBody contains a header and a std::vector containing raw data bytes.
     *
     * This design allows the packet to have variable length, but the size in the header must be updated accordingly.
     *
     * @tparam T_PacketID The packet ID type.
     */
    template <typename T_PacketID>
    struct NEXUS_API Packet
    {
        PacketHeader<T_PacketID> header{};
        std::vector<Uint8> body;

        /**
         * @brief Default constructor to create an empty packet.
         */
        Packet() = default;

        /**
         * @brief Constructor to create a packet with a given ID but without data.
         *
         * @param id The ID for the packet header.
         */
        Packet(T_PacketID id) : header{id, 0, {}} { }

        /**
         * @brief Constructor for creating a packet with a given ID and data.
         *
         * @tparam T_Data The type of data to push into the packet.
         * @param id The ID for the packet header.
         * @param data The data to push into the packet body.
         */
        template<typename T_Data>
        Packet(T_PacketID id, const T_Data& data) : header{id, 0, {}}
        {
            (*this) << data;
        }

        /**
         * @brief Returns the size of the entire Packet body in bytes.
         *
         * @return The size of the packet body.
         */
        inline size_t size() const { return body.size(); }

        /**
         * @brief Clears the content of the Packet body, setting its size to 0.
         *
         * This function clears the body of the Packet and resets the size of the packet
         * body to 0, effectively making the Packet empty. It also returns a reference to
         * the modified Packet for chaining purposes.
         *
         * @return A reference to the modified Packet.
         */
        inline Packet<T_PacketID>& clear()
        {
            header.size = 0;
            body.clear();
            return *this;
        }

        inline void replace(const Uint8* data, size_t size)
        {
            header.size = size;
            body.resize(size);
            std::memcpy(body.data(), data, size);
        }

        /**
         * @brief Override for std::cout compatibility, produces a user-friendly description of the Packet.
         *
         * @param os The output stream.
         * @param pck The packet to output.
         * @return The output stream.
         */
        friend std::ostream& operator << (std::ostream& os, const Packet<T_PacketID>& packet)
        {
            os << "ID:" << packet.header.id << " Size:" << packet.header.size;
            return os;
        }

        // Convenience Operator overloads - These allow us to add and remove data from
        // the body vector as if it were a stack, using a First In, Last Out (FILO) approach.
        // These are templated because the data type can vary, so they handle various data types.
        // NOTE: They assume the data type is Plain Old Data (POD). In short, these functions
        // serialize and deserialize data into/from a vector.

        /**
         * @brief Pushes any Plain Old Data (POD)-like data into the Packet buffer.
         *
         * @tparam T_Data The type of data to push.
         * @param pck The packet to push data into.
         * @param data The data to push.
         * @return The target packet for chaining.
         */
        template<typename T_Data>
        friend Packet<T_PacketID>& operator << (Packet<T_PacketID>& packet, const T_Data& data)
        {
            // Check that the type of the data being pushed is trivially copyable
            static_assert(std::is_standard_layout<T_Data>::value, "Data is too complex to be pushed into the vector");

            // Cache the current size of the vector, as this will be the point where we insert the data
            size_t i = packet.body.size();

            // Resize the vector by the size of the data being pushed
            packet.body.resize(packet.body.size() + sizeof(T_Data));

            // Physically copy the data into the newly allocated vector space
            std::memcpy(packet.body.data() + i, &data, sizeof(T_Data));

            // Recalculate the Packet size
            packet.header.size = packet.size();

            // Return the target Packet so it can be "chained"
            return packet;
        }

        /**
         * @brief Pulls any Plain Old Data (POD)-like data from the Packet buffer.
         *
         * @tparam T_Data The type of data to pull.
         * @param pck The packet to pull data from.
         * @param data The data to pull into.
         * @return The target packet for chaining.
         */
        template<typename T_Data>
        friend Packet<T_PacketID>& operator >> (Packet<T_PacketID>& packet, T_Data& data)
        {
            // Check that the type of the data being pulled is trivially copyable
            static_assert(std::is_standard_layout<T_Data>::value, "Data is too complex to be pulled from the vector");

            // Cache the location towards the end of the vector where the pulled data starts
            size_t i = packet.body.size() - sizeof(T_Data);

            // Physically copy the data from the vector into the user variable
            std::memcpy(&data, packet.body.data() + i, sizeof(T_Data));

            // Shrink the vector to remove read bytes and reset the end position
            packet.body.resize(i);

            // Recalculate the Packet size
            packet.header.size = packet.size();

            // Return the target Packet so it can be "chained"
            return packet;
        }
    };

    // An "owned" Packet is identical to a regular Packet but is associated with
    // a connection. On a server, the owner would be the client that sent the Packet, 
    // on a client, the owner would be the server.

    // Forward declare the connection
    template <typename T_PacketID>
    class Connection;

    /**
     * @brief An "owned" Packet is identical to a regular Packet but is associated with a connection.
     *
     * On a server, the owner would be the client that sent the Packet, on a client, the owner would be the server.
     *
     * @tparam T_PacketID The packet ID type.
     */
    template <typename T_PacketID>
    struct OwnedPacket : Packet<T_PacketID>
    {
        std::shared_ptr<Connection<T_PacketID>> remote;

        OwnedPacket(const Packet<T_PacketID>& packet, const std::shared_ptr<Connection<T_PacketID>>& _remote = nullptr)
            : Packet<T_PacketID>(packet), remote(_remote) { }

        inline Packet<T_PacketID>& operator*() noexcept
        {
            return *(reinterpret_cast<Packet<T_PacketID>*>(this));
        }
    };

}}

#endif //RAYFLEX_NET_PACKET_HPP