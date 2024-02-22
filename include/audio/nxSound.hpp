#ifndef NEXUS_AUDIO_SOUND_HPP
#define NEXUS_AUDIO_SOUND_HPP

#include "./nxSource.hpp"

namespace nexus { namespace audio {

    /**
     * @brief Class representing a sound source in audio processing.
     *
     * The Sound class extends the Source class and is designed to load and play
     * audio files using OpenAL. It reads audio data from a specified file and
     * associates it with an OpenAL buffer, making it ready for playback.
     */
    class NEXUS_API Sound : public Source
    {
      private:
        ALuint buffer;      ///< OpenAL buffer (id)

      public:
        /**
         * @brief Constructor for the Sound class.
         *
         * Loads audio data from the specified file and associates it with an OpenAL buffer.
         *
         * @param filePath The path to the audio file.
         */
        Sound(const std::string& filePath);

        /**
         * @brief Destructor for the Sound class.
         *
         * Stops the source and frees the associated OpenAL buffer.
         */
        ~Sound();
    };

}}

#endif //NEXUS_AUDIO_SOUND_HPP
