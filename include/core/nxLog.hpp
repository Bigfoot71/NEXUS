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
