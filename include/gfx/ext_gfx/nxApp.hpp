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

#ifndef NEXUS_GFX_APP_HPP
#define NEXUS_GFX_APP_HPP

#include "../../core/nxWindow.hpp"
#include "../../core/nxState.hpp"
#include "../../core/nxApp.hpp"
#include "./nxRenderer.hpp"

namespace nexus { namespace gfx {

    class App;

    using State = NEXUS_API _core_impl::State<App>;
    using LoadingState = NEXUS_API _core_impl::LoadingState<App>;

    /**
     * @brief gfx::App is a specialized implementation of the core::App class designed to work with gfx::Renderer.
     *
     * gfx::App is only available with EXTENSION_GFX and is provided, to offer a
     * ready-to-use implementation of the App class using a gfx::Renderer.
     *
     * If you need an App class that does not use gfx::Renderer or any graphics API,
     * or if you want to implement your own version, you can use core::App directly.
     */
    class NEXUS_API App : public _core_impl::App<App, core::Window>
    {
      public:
        Renderer renderer; ///< The renderer associated with the application.

      public:
        /**
         * @brief Constructs an App instance with specified parameters.
         * @param appTitle The title of the application.
         * @param winWidth The width of the window.
         * @param winHeight The height of the window.
         * @param drvIndex The index of the rendering driver to initialize. (default is -1)
         * @param winFlags Additional flags for window creation. (default is WindowFlag::Shown)
         * @param rendererFlags The renderer flags specifying rendering options. (default is RendererFlag::Accelerated)
         */
        App(const std::string& appTitle, int winWidth, int winHeight, int drvIndex = -1, core::WindowFlag winFlags = nexus::core::WindowFlag::Shown, gfx::RendererFlag rendererFlags = gfx::RendererFlag::Accelerated)
        : _core_impl::App<App, core::Window>(appTitle, winWidth, winHeight, winFlags)
        , renderer(this->window, drvIndex, rendererFlags)
        { }

        /**
         * @brief Conversion operator to obtain a reference to the renderer.
         * @return A reference to the renderer.
         */
        operator Renderer&() { return renderer; }

        /**
         * @brief Updates the application state and draws graphics.
         * @param state The state of the application.
         */
        void UpdateAndDraw(State& state) override
        {
            clock.Begin();

                this->ProcessEvents(state);
                state.Update(clock.GetDelta());

                renderer.Clear(gfx::Black);
                    state.Draw();
                renderer.Present();

            clock.End();
        }
    };

}}

#endif //NEXUS_GFX_APP_HPP
