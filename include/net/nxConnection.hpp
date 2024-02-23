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

#ifndef NEXUS_NETWORK_CONNECTION_HPP
#define NEXUS_NETWORK_CONNECTION_HPP

#include "../platform/nxPlatform.hpp"

#include "../utils/nxThreadSafeQueue.hpp"
#include "./nxSecurity.hpp"
#include "./nxPacket.hpp"

#include <condition_variable>
#include <asio.hpp>
#include <deque>
#include <mutex>

/*
    TODO: Overload ReadValidation for server and client
          to allow const to be added to key parameters in
          ConnectToClient and ConnectToServer (where possible) (?)
*/

namespace nexus { namespace net {

    template<typename T_PacketID>
    class ServerInterface;

    /**
     * @brief Represents a network connection, which can be owned by a server or a client.
     *
     * The `Connection` class is responsible for managing a network connection, sending and receiving packets,
     * and performing handshake validation. It can be owned by a server or a client and behaves slightly differently
     * depending on its owner.
     *
     * @tparam T_PacketID The type of packet identifier.
     */
    template<typename T_PacketID>
    class NEXUS_API Connection : public std::enable_shared_from_this<Connection<T_PacketID>>, private _net_impl::HandshakeHandler
    {
      public:
        // A connection is "owned" by either a server or a client, and its
        // behavior is slightly different between the two.
        enum class Owner { SERVER, CLIENT };

      protected:
        asio::io_context& asioContext;                              ///< This context is shared with the whole asio instance
        asio::ip::tcp::socket socket;                               ///< Each connection has a unique socket to a remote
        std::unique_ptr<_net_impl::CryptoHandler> cryptoHandler;    ///< Encrypted communication handler
        utils::TSQueue<Packet<T_PacketID>> packetsOut;              ///< This queue holds all messages to be sent to the remote side of this connection
        utils::TSQueue<OwnedPacket<T_PacketID>>& packetsIn;         ///< This references the incoming queue of the parent object
        Packet<T_PacketID> packetTempIn;                            ///< Store partially assembled incoming messages asynchronously
        Owner ownerType = Owner::SERVER;                            ///< The "Owner" decides how some of the connection behaves
        Uint32 id = 0;                                            ///< ID of this connection that the server will give for example

      public:
        // Constructor: Specify Owner, connect to context, transfer the socket. Provide reference to incoming Packet queue
        Connection(Owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, utils::TSQueue<OwnedPacket<T_PacketID>>& qIn)
            : HandshakeHandler(), asioContext(asioContext), socket(std::move(socket)), packetsIn(qIn), ownerType(parent) { }

        virtual ~Connection() { }

        /**
         * @brief Get the unique ID of this connection, used system-wide.
         *
         * @return The ID of the connection.
         */
        inline Uint32 GetID() const { return id; }

        /**
         * @brief Connect to a client when the connection is owned by a server.
         *
         * @param server The server interface.
         * @param uid The user ID.
         */
        void ConnectToClient(ServerInterface<T_PacketID>* server, Uint32 uid);

        /**
         * @brief Connect to a server when the connection is owned by a client.
         *
         * @param endpoints The list of endpoints to connect to.
         */
        void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);

        /**
         * @brief Disconnect the connection.
         */
        void Disconnect();

        /**
         * @brief Check if the connection is established.
         *
         * @return True if the connection is established, false otherwise.
         */
        bool IsConnected() const;

        /**
         * @brief Prime the connection to wait for incoming messages.
         */
        void StartListening();

        /**
         * @brief ASYNC - Send a Packet to the remote side.
         *
         * @param packet The packet to send.
         */
        void Send(Packet<T_PacketID> packet);

      private:
        void WriteHeader();                                                                                             ///< ASYNC - Prime context to write a Packet header
        void WriteBody();                                                                                               ///< ASYNC - Prime context to write a Packet body

        void ReadHeader();                                                                                              ///< ASYNC - Prime context ready to read a Packet header
        void ReadBody();                                                                                                ///< ASYNC - Prime context ready to read a Packet body

        void WriteValidation();                                                                                         ///< ASYNC - Used by both client and server to write a validation packet
        void ReadValidationFromClient(ServerInterface<T_PacketID>* server, const _net_impl::KeyPair& serverKeyPair);    ///< ASYNC - For the server - TODO: Explain
        void ReadValidationFromServer();                                                                                ///< ASYNC - For the client - TODO: Explain

        void AddToIncomingMessageQueue();                                                                               ///< Once a full Packet is received, add it to the incoming queue
    };


    /* Public Implementation Connection */

    template<typename T_PacketID>
    void Connection<T_PacketID>::ConnectToClient(ServerInterface<T_PacketID>* server, Uint32 uid)
    {
        if (ownerType == Owner::SERVER)
        {
            if (socket.is_open())
            {
                id = uid;

                // Generate a temporary key pair for the generation of the final pair
                _net_impl::KeyPair serverKeyPair;

                // Prepare the handshake and the challenge packet that we
                // will send to the connecting client. The challenge
                // will also contain our generated public key for the occasion
                PrepareHandshakeForClient(serverKeyPair.Public());

                // A client has attempted to connect to the server, but we wish
                // the client to first validate itself, so first write out the
                // handshake data to be validated
                WriteValidation();

                // Next, issue a task to sit and wait asynchronously for precisely
                // the validation data sent back from the client
                ReadValidationFromClient(server, std::move(serverKeyPair));
            }
        }
    }

    template<typename T_PacketID>
    void Connection<T_PacketID>::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
    {
        // Only clients can connect to servers
        if (ownerType == Owner::CLIENT)
        {
            // Request asio attempts to connect to an endpoint
            asio::async_connect(socket, endpoints,
                [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                {
                    // First thing server will do is send a packet to be validated, so wait for that and respond
                    if (!ec) ReadValidationFromServer();
                });
        }
    }

    template<typename T_PacketID>
    void Connection<T_PacketID>::Disconnect()
    {
        if (IsConnected())
            asio::post(asioContext, [this]() { socket.close(); });
    }

    template<typename T_PacketID>
    bool Connection<T_PacketID>::IsConnected() const
    {
        return socket.is_open();
    }

    // ASYNC - Send a Packet, connections are one-to-one so no need to specify
    // the target, for a client, the target is the server and vice versa
    template<typename T_PacketID>
    void Connection<T_PacketID>::Send(Packet<T_PacketID> packet)
    {
        if (cryptoHandler == nullptr)
        {
            std::cout << "CONNECTION: Unencrypted communication attempt aborted" << std::endl;
            return;
        }

        asio::post(asioContext, [this, packet]()
        {
            // Make a copy of the packet to avoid concurrent access
            // and consider the fact that the same packet may be
            // sent to multiple clients simultaneously. Thus, we
            // avoid encrypting the original packet multiple times.

            // Using std::move is the only option that seems to work here (?) (don't remind me, to think about it)
            Packet<T_PacketID> mutablePacket = std::move(packet);

            // Attempt to encrypt the packet
            if (!cryptoHandler->Encrypt(mutablePacket))
            {
                std::cout << "CONNECTION: Encryption of a packet failed, sending canceled" << std::endl;
                return;
            }

            // Add the packet to the queue to be sent
            bool writingMessage = !packetsOut.empty();
            packetsOut.push_back(mutablePacket);
            if (!writingMessage) WriteHeader();
        });
    }


    // PRIVATE - IMPLEMENTATION //

    // ASYNC - Prime the context to write a Packet header
    template<typename T_PacketID>
    void Connection<T_PacketID>::WriteHeader()
    {
        // If this function is called, we know that the outgoing Packet queue must have
        // at least one Packet to send. Allocate a transmission buffer to hold the
        // Packet and issue the work to asio to send these bytes.
        asio::async_write(socket, asio::buffer(&packetsOut.front().header, sizeof(PacketHeader<T_PacketID>)),
            [this](std::error_code ec, std::size_t length)
            {
                // Asio has sent the bytes; if there was no error...
                if (!ec)
                {
                    // Check if the Packet header just sent also has a Packet body...
                    if (packetsOut.front().body.size() > 0)
                    {
                        // It does, so issue the task to write the body bytes
                        WriteBody();
                    }
                    else
                    {
                        // No body, so we are done with this Packet. Remove it from the
                        // outgoing Packet queue.
                        packetsOut.pop_front();

                        // If the queue is not empty, there are more messages to send,
                        // so make this happen by issuing the task to send the next header.
                        if (!packetsOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                }
                else
                {
                    // Asio failed to write the Packet; assume the connection has
                    // died by closing the socket. A future attempt to write to
                    // this client will fail due to the closed socket and will be tidied up.
                    std::cout << "CONNECTION: [ID " << id << "] Write Header Fail. " << ec.message() << std::endl;
                    socket.close();
                }
            });
    }

    // ASYNC - Prime the context to write a Packet body
    template<typename T_PacketID>
    void Connection<T_PacketID>::WriteBody()
    {
        // If this function is called, a header has just been sent, and that header
        // indicated a body existed for this Packet. Populate a transmission buffer
        // with the body data and initiate the asynchronous write operation.
        asio::async_write(socket, asio::buffer(packetsOut.front().body.data(), packetsOut.front().body.size()),
            [this](std::error_code ec, std::size_t length)
            {
                // If the write operation was successful...
                if (!ec)
                {
                    // We are done with the Packet and remove it from the queue.
                    packetsOut.pop_front();

                    // If the queue still has messages, issue the task to send
                    // the next message's header.
                    if (!packetsOut.empty())
                    {
                        WriteHeader();
                    }
                }
                else
                {
                    // The write operation failed; refer to the WriteHeader() equivalent for details.
                    std::cout << "CONNECTION: [ID " << id << "] Write Body Fail. " << ec.message() << std::endl;
                    socket.close();
                }
            });
    }

    // ASYNC - Prime the context ready to read a Packet header
    template<typename T_PacketID>
    void Connection<T_PacketID>::ReadHeader()
    {
        // If this function is called, we expect asio to wait until it receives
        // enough bytes to form the header of a Packet. Allocate a transmission buffer
        // large enough to store it, and construct the Packet in a temporary object.
        asio::async_read(socket, asio::buffer(&packetTempIn.header, sizeof(PacketHeader<T_PacketID>)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    // A complete Packet header has been read; check if this Packet
                    // has a body to follow...
                    if (packetTempIn.header.size > 0)
                    {
                        // It does... So allocate enough space in the message's body
                        // vector and issue asio the task to read the body.
                        packetTempIn.body.resize(packetTempIn.header.size);
                        ReadBody();
                    }
                    else
                    {
                        // There's no body, so add this bodyless Packet to the connection's
                        // incoming Packet queue.
                        AddToIncomingMessageQueue();
                    }
                }
                else
                {
                    // Reading from the client went wrong, likely due to a disconnect.
                    // Close the socket, and let the system tidy it up later.
                    std::cout << "CONNECTION: [ID " << id << "] Read Header Fail. " << ec.message() << std::endl;
                    socket.close();
                }
            });
    }

    // ASYNC - Prime the context ready to read a Packet body
    template<typename T_PacketID>
    void Connection<T_PacketID>::ReadBody()
    {
        // If this function is called, a header has already been read, and that header
        // indicates that we should read a body. The space for that body has already been
        // allocated in the temporary Packet object, so we wait for the bytes to arrive...
        asio::async_read(socket, asio::buffer(packetTempIn.body.data(), packetTempIn.body.size()),
            [this](std::error_code ec, std::size_t length)
            {
                // If the read operation was successful...
                if (!ec)
                {
                    // The Packet is now complete, so add it to the incoming queue.
                    AddToIncomingMessageQueue();
                }
                else
                {
                    // The read operation failed; refer to the comments above.
                    std::cout << "CONNECTION: [ID " << id << "] Read Body Fail. " << ec.message() << std::endl;
                    socket.close();
                }
            });
    }

    // ASYNC - Used by both client and server to write a validation packet
    template<typename T_PacketID>
    void Connection<T_PacketID>::WriteValidation()
    {
        if (ownerType == Owner::SERVER)
        {
            // For servers, asynchronously write the validation packet.
            asio::async_write(socket, asio::buffer(&handshakeOut, sizeof(handshakeOut)),
                [this](std::error_code ec, std::size_t length) { if (ec) socket.close(); });
        }
        else // CLIENT
        {
            // For clients, asynchronously write the validation packet and wait for a response.
            asio::async_write(socket, asio::buffer(&handshakeOut, sizeof(handshakeOut)),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec) ReadHeader();   // Validation data sent; clients should wait for a response or closure.
                    else socket.close();
                });
        }
    }

    template<typename T_PacketID>
    void Connection<T_PacketID>::ReadValidationFromClient(ServerInterface<T_PacketID>* server, const _net_impl::KeyPair& serverKeyPair)
    {
        if (ownerType == Owner::SERVER)
        {
            // For servers, asynchronously read the validation packet from the client.
            asio::async_read(socket, asio::buffer(&handshakeIn, sizeof(handshakeIn)),
                [this, server, serverKeyPair](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        // Buffer to store the client's public key
                        Uint8 clientPublicKey[_net_impl::CryptoKeySize];

                        // Compare the sent data to the expected solution
                        if (IsClientHandshakeValid(clientPublicKey))
                        {
                            // Client provided a valid solution
                            std::cout << "CONNECTION: Client Validated" << std::endl;

                            // Generate an encrypted communication key pair with the client
                            cryptoHandler = std::make_unique<_net_impl::CryptoHandler>(
                                serverKeyPair, clientPublicKey, true);

                            // Allow the client to connect properly
                            server->OnClientValidated(this->shared_from_this());

                            // Sit and wait to receive data now
                            ReadHeader();
                        }
                        else
                        {
                            // Client provided incorrect data, so disconnect
                            std::cout << "CONNECTION: Client Disconnected (Fail Validation)" << std::endl;
                            socket.close();
                        }
                    }
                    else
                    {
                        // A significant failure occurred
                        std::cout << "CONNECTION: Client Disconnected (ReadValidation)" << std::endl;
                        socket.close();
                    }
            });
        }
    }

    template<typename T_PacketID>
    void Connection<T_PacketID>::ReadValidationFromServer()
    {
        if (ownerType == Owner::CLIENT)
        {
            asio::async_read(socket, asio::buffer(&handshakeIn, sizeof(handshakeIn)),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        _net_impl::KeyPair clientKeyPair;                                   // Generate a temporary key pair for the final pair generation
                        Uint8 serverPublicKey[_net_impl::CryptoKeySize];                    // Buffer to store the server's public key

                        ResolveServerHandshake(serverPublicKey, clientKeyPair.Public());    // Connection is a client, so solve the puzzle
                        WriteValidation();                                                  // Write the result

                        // Generate an encrypted communication key pair with the server.
                        // Although it's not guaranteed to work at this moment, we might still
                        // get disconnected by the server if the connection challenge was not successful
                        // (if the challenge fails, the key is invalid).
                        cryptoHandler = std::make_unique<_net_impl::CryptoHandler>(clientKeyPair, serverPublicKey, false);
                    }
                    else
                    {
                        // A significant failure occurred.
                        std::cout << "CONNECTION: Server Disconnected (ReadValidation)" << std::endl;
                        socket.close();
                    }
            });
        }
    }

    // Once a full Packet is received, add it to the incoming queue
    template<typename T_PacketID>
    void Connection<T_PacketID>::AddToIncomingMessageQueue()
    {
        // Decrypt the incoming packet first if the cryptoHandler is already created.
        if (cryptoHandler != nullptr)
        {
            if (!cryptoHandler->Decrypt(packetTempIn))
            {
                std::cout << "CONNECTION: Decryption of a received packet impossible; the packet was ignored" << std::endl;
                return;
            }
        }

        // Add the packet to the incoming queue, converting it to an "owned Packet" by initializing
        // with a shared pointer from this connection object (if the owner is a server).
        packetsIn.push_back(OwnedPacket<T_PacketID>(packetTempIn,
            (ownerType == Owner::SERVER) ? this->shared_from_this() : nullptr));

        // Prime the asio context to receive the next Packet. It will sit and wait for bytes to arrive,
        // and the Packet construction process repeats itself.
        ReadHeader();
    }
}}

#endif //NEXUS_NETWORK_CONNECTION_HPP