#ifndef NEXUS_NET_SERVER_INTERFACE_HPP
#define NEXUS_NET_SERVER_INTERFACE_HPP

#include "../utils/nxThreadSafeQueue.hpp"
#include "./nxConnection.hpp"
#include "./nxPacket.hpp"

namespace nexus { namespace net {

    /**
     * @brief Template class representing the interface for a server.
     * @tparam T_PacketID Type parameter for packet identification.
     */
    template<typename T_PacketID>
    class NEXUS_API ServerInterface
    {
      protected:
        utils::TSQueue<OwnedPacket<T_PacketID>> packetsIn;                      ///< Thread Safe Queue for incoming message packets
        std::deque<std::shared_ptr<Connection<T_PacketID>>> deqConnections;     ///< Container of active validated connections

        std::thread threadContext;              ///< The asio context will be launched in this thread
        asio::io_context asioContext;           ///< Will be used to manage inputs/outputs, timers, signals, all in an efficient and asynchronous way
        asio::ip::tcp::acceptor asioAcceptor;   ///< Handles new incoming connection attempts...

        Uint32 IDCounter = 0;                   ///< Clients will be identified in the "wider system" via an ID

      public:
        /**
         * @brief Constructor to create a server, ready to listen on the specified port.
         * @param port The port on which the server will listen.
         */
        ServerInterface(Uint16 port) : asioAcceptor(asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        {
            _net_impl::InitSodium(); // Initialize libsodium if necessary
        }

        /**
         * @brief Destructor to stop the server.
         */
        virtual ~ServerInterface() { Stop(); }

        /**
         * @brief Starts the server.
         * @return True if the server starts successfully, false otherwise.
         */
        bool Start();

        /**
         * @brief Stops the server.
         */
        void Stop();

        /**
         * @brief ASYNC - Instructs asio to wait for a connection.
         */
        void WaitForClientConnection();

        /**
         * @brief Sends a message to a specific client.
         * @param client The target client to send the message to.
         * @param packet The packet to be sent.
         */
        void SendPacket(std::shared_ptr<Connection<T_PacketID>> client, Packet<T_PacketID>& packet);

        /**
         * @brief Sends a message to all clients, except the specified client.
         * @param packet The packet to be sent.
         * @param clientToIgnore The client to be ignored (not to receive the message).
         */
        void SendPacketToAll(Packet<T_PacketID>& packet, std::shared_ptr<Connection<T_PacketID>> clientToIgnore = nullptr);

        /**
         * @brief Forces the server to respond to incoming messages.
         * @param maxMessages The maximum number of messages to process.
         * @param wait Whether to wait for messages if none are immediately available.
         */
        void Update(size_t maxMessages = -1, bool wait = false);

      protected:
        /**
         * @brief Called when a client connects; can veto the connection by returning false.
         * @param client The connected client.
         * @return True if the connection is accepted, false if vetoed.
         */
        virtual bool OnClientConnect(std::shared_ptr<Connection<T_PacketID>> client) { return false; }

        /**
         * @brief Called when a client appears to have disconnected.
         * @param client The disconnected client.
         */
        virtual void OnClientDisconnect(std::shared_ptr<Connection<T_PacketID>> client) { }

        /**
         * @brief Called when a message arrives.
         * @param client The client that sent the message.
         * @param packet The received packet.
         */
        virtual void OnReceivePacket(std::shared_ptr<Connection<T_PacketID>> client, Packet<T_PacketID>& packet) { }

      public:
        /**
         * @brief Called when a client is validated.
         * @param client The validated client.
         */
        virtual void OnClientValidated(std::shared_ptr<Connection<T_PacketID>> client) { }
    };


    // PUBLIC - IMPLEMENTATION //

    template<typename T_PacketID>
    bool ServerInterface<T_PacketID>::Start()
    {
        try
        {
            // Issue a task to the asio context - This is important
            // as it will prime the context with "work" and stop it
            // from exiting immediately. Since this is a server, we 
            // want it primed and ready to handle clients trying to
            // connect.
            WaitForClientConnection();

            // Launch the asio context in its own thread
            threadContext = std::thread([this]() { asioContext.run(); });
        }
        catch (std::exception& e)
        {
            // Something prohibited the server from listening
            std::cerr << "SERVER: Exception - " << e.what() << "\n";
            return false;
        }

        std::cout << "SERVER: Started!" << "\n";
        return true;
    }

    template<typename T_PacketID>
    void ServerInterface<T_PacketID>::Stop()
    {
        asioContext.stop();                                     ///< Request the context to close
        if (threadContext.joinable()) threadContext.join();     ///< Tidy up the context thread
        std::cout << "SERVER: Stopped!" << "\n";                ///< Inform someone, anybody, if they care...
    }

    template<typename T_PacketID>
    void ServerInterface<T_PacketID>::WaitForClientConnection()
    {
        // Prime context with an instruction to wait until a socket connects. This
        // is the purpose of an "acceptor" object. It will provide a unique socket
        // for each incoming connection attempt
        asioAcceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                // Triggered by incoming connection request
                if (!ec)
                {
                    const auto &incomingAddress = socket.remote_endpoint();

                    // Display some useful(?) information
                    std::cout << "SERVER: New Connection [IP " << incomingAddress << "]" << "\n";

                    // Create a new connection to handle this client 
                    std::shared_ptr<Connection<T_PacketID>> newconn = std::make_shared<Connection<T_PacketID>>(
                        Connection<T_PacketID>::Owner::SERVER,
                        asioContext, std::move(socket), packetsIn);

                    // Give the user server a chance to deny connection
                    if (OnClientConnect(newconn))
                    {								
                        // Connection allowed, so add to container of new connections
                        deqConnections.push_back(std::move(newconn));

                        // And very important! Issue a task to the connection's
                        // asio context to sit and wait for bytes to arrive!
                        deqConnections.back()->ConnectToClient(this, IDCounter++);

                        std::cout << "SERVER: Connection Approved [ID " << deqConnections.back()->GetID() << "] [IP " << incomingAddress << "]" << std::endl;
                    }
                    else
                    {
                        std::cout << "SERVER: Connection Denied [IP " << incomingAddress << "]" << std::endl;

                        // Connection will go out of scope with no pending tasks, so will
                        // get destroyed automagically due to the wonder of smart pointers
                    }
                }
                else
                {
                    // Error has occurred during acceptance
                    std::cout << "SERVER: New Connection Error - " << ec.message() << "\n";
                }

                // Prime the asio context with more work - again simply wait for
                // another connection...
                WaitForClientConnection();
            });
    }

    template<typename T_PacketID>
    void ServerInterface<T_PacketID>::SendPacket(std::shared_ptr<Connection<T_PacketID>> client, Packet<T_PacketID>& packet)
    {
        // Check client is legitimate...
        if (client && client->IsConnected())
        {
            // ...and post the message via the connection
            client->Send(packet);
        }
        else
        {
            // If we can't communicate with the client then we may as 
            // well remove the client - let the server know, it may
            // be tracking it somehow
            OnClientDisconnect(client);

            // Off you go now
            client.reset();

            // Then physically remove it from the container
            deqConnections.erase(std::remove(deqConnections.begin(), deqConnections.end(), client), deqConnections.end());
        }
    }

    template<typename T_PacketID>
    void ServerInterface<T_PacketID>::SendPacketToAll(Packet<T_PacketID>& packet, std::shared_ptr<Connection<T_PacketID>> clientToIgnore)
    {
        bool invalidClientExists = false;

        // Iterate through all clients in the container
        for (auto& client : deqConnections)
        {
            // Check if the client is connected...
            if (client && client->IsConnected())
            {
                if(client != clientToIgnore) client->Send(packet);
            }
            else
            {
                // The client couldn't be contacted, so assume it has disconnected.
                OnClientDisconnect(client);
                client.reset();

                // Set this flag to then remove dead clients from the container
                invalidClientExists = true;
            }
        }

        // Remove dead clients, all in one go - this way, we don't invalidate the
        // container as we iterate through it.
        if (invalidClientExists)
        {
            deqConnections.erase(std::remove(deqConnections.begin(), deqConnections.end(), nullptr), deqConnections.end());
        }
    }

    template<typename T_PacketID>
    void ServerInterface<T_PacketID>::Update(size_t maxMessages, bool wait)
    {
        if (wait) packetsIn.wait();

        // Process as many messages as you can up to the value specified
        size_t messageCount = 0;
        while (messageCount < maxMessages && !packetsIn.empty())
        {
            auto packet = packetsIn.pop_front();                        ///< Grab the front message
            OnReceivePacket(packet.remote, *packet);    ///< Pass to the message handler
            messageCount++;
        }
    }

}}

#endif //NEXUS_NET_SERVER_INTERFACE_HPP
