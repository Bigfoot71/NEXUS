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

#include "audio/nxDevice.hpp"

using namespace nexus::audio;

Device::Device(const char* deviceName)
{
    device = alcOpenDevice(deviceName);
    if (!device) throw core::NexusException("AudioDevice", "Unable to open OpenAL audio device");

#   ifdef ALC_EXT_EFX
        ALint attribs[4] = { ALC_MAX_AUXILIARY_SENDS, MaxSourceEffects, 0, 0 };
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
    if (this->IsCurrent())
    {
        alcMakeContextCurrent(nullptr);             ///< Release the current audio context.
    }

    alcDestroyContext(context);                     ///< Destroy the audio context.
    alcCloseDevice(device);                         ///< Close the audio device.
}