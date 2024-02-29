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

#ifndef NEXUS_AUDIO_SOUND_HPP
#define NEXUS_AUDIO_SOUND_HPP

#include "./impl/nxSource.hpp"

namespace _audio_impl {

    /**
     * @brief Class representing a sound source in audio processing.
     *
     * The Sound class extends the Source class and is designed to load and play
     * audio files using OpenAL. It reads audio data from a specified file and
     * associates it with an OpenAL buffer, making it ready for playback.
     */
    class Sound : public Source
    {
      private:
        ALuint buffer;      ///< OpenAL buffer (id)

      public:
        /**
         * @brief Constructor for the Sound class.
         *
         * Loads audio data from the specified file and associates it with an OpenAL buffer.
         *
         * @param ctx The audio device context.
         * @param filePath The path to the audio file.
         */
        Sound(nexus::audio::Device& ctx, const std::string& filePath);

        /**
         * @brief Destructor for the Sound class.
         *
         * Stops the source and frees the associated OpenAL buffer.
         */
        ~Sound();
    };

}

namespace nexus { namespace audio {

    /**
     * @brief Class representing a collection of audio sounds.
     *
     * The Sound class serves as a container for managing audio sound objects. It
     * provides functionality to load and play audio files using OpenAL, encapsulating
     * the behavior of individual sound sources.
     */
    struct NEXUS_API Sound : public utils::Container<_audio_impl::Sound>
    {
        /**
         * @brief Constructor for the Sound class.
         *
         * Loads audio data from the specified file and associates it with an OpenAL buffer.
         *
         * @param ctx The audio device context.
         * @param filePath The path to the audio file.
         */
        Sound(nexus::audio::Device& ctx, const std::string& filePath)
        : utils::Container<_audio_impl::Sound>(ctx, filePath)
        { }
    };

}}

#endif //NEXUS_AUDIO_SOUND_HPP
