#ifndef NEXUS_INPUTS_GAMEPAD_HPP
#define NEXUS_INPUTS_GAMEPAD_HPP

#include "../platform/nxPlatform.hpp"
#include <SDL_events.h>
#include <limits>

namespace nexus { namespace input {

    /**
     * @brief Type representing a gamepad.
     */
    using Gamepad = SDL_GameController;

    /**
     * @brief Enumeration of gamepad buttons.
     *
     * This enum defines identifiers for different gamepad buttons, including A, B, X, Y, and more.
     */
    enum class GamepadButton : int
    {
        Invalid         = SDL_CONTROLLER_BUTTON_INVALID,
        A               = SDL_CONTROLLER_BUTTON_A,
        B               = SDL_CONTROLLER_BUTTON_B,
        X               = SDL_CONTROLLER_BUTTON_X,
        Y               = SDL_CONTROLLER_BUTTON_Y,
        Back            = SDL_CONTROLLER_BUTTON_BACK,
        Guide           = SDL_CONTROLLER_BUTTON_GUIDE,
        Start           = SDL_CONTROLLER_BUTTON_START,
        LeftStick       = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        RightStick      = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        LeftShoulder    = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        RightShoulder   = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        DPadUp          = SDL_CONTROLLER_BUTTON_DPAD_UP,
        DPadDown        = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        DPadLeft        = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        DPadRight       = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        Misc1           = SDL_CONTROLLER_BUTTON_MISC1,              ///< Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button
        Paddle1         = SDL_CONTROLLER_BUTTON_PADDLE1,            ///< Xbox Elite paddle P1 (upper left, facing the back)
        Paddle2         = SDL_CONTROLLER_BUTTON_PADDLE2,            ///< Xbox Elite paddle P3 (upper right, facing the back)
        Paddle3         = SDL_CONTROLLER_BUTTON_PADDLE3,            ///< Xbox Elite paddle P2 (lower left, facing the back)
        Paddle4         = SDL_CONTROLLER_BUTTON_PADDLE4,            ///< Xbox Elite paddle P4 (lower right, facing the back)
        Touchpad        = SDL_CONTROLLER_BUTTON_TOUCHPAD,           ///< PS4/PS5 touchpad button
    };

    /**
     * @brief Enumeration of gamepad axes.
     *
     * This enum defines identifiers for different gamepad axes, including LeftX, LeftY, RightX, RightY, and more.
     */
    enum class GamepadAxis : int
    {
        Invalid         = SDL_CONTROLLER_AXIS_INVALID,
        LeftX           = SDL_CONTROLLER_AXIS_LEFTX,
        LeftY           = SDL_CONTROLLER_AXIS_LEFTY,
        RightX          = SDL_CONTROLLER_AXIS_RIGHTX,
        RightY          = SDL_CONTROLLER_AXIS_RIGHTY,
        TriggerLeft     = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        TriggerRight    = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    };

    /**
     * @brief Structure representing the state of a gamepad.
     */
    struct NEXUS_API GamepadState
    {
        bool buttons[SDL_CONTROLLER_BUTTON_MAX];
        int axes[SDL_CONTROLLER_AXIS_MAX];
    };

    /**
     * @brief Check if a gamepad is connected by its index.
     *
     * @param gamepadIndex The index of the gamepad.
     * @return True if the gamepad is connected; false otherwise.
     */
    inline bool IsGamepadConnected(int gamepadIndex)
    {
        return SDL_IsGameController(gamepadIndex) == 1;
    }

    /**
     * @brief Open a gamepad by its index.
     *
     * @param gamepadIndex The index of the gamepad to open.
     * @return A pointer to the opened gamepad or nullptr if the gamepad is not found.
     */
    inline Gamepad* OpenGamepad(int gamepadIndex)
    {
        return SDL_GameControllerOpen(gamepadIndex);
    }

    /**
     * @brief Close an opened gamepad.
     *
     * @param gamepad A pointer to the gamepad to close.
     */
    inline void CloseGamepad(Gamepad* gamepad)
    {
        SDL_GameControllerClose(gamepad);
    }

    /**
     * @brief Get the state of a gamepad by its pointer.
     *
     * @param gamepad A pointer to the gamepad.
     * @return The state of the gamepad as a `GamepadState` structure.
     */
    inline GamepadState GetGamepadState(Gamepad* gamepad)
    {
        GamepadState state;

        for (int button = 0; button < SDL_CONTROLLER_BUTTON_MAX; button++)
        {
            state.buttons[button] = SDL_GameControllerGetButton(gamepad, static_cast<SDL_GameControllerButton>(button)) == 1;
        }

        for (int axis = 0; axis < SDL_CONTROLLER_AXIS_MAX; axis++)
        {
            state.axes[axis] = SDL_GameControllerGetAxis(gamepad, static_cast<SDL_GameControllerAxis>(axis));
        }

        return state;
    }

    /**
     * @brief Get the state of a gamepad by its index.
     *
     * @param gamepadIndex The index of the gamepad to retrieve the state from.
     * @return The state of the gamepad as a `GamepadState` structure.
     */
    inline GamepadState GetGamepadState(int gamepadIndex)
    {
        SDL_GameController* gamepad = OpenGamepad(gamepadIndex);

        if (gamepad)
        {
            GamepadState state = GetGamepadState(gamepad);
            CloseGamepad(gamepad);
            return state;
        }

        GamepadState state{}; // Initialize with default values
        return state;
    }

    /**
     * @brief Update the state of a gamepad by its pointer.
     *
     * @param gamepad A pointer to the gamepad to update.
     * @param state The state of the gamepad as a `GamepadState` structure.
     */
    inline void UpdateGamepadState(SDL_GameController* gamepad, GamepadState& state)
    {
        state = GetGamepadState(gamepad);
    }

    /**
     * @brief Update the state of a gamepad by its index.
     *
     * @param gamepadIndex The index of the gamepad to update.
     * @param state The state of the gamepad as a `GamepadState` structure.
     */
    inline void UpdateGamepadState(int gamepadIndex, GamepadState& state)
    {
        SDL_GameController* gamepad = OpenGamepad(gamepadIndex);

        if (gamepad)
        {
            UpdateGamepadState(gamepad, state);
            CloseGamepad(gamepad);
        }
    }

    /**
     * @brief Check if a specific button on a gamepad is pressed.
     *
     * @param gamepad Pointer to the gamepad.
     * @param button The button to check.
     * @return True if the button is pressed, false otherwise.
     */
    inline bool IsGamepadButtonPressed(Gamepad* gamepad, GamepadButton button)
    {
        return SDL_GameControllerGetButton(gamepad, static_cast<SDL_GameControllerButton>(button));
    }

    /**
     * @brief Check if a specific button on a gamepad is pressed.
     *
     * @param gamepadIndex Index of the gamepad.
     * @param button The button to check.
     * @return True if the button is pressed, false otherwise.
     */
    inline bool IsGamepadButtonPressed(int gamepadIndex, GamepadButton button)
    {
        bool result = false;
        Gamepad* gamepad = OpenGamepad(gamepadIndex);

        if (gamepad)
        {
            result = SDL_GameControllerGetButton(gamepad, static_cast<SDL_GameControllerButton>(button));
            CloseGamepad(gamepad);
        }

        return result;
    }

    /**
     * @brief Get the movement of a specific axis on a gamepad.
     *
     * @param gamepad Pointer to the gamepad.
     * @param axis The axis to check.
     * @return The movement of the axis, normalized between -1.0 and 1.0.
     */
    inline float GetGamepadAxisMovement(Gamepad* gamepad, GamepadAxis axis)
    {
        Sint16 iResult = SDL_GameControllerGetAxis(gamepad, static_cast<SDL_GameControllerAxis>(axis));
        return static_cast<float>(iResult) / std::numeric_limits<Sint16>::max();
    }

    /**
     * @brief Get the movement of a specific axis on a gamepad.
     *
     * @param gamepadIndex Index of the gamepad.
     * @param axis The axis to check.
     * @return The movement of the axis, normalized between -1.0 and 1.0. Returns 0.0 if the gamepad is not available.
     */
    inline float GetGamepadAxisMovement(int gamepadIndex, GamepadAxis axis)
    {
        float result = 0.0f;
        Gamepad* gamepad = OpenGamepad(gamepadIndex);

        if (gamepad)
        {
            Sint16 iResult = SDL_GameControllerGetAxis(gamepad, static_cast<SDL_GameControllerAxis>(axis));
            return static_cast<float>(iResult) / std::numeric_limits<Sint16>::max();
        }

        return result;
    }

    /**
     * @brief Get the name of the gamepad.
     *
     * @param gamepad Pointer to the gamepad.
     * @return A pointer to a string containing the name of the gamepad.
     */
    inline const char* GetGamepadName(Gamepad* gamepad)
    {
        return SDL_GameControllerName(gamepad);
    }

    /**
     * @brief Get the name of a gamepad by its index.
     *
     * @param gamepadIndex The index of the gamepad.
     * @return The name of the gamepad or nullptr if the gamepad is not found.
     */
    inline const char* GetGamepadName(int gamepadIndex)
    {
        return SDL_GameControllerNameForIndex(gamepadIndex);
    }

}}

#endif //NEXUS_INPUTS_GAMEPAD_HPP
