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

#ifndef NEXUS_CORE_LOG_HPP
#define NEXUS_CORE_LOG_HPP

#include "../platform/nxPlatform.hpp"

#include <SDL_stdinc.h>
#include <iostream>

namespace nexus { namespace core {

    class NEXUS_API Log
    {
      public:
        enum class Level : Uint8
        {
            All         = 0,
            Debug       = 1,
            Info        = 2,
            Warning     = 3,
            Error       = 4,
            Fatal       = 5,
            None        = 6
        };

      public:
#       ifdef NDEBUG
        inline static Level level = Log::Level::Info;
#       else
        inline static Level level = Log::Level::Debug;
#       endif

      public:
        Log(Level level = Level::Info) : msgLevel(level) { }
        template <typename T> Log& operator<<(const T& msg);
        
      private:
        Level msgLevel;
        bool chain = false;

        std::string LevelToString()
        {
            switch (msgLevel)
            {
                case Level::Debug:      return "DEBUG";
                case Level::Info:       return "INFO";
                case Level::Warning:    return "WARNING";
                case Level::Error:      return "ERROR";
                case Level::Fatal:      return "FATAL";
                default:                return "UNKNOWN";
            }
        }
    };

    template <typename T> Log& Log::operator<<(const T& msg)
    {
        if (static_cast<int>(msgLevel) < static_cast<int>(Log::level)) return *this;
        if (!chain) { std::cout << LevelToString() << ": " << msg; chain = true; }
        else std::cout << msg;
        return *this;
    }

}}

#if !defined(NDEBUG) || defined(SHOW_LOG)
#   define NEXUS_LOG(level) nexus::core::Log( \
        nexus::core::Log::Level::level)
#else
#   define NEXUS_LOG(level) if constexpr(false) nexus::core::Log( \
        nexus::core::Log::Level::level)
#endif

#endif //NEXUS_CORE_LOG_HPP
