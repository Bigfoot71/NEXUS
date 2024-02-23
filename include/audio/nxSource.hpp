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

#ifndef NEXUS_AUDIO_SOURCE_HPP
#define NEXUS_AUDIO_SOURCE_HPP

#include "./nxDevice.hpp"
#include "./nxEffect.hpp"
#include "../math/nxMath.hpp"
#include "../math/nxVec3.hpp"
#include "../math/nxVec4.hpp"
#include "../core/nxException.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/alext.h>
#include <sndfile.hh>

namespace nexus { namespace audio {

    /**
     * @brief The base class for sound and music sources in the audio system.
     */
    class NEXUS_API Source
    {
      protected:
        Uint32 frameCount;                    ///< Total number of frames (considering channels)
        Uint32 sampleRate;                    ///< Frequency (samples per second)
        Uint32 channels;                      ///< Number of channels (1-mono, 2-stereo, ...)
        ALenum format;                          ///< Bit depth and channels count (OpenAL format)

      protected:
        ALuint source;                          ///< OpenAL source (id)

#   ifdef ALC_EXT_EFX

      protected:
        ALuint filter;                          ///< OpenAL filter (id)
        const Effect *effect = nullptr;         ///< Pointer to linked effect

#   endif

      protected:
        /**
         * @brief Exception for spatial audio support.
         * Spatial audio features are only available for mono sources.
         */
        static core::NexusException SpatialSupportException()
        {
            return core::NexusException("Sound", "Spatial audio features are only available for mono sources.");
        }

        /**
         * @brief Get the OpenAL format based on the file handle.
         *
         * @param fileHandle SndfileHandle for the audio file.
         * @return OpenAL format for the given file.
         * @throws core::NexusException if the channel count or ambisonic type is unsupported.
         */
        static ALenum GetALFormat(SndfileHandle& fileHandle)
        {
            const int channels = fileHandle.channels();
            const int ambisonicType = fileHandle.command(SFC_WAVEX_GET_AMBISONIC, nullptr, 0);

            if (channels == 1) return AL_FORMAT_MONO16;
            if (channels == 2) return AL_FORMAT_STEREO16;
            if (channels == 3 && ambisonicType == SF_AMBISONIC_B_FORMAT) return AL_FORMAT_BFORMAT2D_16;
            if (channels == 4 && ambisonicType == SF_AMBISONIC_B_FORMAT) return AL_FORMAT_BFORMAT3D_16;

            throw core::NexusException("AudioDevice", "Unsupported channel count or ambisonic type from file");
        }

      protected:
        /**
         * @brief Set the audio information based on the file handle.
         *
         * @param fileHandle SndfileHandle for the audio file.
         */
        void SetAudioInfo(SndfileHandle& fileHandle)
        {
            frameCount = fileHandle.frames();
            sampleRate = fileHandle.samplerate();
            channels = fileHandle.channels();
            format = GetALFormat(fileHandle);
        }

      protected:
        /**
         * @brief Constructor for the Source class.
         * It generates an OpenAL source and, if EFX is supported, a filter.
         */
        Source()
        {
            alGenSources(1, &source);

#           ifdef ALC_EXT_EFX
                // Set the audio filter and attach filter to the source
                alGenFilters(1, &filter);
                alSourcei(source, AL_DIRECT_FILTER, filter);
#           endif
        }

        /**
         * @brief Destructor for the Source class.
         * It deletes the OpenAL source and, if EFX is supported, the filter.
         */
        virtual ~Source()
        {
#           ifdef ALC_EXT_EFX

                // Detach the potential related effect
                DetachEffect();

                // Detach and free filter
                alSourcei(source, AL_DIRECT_FILTER, AL_FILTER_NULL);
                alDeleteFilters(1, &filter);

#           endif

            // Free source
            alDeleteSources(1, &source);
        }

    public:
        /**
         * @brief Get the number of channels for the sound source.
         * 
         * @return The number of channels.
         */
        virtual Uint32 GetChannelsCount() const
        {
            return channels;
        }

        /**
         * @brief Get the duration of the sound source in seconds.
         * 
         * @return The duration in seconds.
         */
        virtual float GetDuration() const
        {
            return static_cast<float>(frameCount) / static_cast<float>(sampleRate);
        }

        /**
         * @brief Get the volume of the sound source.
         * 
         * @return The volume of the sound source.
         */
        virtual float GetVolume() const
        {
            ALfloat volume;
            alGetSourcef(source, AL_GAIN, &volume);
            return volume;
        }

        /**
         * @brief Set the volume of the sound source.
         * 
         * @param volume The volume to set (clamped between 0.0f and 1.0f).
         */
        virtual void SetVolume(float volume)
        {
            alSourcef(source, AL_GAIN, std::clamp(volume, 0.0f, 1.0f));
        }

        /**
         * @brief Check if the sound source is currently playing.
         * 
         * @return True if the sound source is playing, false otherwise.
         */
        virtual bool IsPlaying() const
        {
            ALint state;
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            return state == AL_PLAYING;
        }

        /**
         * @brief Check if the sound source is currently paused.
         * 
         * @return True if the sound source is paused, false otherwise.
         */
        virtual bool IsPaused() const
        {
            ALint state;
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            return state == AL_PAUSED;
        }

        /**
         * @brief Start playing the sound source.
         */
        virtual void Play()
        {
            alSourcePlay(source);
        }

        /**
         * @brief Pause the playback of the sound source.
         */
        virtual void Pause()
        {
            alSourcePause(source);
        }

        /**
         * @brief Rewind the sound source to the beginning.
         */
        virtual void Rewind()
        {
            alSourceRewind(source);
        }

        /**
         * @brief Stop playback of the sound source.
         */
        virtual void Stop()
        {
            alSourceStop(source);
        }

        /**
         * @brief Set whether the sound source is looping.
         * 
         * @param loop If true, the sound source will loop; otherwise, it won't.
         */
        virtual void SetLooping(bool loop)
        {
            alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        }

        /**
         * @brief Check if the sound source is set to loop.
         * 
         * @return True if the sound source is set to loop, false otherwise.
         */
        virtual bool IsLooping() const
        {
            ALint loop;
            alGetSourcei(source, AL_LOOPING, &loop);
            return loop == AL_TRUE;
        }

        /**
         * @brief Set the position of the sound source in 3D space.
         * 
         * @param x The x-coordinate.
         * @param y The y-coordinate.
         * @param z The z-coordinate.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetPosition(float x, float y, float z)
        {
            if (channels != 1) throw SpatialSupportException();
            alSource3f(source, AL_POSITION, x, y, z);
        }

        /**
         * @brief Set the position of the sound source in 3D space using a Vec3.
         * 
         * @param position The position vector in 3D space.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetPosition(const math::Vec3& position)
        {
            if (channels != 1) throw SpatialSupportException();
            alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        }

        /**
         * @brief Get the position of the sound source in 3D space.
         * 
         * @return The position vector in 3D space.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual math::Vec3 GetPosition() const
        {
            if (channels != 1) throw SpatialSupportException();

            math::Vec3 position;
            alGetSource3f(source, AL_POSITION, &position.x, &position.y, &position.z);
            return position;
        }

        /**
         * @brief Set the orientation of the sound source in 3D space using explicit vectors.
         * 
         * @param atX, atY, atZ The forward direction vector.
         * @param upX, upY, upZ The up direction vector (default is {0, 1, 0}).
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetOrientation(float atX, float atY, float atZ, float upX = 0, float upY = 1, float upZ = 0)
        {
            if (channels != 1) throw SpatialSupportException();
            const ALfloat orientation[6] = { atX, atY, atZ, upX, upY, upZ };
            alSourcefv(source, AL_ORIENTATION, orientation);
        }

        /**
         * @brief Set the orientation of the sound source in 3D space using Vec3 vectors.
         * 
         * @param at The forward direction vector.
         * @param up The up direction vector (default is {0, 1, 0}).
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetOrientation(const math::Vec3& at, const math::Vec3& up = { 0, 1, 0 })
        {
            if (channels != 1) throw SpatialSupportException();
            const ALfloat orientation[6] = { at.x, at.y, at.z, up.x, up.y, up.z };
            alSourcefv(source, AL_ORIENTATION, orientation);
        }

        /**
         * @brief Get the orientation of the sound source in 3D space.
         * 
         * @param up Optional pointer to store the up direction vector.
         * @return The forward direction vector.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual math::Vec3 GetOrientation(math::Vec3* up = nullptr) const
        {
            if (channels != 1) throw SpatialSupportException();

            ALfloat orientation[6];
            alGetSourcefv(source, AL_ORIENTATION, orientation);

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
         * @brief Set the velocity of the sound source in 3D space.
         * 
         * @param x, y, z The velocity vector.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetVelocity(float x, float y, float z)
        {
            if (channels != 1) throw SpatialSupportException();
            alSource3f(source, AL_VELOCITY, x, y, z);
        }

        /**
         * @brief Set the velocity of the sound source in 3D space using a Vec3.
         * 
         * @param velocity The velocity vector.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetVelocity(const math::Vec3& velocity)
        {
            if (channels != 1) throw SpatialSupportException();
            alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        }

        /**
         * @brief Get the velocity of the sound source in 3D space.
         * 
         * @return The velocity vector.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual math::Vec3 GetVelocity() const
        {
            if (channels != 1) throw SpatialSupportException();

            math::Vec3 velocity;
            alGetSource3f(source, AL_POSITION, &velocity.x, &velocity.y, &velocity.z);
            return velocity;
        }

        /**
         * @brief Set the pitch of the sound source.
         * 
         * @param pitch The pitch value.
         */
        virtual void Pitch(float pitch)
        {
            alSourcef(source, AL_PITCH, pitch);
        }

        /**
         * @brief Get the pitch of the sound source.
         * 
         * @return The pitch value.
         */
        virtual float GetPitch() const
        {
            ALfloat pitch;
            alGetSourcef(source, AL_PITCH, &pitch);
            return pitch;
        }

        /**
         * @brief Set the roll-off factor for the sound source.
         * 
         * @param factor The roll-off factor.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetRollOff(float factor)
        {
            if (channels != 1) throw SpatialSupportException();
            alSourcef(source, AL_ROLLOFF_FACTOR, factor);
        }

        /**
         * @brief Get the roll-off factor of the sound source.
         * 
         * @return The roll-off factor.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual float GetRollOff() const
        {
            if (channels != 1) throw SpatialSupportException();

            float factor;
            alGetSourcef(source, AL_ROLLOFF_FACTOR, &factor);
            return factor;
        }

        /**
         * @brief Set the room roll-off factor for the sound source.
         * 
         * @param factor The room roll-off factor.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetRoomRollOff(float factor)
        {
            if (channels != 1) throw SpatialSupportException();
            alSourcef(source, AL_ROOM_ROLLOFF_FACTOR, factor);
        }

        /**
         * @brief Get the room roll-off factor of the sound source.
         * 
         * @return The room roll-off factor.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual float GetRoomRollOff() const
        {
            if (channels != 1) throw SpatialSupportException();

            float factor;
            alGetSourcef(source, AL_ROOM_ROLLOFF_FACTOR, &factor);
            return factor;
        }

        /**
         * @brief Set the cone parameters for the sound source.
         * 
         * @param innerAngle The inner angle of the cone (in degrees).
         * @param outerAngle The outer angle of the cone (in degrees).
         * @param outerVolume The outer volume of the cone.
         * @param outerHighGain The outer high-frequency gain of the cone.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetCone(float innerAngle, float outerAngle, float outerVolume, float outerHighGain)
        {
            if (channels != 1) throw SpatialSupportException();

            alSourcei(source, AL_CONE_INNER_ANGLE, innerAngle);
            alSourcei(source, AL_CONE_OUTER_ANGLE, outerAngle);
            alSourcef(source, AL_CONE_OUTER_GAIN, outerVolume);

#           ifdef ALC_EXT_EFX
                alSourcef(source, AL_CONE_OUTER_GAINHF, outerHighGain);
#           endif
        }

        /**
         * @brief Get the cone parameters of the sound source.
         * 
         * @return A Vec4 containing inner angle, outer angle, outer volume, and outer high-frequency gain.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual math::Vec4 GetCone() const
        {
            if (channels != 1) throw SpatialSupportException();

            math::Vec4 result;

            int innerAngle, outerAngle;
            float outerVolume, outerHighGain;

            alGetSourcei(source, AL_CONE_INNER_ANGLE, &innerAngle);
            alGetSourcei(source, AL_CONE_OUTER_ANGLE, &outerAngle);
            alGetSourcef(source, AL_CONE_OUTER_GAIN, &outerVolume);

#           ifdef ALC_EXT_EFX
                alGetSourcef(source, AL_CONE_OUTER_GAINHF, &outerHighGain);
#           endif

            return {
                static_cast<float>(innerAngle * math::Deg2Rad),
                static_cast<float>(outerAngle * math::Deg2Rad),
                outerVolume, outerHighGain
            };
        }

#   ifdef ALC_EXT_EFX

        /**
         * @brief Set the air absorption factor for the sound source.
         * 
         * @param factor The air absorption factor.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual void SetAirAbsorption(float factor)
        {
            if (channels != 1) throw SpatialSupportException();
            alSourcef(source, AL_AIR_ABSORPTION_FACTOR, factor);
        }

        /**
         * @brief Get the air absorption factor of the sound source.
         * 
         * @return The air absorption factor.
         * @throws core::NexusException if spatial audio features are not supported for the source.
         */
        virtual float GetAirAbsorption() const
        {
            if (channels != 1) throw SpatialSupportException();

            float factor;
            alGetSourcef(source, AL_AIR_ABSORPTION_FACTOR, &factor);
            return factor;
        }

        /**
         * @brief Set the filter type for the sound source.
         * 
         * @param type The filter type to set.
         */
        virtual void SetFilter(Device::Filter type)
        {
            alFilteri(filter, AL_FILTER_TYPE, static_cast<ALint>(type));
        }

        /**
         * @brief Get the filter type of the sound source.
         * 
         * @return The filter type.
         */
        virtual Device::Filter GetFilter() const
        {
            ALint type;
            alGetFilteri(filter, AL_FILTER_TYPE, &type);
            return static_cast<Device::Filter>(type);
        }

        /**
         * @brief Attach an effect to the sound source.
         * 
         * @param effect The effect to attach.
         */
        virtual void AttachEffect(const Effect* effect)
        {
            alSource3i(source, AL_AUXILIARY_SEND_FILTER, effect->GetSlot(), effect->GetID(), 0);
            this->effect = effect;
        }

        /**
         * @brief Detach the currently attached effect from the sound source.
         */
        virtual void DetachEffect()
        {
            if (effect != nullptr)
            {
                alSource3i(source, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, AL_EFFECT_NULL, 0);
                effect = nullptr;
            }
        }

        /**
         * @brief Check if the sound source has an effect attached.
         * 
         * @return True if an effect is attached, false otherwise.
         */
        virtual bool HasEffect() const
        {
            return effect != nullptr;
        }

        /**
         * @brief Get the currently attached effect to the sound source.
         * 
         * @return A pointer to the attached effect, or nullptr if no effect is attached.
         */
        virtual const Effect* GetEffect() const
        {
            return effect;
        }

#   endif

    };

}}

#endif
