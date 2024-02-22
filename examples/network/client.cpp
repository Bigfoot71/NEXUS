#include "common.hpp"
#include "core/nxText.hpp"

using namespace nexus;

class Game : public gfx::State, net::ClientInterface<GameMsg>
{
  private:
    gfx::Font font;

  private:
	std::unordered_map<uint32_t, PlayerDescription> mapObjects;
	PlayerDescription descPlayer;
	uint32_t playerID = 0;

    core::RandomGenerator gen;
    float waitingTimeAnim = 0.0f;
	bool waitingForConnection = true;

  public:

    /* CLIENT PART */

    void GetPackets()
    {
		if (!IsConnected()) return;

        while (!Incoming().empty())
        {
            auto packet = *(Incoming().pop_front());

            switch (packet.header.id)
            {
                // Après la connexion, nous devrions recevoir cette reponse d'acceptation
                // Si nous la recevons nous lui envoyons donc notre entité (joueur)
                case(GameMsg::Client_Accepted):
                {
                    NEXUS_LOG(Info) << "Server accepted client!\n";
                    net::Packet<GameMsg> response;
                    response.header.id = GameMsg::Client_RegisterWithServer;
                    descPlayer.position = gen.RandomVec2({ 0, 0 }, app.window.GetSize());
                    for (int i = 0; i < 7; i++) descPlayer.name[i] = gen.RandomChar();
                    descPlayer.name[7] = '\0';
                    response << descPlayer;
                    Send(response);
                    break;
                }

                // Après que nous ayaons envoyé notre entité,
                // le serveur nous renvoie un ID unique
                case(GameMsg::Client_AssignID):
                {
                    packet >> playerID;
                    NEXUS_LOG(Info) << "Assigned Client ID! [ID " << playerID << "]\n";
                    break;
                }

                // Ici nous gérons tous les nouveaux joueurs entrant
                // Notemment nous même juste après GameMsg::AssignID
                case(GameMsg::Game_AddPlayer):
                {
                    PlayerDescription desc;
                    packet >> desc;
                    mapObjects.insert_or_assign(desc.uniqueID, desc);
                    if (desc.uniqueID == playerID) waitingForConnection = false;    // Si le nouveau joueur s'agit de nous, nous n'avons plus à attendre
                    break;
                }

                // Ici nous gérons les deconnexions
                case(GameMsg::Game_RemovePlayer):
                {
                    uint32_t removalID = 0;
                    packet >> removalID;
                    mapObjects.erase(removalID);
                    break;
                }

                // Ici nous gérons les mise à jour
                case(GameMsg::Game_UpdatePlayer):
                {
                    PlayerDescription desc;
                    packet >> desc;
                    mapObjects.insert_or_assign(desc.uniqueID, desc);
                    break;
                }

                default: break;
            }
        }
    }

    void SendPacket()
    {
        // Send player description
        net::Packet<GameMsg> ownPacket;
        ownPacket.header.id = GameMsg::Game_UpdatePlayer;
        ownPacket << mapObjects[playerID];
        Send(ownPacket);
    }

    /* STATE PART */

    Game(gfx::App& app) : State(app)
    , font(app.renderer, RESOURCES_PATH "fonts/Futurette.otf", 24)
    { }

    void Enter() override
    {
        if (!Connect("127.0.0.1", 60000))
        {
            NEXUS_LOG(Error) << "Unable to connect to server\n";
            app.Finish();
        }
    }

    void Exit() override
    {
        Disconnect();
    }

    void Update(float dt) override
    {
        GetPackets();

        if (waitingForConnection)
        {
            waitingTimeAnim += dt;
            return;
        }

		// Control of Player Object
		mapObjects[playerID].velocity = math::Vec2(
            static_cast<float>(input::IsKeyPressed(input::Scancode::Right) - input::IsKeyPressed(input::Scancode::Left)),
            static_cast<float>(input::IsKeyPressed(input::Scancode::Down) - input::IsKeyPressed(input::Scancode::Up))
        ).Normalized();

		// Update objects locally
		for (auto& object : mapObjects)
		{
			math::Vec2 potentialPosition = object.second.position + object.second.velocity * object.second.speed * dt;
			object.second.position = potentialPosition;
		}

        // send our packets
        SendPacket();
    }

    void Draw() override
    {
		if (waitingForConnection)
		{
            static const char text[] = "Waiting to connect...";
            int len = (sizeof(text) - 1) - (3 - (static_cast<int>(2.5f*waitingTimeAnim)%4));
            font->Draw(core::TextSubtext(text, 0, len), 10, 10);
			return;
		}

		for (const auto& object : mapObjects)
		{
            const auto &desc = object.second;

			// Draw entity
            app.renderer.SetColor(gfx::Red);
            app.renderer.DrawCircle(desc.position, desc.fRadius);

            // Draw name
            app.renderer.SetColor(gfx::White);
            font->Draw(desc.name, desc.position - desc.fRadius);
		}
    }
};

int main(int argv, char** args)
{
    gfx::App app("Network - Client", 800, 600);
    app.AddState<Game>("game");
    return app.Run("game");
}
