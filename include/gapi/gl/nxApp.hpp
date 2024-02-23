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

#ifndef NEXUS_GL_APP_HPP
#define NEXUS_GL_APP_HPP

#include "../../core/nxState.hpp"
#include "../../core/nxApp.hpp"
#include "./nxWindow.hpp"

namespace nexus { namespace gl {

    class App;

    using State = NEXUS_API _core_impl::State<App>;
    using LoadingState = NEXUS_API _core_impl::LoadingState<App>;

    class NEXUS_API App : public _core_impl::App<App, Window>
    {
      public:
        using _core_impl::App<App, Window>::App;

        operator Context&() { return window; }

        void UpdateAndDraw(State& state) override
        {
            clock.Begin();

                this->ProcessEvents(state);
                state.Update(clock.GetDelta());

                window.Begin();
                    state.Draw();
                window.End();

            clock.End();
        }
    };

}}

#endif //NEXUS_GL_APP_HPP
