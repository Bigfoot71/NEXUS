#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Font 3D", 800, 600);

    gl::Font font(window, RESOURCES_PATH "fonts/Futurette.otf", 32);
    font->SetStyle(sr::Font::Style::Bold);

    gl::Camera3D camera(
        { -40, 40, -40 },
        { 0, 0, 0 },
        { 0, 1, 0 },
        60.0f,
        window.GetAspect());

    core::Clock clock;
    core::Event event;

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

        camera.Update(clock.GetDelta(), gl::Camera3D::Mode::Orbital);

        window.Begin().Clear();
            camera.Begin(window);
                gl::DrawGrid(window, 10, 10);
                font->Draw3D("Hello World! <3", { 0, 10, 0 }, {}, 0, 8, gfx::Red);
                font->Draw3D("3D facing the camera", camera, { 0, 10, -25 }, 2, gfx::Green);
            camera.End();
        window.End();

        clock.End();
    }

    return 0;
}