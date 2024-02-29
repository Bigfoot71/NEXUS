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
#include "../utils/nxContextual.hpp"

#include "./nxEnums.hpp"
#include <functional>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>

namespace nexus { namespace audio {

    class Device;   ///< Context device forward declaration

    enum class Reverb : ALenum
    {
        Density                     = AL_REVERB_DENSITY,
        Diffusion                   = AL_REVERB_DIFFUSION,
        Gain                        = AL_REVERB_GAIN,
        GainHF                      = AL_REVERB_GAINHF,
        DecayTime                   = AL_REVERB_DECAY_TIME,
        DecayHFRatio                = AL_REVERB_DECAY_HFRATIO,
        ReflectionsGain             = AL_REVERB_REFLECTIONS_GAIN,
        ReflectionsDelay            = AL_REVERB_REFLECTIONS_DELAY,
        LateReverbGain              = AL_REVERB_LATE_REVERB_GAIN,
        LateReverbDelay             = AL_REVERB_LATE_REVERB_DELAY,
        AirAbsortionGainHF          = AL_REVERB_AIR_ABSORPTION_GAINHF,
        RoomRollOffFactor           = AL_REVERB_ROOM_ROLLOFF_FACTOR,
        DecayHFLimit                = AL_REVERB_DECAY_HFLIMIT
    };

    enum class EAXReverb : ALenum
    {
        Density                     = AL_EAXREVERB_DENSITY,
        Diffusion                   = AL_EAXREVERB_DIFFUSION,
        Gain                        = AL_EAXREVERB_GAIN,
        GainHF                      = AL_EAXREVERB_GAINHF,
        GainLF                      = AL_EAXREVERB_GAINLF,
        DecayTime                   = AL_EAXREVERB_DECAY_TIME,
        DecayHFRatio                = AL_EAXREVERB_DECAY_HFRATIO,
        DecayLFRatio                = AL_EAXREVERB_DECAY_LFRATIO,
        ReflectionsGain             = AL_EAXREVERB_REFLECTIONS_GAIN,
        ReflectionsDelay            = AL_EAXREVERB_REFLECTIONS_DELAY,
        ReflectionsPan              = AL_EAXREVERB_REFLECTIONS_PAN,
        LateReverbGain              = AL_EAXREVERB_LATE_REVERB_GAIN,
        LateReverbDelay             = AL_EAXREVERB_LATE_REVERB_DELAY,
        LateReverbPan               = AL_EAXREVERB_LATE_REVERB_PAN,
        EchoTime                    = AL_EAXREVERB_ECHO_TIME,
        EchoDepth                   = AL_EAXREVERB_ECHO_DEPTH,
        ModulationTime              = AL_EAXREVERB_MODULATION_TIME,
        ModulationDepth             = AL_EAXREVERB_MODULATION_DEPTH,
        AirAbsorptionGainHF         = AL_EAXREVERB_AIR_ABSORPTION_GAINHF,
        HFReference                 = AL_EAXREVERB_HFREFERENCE,
        LFReference                 = AL_EAXREVERB_LFREFERENCE,
        RoomRolloffFactor           = AL_EAXREVERB_ROOM_ROLLOFF_FACTOR,
        DecayHFLimit                = AL_EAXREVERB_DECAY_HFLIMIT
    };

    enum class Chorus : ALenum
    {
        Waveform                    = AL_CHORUS_WAVEFORM,
        Phase                       = AL_CHORUS_PHASE,
        Rate                        = AL_CHORUS_RATE,
        Depth                       = AL_CHORUS_DEPTH,
        Feedback                    = AL_CHORUS_FEEDBACK,
        Delay                       = AL_CHORUS_DELAY
    };

    enum class Distortion : ALenum
    {
        Edge                        = AL_DISTORTION_EDGE,
        Gain                        = AL_DISTORTION_GAIN,
        LowpassCutoff               = AL_DISTORTION_LOWPASS_CUTOFF,
        EQCenter                    = AL_DISTORTION_EQCENTER,
        EQBandwidth                 = AL_DISTORTION_EQBANDWIDTH
    };

    enum class Echo : ALenum
    {
        Delay                       = AL_ECHO_DELAY,
        LRDelay                     = AL_ECHO_LRDELAY,
        Damping                     = AL_ECHO_DAMPING,
        Feedback                    = AL_ECHO_FEEDBACK,
        Spread                      = AL_ECHO_SPREAD
    };

    enum class Flanger : ALenum
    {
        Waveform                    = AL_FLANGER_WAVEFORM,
        Phase                       = AL_FLANGER_PHASE,
        Rate                        = AL_FLANGER_RATE,
        Depth                       = AL_FLANGER_DEPTH,
        Feedback                    = AL_FLANGER_FEEDBACK,
        Delay                       = AL_FLANGER_DELAY
    };

    enum class FrequencyShifter : ALenum
    {
        Frequency                   = AL_FREQUENCY_SHIFTER_FREQUENCY,
        LeftDirection               = AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,
        RightDirection              = AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION
    };

    enum class VocalMorpher : ALenum
    {
        PhonemeA                    = AL_VOCAL_MORPHER_PHONEMEA,
        PhonemeACoarseTuning        = AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING,
        PhonemeB                    = AL_VOCAL_MORPHER_PHONEMEB,
        PhonemeBCoarseTuning        = AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING,
        Waveform                    = AL_VOCAL_MORPHER_WAVEFORM,
        Rate                        = AL_VOCAL_MORPHER_RATE
    };

    enum class PitchShifter : ALenum
    {
        CoarseTune                  = AL_PITCH_SHIFTER_COARSE_TUNE,
        FineTune                    = AL_PITCH_SHIFTER_FINE_TUNE
    };

    enum class RingModulator : ALenum
    {
        Frequency                   = AL_RING_MODULATOR_FREQUENCY,
        HighpassCutoff              = AL_RING_MODULATOR_HIGHPASS_CUTOFF,
        Waveform                    = AL_RING_MODULATOR_WAVEFORM
    };

    enum class AutoWah : ALenum
    {
        AttackTime                  = AL_AUTOWAH_ATTACK_TIME,
        ReleaseTime                 = AL_AUTOWAH_RELEASE_TIME,
        Resonance                   = AL_AUTOWAH_RESONANCE,
        PeakGain                    = AL_AUTOWAH_PEAK_GAIN
    };

    enum class Compressor : ALenum
    {
        OnOff                       = AL_COMPRESSOR_ONOFF
    };

    enum class Equalizer : ALenum
    {
        LowGain                     = AL_EQUALIZER_LOW_GAIN,
        LowCutoff                   = AL_EQUALIZER_LOW_CUTOFF,
        Mid1Gain                    = AL_EQUALIZER_MID1_GAIN,
        Mid1Center                  = AL_EQUALIZER_MID1_CENTER,
        Mid1Width                   = AL_EQUALIZER_MID1_WIDTH,
        Mid2Gain                    = AL_EQUALIZER_MID2_GAIN,
        Mid2Center                  = AL_EQUALIZER_MID2_CENTER,
        Mid2Width                   = AL_EQUALIZER_MID2_WIDTH,
        HighGain                    = AL_EQUALIZER_HIGH_GAIN,
        HighCutoff                  = AL_EQUALIZER_HIGH_CUTOFF
    };

}}

namespace _audio_impl {

    /**
     * @brief Base class for audio effects in the OpenAL EFX framework.
     *
     * @warning: For proper memory management, it is recommended to use core::AudioDevice
     *          to create and manage effects instances.
     */
    class Effect : public nexus::utils::Contextual<nexus::audio::Device>
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
         * @param ctx The audio device context.
         * @param type The type of audio effect to create.
         */
        Effect(nexus::audio::Device& ctx, Type type)
        : Contextual<nexus::audio::Device>(ctx)
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
         * @brief Set an integer parameter for the audio effect.
         *
         * @tparam T_Enum One of the effect configuration definitions provided by OpenAL Soft
         * or one of the enum class definitions provided by Nexus such as Reverb, Echo, VocalMorpher, etc.
         *
         * @param param The parameter to set.
         * @param value The integer value to set.
         */
        template <typename T_Enum>
        void SetParameter(T_Enum param, ALint value)
        {
            static_assert(std::is_convertible<T_Enum, ALenum>::value, "T_Enum must be convertible to ALenum");
            alEffecti(effect, static_cast<ALenum>(param), value);
        }

        /**
         * @brief Set a floating-point parameter for the audio effect.
         *
         * @tparam T_Enum One of the effect configuration definitions provided by OpenAL Soft
         * or one of the enum class definitions provided by Nexus such as Reverb, Echo, VocalMorpher, etc.
         *
         * @param param The parameter to set.
         * @param value The floating-point value to set.
         */
        template <typename T_Enum>
        void SetParameter(T_Enum param, ALfloat value)
        {
            static_assert(std::is_convertible<T_Enum, ALenum>::value, "T_Enum must be convertible to ALenum");
            alEffectf(effect, static_cast<ALenum>(param), value);
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

}

namespace nexus { namespace audio {

    /**
     * @brief Structure representing a collection of audio effects.
     *
     * The Effect structure serves as a container for managing audio effect objects. It
     * provides functionality to apply various audio effects using OpenAL, encapsulating
     * the behavior of individual effect sources.
     */
    struct NEXUS_API Effect : public utils::Container<_audio_impl::Effect>
    {
        using Type = _audio_impl::Effect::Type;

        /**
         * @brief Constructor for the Effect structure.
         *
         * Creates an audio effect of the specified type.
         *
         * @param ctx The audio device context.
         * @param type The type of audio effect to create.
         */
        Effect(nexus::audio::Device& ctx, Type type)
        : Container<_audio_impl::Effect>(ctx, type)
        { }
    };

}}

#endif //NEXUS_AUDIO_EFFECT_HPP