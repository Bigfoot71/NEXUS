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

#ifndef NEXUS_CORE_EVENT_HPP
#define NEXUS_CORE_EVENT_HPP

#include "../platform/nxPlatform.hpp"
#include "core/nxException.hpp"
#include <SDL.h>

namespace nexus { namespace core {

    class NEXUS_API Event
    {
      public:
        using Common = SDL_CommonEvent;                             ///< Common event data
        using Display = SDL_DisplayEvent;                           ///< Display event data
        using Window = SDL_WindowEvent;                             ///< Window event data
        using Keyboard = SDL_KeyboardEvent;                         ///< Keyboard event data
        using TextEditing = SDL_TextEditingEvent;                   ///< Text editing event data
        using TextEditingExt = SDL_TextEditingExtEvent;             ///< Extended text editing event data
        using TextInput = SDL_TextInputEvent;                       ///< Text input event data
        using MouseMotion = SDL_MouseMotionEvent;                   ///< Mouse motion event data
        using MouseButton = SDL_MouseButtonEvent;                   ///< Mouse button event data
        using MouseWheel = SDL_MouseWheelEvent;                     ///< Mouse wheel event data
        using JoyAxis = SDL_JoyAxisEvent;                           ///< Joystick axis event data
        using JoyBall = SDL_JoyBallEvent;                           ///< Joystick ball event data
        using JoyHat = SDL_JoyHatEvent;                             ///< Joystick hat event data
        using JoyButton = SDL_JoyButtonEvent;                       ///< Joystick button event data
        using JoyDevice = SDL_JoyDeviceEvent;                       ///< Joystick device change event data
        using JoyBattery = SDL_JoyBatteryEvent;                     ///< Joystick battery event data
        using ControllerAxis = SDL_ControllerAxisEvent;             ///< Game Controller axis event data
        using ControllerButton = SDL_ControllerButtonEvent;         ///< Game Controller button event data
        using ControllerDevice = SDL_ControllerDeviceEvent;         ///< Game Controller device event data
        using ControllerTouchpad = SDL_ControllerTouchpadEvent;     ///< Game Controller touchpad event data
        using ControllerSensor = SDL_ControllerSensorEvent;         ///< Game Controller sensor event data
        using AudioDevice = SDL_AudioDeviceEvent;                   ///< Audio device event data
        using Sensor = SDL_SensorEvent;                             ///< Sensor event data
        using Quit = SDL_QuitEvent;                                 ///< Quit request event data
        using User = SDL_UserEvent;                                 ///< Custom event data
        using SysWM = SDL_SysWMEvent;                               ///< System dependent window event data
        using TouchFinger = SDL_TouchFingerEvent;                   ///< Touch finger event data
        using MultiGesture = SDL_MultiGestureEvent;                 ///< Gesture event data
        using DollarGesture = SDL_DollarGestureEvent;               ///< Gesture event data
        using Drop = SDL_DropEvent;                                 ///< Drag and drop event data

      public:
        enum class Type : Uint32
        {
            FirstEvent                  = SDL_FIRSTEVENT,               ///< Unused (do not remove)

            // Application events

            Quit                        = SDL_QUIT,                     ///< User-requested quit

            // These application events have special meaning on iOS, see sdl/README-ios.md for details

            Teraminating                = SDL_APP_TERMINATING,          /**< The application is being terminated by the OS
                                                                                Called on iOS in applicationWillTerminate()
                                                                                Called on Android in onDestroy()
                                                                        */

            LowMemory                   = SDL_APP_LOWMEMORY,            /**< The application is low on memory, free memory if possible.
                                                                                Called on iOS in applicationDidReceiveMemoryWarning()
                                                                                Called on Android in onLowMemory()
                                                                        */

            WillEnterBackground         = SDL_APP_WILLENTERBACKGROUND,  /**< The application is about to enter the background
                                                                                Called on iOS in applicationWillResignActive()
                                                                                Called on Android in onPause()
                                                                        */

            DidEnterBackground          = SDL_APP_DIDENTERBACKGROUND,   /**< The application did enter the background and may not get CPU for some time
                                                                                Called on iOS in applicationDidEnterBackground()
                                                                                Called on Android in onPause()
                                                                        */

            WillEnterForeground         = SDL_APP_WILLENTERFOREGROUND,  /**< The application is about to enter the foreground
                                                                                Called on iOS in applicationWillEnterForeground()
                                                                                Called on Android in onResume()
                                                                        */

            DidEnterForeground          = SDL_APP_DIDENTERFOREGROUND,   /**< The application is now interactive
                                                                                Called on iOS in applicationDidBecomeActive()
                                                                                Called on Android in onResume()
                                                                        */

            LocaleChanged               = SDL_LOCALECHANGED,            ///< The user's locale preferences have changed.

            // Display events

            DisplayEvent                = SDL_DISPLAYEVENT,             ///< Display state change

            // Window events

            WindowEvent                 = SDL_WINDOWEVENT,              ///< Window state change
            SysWMEvent                  = SDL_SYSWMEVENT,               ///< System specific event

            // Keyboard events

            KeyDown                     = SDL_KEYDOWN,                  ///< Key pressed
            KeyUp                       = SDL_KEYUP,                    ///< Key released
            TextEditing                 = SDL_TEXTEDITING,              ///< Keyboard text editing (composition)
            TextInput                   = SDL_TEXTINPUT,                ///< Keyboard text input
            KeyMapChanged               = SDL_KEYMAPCHANGED,            ///< Keymap changed due to a system event such as an input language or keyboard layout change.
            TextEditingExt              = SDL_TEXTEDITING_EXT,          ///< Extended keyboard text editing (composition)

            // Mouse events

            MouseMotion                 = SDL_MOUSEMOTION,              ///< Mouse moved
            MouseButtonDown             = SDL_MOUSEBUTTONDOWN,          ///< Mouse button pressed
            MouseButtonUp               = SDL_MOUSEBUTTONUP,            ///< Mouse button released
            MouseButtonWheel            = SDL_MOUSEWHEEL,               ///< Mouse wheel motion

            // Joystick events

            JoyAxisMotion               = SDL_JOYAXISMOTION,            ///< Joystick axis motion
            JoyBallMotion               = SDL_JOYBALLMOTION,            ///< Joystick trackball motion
            JoyHatMotion                = SDL_JOYHATMOTION,             ///< Joystick hat position change
            JoyButtonDown               = SDL_JOYBUTTONDOWN,            ///< Joystick button pressed
            JoyButtonUp                 = SDL_JOYBUTTONUP,              ///< Joystick button released
            JoyDeviceAdded              = SDL_JOYDEVICEADDED,           ///< A new joystick has been inserted into the system
            JoyDeviceRemoved            = SDL_JOYDEVICEREMOVED,         ///< An opened joystick has been removed
            JoyBatteryUpdated           = SDL_JOYBATTERYUPDATED,        ///< Joystick battery level change

            // Game controller events

            ControllerAxisMotion        = SDL_CONTROLLERAXISMOTION,     ///< Game controller axis motion
            ControllerButtonDown        = SDL_CONTROLLERBUTTONDOWN,     ///< Game controller button pressed
            ControllerButtonUp          = SDL_CONTROLLERBUTTONUP,       ///< Game controller button released
            ControllerDeviceAdded       = SDL_CONTROLLERDEVICEADDED,    ///< A new Game controller has been inserted into the system
            ControllerDeviceRemoved     = SDL_CONTROLLERDEVICEREMOVED,  ///< An opened Game controller has been removed
            ControllerDeviceRemapped    = SDL_CONTROLLERDEVICEREMAPPED, ///< The controller mapping was updated
            ControllerTouchPadDown      = SDL_CONTROLLERTOUCHPADDOWN,   ///< Game controller touchpad was touched
            ControllerTouchPadMotion    = SDL_CONTROLLERTOUCHPADMOTION, ///< Game controller touchpad finger was moved
            ControllerTouchPadUp        = SDL_CONTROLLERTOUCHPADUP,     ///< Game controller touchpad finger was lifted
            ControllerSensorUpdate      = SDL_CONTROLLERSENSORUPDATE,   ///< Game controller sensor was updated

            // Touch events

            FingerDown                  = SDL_FINGERDOWN,
            FingerUp                    = SDL_FINGERUP,
            FingerMotion                = SDL_FINGERMOTION,

            // Gesture events

            DollarGesture               = SDL_DOLLARGESTURE,
            DollarRecord                = SDL_DOLLARRECORD,
            MultiGesture                = SDL_MULTIGESTURE,

            // Clipboard events

            ClipboardUpdate             = SDL_CLIPBOARDUPDATE,          ///< The clipboard or primary selection changed

            // Drag and drop events
            DropFile                    = SDL_DROPFILE,                 ///< The system requests a file open
            DropText                    = SDL_DROPTEXT,                 ///< text/plain drag-and-drop event
            DropBegin                   = SDL_DROPBEGIN,                ///< A new set of drops is beginning (NULL filename)
            DropComplete                = SDL_DROPCOMPLETE,             ///< Current set of drops is now complete (NULL filename)

            // Audio hotplug events

            AudioDeviceAdded            = SDL_AUDIODEVICEADDED,         ///< A new audio device is available
            AudioDeviceRemoved          = SDL_AUDIODEVICEREMOVED,       ///< An audio device has been removed.

            // Sensor events

            SensorUpdate                = SDL_SENSORUPDATE,             ///< A sensor was updated

            // Render events

            RenderTargetReset           = SDL_RENDER_TARGETS_RESET,     ///< The render targets have been reset and their contents need to be updated
            RenderDeviceReset           = SDL_RENDER_DEVICE_RESET,      ///< The device has been reset and all textures need to be recreated

            // Internal events

            PollSentinel                = SDL_POLLSENTINEL,             ///< Signals the end of an event poll cycle
            UserEvent                   = SDL_USEREVENT,                ///< Events ::SDL_USEREVENT through ::SDL_LASTEVENT are for your use, and should be allocated with SDL_RegisterEvents()
            LastEvent                   = SDL_LASTEVENT                 ///< This last event is only for bounding internal arrays
        };

      private:
        static inline bool initialized = false;
        SDL_Event event;

      public:
        Event()
        {
            if (!SDL_WasInit(SDL_INIT_EVENTS))
            {
                SDL_InitSubSystem(SDL_INIT_EVENTS);
                initialized = true;
            }
            else if (initialized)
            {
                throw NexusException("Event",
                    "Attempt to create a second instance of Event. Only one instance can be created.");
            }
        }

        ~Event()
        {
            if (SDL_WasInit(SDL_INIT_EVENTS))
            {
                SDL_QuitSubSystem(SDL_INIT_EVENTS);
                initialized = false;
            }
        }

        operator SDL_Event&()
        {
            return event;
        }

        operator const SDL_Event&() const
        {
            return event;
        }

        int Poll()
        {
            return SDL_PollEvent(&event);
        }

        Type GetType() const
        {
            return static_cast<Type>(event.type);
        }

        const SDL_Event* operator->() const
        {
            return &event;
        }
    };

}}

#endif //NEXUS_CORE_EVENT_HPP
