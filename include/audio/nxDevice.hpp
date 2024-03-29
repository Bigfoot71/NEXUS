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

#ifndef NEXUS_AUDIO_DEVICE_HPP
#define NEXUS_AUDIO_DEVICE_HPP

#include "../platform/nxPlatform.hpp"

#include "../core/nxLog.hpp"
#include "../math/nxVec3.hpp"
#include "../core/nxException.hpp"

#include "./nxEffect.hpp"
#include <algorithm>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/alext.h>
#include <sndfile.hh>

namespace nexus { namespace audio {

    /**
     * @brief The Device class manages the OpenAL audio context, listener, and effects, and provides
     *        a list of available effects.
     *
     * The Device class is responsible for handling the OpenAL device, creating and managing the audio context,
     * and providing functionality related to audio effects. When initialized, it opens an OpenAL audio device,
     * creates an audio context, and sets the context as the current one.
     */
    class NEXUS_API Device
    {
      private:
        ALCdevice *device;      ///< OpenAL audio device.
        ALCcontext *context;    ///< OpenAL audio context.

#ifdef ALC_EXT_EFX

      public:
        static constexpr int MaxSourceEffects = 64;     ///< Maximum number of effects that can be applied to a source.

      public:
        /**
         * @brief Enumeration representing filter types for audio effects.
         */
        enum class Filter : ALenum
        {
            None            = AL_FILTER_NULL,           ///< No filter.
            LowPass         = AL_FILTER_LOWPASS,        ///< Low-pass filter.
            HighPass        = AL_FILTER_HIGHPASS,       ///< High-pass filter.
            BandPass        = AL_FILTER_BANDPASS        ///< Band-pass filter.
        };

#endif

      public:
        /**
         * @brief Constructor for the Device class.
         *
         * Initializes the OpenAL audio device and context. Throws a NexusException if any step of the initialization fails.
         *
         * @param deviceName The name of the OpenAL audio device to open. If set to nullptr, the default device is used.
         *
         * @throws core::NexusException if the audio device cannot be opened or the audio context cannot be created.
         */
        Device(const char* deviceName = nullptr);

        /**
         * @brief Destructor for the Device class.
         *
         * Clears the effects (if EFX extension is supported), releases the audio context, and closes the audio device.
         */
        ~Device();

        /**
         * @brief Checks if the audio device's context is current.
         *
         * This function checks whether the current OpenAL context matches the context
         * associated with the audio device, indicating whether the audio device's context
         * is currently active.
         *
         * @return True if the audio device's context is current, false otherwise.
         */
        bool IsCurrent() const
        {
            return context == alcGetCurrentContext();
        }

        /**
         * @brief Makes the audio device's context current.
         *
         * This function sets the current OpenAL context to the context associated with
         * the audio device. This is necessary for making OpenAL API calls that affect
         * the current context.
         */
        void MakeCurrent() const
        {
            alcMakeContextCurrent(context);
        }

        /**
         * @brief Set the listener's volume.
         *
         * @param gain The gain (volume) to set for the listener. Should be a value between 0.0 (silent) and 1.0 (full volume).
         */
        void SetListenerVolume(float gain) const
        {
            alListenerf(AL_GAIN, gain);
        }

        /**
         * @brief Get the listener's volume.
         *
         * @return The current gain (volume) of the listener.
         */
        float GetListenerVolume() const
        {
            float gain;
            alGetListenerf(AL_GAIN, &gain);
            return gain;
        }

        /**
         * @brief Set the position of the listener in 3D space.
         *
         * @param x The X-coordinate of the listener's position.
         * @param y The Y-coordinate of the listener's position.
         * @param z The Z-coordinate of the listener's position.
         */
        void SetListenerPosition(float x, float y, float z) const
        {
            alListener3f(AL_POSITION, x, y, z);
        }

        /**
         * @brief Set the position of the listener in 3D space using a Vec3.
         *
         * @param position The position of the listener in 3D space.
         */
        void SetListenerPosition(const math::Vec3& position) const
        {
            alListener3f(AL_POSITION, position.x, position.y, position.z);
        }

        /**
         * @brief Get the position of the listener in 3D space.
         *
         * @return The position of the listener as a Vec3.
         */
        math::Vec3 GetListenerPosition() const
        {
            math::Vec3 position;
            alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z);
            return position;
        }

        /**
         * @brief Set the orientation of the listener in 3D space.
         *
         * @param atX The X-coordinate of the listener's "at" direction.
         * @param atY The Y-coordinate of the listener's "at" direction.
         * @param atZ The Z-coordinate of the listener's "at" direction.
         * @param upX The X-coordinate of the listener's "up" direction.
         * @param upY The Y-coordinate of the listener's "up" direction.
         * @param upZ The Z-coordinate of the listener's "up" direction.
         */
        void SetListenerOrientation(float atX, float atY, float atZ, float upX = 0, float upY = 1, float upZ = 0) const
        {
            const ALfloat orientation[6] = { atX, atY, atZ, upX, upY, upZ };
            alListenerfv(AL_ORIENTATION, orientation);
        }

        /**
         * @brief Set the orientation of the listener in 3D space using Vec3 directions.
         *
         * @param at The "at" direction of the listener.
         * @param up The "up" direction of the listener.
         */
        void SetListenerOrientation(const math::Vec3& at, const math::Vec3& up = { 0, 1, 0 }) const
        {
            const ALfloat orientation[6] = { at.x, at.y, at.z, up.x, up.y, up.z };
            alListenerfv(AL_ORIENTATION, orientation);
        }

        /**
         * @brief Get the orientation of the listener in 3D space.
         *
         * @param up If provided, it will be filled with the "up" direction.
         * @return The "at" direction of the listener as a Vec3.
         */
        math::Vec3 GetListenerOrientation(math::Vec3* up = nullptr) const
        {
            ALfloat orientation[6];
            alGetListenerfv(AL_ORIENTATION, orientation);

            if (up != nullptr)
            {
                up->x = orientation[3];
                up->y = orientation[4];
                up->z = orientation[5];
            }

            return {
                orientation[0],
                orientation[1],
                orientation[2]
            };
        }

        /**
         * @brief Set the velocity of the listener in 3D space.
         *
         * @param x The X-coordinate of the listener's velocity.
         * @param y The Y-coordinate of the listener's velocity.
         * @param z The Z-coordinate of the listener's velocity.
         */
        void SetListenerVelocity(float x, float y, float z) const
        {
            alListener3f(AL_VELOCITY, x, y, z);
        }

        /**
         * @brief Set the velocity of the listener in 3D space using a Vec3.
         *
         * @param velocity The velocity of the listener.
         */
        void SetListenerVelocity(const math::Vec3& velocity) const
        {
            alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        }

        /**
         * @brief Get the velocity of the listener in 3D space.
         *
         * @return The velocity of the listener as a Vec3.
         */
        math::Vec3 GetListenerVelocity() const
        {
            math::Vec3 velocity;
            alGetListener3f(AL_POSITION, &velocity.x, &velocity.y, &velocity.z);
            return velocity;
        }
    };

}}

#endif //NEXUS_AUDIO_DEVICE_HPP
