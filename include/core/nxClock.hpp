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

#ifndef NEXUS_CORE_CLOCK_HPP
#define NEXUS_CORE_CLOCK_HPP

#include "../platform/nxPlatform.hpp"

#include <SDL_timer.h>
#include <cmath>
#include <array>

namespace nexus { namespace core {

    /**
     * @brief Class for controlling framerate and timing.
     */
    class NEXUS_API Clock
    {
      private:
        Uint64 startTicks;          ///< Timestamp at the beginning of a frame.
        Uint64 endTicks;            ///< Timestamp at the end of a frame.
        Uint32 targetFPS;           ///< Desired frames per second.
        Uint32 currentFPS;          ///< Current frames per second.
        Uint32 averageFPS;          ///< Average frames per second.
        Uint32 frameCount;          ///< Frame counter to get average FPS.
        float timeCount;            ///< Time counter to get average FPS.
        float targetDelta;          ///< Target time in seconds between each frame.
        float currentDelta;         ///< Current time in seconds between each frame.

      public:
        /**
         * @brief Constructs a Clock with a specified target frames per second (FPS).
         *
         * @param _targetFPS The desired frames per second for frame rate control.
         */
        Clock(Uint32 _targetFPS = 60)
        : startTicks(0), endTicks(0), targetFPS(_targetFPS), currentFPS(0)
        , frameCount(0), targetDelta(1.0f / _targetFPS), currentDelta(0)
        { }

        /**
         * @brief Start measuring the time for the current frame.
         */
        void Begin()
        {
            startTicks = SDL_GetPerformanceCounter();
        }

        /**
         * @brief End the frame and control frame rate.
         *
         * This function calculates the time elapsed for the current frame and controls the frame rate.
         * It also attempts to delay execution to achieve the target frame rate.
         */
        void End()
        {
            endTicks = SDL_GetPerformanceCounter();
            currentDelta = static_cast<float>(endTicks - startTicks) / SDL_GetPerformanceFrequency(); // time elapsed in seconds since Begin

            if (currentDelta < targetDelta)
            {
                SDL_Delay((targetDelta - currentDelta) * 1000);
                currentDelta += static_cast<float>(SDL_GetPerformanceCounter() - endTicks) / SDL_GetPerformanceFrequency();
            }

            currentFPS = static_cast<Uint32>(1.0f / currentDelta);
            timeCount += currentDelta, frameCount++;

            if (timeCount >= 1.0f)
            {
                averageFPS = static_cast<float>(frameCount) / timeCount;
                frameCount = timeCount = 0;
            }
        }

        /**
         * @brief Get the time elapsed in seconds for the current frame.
         *
         * @return The time elapsed in seconds.
         */
        float GetDelta() const
        {
            return currentDelta;
        }

        /**
         * @brief Get the target time in seconds between frames.
         *
         * @return The target time in seconds.
         */
        float GetTargetDelta() const
        {
            return targetDelta;
        }

        /**
         * @brief Get the current frames per second.
         *
         * @return The current frames per second.
         */
        Uint32 GetFPS() const
        {
            return currentFPS;
        }

        /**
         * @brief Get the average frames per second.
         *
         * @return The average frames per second.
         */
        float GetAverageFPS() const
        {
            return averageFPS;
        }

        /**
         * @brief Get the target frames per second.
         *
         * @return The desired frames per second for frame rate control.
         */
        Uint32 GetTargetFPS() const
        {
            return targetFPS;
        }

        /**
         * @brief Set a new target frames per second.
         *
         * @param target The new desired frames per second for frame rate control.
         */
        void SetTargetFPS(Uint32 target)
        {
            targetDelta = 1.0f / target;
            targetFPS = target;
        }
    };

}}

#endif //NEXUS_CORE_CLOCK_HPP
