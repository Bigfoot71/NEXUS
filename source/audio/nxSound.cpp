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

#include "audio/nxSound.hpp"

using namespace nexus;

_audio_impl::Sound::Sound(audio::Device& ctx, const std::string& filePath) : _audio_impl::Source(ctx)
{
    SndfileHandle fileHandle(filePath);

    if (fileHandle.error())
    {
        throw core::NexusException(
            "Sound", "Failed to load audio file [" + filePath + "].",
            "SNDFILE", fileHandle.strError());
    }

    // Get info (frameCount, sampleRate, channels)
    SetAudioInfo(fileHandle);

    // Generate OpenAL buffer
    alGenBuffers(1, &buffer);

    // Allocate memory to store audio data and read the data
    std::vector<short> audioData(frameCount * channels);
    sf_count_t framesRead = fileHandle.readf(audioData.data(), frameCount);

    if (framesRead > 0)
    {
        // Set audio data for the OpenAL buffer and attach the buffer to the source
        alBufferData(buffer, format, audioData.data(), audioData.size(), sampleRate);
        alSourcei(source, AL_BUFFER, buffer);
    }
    else
    {
        throw core::NexusException("audio::Sound",
            "Failed to read audio data from file " + filePath);
    }
}

_audio_impl::Sound::~Sound()
{
    // Stop source before if needed
    alSourceStop(source);

    // Detach and free buffer
    alSourcei(source, AL_BUFFER, 0);
    alDeleteBuffers(1, &buffer);
}
