#ifndef NEXUS_SR_DEPTH_BUFFER_HPP
#define NEXUS_SR_DEPTH_BUFFER_HPP

#include "../../platform/nxPlatform.hpp"

#include <SDL_stdinc.h>
#include <algorithm>
#include <vector>
#include <limits>

namespace nexus { namespace sr {

    /**
     * @brief Represents a depth buffer for 2D graphics rendering.
     */
    struct NEXUS_API DepthBuffer
    {
        static constexpr float MaxDepth = std::numeric_limits<float>::max();

        std::vector<float> buffer;  ///< The depth buffer storing depth values for each pixel.
        Uint32 width;               ///< Width of the depth buffer.

        /**
         * @brief Constructor for DepthBuffer.
         * @param w Width of the depth buffer.
         * @param h Height of the depth buffer.
         */
        DepthBuffer(int w, int h)
        : buffer(w * h, MaxDepth)
        , width(w)
        { }

        /**
         * @brief Resizes the depth buffer to the specified dimensions.
         * 
         * This function resizes the depth buffer to accommodate the given width and height.
         * It adjusts the internal buffer size accordingly, discarding any previous depth data.
         * 
         * @param w New width of the depth buffer.
         * @param h New height of the depth buffer.
         */
        void Resize(int w, int h)
        {
            buffer.resize(w * h, MaxDepth);
            width = w;
        }

        /**
         * @brief Clears the depth buffer by setting all values to maximum float.
         */
        void Clear()
        {
            std::fill(buffer.begin(), buffer.end(), MaxDepth);
        }

        /**
         * @brief Gets the depth value at the specified position.
         *
         * @warning: Does not check buffer bounds.
         *
         * @param x X-coordinate.
         * @param y Y-coordinate.
         *
         * @return Reference to the depth value at the specified position.
         */
        float& Get(int x, int y)
        {
            return buffer[y * width + x];
        }

        /**
         * @brief Gets the depth value at the specified position (using linear index).
         *
         * @warning: Does not check buffer bounds.
         *
         * @param i Linear index representing the position in the depth buffer.
         *
         * @return Reference to the depth value at the specified position.
         */
        float& Get(int i)
        {
            return buffer[i];
        }

        /**
         * @brief Gets the depth value at the specified position (const version).
         *
         * @warning: Does not check buffer bounds.
         *
         * @param x X-coordinate.
         * @param y Y-coordinate.
         *
         * @return Depth value at the specified position.
         */
        float Get(int x, int y) const
        {
            return buffer[y * width + x];
        }

        /**
         * @brief Gets the depth value at the specified position (const version using linear index).
         *
         * @warning: Does not check buffer bounds.
         *
         * @param i Linear index representing the position in the depth buffer.
         *
         * @return Depth value at the specified position.
         */
        float Get(int i) const
        {
            return buffer[i];
        }

        /**
         * @brief Sets the depth value at the specified position if the given Z position is shallower.
         *
         * @warning: Does not check buffer bounds.
         *
         * @param x X-coordinate.
         * @param y Y-coordinate.
         * @param z Z-coordinate (depth value).
         *
         * @return True if the depth has changed, false otherwise.
         */
        bool SetDepth(int x, int y, float z)
        {
            float &zBufferPosition = buffer[y * width + x];
            if (z > zBufferPosition) return false;
            zBufferPosition = z;
            return true;
        }

        /**
         * @brief Sets the depth value at the specified position if the given Z position is shallower.
         *
         * @warning: Does not check buffer bounds.
         *
         * @param i Linear index representing the position in the depth buffer.
         * @param z Z-coordinate (depth value).
         *
         * @return True if the depth has changed, false otherwise.
         */
        bool SetDepth(int i, float z)
        {
            float &zBufferPosition = buffer[i];
            if (z > zBufferPosition) return false;
            zBufferPosition = z;
            return true;
        }

        /**
         * @brief Forces the depth value at the specified position.
         *
         * @warning: Does not check buffer bounds.
         *
         * @param x X-coordinate.
         * @param y Y-coordinate.
         * @param z Z-coordinate (depth value).
         */
        void ForceDepth(int x, int y, float z)
        {
            buffer[y * width + x] = z;
        }

        /**
         * @brief Forces the depth value at the specified position (using linear index).
         *
         * @warning: Does not check buffer bounds.
         *
         * @param i Linear index representing the position in the depth buffer.
         * @param z Z-coordinate (depth value).
         */
        void ForceDepth(int i, float z)
        {
            buffer[i] = z;
        }
    };

}}

#endif //NEXUS_SR_DEPTH_BUFFER_HPP
