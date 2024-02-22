#include "net/nxSecurity.hpp"
#include <SDL_stdinc.h>
#include <algorithm>
#include <cstring>

using namespace nexus;

/* LibSodium management */

namespace { bool SodiumInitialized = false; }

void _net_impl::InitSodium()
{
    if (SodiumInitialized)
    {
        return;
    }

    if (sodium_init() < 0)
    {
        throw std::runtime_error("Sodium initialization failed");
    }

    SodiumInitialized = true;
}

/* HandshakeHandler implementation */

void _net_impl::HandshakeHandler::Scramble(Uint8 data[ChallengeSize]) const
{
    // NOTE: This is a reversible mixing function

    // Loop through 8-byte blocks
    for (Uint8 i = 0; i < ChallengeSize; i += 8)
    {
        Uint64* block = reinterpret_cast<Uint64*>(data + i);

        Uint64 scrambled = 0;
        constexpr Uint64 mask = 0x00000000000000FF;

        // Mix the bytes in the block
        scrambled |= ((*block & mask) << 56);
        scrambled |= ((*block & (mask << 8)) << 40);
        scrambled |= ((*block & (mask << 16)) << 24);
        scrambled |= ((*block & (mask << 24)) << 8);
        scrambled |= ((*block & (mask << 32)) >> 8);
        scrambled |= ((*block & (mask << 40)) >> 24);
        scrambled |= ((*block & (mask << 48)) >> 40);
        scrambled |= ((*block & (mask << 56)) >> 56);

        *block = scrambled;
    }
}

void _net_impl::HandshakeHandler::PrepareHandshakeForClient(const Uint8 serverPublicKey[CryptoKeySize])
{
    auto packet = reinterpret_cast<Handshake*>(handshakeOut);

    // Generate a random challenge for the client
    randombytes(handshakeCheck, sizeof(handshakeCheck));
    std::memcpy(packet->challenge, handshakeCheck, sizeof(handshakeCheck));

    // Mix the random challenge; the client must unscramble it
    Scramble(packet->challenge);

    // Copy the server's public key into the packet
    std::memcpy(packet->publicKey, serverPublicKey, CryptoKeySize);

    // The packet is now ready for transmission!
}

bool _net_impl::HandshakeHandler::IsClientHandshakeValid(Uint8 clientPublicKey[CryptoKeySize]) const
{
    auto packet = reinterpret_cast<const Handshake*>(handshakeIn);

    // Check if the challenge sent matches the expected challenge
    for (Uint8 i = 0; i < ChallengeSize; i++)
    {
        if (packet->challenge[i] != handshakeCheck[i])
        {
            return false;
        }
    }

    // Client successfully solved the challenge; retrieve their public key
    std::memcpy(clientPublicKey, packet->publicKey, CryptoKeySize);

    return true;
}

void _net_impl::HandshakeHandler::ResolveServerHandshake(Uint8 serverPublicKey[CryptoKeySize], const Uint8 clientPublicKey[CryptoKeySize])
{
    std::memcpy(handshakeOut, handshakeIn, sizeof(Handshake));
    auto packet = reinterpret_cast<Handshake*>(handshakeOut);

    // Unscramble the challenge value
    Scramble(packet->challenge);

    // Copy the server's public key from the packet
    std::memcpy(serverPublicKey, packet->publicKey, CryptoKeySize);

    // Write our own public key into the packet to be sent back
    std::memcpy(packet->publicKey, clientPublicKey, CryptoKeySize);

    // The packet is now ready for transmission!
}
