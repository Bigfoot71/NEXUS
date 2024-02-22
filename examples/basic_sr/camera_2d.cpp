#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    sr::Window window("GL - Camera 2D", 800, 600);

    sr::Camera2D camera;
    camera.zoom = 1.0f;

    core::Clock clock;
    core::Event event;

    bool isRunning = true;

    while (isRunning)
    {
        clock.Begin();

        float wheel = 0;

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::WindowEvent:
                    isRunning = !(event->window.event == core::WindowEvent::Close);
                    break;

                case core::Event::Type::MouseButtonWheel:
                    wheel = event->wheel.y;
                    break;

                default:
                    break;
            }
        }

        math::IVec2 mDelta = input::GetMouseDelta();

        if (input::IsMouseButtonPressed()) camera.Translate(-mDelta);
        if (wheel != 0) camera.ZoomToMouse(wheel * 0.1f);

        window.Begin().Clear();

            int i = 0;

            camera.Begin(window);

            for (int y = -500; y < 500; y += 20)
            {
                for (int x = -500; x < 500; x += 20)
                {
                    sr::DrawRectangle(window, x, y, 20, 20, i % 2 == 0 ? gfx::White : gfx::Black);
                    i++;
                }
                i++;
            }

            camera.End();

        window.End();

        clock.End();
    }

    return 0;
}
