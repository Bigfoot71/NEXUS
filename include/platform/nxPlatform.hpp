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
