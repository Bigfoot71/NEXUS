#ifndef COMMON_HPP
#define COMMON_HPP

#include <nexus.hpp>

static constexpr Uint64 HANDSHAKE_SECRET = 0x53757065724C4F4C;

enum class GameMsg : Uint32
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
};

struct PlayerDescription
{
	Uint32 uniqueID = 0;
	char name[8]{};

    float speed     = 64.0f;
	float fRadius   = 24.0f;

	nexus::math::Vec2 position;
	nexus::math::Vec2 velocity;
};

#endif //COMMON_HPP