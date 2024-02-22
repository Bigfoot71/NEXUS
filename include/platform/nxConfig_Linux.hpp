#ifndef NEXUS_PLATFORM_LINUX_HPP
#define NEXUS_PLATFORM_LINUX_HPP

#if defined(BUILD_SHARED)
#   define NEXUS_API __attribute__((visibility("default")))
#else
#   define NEXUS_API
#endif

#endif //NEXUS_PLATFORM_LINUX_HPP
