#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    sr::Window window("SR - Target Texture", 800, 600);

    core::Clock clock;
    core::Event event;

    sr::TargetTexture target(window, 400, 300);

    sr::Camera3D camera = {
        { -25, 25, -25 },
        { 0, 0, 0 },
        { 0, 1, 0 },
        60.0f,
        window.GetAspect()
    };

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

        camera.Update(clock.GetDelta(), camera.Orbital);

        target->Begin().Clear(gfx::White);

            camera.Begin(window);
                sr::DrawGrid(window, 10, 10);
                sr::DrawCubeWires(window, { 0, 10, 0 }, { 5, 5, 5 }, gfx::Red);
            camera.End();

        target->End();

        window.Begin().Clear();
            target->Draw(target->GetRectSize(), window.GetRectSize());
        window.End();

        clock.End();
    }

    return 0;
}
