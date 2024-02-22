#ifndef NEXUS_NETWORK_SECURITY_HPP
#define NEXUS_NETWORK_SECURITY_HPP

#include "./nxPacket.hpp"

#include <SDL_stdinc.h>
#include <stdexcept>
#include <sodium.h>
#include <random>
#include <chrono>
#include <vector>

namespace _net_impl {

    // Size of encryption keys
    constexpr Uint8 CryptoKeySize = 32;

    /**
     * @brief Initializes the LibSodium library.
     * If LibSodium is already initialized, this function does nothing.
     */
    void InitSodium();

    /**
     * @brief A handshake handler for nexus::net::Connection.
     * Manages handshakes to ensure pairs can communicate correctly,
     * challenges each new connection with a random challenge, and
     * exchanges public keys for generating encryption/decryption keys locally.
     */
    class HandshakeHandler
    {
      private:
        static constexpr Uint8 ChallengeSize = 32;

      private:
        struct Handshake
        {
            Uint8 publicKey[CryptoKeySize];               ///< Public key of the sending pair
            Uint8 challenge[ChallengeSize];               ///< Randomly generated challenge for the client to solve
        };

      private:
        /**
         * @brief Reversibly scrambles the data in blocks of 8 bytes.
         * @param data The data to be scrambled.
         */
        void Scramble(Uint8 data[ChallengeSize]) const;   ///< Reversible mixing function for handshake challenge

      protected:
        Uint8 handshakeOut[sizeof(Handshake)];            ///< Handshake packet to be sent
        Uint8 handshakeIn[sizeof(Handshake)];             ///< Received handshake packet
        Uint8 handshakeCheck[ChallengeSize];              ///< Expected result by the server

        /* SERVER-SIDE Operations */

        /**
         * @brief Prepares a handshake packet for a client.
         * @param serverPublicKey The public key of the server.
         */
        void PrepareHandshakeForClient(const Uint8 serverPublicKey[CryptoKeySize]);

        /**
         * @brief Checks if the client's handshake is valid.
         * @param clientPublicKey The public key of the client.
         * @return True if the client's handshake is valid, false otherwise.
         */
        bool IsClientHandshakeValid(Uint8 clientPublicKey[CryptoKeySize]) const;

        /* CLIENT-SIDE Operations */

        /**
         * @brief Resolves the server's handshake.
         * @param serverPublicKey The public key of the server.
         * @param clientPublicKey The public key of the client.
         */
        void ResolveServerHandshake(Uint8 serverPublicKey[CryptoKeySize], const Uint8 clientPublicKey[CryptoKeySize]);
    };

    /**
     * @brief A utility class for generating pairs of random keys
     * for use with CryptoHandler to generate encryption/decryption keys.
     * (asymmetric encryption)
     */
    class KeyPair
    {
      private:
        Uint8 publicKey[CryptoKeySize];
        Uint8 privateKey[CryptoKeySize];

      public:
        KeyPair() { crypto_kx_keypair(publicKey, privateKey); }
        inline const Uint8* Public() const noexcept { return publicKey; }
        inline const Uint8* Private() const noexcept { return privateKey; }
    };

    /**
     * @brief Used by Network::Connection for asymmetric encryption of communications.
     */
    class CryptoHandler
    {
      private:
        Uint8 decryptKey[CryptoKeySize];
        Uint8 encryptKey[CryptoKeySize];

      public:
        CryptoHandler() : decryptKey{}, encryptKey{} { }

        /**
         * @brief Constructor for CryptoHandler.
         * @param ownKeys The key pair of the local entity.
         * @param targetPublicKey The public key of the target entity.
         * @param isServer Flag indicating if the local entity is a server.
         */
        CryptoHandler(const KeyPair& ownKeys, const Uint8* targetPublicKey, bool isServer)
        {
            auto genKeys = (isServer ? crypto_kx_server_session_keys : crypto_kx_client_session_keys);
            if (genKeys(decryptKey, encryptKey, ownKeys.Public(), ownKeys.Private(), targetPublicKey) != 0)
            {
                throw std::runtime_error("Suspicious target public key"); // TODO: Handled this type of case
            }
        }

        /**
         * @brief Encrypts the packet using asymmetric encryption.
         * @tparam T_PacketID Type of packet identifier.
         * @param packet The packet to be encrypted.
         * @return True if encryption is successful, false otherwise.
         */
        template <typename T_PacketID>
        bool Encrypt(nexus::net::Packet<T_PacketID>& packet) const
        {
            // If the packet does not contain a body, there is no need to encrypt it
            if (packet.header.size == 0) return true;

            // Generate a unique nonce for each packet
            randombytes_buf(packet.header.nonce, crypto_secretbox_NONCEBYTES);

            // Encrypt the packet body (returns false if failure)
            Uint8 encrypted[packet.body.size() + crypto_secretbox_MACBYTES];
            if (crypto_secretbox_easy(encrypted, packet.body.data(), packet.body.size(), packet.header.nonce, encryptKey) != 0)
            {
                return false;
            }

            // Replaced the body of the packet with its encrypted version
            packet.replace(encrypted, sizeof(encrypted));

            return true;
        }

        /**
         * @brief Decrypts the packet using asymmetric encryption.
         * @tparam T_PacketID Type of packet identifier.
         * @param packet The packet to be decrypted.
         * @return True if decryption is successful, false otherwise.
         */
        template <typename T_PacketID>
        bool Decrypt(nexus::net::Packet<T_PacketID>& packet) const
        {
            // If the packet does not contain a body, no need to decrypt
            if (packet.header.size == 0) return true;

            // Decrypt the body of the packet (returns false if unsuccessful)
            Uint8 decrypted[packet.body.size() - crypto_secretbox_MACBYTES];
            if (crypto_secretbox_open_easy(decrypted, packet.body.data(), packet.body.size(), packet.header.nonce, decryptKey) != 0)
            {
                return false;
            }

            // Replace the body of the packet with its decrypted version
            packet.replace(decrypted, sizeof(decrypted));

            // Fill the nonce with zeros (so we can recognize if it is encrypted or not)
            std::fill(packet.header.nonce, packet.header.nonce + crypto_secretbox_NONCEBYTES, 0);

            return true;
        }
    };

    /**
     * @brief Manages encryption and decryption of packets for Network::Connection.
     */
    class EncryptionHandler
    {
    private:
        Uint8 key[crypto_hash_sha256_BYTES];  // Encryption/decryption key

    public:
        /**
         * @brief Constructor for EncryptionHandler.
         * @param password The password used to derive the encryption key.
         */
        EncryptionHandler(const std::string& password)
        {
            if (crypto_hash_sha256(key, reinterpret_cast<const Uint8*>(password.data()), password.size()) != 0)
            {
                throw std::runtime_error("SHA-256 hashing failed");
            }
        }

        /**
         * @brief Checks if the packet is encrypted.
         * @tparam T_PacketID Type of packet identifier.
         * @param packet The packet to check for encryption.
         * @return True if the packet is encrypted, false otherwise.
         */
        template <typename T_PacketID>
        bool IsEncrypted(const nexus::net::Packet<T_PacketID>& packet)
        {
            auto nonce = reinterpret_cast<const Uint64*>(packet.header.nonce);
            return (nonce[0] | nonce[1] | nonce[2]) != 0;
        }

        /**
         * @brief Encrypts the packet using symmetric encryption.
         * @tparam T_PacketID Type of packet identifier.
         * @param packet The packet to be encrypted.
         * @return True if encryption is successful, false otherwise.
         */
        template <typename T_PacketID>
        bool Encrypt(nexus::net::Packet<T_PacketID>& packet)
        {
            // If the packet does not contain a body or is already encrypted
            if (packet.header.size == 0 || IsEncrypted(packet))
            {
                return true;
            }

            // Generate a unique nonce for each packet
            randombytes_buf(packet.header.nonce, crypto_secretbox_NONCEBYTES);

            // Encrypt the body of the packet (returns false if unsuccessful)
            Uint8 encrypted[packet.body.size() + crypto_secretbox_MACBYTES];
            if (crypto_secretbox_easy(encrypted, packet.body.data(), packet.body.size(), packet.header.nonce, key) != 0)
            {
                return false;
            }

            // Replace the body of the packet with its encrypted version
            packet.replace(encrypted, sizeof(encrypted));

            return true;
        }

        /**
         * @brief Decrypts the packet using symmetric encryption.
         * @tparam T_PacketID Type of packet identifier.
         * @param packet The packet to be decrypted.
         * @return True if decryption is successful, false otherwise.
         */
        template <typename T_PacketID>
        bool Decrypt(nexus::net::Packet<T_PacketID>& packet)
        {
            // If the packet does not contain a body or is not encrypted
            if (packet.header.size == 0 || !IsEncrypted(packet))
            {
                return true;
            }

            // Decrypt the body of the packet (returns false if unsuccessful)
            Uint8 decrypted[packet.body.size() - crypto_secretbox_MACBYTES];
            if (crypto_secretbox_open_easy(decrypted, packet.body.data(), packet.body.size(), packet.header.nonce, key) != 0)
            {
                return false;
            }

            // Replace the body of the packet with its decrypted version
            packet.replace(decrypted, sizeof(decrypted));

            // Fill the nonce with zeros (so we can recognize if it is encrypted or not)
            std::fill(packet.header.nonce, packet.header.nonce + crypto_secretbox_NONCEBYTES, 0);

            return true;
        }
    };

}

#endif //NEXUS_NETWORK_SECURITY_HPP
