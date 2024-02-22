#include "audio/nxDevice.hpp"

using namespace nexus::audio;

Device::Device(const char* deviceName)
{
    device = alcOpenDevice(deviceName);
    if (!device) throw core::NexusException("AudioDevice", "Unable to open OpenAL audio device");

#   ifdef ALC_EXT_EFX
        ALint attribs[4] = { ALC_MAX_AUXILIARY_SENDS, MAX_SOURCE_EFFECTS, 0, 0 };
#   else
        ALint *attribs = nullptr;
#   endif

    context = alcCreateContext(device, attribs);
    if (!context) throw core::NexusException("AudioDevice", "Unable to create an OpenAL audio context");

    if (!alcMakeContextCurrent(context))
    {
        throw core::NexusException("AudioDevice", "Unable to set current OpenAL audio context");
    }
}

Device::~Device()
{
#   ifdef ALC_EXT_EFX
        effects.clear();                            ///< Clear the map of audio effects.
#   endif

    alcMakeContextCurrent(nullptr);                 ///< Release the current audio context.
    alcDestroyContext(context);                     ///< Destroy the audio context.
    alcCloseDevice(device);                         ///< Close the audio device.
}