#include "common.hpp"

using namespace nexus;

class GameServer : public net::ServerInterface<GameMsg>
{
  public:
    std::unordered_map<Uint32, PlayerDescription> mapPlayerRoster;
    std::vector<Uint32> garbageIDs;

    GameServer(Uint16 port) : net::ServerInterface<GameMsg>(port) { }

  protected:
    bool OnClientConnect(std::shared_ptr<net::Connection<GameMsg>> client) override
    {
        return true; // Here we accept everyone
    }

    void OnClientValidated(std::shared_ptr<net::Connection<GameMsg>> client) override
    {
        // The client has passed the validation check, so they are sent
        // a message informing them that they can continue communicating
        net::Packet<GameMsg> packet(GameMsg::Client_Accepted);
        client->Send(packet);
    }

    void OnClientDisconnect(std::shared_ptr<net::Connection<GameMsg>> client) override
    {
        if (client)
        {
            // We remove the client from the server, only if it has already been added
            if (mapPlayerRoster.find(client->GetID()) != mapPlayerRoster.end())
            {
                auto& pd = mapPlayerRoster[client->GetID()];
                std::cout << "SERVER: Ungraceful removal [ID " << std::to_string(pd.uniqueID) << "]" << std::endl;
                mapPlayerRoster.erase(client->GetID());
                garbageIDs.push_back(client->GetID());
            }
        }

    }

    void OnReceivePacket(std::shared_ptr<net::Connection<GameMsg>> client, net::Packet<GameMsg>& packet) override
    {
        // First of all, if a customer logged out we
        // kept their ID, so we sent their old ID to
        // all customers to let them know that it should be removed
        if (!garbageIDs.empty())
        {
            for (auto pid : garbageIDs)
            {
                std::cout << "Removing " << pid << std::endl;
                net::Packet<GameMsg> packet(GameMsg::Game_RemovePlayer, pid);
                SendPacketToAll(packet);
            }
            garbageIDs.clear();
        }

        /* Management of received packets */

        switch (packet.header.id)
        {
            case GameMsg::Client_RegisterWithServer:
            {
                PlayerDescription desc;
                packet >> desc;
                desc.uniqueID = client->GetID();
                mapPlayerRoster.insert_or_assign(desc.uniqueID, desc);

                // Send unique ID to new player
                net::Packet<GameMsg> p1(GameMsg::Client_AssignID, desc.uniqueID);
                SendPacket(client, p1);

                // Send new player description to all
                net::Packet<GameMsg> p2(GameMsg::Game_AddPlayer, desc);
                SendPacketToAll(p2);

                // Send all players descriptions to new player
                for (const auto& player : mapPlayerRoster)
                {
                    net::Packet<GameMsg> p(GameMsg::Game_AddPlayer, player.second);
                    SendPacket(client, p);
                }

                break;
            }

            case GameMsg::Client_UnregisterWithServer:
            {
                break;
            }

            case GameMsg::Game_UpdatePlayer:
            {
                // A client sent their entity (player) update
                // So we send the update to everyone, except the incoming client.
                SendPacketToAll(packet, client);
                break;
            }

            default: break;
        }
    }
};

int main(int argv, char** args)
{
    GameServer server(60000);
    server.Start();

    for (;;)
    {
        server.Update(-1, true);
    }

    return 0;
}
