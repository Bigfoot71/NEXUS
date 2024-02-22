#ifndef NEXUS_NET_CLIENT_INTERFACE_HPP
#define NEXUS_NET_CLIENT_INTERFACE_HPP

#include "../utils/nxThreadSafeQueue.hpp"
#include "./nxConnection.hpp"
#include "../core/nxLog.hpp"
#include "./nxPacket.hpp"

namespace nexus { namespace net {

    /**
     * @brief Template class representing the interface for a client.
     * @tparam T_PacketID Type parameter for packet identification.
     */
    template <typename T_PacketID>
    class NEXUS_API ClientInterface
    {
      private:
        utils::TSQueue<OwnedPacket<T_PacketID>> packetsIn;      ///< This is the thread-safe queue of incoming messages from the server

      protected:
        asio::io_context asioContext;                           ///< ASIO context handles the data transfer...
        std::thread threadContext;                              ///< ...but needs a thread of its own to execute its work commands
        std::unique_ptr<Connection<T_PacketID>> connection;     ///< The client has a single instance of a "connection" object, which handles data transfer

      public:
        /**
         * @brief Constructor for ClientInterface.
         */
        ClientInterface()
        {
            _net_impl::InitSodium();    // Initialize libsodium if necessary
        }
        
        /**
         * @brief Destructor for ClientInterface. Disconnects from the server.
         */
        virtual ~ClientInterface() { Disconnect(); }

        /**
         * @brief Connects to the server with the specified hostname/ip-address and port.
         * @param host The hostname or IP address of the server.
         * @param port The port number to connect to.
         * @return True if the connection is successful, false otherwise.
         */
        bool Connect(const std::string& host, const Uint16 port);

        /**
         * @brief Disconnects from the server.
         */
        void Disconnect();

        /**
         * @brief Checks if the client is connected to a server.
         * @return True if connected, false otherwise.
         */
        inline bool IsConnected() const
        {
            return (connection ? connection->IsConnected() : false);
        }

        /**
         * @brief Sends a packet to the server.
         * @param packet The packet to be sent.
         */
        inline void Send(Packet<T_PacketID>& packet)
        {
            if (IsConnected()) connection->Send(packet);
        }

        /**
         * @brief Retrieves the queue of incoming messages from the server.
         * @return The thread-safe queue of incoming messages.
         */
        utils::TSQueue<OwnedPacket<T_PacketID>>& Incoming()
        {
            return packetsIn;
        }
    };


    /* Public Implementation ClientInterface */

    template <typename T_PacketID>
    bool ClientInterface<T_PacketID>::Connect(const std::string& host, const Uint16 port)
    {
        try
        {
            // Resolve hostname/ip-address into tangible physical address
            asio::ip::tcp::resolver resolver(asioContext);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            // Create connection
            connection = std::make_unique<Connection<T_PacketID>>(
                Connection<T_PacketID>::Owner::CLIENT,
                asioContext, asio::ip::tcp::socket(asioContext), packetsIn);

            // Tell the connection object to connect to the server
            connection->ConnectToServer(endpoints);

            // Start Context Thread
            threadContext = std::thread([this]() { asioContext.run(); });
        }
        catch (std::exception& e)
        {
            NEXUS_LOG(Error) << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    template <typename T_PacketID>
    void ClientInterface<T_PacketID>::Disconnect()
    {
        // If connection exists, and it's connected, then disconnect from the server gracefully
        if(IsConnected()) connection->Disconnect();

        // Either way, we're also done with the ASIO context...
        asioContext.stop();
        // ...and its thread
        if (threadContext.joinable())
            threadContext.join();

        // Destroy the connection object
        connection.release();
    }

}}

#endif //NEXUS_NET_CLIENT_INTERFACE_HPP
