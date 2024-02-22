#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Target Texture", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::TargetTexture target(window, 128, 128);

    target->Begin().Clear(gfx::White);
        gl::DrawCircle(window, 64, 64, 32, gfx::Blue);
    target->End();

    bool isRunning = true;

    while (isRunning)
    {
        clock.Begin();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::WindowEvent:
                    isRunning = !(event->window.event == core::WindowEvent::Close);
                    break;

                default:
                    break;
            }
        }

        window.Begin().Clear();
            target->Draw({ 400, 300 }, 45.0f, { 64*2, 64*2 }, { 2, 2 });
        window.End();

        clock.End();
    }

    return 0;
}
