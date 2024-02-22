#ifndef NEXUS_PLATFORM_HPP
#define NEXUS_PLATFORM_HPP

#ifdef _WIN32
#   include "./nxConfig_Win32.hpp"
#elif __linux__
#   include "./nxConfig_Linux.hpp"
#elif __APPLE__
#   include "./nxConfig_Mac.hpp"
#elif __ANDROID__
#   include "./nxConfig_Android.hpp"
#endif

#endif //NEXUS_PLATFORM_HPP
