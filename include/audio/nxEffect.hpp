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

#ifndef NEXUS_AUDIO_EFFECT_HPP
#define NEXUS_AUDIO_EFFECT_HPP

#include "../platform/nxPlatform.hpp"

#include "./nxMiscs.hpp"
#include <functional>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>

namespace nexus { namespace audio {

    /**
     * @brief Base class for audio effects in the OpenAL EFX framework.
     *
     * @warning: For proper memory management, it is recommended to use core::AudioDevice
     *          to create and manage effects instances.
     */
    class NEXUS_API Effect
    {
      public:
        /**
        * @brief Enumeration representing different types of audio effects.
        */
        enum class Type : ALint
        {
            None                = AL_EFFECT_NULL,
            Reverb              = AL_EFFECT_REVERB,
            Chorus              = AL_EFFECT_CHORUS,
            Distortion          = AL_EFFECT_DISTORTION,
            Echo                = AL_EFFECT_ECHO,
            Flanger             = AL_EFFECT_FLANGER,
            FrequencyShifter    = AL_EFFECT_FREQUENCY_SHIFTER,

            // NOT IMPLEMENTED //

            VocalMorpher        = AL_EFFECT_VOCAL_MORPHER,
            PitchShifter        = AL_EFFECT_PITCH_SHIFTER,
            RingModulator       = AL_EFFECT_RING_MODULATOR,
            AutoWah             = AL_EFFECT_AUTOWAH,
            Compressor          = AL_EFFECT_COMPRESSOR,
            Equalizer           = AL_EFFECT_EQUALIZER,
            EAXReverb           = AL_EFFECT_EAXREVERB,
        };

      protected:
        Type type       = Type::None;           ///< Type of the audio effect.
        ALuint effect   = AL_EFFECT_NULL;       ///< OpenAL effect ID.
        ALuint slot     = AL_EFFECTSLOT_NULL;   ///< OpenAL effect slot ID.

      public:
        /**
         * @brief Constructor to create an Effect of the specified type.
         *
         * This constructor generates an OpenAL effect and an auxiliary effect slot,
         * linking them together to create a functional audio effect.
         *
         * @param type The type of audio effect to create.
         */
        Effect(Type type)
        {
            // Create the effect
            alGenEffects(1, &effect);
            alEffecti(effect, AL_EFFECT_TYPE, static_cast<ALint>(type));

            // Create the auxiliary effect slot
            alGenAuxiliaryEffectSlots(1, &slot);
            alAuxiliaryEffectSloti(slot, AL_EFFECTSLOT_EFFECT, effect);
        }

        /**
         * @brief Destructor to release resources associated with the Effect.
         *
         * This destructor deletes the auxiliary effect slot and the effect.
         */
        ~Effect()
        {
            if (effect != AL_EFFECT_NULL)
            {
                alDeleteAuxiliaryEffectSlots(1, &slot);
                alDeleteEffects(1, &effect);
            }
        }

        /**
         * @brief Deleted copy constructor to prevent copying Effects.
         */
        Effect(const Effect&) = delete;

        /**
         * @brief Deleted copy assignment operator to prevent copying Effects.
         */
        Effect& operator=(const Effect&) = delete;

        /**
         * @brief Move constructor to transfer ownership of an Effect.
         *
         * @param other The rvalue reference to the source Effect.
         */
        Effect(Effect&& other) noexcept : type(other.type), effect(other.effect), slot(other.slot)
        {
            other.type = Type::None;
            other.effect = AL_EFFECT_NULL;
            other.slot = AL_EFFECTSLOT_NULL;
        }

        /**
         * @brief Move assignment operator to transfer ownership of an Effect.
         *
         * @param other The rvalue reference to the source Effect.
         * @return A reference to the modified Effect.
         */
        Effect& operator=(Effect&& other) noexcept
        {
            if (this != &other)
            {
                type = other.type;
                effect = other.effect;
                slot = other.slot;

                other.type = Type::None;
                other.effect = AL_EFFECT_NULL;
                other.slot = AL_EFFECTSLOT_NULL;
            }

            return *this;
        }

        /**
         * @brief Set an integer parameter for the audio effect.
         *
         * @param param The parameter to set.
         * @param value The integer value to set.
         */
        void SetParameter(ALenum param, ALint value)
        {
            alEffecti(effect, param, value);
        }

        /**
         * @brief Set a floating-point parameter for the audio effect.
         *
         * @param param The parameter to set.
         * @param value The floating-point value to set.
         */
        void SetParameter(ALenum param, ALfloat value)
        {
            alEffectf(effect, param, value);
        }

        /**
         * @brief Get the OpenAL ID of the audio effect.
         *
         * @return The OpenAL ID of the audio effect.
         */
        ALuint GetID() const { return effect; }

        /**
         * @brief Get the OpenAL ID of the auxiliary effect slot.
         *
         * @return The OpenAL ID of the auxiliary effect slot.
         */
        ALuint GetSlot() const { return slot; }

        /**
         * @brief Get the type of the audio effect.
         *
         * @return The type of the audio effect.
         */
        Type GetType() const { return type; }
    };

    /**
     * @brief Class representing a reverb effect in audio processing.
     *
     * This class is derived from the base Effect class and provides a convenient
     * interface for managing parameters specific to reverb effects in OpenAL.
     */
    class NEXUS_API ReverbEffect : public Effect
    {
    public:
        /**
         * @brief Structure holding parameters for configuring a reverb effect.
         */
        struct Parameters
        {
            float density;               ///< Density of the reverb effect.
            float diffusion;             ///< Diffusion of the reverb effect.
            float gain;                  ///< Gain of the reverb effect.
            float gainHF;                ///< High-frequency gain of the reverb effect.
            float decayTime;             ///< Decay time of the reverb effect.
            float decayHFRatio;          ///< High-frequency decay ratio of the reverb effect.
            float reflectionsGain;       ///< Gain of the reflections in the reverb effect.
            float reflectionsDelay;      ///< Delay time for reflections in the reverb effect.
            float lateReverbGain;        ///< Gain of the late reverberation in the reverb effect.
            float lateReverbDelay;       ///< Delay time for late reverberation in the reverb effect.
            float airAbsorptionGainHF;   ///< High-frequency air absorption gain in the reverb effect.
            float roomRolloffFactor;     ///< Room rolloff factor in the reverb effect.
            float decayHFLimit;          ///< High-frequency decay limit in the reverb effect.

            /**
             * @brief Default constructor for ReverbEffect::Parameters.
             */
            Parameters() :
                density(1.0f), diffusion(1.0f), gain(0.32f), gainHF(0.89f), decayTime(1.49f),
                decayHFRatio(0.83f), reflectionsGain(0.05f), reflectionsDelay(0.03f),
                lateReverbGain(1.26f), lateReverbDelay(0.02f), airAbsorptionGainHF(0.994f),
                roomRolloffFactor(0.0f), decayHFLimit(1.0f)
            {}
        };

    public:
        /**
         * @brief Constructor for the ReverbEffect class.
         *
         * @param param The parameters to configure the reverb effect.
         */
        ReverbEffect(const Parameters& param = {}) : Effect(Type::Reverb)
        {
            SetParameters(param);
        }

        /**
         * @brief Set the parameters of the reverb effect.
         *
         * @param param The parameters to set for the reverb effect.
         */
        void SetParameters(const Parameters& param)
        {
            SetParameter(AL_REVERB_DENSITY, param.density);
            SetParameter(AL_REVERB_DIFFUSION, param.diffusion);
            SetParameter(AL_REVERB_GAIN, param.gain);
            SetParameter(AL_REVERB_GAINHF, param.gainHF);
            SetParameter(AL_REVERB_DECAY_TIME, param.decayTime);
            SetParameter(AL_REVERB_DECAY_HFRATIO, param.decayHFRatio);
            SetParameter(AL_REVERB_REFLECTIONS_GAIN, param.reflectionsGain);
            SetParameter(AL_REVERB_REFLECTIONS_DELAY, param.reflectionsDelay);
            SetParameter(AL_REVERB_LATE_REVERB_GAIN, param.lateReverbGain);
            SetParameter(AL_REVERB_LATE_REVERB_DELAY, param.lateReverbDelay);
            SetParameter(AL_REVERB_AIR_ABSORPTION_GAINHF, param.airAbsorptionGainHF);
            SetParameter(AL_REVERB_ROOM_ROLLOFF_FACTOR, param.roomRolloffFactor);
            SetParameter(AL_REVERB_DECAY_HFLIMIT, param.decayHFLimit);
        }
    };

    /**
     * @brief Class representing a chorus effect in audio processing.
     *
     * This class is derived from the base Effect class and provides a convenient
     * interface for managing parameters specific to chorus effects in OpenAL.
     */
    class NEXUS_API Chorus : public Effect
    {
    public:
        /**
         * @brief Structure holding parameters for configuring a chorus effect.
         */
        struct Parameters
        {
            float rate;          ///< Rate of the chorus effect.
            float depth;         ///< Depth of the chorus effect.
            float feedback;      ///< Feedback of the chorus effect.
            float delay;         ///< Delay time of the chorus effect.
            Wave waveform;       ///< Waveform of the chorus effect.
            float phase;         ///< Phase of the chorus effect.

            /**
             * @brief Default constructor for Chorus::Parameters.
             */
            Parameters() :
                rate(1.1f), depth(0.1f), feedback(0.25f), delay(0.016f),
                waveform(Wave::Sine), phase(90.0f)
            {}
        };

    public:
        /**
        * @brief Constructor for the Chorus class.
        *
        * @param param The parameters to configure the chorus effect.
        */
        Chorus(const Parameters& param = {}) : Effect(Type::Chorus)
        {
            SetParameters(param);
        }

        /**
        * @brief Set the parameters of the chorus effect.
        *
        * @param param The parameters to set for the chorus effect.
        */
        void SetParameters(const Parameters& param)
        {
            SetParameter(AL_CHORUS_RATE, param.rate);
            SetParameter(AL_CHORUS_DEPTH, param.depth);
            SetParameter(AL_CHORUS_FEEDBACK, param.feedback);
            SetParameter(AL_CHORUS_DELAY, param.delay);
            SetParameter(AL_CHORUS_WAVEFORM, param.waveform);
            SetParameter(AL_CHORUS_PHASE, param.phase);
        }
    };

    /**
     * @brief Class representing a distortion effect in audio processing.
     *
     * This class is derived from the base Effect class and provides a convenient
     * interface for managing parameters specific to distortion effects in OpenAL.
     */
    class NEXUS_API Distortion : public Effect
    {
    public:
        /**
         * @brief Structure holding parameters for configuring a distortion effect.
         */
        struct Parameters
        {
            float edge;             ///< Edge parameter of the distortion effect.
            float gain;             ///< Gain parameter of the distortion effect.
            float lowpassCutoff;    ///< Lowpass cutoff parameter of the distortion effect.
            float EQCenter;         ///< EQ center parameter of the distortion effect.
            float EQBandwidth;      ///< EQ bandwidth parameter of the distortion effect.

            /**
             * @brief Default constructor for Distortion::Parameters.
             */
            Parameters() :
                edge(0.2f), gain(0.05f), lowpassCutoff(0.8f),
                EQCenter(3600.0f), EQBandwidth(3600.0f)
            {}
        };

    public:
        /**
         * @brief Constructor for the Distortion class.
         *
         * @param param The parameters to configure the distortion effect.
         */
        Distortion(const Parameters& param = {}) : Effect(Type::Distortion)
        {
            SetParameters(param);
        }

        /**
         * @brief Set the parameters of the distortion effect.
         *
         * @param param The parameters to set for the distortion effect.
         */
        void SetParameters(const Parameters& param)
        {
            SetParameter(AL_DISTORTION_EDGE, param.edge);
            SetParameter(AL_DISTORTION_GAIN, param.gain);
            SetParameter(AL_DISTORTION_LOWPASS_CUTOFF, param.lowpassCutoff);
            SetParameter(AL_DISTORTION_EQCENTER, param.EQCenter);
            SetParameter(AL_DISTORTION_EQBANDWIDTH, param.EQBandwidth);
        }
    };

    /**
     * @brief Class representing an echo effect in audio processing.
     *
     * This class is derived from the base Effect class and provides a convenient
     * interface for managing parameters specific to echo effects in OpenAL.
     */
    class NEXUS_API Echo : public Effect
    {
    public:
        /**
         * @brief Structure holding parameters for configuring an echo effect.
         */
        struct Parameters
        {
            float delay;        ///< Delay parameter of the echo effect.
            float lrDelay;      ///< Left-right delay parameter of the echo effect.
            float damping;      ///< Damping parameter of the echo effect.
            float feedback;     ///< Feedback parameter of the echo effect.

            /**
             * @brief Default constructor for Echo::Parameters.
             */
            Parameters() :
                delay(0.1f), lrDelay(0.1f), damping(0.5f), feedback(0.5f)
            {}
        };

    public:
        /**
         * @brief Constructor for the Echo class.
         *
         * @param param The parameters to configure the echo effect.
         */
        Echo(const Parameters& param = {}) : Effect(Type::Echo)
        {
            SetParameters(param);
        }

        /**
         * @brief Set the parameters of the echo effect.
         *
         * @param param The parameters to set for the echo effect.
         */
        void SetParameters(const Parameters& param)
        {
            SetParameter(AL_ECHO_DELAY, param.delay);
            SetParameter(AL_ECHO_LRDELAY, param.lrDelay);
            SetParameter(AL_ECHO_DAMPING, param.damping);
            SetParameter(AL_ECHO_FEEDBACK, param.feedback);
        }
    };

    /**
     * @brief Class representing a flanger effect in audio processing.
     *
     * This class is derived from the base Effect class and provides a convenient
     * interface for managing parameters specific to flanger effects in OpenAL.
     */
    class NEXUS_API Flanger : public Effect
    {
    public:
        /**
         * @brief Structure holding parameters for configuring a flanger effect.
         */
        struct Parameters
        {
            float rate;         ///< Rate parameter of the flanger effect.
            float depth;        ///< Depth parameter of the flanger effect.
            float feedback;     ///< Feedback parameter of the flanger effect.
            float delay;        ///< Delay parameter of the flanger effect.
            Wave waveform;      ///< Waveform parameter of the flanger effect.
            float phase;        ///< Phase parameter of the flanger effect.

            /**
             * @brief Default constructor for Flanger::Parameters.
             */
            Parameters() :
                rate(0.27f), depth(1.0f), feedback(-0.5f),
                delay(0.002f), waveform(Wave::Sine), phase(0.0f)
            {}
        };

    public:
        /**
         * @brief Constructor for the Flanger class.
         *
         * @param param The parameters to configure the flanger effect.
         */
        Flanger(const Parameters& param = {}) : Effect(Type::Flanger)
        {
            SetParameters(param);
        }

        /**
         * @brief Set the parameters of the flanger effect.
         *
         * @param param The parameters to set for the flanger effect.
         */
        void SetParameters(const Parameters& param)
        {
            SetParameter(AL_FLANGER_RATE, param.rate);
            SetParameter(AL_FLANGER_DEPTH, param.depth);
            SetParameter(AL_FLANGER_FEEDBACK, param.feedback);
            SetParameter(AL_FLANGER_DELAY, param.delay);
            SetParameter(AL_FLANGER_WAVEFORM, param.waveform);
            SetParameter(AL_FLANGER_PHASE, param.phase);
        }
    };

    /**
     * @brief Class representing a frequency shifter effect in audio processing.
     *
     * This class is derived from the base Effect class and provides a convenient
     * interface for managing parameters specific to frequency shifter effects in OpenAL.
     */
    class NEXUS_API FrequencyShifter : public Effect
    {
    public:
        /**
         * @brief Structure holding parameters for configuring a frequency shifter effect.
         */
        struct Parameters
        {
            float frequency;        ///< Frequency parameter of the frequency shifter effect.
            float leftDirection;    ///< Left direction parameter of the frequency shifter effect.
            float rightDirection;   ///< Right direction parameter of the frequency shifter effect.

            /**
             * @brief Default constructor for FrequencyShifter::Parameters.
             */
            Parameters() : frequency(0.0f), leftDirection(1.0f), rightDirection(1.0f) {}
        };

    public:
        /**
         * @brief Constructor for the FrequencyShifter class.
         *
         * @param param The parameters to configure the frequency shifter effect.
         */
        FrequencyShifter(const Parameters& param = {}) : Effect(Type::FrequencyShifter)
        {
            SetParameters(param);
        }

        /**
         * @brief Set the parameters of the frequency shifter effect.
         *
         * @param param The parameters to set for the frequency shifter effect.
         */
        void SetParameters(const Parameters& param)
        {
            SetParameter(AL_FREQUENCY_SHIFTER_FREQUENCY, param.frequency);
            SetParameter(AL_FREQUENCY_SHIFTER_LEFT_DIRECTION, param.leftDirection);
            SetParameter(AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION, param.rightDirection);
        }
    };

}}

#endif //NEXUS_AUDIO_EFFECT_HPP