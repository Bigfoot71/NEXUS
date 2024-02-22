#include "audio/nxSound.hpp"

using namespace nexus::audio;

Sound::Sound(const std::string& filePath) : Source()
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
        throw core::NexusException("Sound:", "Failed to read audio data from file " + filePath);
    }
}

Sound::~Sound()
{
    // Stop source before if needed
    alSourceStop(source);

    // Detach and free buffer
    alSourcei(source, AL_BUFFER, 0);
    alDeleteBuffers(1, &buffer);
}
