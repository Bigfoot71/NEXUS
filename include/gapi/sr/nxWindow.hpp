#ifndef NEXUS_SF_WINDOW_HPP
#define NEXUS_SF_WINDOW_HPP

#include "../../core/nxException.hpp"
#include "../../core/nxWindow.hpp"
#include "./nxFramebuffer.hpp"
#include "./nxContext.hpp"
#include "gfx/nxColor.hpp"
#include <SDL_video.h>
#include <memory>

namespace nexus { namespace sr {

    /**
     * @class Window
     * @brief A class that represents an SDL window with an sr::Context.
     *
     * The gl::Window class is derived from the base core::Window class and adds OpenGL-specific functionality.
     * It allows you to create an SDL window with an associated sr::Context for rendering.
     */
    class NEXUS_API Window : public core::Window
    {
      public:
        using CtxType = Context;                    ///< Used by template functions for drawing primitives, making it possible to ensure that the type of the Context is obtained, whether we give a gapi::Window or gapi::Context (allow implicit conversion in template)

      public:
        std::unique_ptr<Context> ctx = nullptr;     ///< A unique pointer to the sr::Context.
        std::unique_ptr<Framebuffer> framebuffer;   ///< Window framebuffer.

      public:
        /**
         * @brief Create an instance of an inactive window where no allocation or context creation will be made.
         *
         * This constructor creates an instance of gl::Window that is initially inactive, meaning no memory
         * allocation or sr::Context creation will occur. This constructor can be used for deferred
         * window creation.
         */
        Window() : nexus::core::Window() { }

        /**
         * @brief Create a new window along with a new sr::Context.
         *
         * This constructor creates a new SDL window with an associated sr::Context. The window is created
         * with the specified title, width, and height. Additional window flags and sr::Context attributes
         * can be provided.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         */
        Window(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags = core::WindowFlag::Shown);

        ~Window();

        operator Context*() { return ctx.get(); }
        operator Context&() { return *ctx.get(); }

        operator gapi::Context*() { return static_cast<gapi::Context*>(ctx.get()); }
        operator gapi::Context&() { return *ctx.get(); }

        /**
         * @brief Create the window and associated sr::Context.
         *
         * This function creates the window and associated sr::Context with the specified title, width, and height.
         *
         * @param title The title of the window.
         * @param width The width of the window.
         * @param height The height of the window.
         */
        void Create(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags = core::WindowFlag::Shown) override;

        /**
         * @brief Destroy the window and associated sr::Context.
         *
         * This function destroys the window and associated sr::Context.
         */
        void Destroy() override;

        /**
         * @brief Clear the window frambuffer with a specified color.
         *
         * This function clears the window frambuffer with the specified
         * color as well as the framebuffer and its depth.
         *
         * @param color The color to clear the window framebuffer with.
         */
        void Clear(const gfx::Color& color = gfx::Black);

        /**
         * @brief Begin rendering.
         *
         * This function begins the rendering process
         * using the software rasterizer.
         */
        sr::Window& Begin();

        /**
         * @brief End rendering.
         *
         * This function ends the rendering process, 
         * updating and drawing the internal render batch.
         */
        sr::Window& End();
    };

}}

#endif //NEXUS_SF_WINDOW_HPP
