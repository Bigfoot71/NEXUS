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

#ifndef NEXUS_INPUTS_MOUSE_HPP
#define NEXUS_INPUTS_MOUSE_HPP

#include "../platform/nxPlatform.hpp"
#include "../math/nxVec2.hpp"

#include <SDL_events.h>
#include <SDL_mouse.h>

namespace nexus { namespace input {

    /**
     * @brief Enumeration of mouse button identifiers.
     *
     * This enum defines identifiers for mouse buttons, such as Left, Middle, Right, X1, and X2.
     */
    enum class MouseButton : int
    {
        Left    = SDL_BUTTON_LEFT,    ///< Left mouse button.
        Middle  = SDL_BUTTON_MIDDLE,  ///< Middle mouse button.
        Right   = SDL_BUTTON_RIGHT,   ///< Right mouse button.
        X1      = SDL_BUTTON_X1,      ///< Extra mouse button 1.
        X2      = SDL_BUTTON_X2       ///< Extra mouse button 2.
    };

    /**
     * @brief Equality comparison between MouseButton and Uint8.
     */
    constexpr bool operator==(MouseButton button, Uint8 value)
    {
        return static_cast<Uint8>(button) == value;
    }

    /**
     * @brief Equality comparison between Uint8 and MouseButton.
     */
    constexpr bool operator==(Uint8 value, MouseButton button)
    {
        return button == value;
    }

    /**
     * @brief Inequality comparison between MouseButton and Uint8.
     */
    constexpr bool operator!=(MouseButton button, Uint8 value)
    {
        return static_cast<Uint8>(button) != value;
    }

    /**
     * @brief Inequality comparison between Uint8 and MouseButton.
     */
    constexpr bool operator!=(Uint8 value, MouseButton button)
    {
        return button != value;
    }

    /**
     * @brief Less than comparison between MouseButton and Uint8.
     */
    constexpr bool operator<(MouseButton button, Uint8 value)
    {
        return static_cast<Uint8>(button) < value;
    }

    /**
     * @brief Less than comparison between Uint8 and MouseButton.
     */
    constexpr bool operator<(Uint8 value, MouseButton button)
    {
        return button < value;
    }

    /**
     * @brief Less than or equal to comparison between MouseButton and Uint8.
     */
    constexpr bool operator<=(MouseButton button, Uint8 value)
    {
        return static_cast<Uint8>(button) <= value;
    }

    /**
     * @brief Less than or equal to comparison between Uint8 and MouseButton.
     */
    constexpr bool operator<=(Uint8 value, MouseButton button)
    {
        return button <= value;
    }

    /**
     * @brief Greater than comparison between MouseButton and Uint8.
     */
    constexpr bool operator>(MouseButton button, Uint8 value)
    {
        return static_cast<Uint8>(button) > value;
    }

    /**
     * @brief Greater than comparison between Uint8 and MouseButton.
     */
    constexpr bool operator>(Uint8 value, MouseButton button)
    {
        return button > value;
    }

    /**
     * @brief Greater than or equal to comparison between MouseButton and Uint8.
     */
    constexpr bool operator>=(MouseButton button, Uint8 value)
    {
        return static_cast<Uint8>(button) >= value;
    }

    /**
     * @brief Greater than or equal to comparison between Uint8 and MouseButton.
     */
    constexpr bool operator>=(Uint8 value, MouseButton button)
    {
        return button >= value;
    }

    /**
     * @brief Get the mouse position within the active window.
     *
     * This function retrieves the mouse position within the active window and returns it as a 2D vector.
     *
     * @return The mouse position within the active window as a 2D vector.
     */
    inline math::IVec2 GetMousePosition()
    {
        math::IVec2 position;
        SDL_GetMouseState(&position.x, &position.y);
        return position;
    }

    /**
     * @brief Get the global mouse position on the screen.
     *
     * This function retrieves the global mouse position on the screen and returns it as a 2D vector.
     *
     * @return The global mouse position on the screen as a 2D vector.
     */
    inline math::IVec2 GetMousePositionGlobal()
    {
        math::IVec2 position;
        SDL_GetGlobalMouseState(&position.x, &position.y);
        return position; 
    }

    /**
     * @brief Get the mouse movement delta.
     *
     * This function retrieves the mouse movement delta (change in position) since the last call and returns it as a 2D vector.
     *
     * @return The mouse movement delta as a 2D vector.
     */
    inline math::IVec2 GetMouseDelta()
    {
        math::IVec2 delta;
        SDL_GetRelativeMouseState(&delta.x, &delta.y);
        return delta;
    }

    /**
     * @brief Check if a mouse button is currently pressed.
     *
     * This function checks if any mouse button is currently pressed.
     *
     * @return `true` if the button is pressed; otherwise, `false`.
     */
    inline bool IsMouseButtonPressed()
    {
        return SDL_GetMouseState(nullptr, nullptr);
    }

    /**
     * @brief Check if a mouse button is currently pressed.
     *
     * This function checks if the specified mouse button is currently pressed.
     *
     * @param button The mouse button to check.
     * @return `true` if the button is pressed; otherwise, `false`.
     */
    inline bool IsMouseButtonPressed(MouseButton button)
    {
        return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(static_cast<int>(button));
    }

    /**
     * @brief Set the mouse cursor position.
     *
     * This function sets the mouse cursor position to the specified coordinates within the active window.
     *
     * @param x The X-coordinate of the new mouse cursor position.
     * @param y The Y-coordinate of the new mouse cursor position.
     */
    inline void SetMousePosition(int x, int y)
    {
        SDL_WarpMouseInWindow(nullptr, x, y);
    }

    /**
     * @brief Set the mouse cursor position using a 2D vector.
     *
     * This function sets the mouse cursor position to the coordinates specified by the provided 2D vector within the active window.
     *
     * @param position The 2D vector representing the new mouse cursor position.
     */
    inline void SetMousePosition(const math::IVec2& position)
    {
        SDL_WarpMouseInWindow(nullptr, position.x, position.y);
    }

    /**
     * @brief Show the mouse cursor.
     *
     * This function shows the mouse cursor on the screen.
     */
    inline void ShowMouseCursor()
    {
        SDL_ShowCursor(SDL_ENABLE);
    }

    /**
     * @brief Hide the mouse cursor.
     *
     * This function hides the mouse cursor on the screen.
     */
    inline void HideMouseCursor()
    {
        SDL_ShowCursor(SDL_DISABLE);
    }

    /**
     * @brief Capture the mouse to enable relative mouse movement.
     *
     * This function captures the mouse to enable relative mouse movement (useful for first-person camera controls).
     */
    inline void CaptureMouse()
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    /**
     * @brief Release the captured mouse to restore normal mouse movement.
     *
     * This function releases the captured mouse to restore normal mouse movement.
     */
    inline void ReleaseMouse()
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    /**
     * @brief Check if the mouse is currently captured.
     *
     * This function checks if the mouse is currently captured (in relative mouse mode).
     *
     * @return `true` if the mouse is captured; otherwise, `false`.
     */
    inline bool IsMouseCaptured()
    {
        return SDL_GetRelativeMouseMode();
    }

    /**
     * @brief Toggle mouse capture mode.
     *
     * This function toggles the mouse capture mode. If the mouse is currently captured, it will be released, and vice versa.
     */
    inline void ToggleMouseCapture()
    {
        (IsMouseCaptured() ? ReleaseMouse : CaptureMouse)();
    }

}}

#endif // NEXUS_INPUTS_MOUSE_HPP
