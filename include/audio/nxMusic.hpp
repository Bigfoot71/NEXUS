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

#ifndef NEXUS_AUDIO_MUSIC_HPP
#define NEXUS_AUDIO_MUSIC_HPP

#include "../platform/nxPlatform.hpp"

#include "core/nxException.hpp"
#include "./nxDevice.hpp"
#include "./nxSource.hpp"
#include <sndfile.hh>
#include <thread>

namespace nexus { namespace audio {

    /**
     * @brief Music class for handling audio music playback.
     * 
     * The `Music` class is derived from the `Source` class and adds functionality
     * specific to handling music playback, such as streaming from file.
     */
    class NEXUS_API Music : public Source
    {
      private:
        static constexpr int BUFFER_SAMPLES = 8192;
        static constexpr int NUM_BUFFERS = 4;

      private:
        ALuint buffers[NUM_BUFFERS]; ///< Array of OpenAL buffer IDs.
        SndfileHandle fileHandle;    ///< Handle to the opened sound file.
        short* membuf;               ///< Memory buffer for storing decoded audio samples.

      private:
        std::thread decodingThread;   ///< Thread for asynchronous audio decoding.
        bool decodingThreadRunning;   ///< Flag indicating if the decoding thread is running.
        sf_count_t currentPosition;   ///< Current position in the audio file.

      private:
        /**
         * @brief Fills an OpenAL buffer with audio data from the sound file.
         * 
         * This method reads a chunk of audio data from the sound file associated
         * with the Music object and fills the specified OpenAL buffer with that data.
         * The buffer is then queued for playback on the OpenAL source. It is intended
         * to be used in the decoding thread to continuously provide audio data for
         * seamless playback.
         * 
         * @param bufferID The ID of the OpenAL buffer to fill.
         *                 It represents the buffer that will be filled with audio data
         *                 and queued for playback on the OpenAL source.
         * 
         * @throw core::NexusException If there is an error while buffering the music data.
         *                             This exception is thrown if an OpenAL error occurs
         *                             during the process of filling and queuing the buffer.
         */
        void FillBuffer(ALuint bufferID);

        /**
         * @brief Function executed by the decoding thread for asynchronous decoding and buffering.
         * 
         * This function is the main loop of the decoding thread responsible for continuously
         * decoding and buffering audio data for seamless playback. It monitors the state of
         * the OpenAL source and the processed buffers, unqueues processed buffers, fills them
         * with new audio data using the FillBuffer method, and ensures continuous playback
         * by checking and restarting the source if needed. The thread sleeps for a short
         * interval (8 milliseconds) between iterations to control the decoding rate.
         * 
         * The decoding thread runs as long as the decodingThreadRunning flag is set to true.
         * 
         * Note: This function is designed to be executed asynchronously in a separate thread
         * to avoid blocking the main application thread during the decoding and buffering
         * process for a smoother user experience.
         * 
         * TODO: Consider implementing a more sophisticated solution, such as signal handling
         * or synchronization with playback, to resume decoding at the appropriate moment
         * instead of using a fixed 8ms sleep interval. This would improve efficiency and
         * responsiveness in handling buffer processing and playback synchronization.
         */
        void DecodingThreadFunc();

      public:
        /**
         * @brief Constructor for the Music class.
         * 
         * Initializes the Music object by allocating OpenAL buffers,
         * opening the specified audio file, setting audio information,
         * and preparing a memory buffer for audio decoding.
         * 
         * @param filePath The path to the audio file to be loaded.
         */
        Music(const std::string& filePath);

        /**
         * @brief Destructor for the Music class.
         * 
         * Stops the source, finishes the decoding thread (if joinable),
         * and frees memory associated with the Music object.
         */
        ~Music();

        /**
         * @brief Play method for the Music class.
         * 
         * Plays the music by rewinding the source, clearing existing buffers,
         * and filling OpenAL buffers with audio data for playback. It then
         * queues the buffers, starts the source, and initiates the decoding thread.
         * 
         * Overrides the base class method.
         * 
         * @throws core::NexusException if there is an error buffering for playback.
         */
        void Play() override;

        /**
         * @brief Pause method for the Music class.
         * 
         * Pauses the music playback by pausing the OpenAL source and stopping
         * the decoding thread if it is running. It waits for the decoding thread
         * to finish if it is joinable.
         * 
         * Overrides the base class method.
         */
        void Pause() override;

        /**
         * @brief Rewind method for the Music class.
         * 
         * Stops the decoding thread if it is running and rewinds the OpenAL source
         * and the audio file to the beginning. Then, it initiates the Play method
         * to start playback from the beginning.
         * 
         * Overrides the base class method.
         */
        void Rewind() override;

        /**
         * @brief Stop method for the Music class.
         * 
         * Stops the decoding thread if it is running, rewinds the OpenAL source
         * and the audio file to the beginning, and stops the OpenAL source.
         * It waits for the decoding thread to finish if it is joinable.
         * 
         * Overrides the base class method.
         */
        void Stop() override;
    };

}}

#endif //NEXUS_AUDIO_MUSIC_HPP
