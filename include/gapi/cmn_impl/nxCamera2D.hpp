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

#ifndef NEXUS_GAPI_IMPL_CAMERA_2D_HPP
#define NEXUS_GAPI_IMPL_CAMERA_2D_HPP

#include "../../math/nxMat4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"

namespace _gapi_impl {

    /**
     * @brief Represents a 2D camera for handling offset, target, rotation, and zoom.
     */
    struct Camera2D
    {
        nexus::math::Vec2 offset;   ///< The offset of the camera.
        nexus::math::Vec2 target;   ///< The target position of the camera.
        float rotation;             ///< The rotation angle of the camera.
        float zoom;                 ///< The zoom level of the camera.

        /**
         * @brief Constructs a new Camera2D object.
         * @param offset The initial offset of the camera.
         * @param target The initial target position of the camera.
         * @param rotation The initial rotation angle of the camera.
         * @param zoom The initial zoom level of the camera.
         */
        Camera2D(const nexus::math::Vec2& offset = { 0, 0 }, const nexus::math::Vec2& target = { 0, 0 }, float rotation = 0.0f, float zoom = 1.0f);

        /**
         * Translates the camera by the specified delta while optionally maintaining the proportionate movement relative to the zoom level.
         * @param delta The translation delta to apply.
         * @param zoomIndependent Whether the translation should be independent of the zoom level and maintain proportionate movement (default is true).
         */
        void Translate(const nexus::math::Vec2& delta, bool zoomIndependent = true);

        /**
         * @brief Adjusts the zoom level of the camera.
         * @param inc The increment to apply to the zoom level.
         * @param min The minimum zoom level allowed.
         * @param max The maximum zoom level allowed.
         */
        void Zoom(float inc, float min = 0.1f, float max = 10.0f, bool incZoomIndependant = true);

        /**
         * Zooms the camera towards the specified world position with a specified increment, minimum, and maximum zoom levels.
         * @param worldPosition The world position to zoom towards.
         * @param inc The zoom increment.
         * @param min The minimum zoom level (default is 0.1f).
         * @param max The maximum zoom level (default is 10.0f).
         */
        void ZoomToWorldPosition(const nexus::math::Vec2& worldPosition, float inc, float min = 0.1f, float max = 10.0f, bool incZoomIndependant = true);

        /**
         * Zooms the camera towards the specified screen position with a specified increment, minimum, and maximum zoom levels.
         * @param screenPosition The screen position to zoom towards.
         * @param inc The zoom increment.
         * @param min The minimum zoom level (default is 0.1f).
         * @param max The maximum zoom level (default is 10.0f).
         */
        void ZoomToScreenPosition(const nexus::math::Vec2& screenPosition, float inc, float min = 0.1f, float max = 10.0f, bool incZoomIndependant = true);

        /**
         * Zooms the camera to the mouse position with a specified increment, minimum and maximum zoom levels.
         * @param inc The zoom increment.
         * @param min The minimum zoom level (default is 0.1f).
         * @param max The maximum zoom level (default is 10.0f).
         */
        void ZoomToMouse(float inc, float min = 0.1f, float max = 10.0f, bool incZoomIndependant = true);

        /**
         * @brief Converts a point from world coordinates to screen coordinates.
         * @param point The point in world coordinates to convert.
         * @return The point converted to screen coordinates.
         */
        nexus::math::Vec2 ToScreen(const nexus::math::Vec2& point) const;

        /**
         * @brief Converts a point from screen coordinates to world coordinates.
         * @param point The point in screen coordinates to convert.
         * @return The point converted to world coordinates.
         */
        nexus::math::Vec2 ToWorld(const nexus::math::Vec2& point) const;

        /**
         * @brief Retrieves the 2D camera matrix.
         * @return The 2D camera matrix.
         */
        nexus::math::Mat4 GetCameraMatrix2D() const;
    };

}

#endif //NEXUS_GAPI_IMPL_CAMERA_2D_HPP
