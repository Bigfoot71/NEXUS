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

#ifndef NEXUS_CORE_WINDOW_HPP
#define NEXUS_CORE_WINDOW_HPP

#include "../gfx/nxSurface.hpp"
#include "../math/nxVec2.hpp"
#include "./nxException.hpp"
#include <SDL_stdinc.h>
#include <SDL_video.h>
#include <SDL_error.h>
#include <memory>
#include <string>

namespace nexus { namespace core {

    /**
     * @enum WindowFlag
     * @brief Enumeration of window flags for configuring SDL windows.
     *
     * This enumeration defines various window flags that can be used to configure
     * the behavior and appearance of SDL windows.
     */
    enum class WindowFlag : Uint32
    {
        Fullscreen          = SDL_WINDOW_FULLSCREEN,            ///< Fullscreen mode flag.
        Shown               = SDL_WINDOW_SHOWN,                 ///< Window is shown.
        Hidden              = SDL_WINDOW_HIDDEN,                ///< Window is hidden.
        Borderless          = SDL_WINDOW_BORDERLESS,            ///< Window has no border.
        Resizable           = SDL_WINDOW_RESIZABLE,             ///< Window is resizable.
        Minimized           = SDL_WINDOW_MINIMIZED,             ///< Window is minimized.
        Maximized           = SDL_WINDOW_MAXIMIZED,             ///< Window is maximized.
        MouseGrabbed        = SDL_WINDOW_MOUSE_GRABBED,         ///< Mouse is grabbed.
        InputFocus          = SDL_WINDOW_INPUT_FOCUS,           ///< Window has input focus.
        MouseFocus          = SDL_WINDOW_MOUSE_FOCUS,           ///< Mouse is in focus.
        FullscreenDesktop   = SDL_WINDOW_FULLSCREEN_DESKTOP,    ///< Fullscreen mode on the desktop.
        Foreign             = SDL_WINDOW_FOREIGN,               ///< Foreign window.
        AllowHighDPI        = SDL_WINDOW_ALLOW_HIGHDPI,         ///< Allow high DPI scaling.
        MouseCapture        = SDL_WINDOW_MOUSE_CAPTURE,         ///< Mouse is captured.
        AlwaysOnTop         = SDL_WINDOW_ALWAYS_ON_TOP,         ///< Window is always on top.
        SkipTaskbar         = SDL_WINDOW_SKIP_TASKBAR,          ///< Skip the taskbar.
        Utility             = SDL_WINDOW_UTILITY,               ///< Utility window.
        Tooltip             = SDL_WINDOW_TOOLTIP,               ///< Tooltip window.
        PopUpMenu           = SDL_WINDOW_POPUP_MENU,            ///< Pop-up menu window.
        KeyboardGrabbed     = SDL_WINDOW_KEYBOARD_GRABBED,      ///< Keyboard is grabbed.
        InputGrabbed        = SDL_WINDOW_INPUT_GRABBED          ///< Input is grabbed.
    };

    /**
     * @brief Bitwise OR operator for combining window flags.
     *
     * @param a The first flag to combine.
     * @param b The second flag to combine.
     * @return The result of combining the two flags using the OR operator.
     */
    NEXUS_API constexpr WindowFlag operator|(WindowFlag a, WindowFlag b)
    {
        return static_cast<WindowFlag>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
    }

    /**
     * @brief Bitwise OR operator for combining window flags.
     *
     * @param a The first flag to combine.
     * @param b The second flag to combine.
     * @return The result of combining the two flags using the OR operator.
     */
    NEXUS_API constexpr WindowFlag operator|(WindowFlag a, Uint32 b)
    {
        return static_cast<WindowFlag>(static_cast<Uint32>(a) | b);
    }

    /**
     * @brief Bitwise AND operator for checking if both flags are set.
     *
     * @param a The first flag to check.
     * @param b The second flag to check.
     * @return True if both flags are set, otherwise false.
     */
    NEXUS_API constexpr WindowFlag operator&(WindowFlag a, WindowFlag b)
    {
        return static_cast<WindowFlag>(static_cast<Uint32>(a) & static_cast<Uint32>(b));
    }

    /**
     * @brief Bitwise AND operator for checking if both flags are set.
     *
     * @param a The first flag to check.
     * @param b The second flag to check.
     * @return True if both flags are set, otherwise false.
     */
    NEXUS_API constexpr WindowFlag operator&(WindowFlag a, Uint32 b)
    {
        return static_cast<WindowFlag>(static_cast<Uint32>(a) & b);
    }

    /**
     * @brief Bitwise NOT operator for inverting a flag's value.
     *
     * @param a The flag to invert.
     * @return The inverted flag.
     */
    NEXUS_API constexpr WindowFlag operator~(WindowFlag a)
    {
        return static_cast<WindowFlag>(~static_cast<Uint32>(a));
    }

    /**
     * @brief Bitwise OR assignment operator for combining a flag with another.
     *
     * @param a The flag to combine.
     * @param b The flag to combine with.
     * @return The result of combining the two flags using the OR operator.
     */
    NEXUS_API constexpr WindowFlag operator|=(WindowFlag& a, WindowFlag b)
    {
        a = static_cast<WindowFlag>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
        return a;
    }

    /**
     * @brief Bitwise OR assignment operator for combining a flag with another.
     *
     * @param a The flag to combine.
     * @param b The flag to combine with.
     * @return The result of combining the two flags using the OR operator.
     */
    NEXUS_API constexpr WindowFlag operator|=(WindowFlag& a, Uint32 b)
    {
        a = static_cast<WindowFlag>(static_cast<Uint32>(a) | b);
        return a;
    }

    /**
     * @brief Enumeration representing window events in a C++ program using SDL.
     *
     * The WindowEvent enum class defines various events related to window management,
     * such as window showing, hiding, moving, resizing, and more. Each enumerator corresponds
     * to an SDL window event, providing a convenient interface for handling window-related actions.
     */
    enum class WindowEvent : Uint8
    {
        None                = SDL_WINDOWEVENT_NONE,             ///< Never used
        Shown               = SDL_WINDOWEVENT_SHOWN,            ///< Window has been shown
        Hidden              = SDL_WINDOWEVENT_HIDDEN,           ///< Window has been hidden
        Exposed             = SDL_WINDOWEVENT_EXPOSED,          ///< Window has been exposed and should be redrawn
        Moved               = SDL_WINDOWEVENT_MOVED,            ///< Window has been moved to data1, data2
        Resized             = SDL_WINDOWEVENT_RESIZED,          ///< Window has been resized to data1xdata2
        SizeChanged         = SDL_WINDOWEVENT_SIZE_CHANGED,     ///< The window size has changed, either as a result of an API call or through the system or user changing the window size.
        Minimized           = SDL_WINDOWEVENT_MINIMIZED,        ///< Window has been minimized
        Maximized           = SDL_WINDOWEVENT_MAXIMIZED,        ///< Window has been maximized
        Restored            = SDL_WINDOWEVENT_RESTORED,         ///< Window has been restored to normal size and position
        Enter               = SDL_WINDOWEVENT_ENTER,            ///< Window has gained mouse focus
        Leave               = SDL_WINDOWEVENT_LEAVE,            ///< Window has lost mouse focus
        FocusGained         = SDL_WINDOWEVENT_FOCUS_GAINED,     ///< Window has gained keyboard focus
        FocusLost           = SDL_WINDOWEVENT_FOCUS_LOST,       ///< Window has lost keyboard focus
        Close               = SDL_WINDOWEVENT_CLOSE,            ///< The window manager requests that the window be closed
        TakeFocus           = SDL_WINDOWEVENT_TAKE_FOCUS,       ///< Window is being offered focus (should SetWindowInputFocus() on itself or a subwindow, or ignore)
        HitTest             = SDL_WINDOWEVENT_HIT_TEST,         ///< Window had a hit test that wasn't SDL_HITTEST_NORMAL.
        IccProfileChanged   = SDL_WINDOWEVENT_ICCPROF_CHANGED,  ///< The ICC profile of the window's display has changed.
        DisplayChanged      = SDL_WINDOWEVENT_DISPLAY_CHANGED   ///< Window has been moved to display data1.
    };

    /**
     * @brief Equality comparison between a WindowEvent and an unsigned 8-bit integer.
     *
     * @param a The WindowEvent to compare.
     * @param b The unsigned 8-bit integer to compare.
     * @return True if the WindowEvent is equal to the integer, false otherwise.
     */
    NEXUS_API constexpr bool operator==(WindowEvent a, Uint8 b)
    {
        return static_cast<Uint8>(a) == b;
    }

    /**
     * @brief Equality comparison between an unsigned 8-bit integer and a WindowEvent.
     *
     * @param a The unsigned 8-bit integer to compare.
     * @param b The WindowEvent to compare.
     * @return True if the integer is equal to the WindowEvent, false otherwise.
     */
    NEXUS_API constexpr bool operator==(Uint8 a, WindowEvent b)
    {
        return b == a;
    }

    /**
     * @brief Inequality comparison between a WindowEvent and an unsigned 8-bit integer.
     *
     * @param a The WindowEvent to compare.
     * @param b The unsigned 8-bit integer to compare.
     * @return True if the WindowEvent is not equal to the integer, false otherwise.
     */
    NEXUS_API constexpr bool operator!=(WindowEvent a, Uint8 b)
    {
        return !(a == b);
    }

    /**
     * @brief Inequality comparison between an unsigned 8-bit integer and a WindowEvent.
     *
     * @param a The unsigned 8-bit integer to compare.
     * @param b The WindowEvent to compare.
     * @return True if the integer is not equal to the WindowEvent, false otherwise.
     */
    NEXUS_API constexpr bool operator!=(Uint8 a, WindowEvent b)
    {
        return !(a == b);
    }

    /**
     * @brief The Window class represents an SDL window.
     */
    class NEXUS_API Window
    {
      protected:
        static inline Uint32 windowCount = 0;       ///< The count of open windows.
        SDL_Window *window;                         ///< The SDL window pointer.

      public:
        /**
        * @brief Get the count of open windows.
        *
        * This static method allows you to retrieve the current count of open windows.
        * @return The count of open windows.
        */
        static Uint32 GetCount()
        {
            return windowCount;
        }

      public:
        /**
         * @brief Default constructor for the Window class.
         * 
         * This constructor initializes the window pointer to nullptr, allowing
         * you to create the window later using the Create() method.
         */
        Window();
      
        /**
         * @brief Constructor to create a Window object.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param flags The initial window flags.
         */
        Window(const std::string& title, Uint32 width, Uint32 height, WindowFlag flags = WindowFlag::Shown);

        /**
         * @brief Destructor to clean up the Window object.
         */
        ~Window();

        /**
         * @brief Implicit conversion operator to retrieve a non-const pointer to the SDL_Window.
         *
         * This operator allows implicit conversion to a non-const pointer to the SDL_Window.
         * It can be used to access the underlying SDL_Window data, but use it with caution to prevent unintended modifications.
         *
         * @return A non-const pointer to the SDL_Window.
         */
        operator SDL_Window*() { return window; }

        /**
         * @brief Implicit conversion operator to retrieve a const pointer to the SDL_Window.
         *
         * This operator allows implicit conversion to a const pointer to the SDL_Window.
         * It can be used for read-only access to the underlying SDL_Window data.
         *
         * @return A const pointer to the SDL_Window.
         */
        operator const SDL_Window*() const { return window; }

        /**
         * @brief Create the SDL window.
         *
         * This virtual function creates an SDL window with the specified title, width, height,
         * and flags. If the window creation fails, it throws a throw with an
         * error message indicating the SDL error.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param flags The window flags to configure the window's behavior.
         *
         * @throws core::NexusException if the window creation fails, providing an SDL error message.
         */
        virtual void Create(const std::string& title, Uint32 width, Uint32 height, WindowFlag flags = WindowFlag::Shown)
        {
            window = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                width, height, static_cast<Uint32>(flags));

            if (!window)
            {
                throw core::NexusException("Window", "Window creation failed.",
                                           "SDL", SDL_GetError());
            }
        }

        /**
         * @brief Destroy the SDL window.
         *
         * This virtual function destroys the SDL window if it exists and sets the window
         * pointer to nullptr. Subclasses may override this method to add custom behavior
         * before or after destroying the window.
         */
        virtual void Destroy()
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = nullptr;
            }
        }

        /**
         * @brief Get the SDL_Window pointer.
         *
         * @return A pointer to the SDL window.
         */
        SDL_Window* Get() const;

        /**
         * @brief Get the ID of the SDL window.
         *
         * @return The window's unique identifier.
         */
        Uint32 GetID() const;

        /**
         * @brief Get the index of the monitor that the SDL window is currently on.
         *
         * This function returns the index of the monitor that the SDL window is currently
         * displayed on. The monitor index can be used to retrieve information about the
         * monitor using other SDL functions.
         *
         * @return The index of the monitor, or -1 if the window is not currently visible.
         */
        int GetMonitor() const;

        /**
         * @brief Get the flags used to configure the window's behavior.
         *
         * This function returns the flags that were used to configure the behavior of the window.
         * These flags determine various settings such as windowed mode, fullscreen, and more.
         *
         * @return The window flags specifying the window's behavior.
         */
        WindowFlag GetState() const;

        /**
         * @brief Get the width of the window.
         *
         * @return The width of the window.
         */
        int GetWidth() const;

        /**
         * @brief Get the height of the window.
         *
         * @return The height of the window.
         */
        int GetHeight() const;

        /**
         * @brief Get the size of the window as a math::IVec2 object.
         *
         * @return The window size as a math::IVec2.
         */
        math::IVec2 GetSize() const;

        /**
         * @brief Get the size of the window as a 2D rectangle.
         *
         * This method returns the size of the window as a 2D rectangle object.
         * The rectangle is defined by its top-left corner and its width and height.
         * Use this method when you need the window size represented as a rectangle.
         *
         * @return The window size as a shape2D::Rectangle object.
         */
        shape2D::Rectangle GetRectSize() const;

        /**
         * @brief Get the aspect ratio of the window.
         *
         * This function calculates and returns the aspect ratio of the window.
         * The aspect ratio is defined as the ratio of the width to the height of the window.
         *
         * @return The aspect ratio of the window.
         */
        float GetAspect() const;

        /**
         * @brief Get the gfx::Surface associated with the window.
         *
         * A new surface will be created with the optimal format for the window, if
         * necessary. This surface will be freed when the window is destroyed. Do not
         * free this surface yourself.
         *
         * This surface will be invalidated if the window is resized. After resizing a
         * window, this function must be called again to return a valid surface.
         *
         * You may not combine this with 3D or the rendering API on this window.
         *
         * This function is affected by `SDL_HINT_FRAMEBUFFER_ACCELERATION`.
         *
         * @return Returns a gfx::Surface enveloping the SDL_Surface of the window.
         * @throws core::NexusException If there is an error getting the window surface.
         */
        gfx::Surface GetSurface();

        /**
         * @brief Update the window surface.
         *
         * This function updates the window surface, allowing changes to be visible.
         *
         * @throws core::NexusException If there is an error updating the window surface.
         */
        void UpdateSurface();

        /**
         * @brief Check if the window is ready.
         *
         * @return True if the window is ready, false otherwise.
         */
        bool IsValid();

        /**
         * @brief Check if the window has a specific state.
         *
         * @param flags The window flags to check.
         * @return True if the window has the specified state, false otherwise.
         */
        bool IsState(WindowFlag flags);

        /**
         * @brief Toggle the window between fullscreen and windowed modes.
         */
        void ToggleFullscreen();

        /**
         * @brief Toggle the window between fullscreen and fullscreen desktop modes.
         */
        void ToggleFullscreenDesktop();

        /**
         * @brief Set the position of the window.
         *
         * @param x The X coordinate of the new position.
         * @param y The Y coordinate of the new position.
         */
        void SetPosition(Uint32 x, Uint32 y);

        /**
         * @brief Set the size of the window.
         *
         * @param width The new width of the window.
         * @param height The new height of the window.
         */
        void SetSize(Uint32 width, Uint32 height);

        /**
         * @brief Set the window icon from a graphics surface.
         *
         * Sets the window icon using the provided graphics surface. The surface
         * should contain the desired icon image. Note that the actual appearance
         * of the icon may vary depending on the platform and the surface's format.
         *
         * @param icon The graphics surface containing the icon image.
         */
        void SetIcon(gfx::Surface& icon)
        {
            SDL_SetWindowIcon(window, icon);
        }

        /**
         * @brief Set the window title.
         *
         * Sets the title of the window to the specified string. The window title
         * is displayed in the title bar of the window.
         *
         * @param title The new title for the window.
         */
        void SetTitle(const std::string& title)
        {
            SDL_SetWindowTitle(window, title.c_str());
        }
    };

}}

#endif //NEXUS_CORE_WINDOW_HPP
