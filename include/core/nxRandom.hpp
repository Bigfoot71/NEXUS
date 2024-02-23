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

#ifndef NEXUS_CORE_RANDOM_HPP
#define NEXUS_CORE_RANDOM_HPP

#include "../platform/nxPlatform.hpp"

#include "../gfx/nxColor.hpp"
#include "../math/nxVec2.hpp"
#include "../math/nxVec3.hpp"
#include "../math/nxVec4.hpp"

#include <chrono>
#include <random>
#include <ctime>

namespace nexus { namespace core {

    /**
     * @brief The RandomGenerator class provides functionality for generating random values.
     */
    class NEXUS_API RandomGenerator
    {
      private:
        std::mt19937 generator;     ///< Mersenne Twister 19937 random number generator.
        unsigned long seed;         ///< Seed used for the random number generator.

      public:
        /**
         * @brief Constructs a RandomGenerator with an optional seed.
         * @param seed The seed for the random number generator. If not provided, it is generated from the current time.
         */
        RandomGenerator(unsigned long seed = 0)
        {
            SetSeed(seed);
        }

        /**
         * @brief Generates a random value using a discrete distribution.
         * @tparam T The type of the random value to be generated.
         * @param distribution The discrete distribution to use for generating the random value.
         * @return A random value generated based on the provided discrete distribution.
         */
        template<typename T>
        T operator()(std::discrete_distribution<T>& distribution)
        {
            return distribution(generator);
        }

        /**
         * @brief Sets the seed for the random number generator.
         * @param seed The seed to set. If not provided or set to 0, it is generated from the current time.
         */
        void SetSeed(unsigned long seed = 0)
        {
            if (seed == 0)
            {
                seed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            }
            generator.seed(seed);
            this->seed = seed;
        }

        /**
         * @brief Gets the current seed used by the random number generator.
         * @return The current seed.
         */
        unsigned long GetSeed() const
        {
            return seed;
        }

        /**
         * @brief Generates a random integral value within the specified range.
         * @tparam T The type of the value to generate (integral).
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         * @return A random integral value within the specified range.
         */
        template <typename T>
        T Random(T min, T max, std::enable_if_t<std::is_integral<T>::value>* = nullptr)
        {
            return std::uniform_int_distribution<T>(min, max)(generator);
        }

        /**
         * @brief Generates a random floating-point value within the specified range.
         * @tparam T The type of the value to generate (floating-point).
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         * @return A random floating-point value within the specified range.
         */
        template <typename T>
        T Random(T min, T max, std::enable_if_t<std::is_floating_point<T>::value>* = nullptr)
        {
            return std::uniform_real_distribution<T>(min, max)(generator);
        }

        /**
         * @brief Generates a random Vector2 within the specified range.
         * @param min The minimum values of the range for x and y.
         * @param max The maximum values of the range for x and y.
         * @return A random Vector2 within the specified range.
         */
        math::Vec2 RandomVec2(const math::Vec2& min, const math::Vec2& max)
        {
            return {
                std::uniform_real_distribution(min.x, max.x)(generator),
                std::uniform_real_distribution(min.y, max.y)(generator)
            };
        }

        /**
         * @brief Generates a random Vector3 within the specified range.
         * @param min The minimum values of the range for x, y, and z.
         * @param max The maximum values of the range for x, y, and z.
         * @return A random Vector3 within the specified range.
         */
        math::Vec3 RandomVec3(const math::Vec3& min, const math::Vec3& max)
        {
            return {
                std::uniform_real_distribution(min.x, max.x)(generator),
                std::uniform_real_distribution(min.y, max.y)(generator),
                std::uniform_real_distribution(min.z, max.z)(generator)
            };
        }

        /**
         * @brief Generates a random Vector4 within the specified range.
         * @param min The minimum values of the range for x, y, z, and w.
         * @param max The maximum values of the range for x, y, z, and w.
         * @return A random Vector4 within the specified range.
         */
        math::Vec4 RandomVec4(const math::Vec4& min, const math::Vec4& max)
        {
            return {
                std::uniform_real_distribution(min.x, max.x)(generator),
                std::uniform_real_distribution(min.y, max.y)(generator),
                std::uniform_real_distribution(min.z, max.z)(generator),
                std::uniform_real_distribution(min.w, max.w)(generator)
            };
        }

        /**
         * @brief Generates a random Color with the specified saturation and value.
         * @param sat The saturation value for the generated color.
         * @param val The value (brightness) value for the generated color.
         * @param a The alpha value for the generated color.
         * @return A random Color with the specified saturation and value.
         */
        gfx::Color RandomColor(float sat = 1.0f, float val = 1.0f, float a = 1.0f)
        {
            return gfx::Color({ std::uniform_real_distribution(0.0f, 360.0f)(generator), sat, val }, a);
        }

        /**
         * @brief Generates a random character from a given set of characters.
         * @param characters The set of characters to choose from.
         * @return A random character from the specified set.
         */
        char RandomChar(const std::string& characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
        {
            return characters[std::uniform_int_distribution<Uint32>(0, characters.size() - 1)(generator)];
        }

        /**
         * @brief Generates a random string of the specified length using a given set of characters.
         * @param length The length of the random string to generate.
         * @param characters The set of characters to choose from.
         * @return A random string of the specified length.
         */
        std::string RandomString(int length, const std::string& characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
        {
            std::uniform_int_distribution<Uint32> distribution(0, characters.size() - 1);
            std::string result(length, '\0');

            for (int i = 0; i < length; i++)
            {
                result[i] = characters[distribution(generator)];
            }

            return result;
        }
    };

}}

#endif //NEXUS_CORE_RANDOM_HPP
