#ifndef NEXUS_AUDIO_MISCS_HPP
#define NEXUS_AUDIO_MISCS_HPP

#include "../platform/nxPlatform.hpp"
#include <AL/al.h>

namespace nexus { namespace audio {

    enum NEXUS_API Wave : ALint
    {
        Sine        = 0,
        Triangle    = 1
    };

}}

#endif //NEXUS_AUDIO_MISCS_HPP
