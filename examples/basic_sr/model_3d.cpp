#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Model 3D", 800, 600);

    core::Clock clock;
    core::Event event;

    sr::Camera3D camera({ -50, 50, -50 }, { 0, 0, 0 },
        { 0, 1, 0 }, 60.0f, window.GetAspect());

    sr::Model model(window, RESOURCES_PATH "models/psionic/psionic.fbx");

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

        camera.Update(clock.GetDelta(), sr::Camera3D::Mode::Orbital);

        window.Begin().Clear();
            camera.Begin(window);
                model->Draw({ 0, 0, 0 }, 1.0f, gfx::White);
            camera.End();
        window.End();

        clock.End();
    }

    return 0;
}