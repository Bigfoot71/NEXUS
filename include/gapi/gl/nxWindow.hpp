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

#ifndef NEXUS_GL_WINDOW_HPP
#define NEXUS_GL_WINDOW_HPP

#include "../../core/nxException.hpp"
#include "../../core/nxWindow.hpp"
#include "./nxContext.hpp"
#include <SDL_video.h>
#include <memory>

namespace nexus { namespace gl {

    /**
     * @class Window
     * @brief A class that represents an SDL window with an OpenGL rendering context.
     *
     * The gl::Window class is derived from the base core::Window class and adds OpenGL-specific functionality.
     * It allows you to create an SDL window with an associated OpenGL context for rendering.
     */
    class NEXUS_API Window : public core::Window
    {
      public:
        using CtxType = Context;                  ///< Used by template functions for drawing primitives, making it possible to ensure that the type of the Context is obtained, whether we give a gapi::Window or gapi::Context (allow implicit conversion in template)

      public:
        std::unique_ptr<Context> ctx = nullptr;   ///< A unique pointer to the OpenGL rendering context.

      public:
        /**
         * @brief Create an instance of an inactive window where no allocation or context creation will be made.
         *
         * This constructor creates an instance of gl::Window that is initially inactive, meaning no memory
         * allocation or OpenGL context creation will occur. This constructor can be used for deferred
         * window creation.
         */
        Window() : nexus::core::Window() { }

        /**
         * @brief Create a new window along with a new OpenGL context.
         *
         * This constructor creates a new SDL window with an associated OpenGL context. The window is created
         * with the specified title, width, and height. Additional window flags and OpenGL context attributes
         * can be provided.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param flags Additional flags for window creation.
         * @param attributes Configuration attributes for the OpenGL context.
         */
        Window(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags = core::WindowFlag::Shown, const Attributes& attr = {});

        /**
         * @brief Create a new window using an existing OpenGL context (but creating a new instance of RLGL).
         *
         * This constructor creates a new SDL window using an existing OpenGL context. The window is created
         * with the specified title, width, and height. The provided Context instance is used as the
         * underlying OpenGL context. Optionally, additional window flags can be specified.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param ctxInherit An existing Context instance to be used for the OpenGL context.
         * @param flags Additional flags for window creation.
         */
        Window(const std::string& title, Uint32 width, Uint32 height, Context* ctxInherit, core::WindowFlag flags = core::WindowFlag::Shown);

        /**
         * @brief Create a new window with the same dimensions and context as the one given.
         *
         * This constructor creates a new SDL window with the same dimensions and OpenGL context as the provided gl::Window.
         * Optionally, additional window flags can be specified.
         *
         * @param title The title of the new window.
         * @param window The gl::Window instance to inherit dimensions and context from.
         * @param flags Additional flags for window creation.
         */
        Window(const std::string& title, Window& window, core::WindowFlag flags = core::WindowFlag::Shown);

        ~Window();

        operator SDL_GLContext() { return *ctx; }

        operator Context*() { return ctx.get(); }
        operator Context&() { return *ctx.get(); }

        operator gapi::Context*() { return static_cast<gapi::Context*>(ctx.get()); }
        operator gapi::Context&() { return *ctx.get(); }

        /**
         * @brief Set the OpenGL context attributes.
         *
         * This function sets the OpenGL context attributes if the context is not already created.
         *
         * @param attributes The OpenGL context configuration.
         */
        void SetAttributes(const Attributes& attr = {}) const
        {
            if (ctx == nullptr) attr.Apply();
        }

        /**
         * @brief Create the window and associated OpenGL context.
         *
         * This function creates the window and associated OpenGL context with the specified title, width, and height.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param ctxInherit The existing OpenGL context to inherit (optional).
         * @param flags The window flags.
         */
        void Create(const std::string& title, Uint32 width, Uint32 height, Context* ctxInherit = nullptr, core::WindowFlag flags = core::WindowFlag::Shown);

        /**
         * @brief Create the window and associated OpenGL context.
         *
         * This function creates the window and associated OpenGL context with the specified title, width, and height.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param flags The window flags.
         */
        void Create(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags = core::WindowFlag::Shown) override;

        /**
         * @brief Destroy the window and associated OpenGL context.
         *
         * This function destroys the window and associated OpenGL context.
         */
        void Destroy() override;

        /**
         * @brief Check if the window OpenGL context is the current rendering context.
         *
         * This function checks whether the associated OpenGL context is the current rendering context.
         *
         * @return True if the OpenGL context is the current rendering context, false otherwise.
         */
        bool IsCurrent() const
        {
            return window == SDL_GL_GetCurrentWindow();
        }

        /**
         * @brief Make the OpenGL context current for rendering.
         *
         * This function makes the OpenGL context current for rendering.
         */
        void MakeCurrent();

        /**
         * @brief Clear the color buffer with a specified color.
         *
         * This function clears the color buffer of the OpenGL rendering context with the specified color.
         *
         * @param color The color to clear the buffer with.
         */
        void Clear(const gfx::Color& color = gfx::Black) const;

        /**
         * @brief Begin rendering.
         *
         * This function begins the rendering process.
         */
        const gl::Window& Begin() const;

        /**
         * @brief End rendering.
         *
         * This function ends the rendering process, updating and drawing the internal render batch and swapping buffers.
         */
        const gl::Window& End() const;
    };

}}

#endif //NEXUS_GL_WINDOW_HPP
