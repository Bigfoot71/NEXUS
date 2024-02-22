#ifndef NEXUS_PLATFORM_MAC_HPP
#define NEXUS_PLATFORM_MAC_HPP

#ifdef defined(BUILD_SHARED)
#   define NEXUS_API __attribute__ ((visibility("default"))) 
#else
#   define NEXUS_API
#endif

#endif //NEXUS_PLATFORM_MAC_HPP
