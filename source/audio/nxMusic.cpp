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

#include "audio/nxMusic.hpp"

using namespace nexus;

void _audio_impl::Music::FillBuffer(ALuint bufferID)
{
    // Read a chunk of audio data from the sound file into the memory buffer
    sf_count_t len = fileHandle.readf(membuf, BUFFER_SAMPLES);

    // Check if data was read successfully
    if (len > 0)
    {
        // Calculate the size of the audio data in bytes
        len *= channels * static_cast<sf_count_t>(sizeof(short));

        // Fill the OpenAL buffer with the audio data
        alBufferData(bufferID, format, membuf, static_cast<ALsizei>(len), sampleRate);

        // Queue the filled buffer for playback on the OpenAL source
        alSourceQueueBuffers(source, 1, &bufferID);
    }

    // Check for OpenAL errors
    if (alGetError() != AL_NO_ERROR)
    {
        // Throw an exception if there is an error during the buffering process
        throw core::NexusException("Music", "Error buffering music data");
    }
}

void _audio_impl::Music::DecodingThreadFunc()
{
    while (decodingThreadRunning)
    {
        // Get the current state of the OpenAL source
        ALint processed, state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        // Get the number of processed buffers
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

        // Unqueue and fill processed buffers with new audio data
        while (processed > 0)
        {
            ALuint bufferID;
            alSourceUnqueueBuffers(source, 1, &bufferID);
            processed--;
            FillBuffer(bufferID);
        }

        // Check if the source is not playing or paused
        if (state != AL_PLAYING && state != AL_PAUSED)
        {
            // Get the number of queued buffers
            ALint queued;
            alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

            // If no buffers are queued, return from the function
            if (queued == 0) return;

            // Restart playback of the source
            alSourcePlay(source);
        }

        // Sleep for a short interval to control the decoding rate
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

_audio_impl::Music::Music(audio::Device& ctx, const std::string& filePath)
: Source(ctx), decodingThreadRunning(false)
{
    // Generate OpenAL buffers
    alGenBuffers(NUM_BUFFERS, buffers);

    // Open the specified audio file using the SndfileHandle
    fileHandle = SndfileHandle(filePath);

    // Check for errors during file opening
    if (fileHandle.error())
    {
        throw core::NexusException("Music", "Failed to load audio file [" + filePath + "].",
                                    "SNDFILE", fileHandle.strError());
    }

    // Set audio information based on the file
    SetAudioInfo(fileHandle);

    // Allocate memory buffer for audio decoding
    std::size_t frame_size = (static_cast<size_t>(BUFFER_SAMPLES * fileHandle.channels())) * sizeof(short);
    membuf = static_cast<short*>(malloc(frame_size));
}

_audio_impl::Music::~Music()
{
    // Stop the source before cleaning up
    alSourceStop(source);

    // Finish the decoding thread if it is joinable
    decodingThreadRunning = false;
    if (decodingThread.joinable())
    {
        decodingThread.join();
    }

    // Free, detach, and delete OpenAL buffers
    free(membuf);
    alSourcei(source, AL_BUFFER, 0);
    alDeleteBuffers(NUM_BUFFERS, buffers);
}

void _audio_impl::Music::Play()
{
    // Clear any previous OpenAL errors
    alGetError();

    // Rewind the source and clear existing buffers
    alSourceRewind(source);
    alSourcei(source, AL_BUFFER, 0);

    // Fill OpenAL buffers with audio data for playback
    for (ALsizei i = 0; i < NUM_BUFFERS; i++)
    {
        sf_count_t len = fileHandle.readf(membuf, BUFFER_SAMPLES);
        if (len < 1) break;

        len *= fileHandle.channels() * sizeof(short);
        alBufferData(buffers[i], format, membuf, (ALsizei)len, fileHandle.samplerate());
    }

    // Check for OpenAL errors after buffering
    if (alGetError() != AL_NO_ERROR)
    {
        throw core::NexusException("Music", "Error buffering for playback");
    }

    // Queue the buffers, start the source, and initiate the decoding thread
    alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
    alSourcePlay(source);

    // Start the decoding thread
    decodingThread = std::thread(&Music::DecodingThreadFunc, this);
    decodingThreadRunning = true;
}

void _audio_impl::Music::Pause()
{
    // Pause the OpenAL source
    alSourcePause(source);

    // Stop the decoding thread and wait for it to finish
    decodingThreadRunning = false;
    if (decodingThread.joinable())
    {
        decodingThread.join();
    }
}

void _audio_impl::Music::Rewind()
{
    // Stop the decoding thread and wait for it to finish
    decodingThreadRunning = false;
    if (decodingThread.joinable())
    {
        decodingThread.join();
    }

    // Rewind the OpenAL source and the audio file to the beginning
    alSourceRewind(source);
    fileHandle.seek(0, SEEK_SET);

    // Start playback from the beginning
    Play();
}

void _audio_impl::Music::Stop()
{
    // Stop the decoding thread and wait for it to finish
    decodingThreadRunning = false;
    if (decodingThread.joinable())
    {
        decodingThread.join();
    }

    // Stop the OpenAL source, rewind to the beginning, and stop the audio file
    alSourceStop(source);
    fileHandle.seek(0, SEEK_SET);
}
