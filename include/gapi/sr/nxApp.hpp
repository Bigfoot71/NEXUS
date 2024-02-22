#ifndef NEXUS_SR_APP_HPP
#define NEXUS_SR_APP_HPP

#include "../../core/nxState.hpp"
#include "../../core/nxApp.hpp"
#include "./nxWindow.hpp"

namespace nexus { namespace sr {

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

#endif //NEXUS_SR_APP_HPP
