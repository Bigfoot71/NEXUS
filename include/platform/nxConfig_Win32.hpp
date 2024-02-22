#ifndef NEXUS_PLATFORM_WINDOWS_HPP
#define NEXUS_PLATFORM_WINDOWS_HPP

#if defined(BUILD_SHARED)
#   define NEXUS_API    __declspec(dllexport)
#elif defined(USE_SHARED)
#    define NEXUS_API   __declspec(dllimport)
#else
#   define NEXUS_API
#endif

#endif //NEXUS_PLATFORM_WINDOWS_HPP
