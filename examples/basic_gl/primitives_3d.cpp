#include "gapi/gl/nxPrimitives3D.hpp"
#include "gfx/nxColor.hpp"
#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Primitives 3D", 800, 600);

    gl::Camera3D camera(
        { 0, 10, -50 },
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

                gl::DrawSphere(window, { 10, 4, 0 }, 4.0f, 8, 8, gfx::Gray);
                gl::DrawSphereWires(window, { 10, 4, 0 }, 4.0f, 8, 8, gfx::Green);

                gl::DrawCapsule(window, { 0, 4 }, { 0, 10 }, 4.0f, 8, 8, gfx::Gray);
                gl::DrawCapsuleWires(window, { 0, 4 }, { 0, 10 }, 4.0f, 8, 8, gfx::Green);

                gl::DrawCube(window, { -10, 5, 0 }, { 5, 10, 5 }, gfx::Gray);
                gl::DrawCubeWires(window, { -10, 5, 0 }, { 5, 10, 5 }, gfx::Green);

                gl::DrawCylinder(window, { -20, 0, 0 }, 2.5f, 2.5f, 10, 36, gfx::Gray);
                gl::DrawCylinderWires(window, { -20, 0, 0 }, 2.5f, 2.5f, 10, 36, gfx::Green);

                gl::DrawCylinder(window, { 20, 0, 0 }, 0, 2.5f, 10, 36, gfx::Gray);
                gl::DrawCylinderWires(window, { 20, 0, 0 }, 0, 2.5f, 10, 36, gfx::Green);

                gl::DrawGrid(window, 10, 10);

            camera.End();

        window.End();

        clock.End();
    }

    return 0;
}