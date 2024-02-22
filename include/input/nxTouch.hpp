#ifndef NEXUS_INPUTS_TOUCH_HPP
#define NEXUS_INPUTS_TOUCH_HPP

#include "../platform/nxPlatform.hpp"
#include "../math/nxVec2.hpp"
#include <SDL_events.h>

// TODO: Add a touch management class

namespace nexus { namespace input {

    /**
     * @brief Type representing the ID of a touch device.
     */
    using TouchDeviceID = SDL_TouchID;

    /**
     * @brief Enumeration of touch device types.
     *
     * This enum defines identifiers for different types of touch devices, including invalid, direct, absolute, and relative devices.
     */
    enum class TouchDeviceType : int
    {
        Invalid     = SDL_TOUCH_DEVICE_INVALID,
        Direct      = SDL_TOUCH_DEVICE_DIRECT,              ///< Touch screen with window-relative coordinates.
        Absolute    = SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE,   ///< Trackpad with absolute device coordinates.
        Relative    = SDL_TOUCH_DEVICE_INDIRECT_RELATIVE    ///< Trackpad with screen cursor-relative coordinates.
    };

    /**
     * @brief Class representing a touch point.
     *
     * This class represents a touch point and provides information about its position, delta, and ID.
     */
    class NEXUS_API TouchPoint
    {
      private:
        SDL_Finger previous;    ///< Stores the previous state of the touch point
        SDL_Finger *current;    ///< Pointer to the current state of the touch point

      public:
        TouchPoint(SDL_Finger* finger) : current(finger) { }

        /**
         * @brief Update the previous state of the touch point.
         *
         * This function updates the previous state of the touch point to the current state.
         */
        void UpdatePreviousState()
        {
            std::copy(current, current + sizeof(SDL_Finger), &previous);
        }

        /**
         * @brief Get the current position of the touch point.
         *
         * @return A 2D vector representing the current position of the touch point.
         */
        math::Vec2 GetPosition()
        {
            return math::Vec2(current->x, current->y);
        }

        /**
         * @brief Get the delta between the current and previous positions of the touch point.
         *
         * @return A 2D vector representing the delta between the current and previous positions of the touch point.
         */
        math::Vec2 GetDelta()
        {
            return math::Vec2(current->x, current->y) - math::Vec2(previous.x, previous.y);
        }

        /**
         * @brief Get the unique identifier of the touch point.
         *
         * @return The ID of the touch point.
         */
        Sint64 GetID()
        {
            return current->id;
        }
    };

    /**
     * @brief Get the number of available touch devices.
     *
     * @return The number of available touch devices.
     */
    inline int GetTouchDevicesCount()
    {
        return SDL_GetNumTouchDevices();
    }

    /**
     * @brief Get the ID of a touch device by its index.
     *
     * @param touchDeviceIndex The index of the touch device.
     * @return The ID of the touch device.
     */
    inline TouchDeviceID GetTouchDeviceID(int touchDeviceIndex)
    {
        return SDL_GetTouchDevice(touchDeviceIndex);
    }

    /**
     * @brief Get the name of a touch device.
     *
     * @param touchDeviceId The ID of the touch device.
     * @return The name of the touch device as reported by the driver, or NULL if the ID is invalid.
     */
    inline const char* GetTouchDeviceName(TouchDeviceID touchDeviceId)
    {
        return SDL_GetTouchName(touchDeviceId);
    }

    /**
     * @brief Get the type of a touch device.
     *
     * @param touchDeviceId The ID of the touch device.
     * @return The type of the touch device.
     */
    inline TouchDeviceType GetTouchDeviceType(TouchDeviceID touchDeviceId)
    {
        return static_cast<TouchDeviceType>(SDL_GetTouchDeviceType(touchDeviceId));
    }

    /**
     * @brief Get the number of touch points associated with a touch device.
     *
     * @param touchDeviceId The ID of the touch device.
     * @return The number of touch points associated with the device.
     */
    inline int GetTouchPointCount(TouchDeviceID touchDeviceId)
    {
        return SDL_GetNumTouchFingers(touchDeviceId);
    }

    /**
     * @brief Get information about a touch point associated with a touch device.
     *
     * @param touchDeviceId The ID of the touch device.
     * @param fingerIndex The index of the touch point.
     * @return A `TouchPoint` object representing the touch point.
     */
    inline TouchPoint GetTouchPoint(TouchDeviceID touchDeviceId, int fingerIndex)
    {
        return TouchPoint(SDL_GetTouchFinger(touchDeviceId, fingerIndex));
    }

}}

#endif //NEXUS_INPUTS_TOUCH_HPP
