#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Camera 3D", 800, 600);

    gl::Camera3D camera(
        { -10, 10, -10 },
        { 0, 0, 0 },
        { 0, 1, 0 },
        60.0f,
        window.GetAspect());

    input::CaptureMouse();

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
                case core::Event::Type::KeyDown:
                    isRunning = !(event->key.keysym.scancode == input::Scancode::Escape);

                default:
                    break;
            }
        }

        camera.Update(clock.GetDelta(), gl::Camera3D::Mode::Free);

        window.Begin();
        window.Clear();

            camera.Begin(window);

                gl::DrawGrid(window, 10, 10);
                gl::DrawCube(window, { 0, 5, 0 }, { 10, 10, 10 }, gfx::Red);
                gl::DrawCubeWires(window, { 0, 5, 0 }, { 10, 10, 10 }, gfx::Blue);

            camera.End();

        window.End();

        clock.End();
    }

    return 0;
}