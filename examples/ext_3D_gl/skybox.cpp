#include "gapi/gl/nxPrimitives3D.hpp"
#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 3D - Skybox", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Camera3D camera({ 0, 25, 50 }, { 0, 0, 0 },
        { 0, 1, 0 }, 60.0f, window.GetAspect());

    gl::Skybox skybox(window, RESOURCES_PATH "images/skybox.png");

    camera.movementSpeed = 1.0f;

    input::CaptureMouse();

    float phase = 0.0f;
    bool isRunning = true;

    while (isRunning)
    {
        // UPDATE
        clock.Begin();
        const float dt = clock.GetDelta();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::KeyDown:
                    if (event->key.keysym.scancode == input::Scancode::Escape)
                        isRunning = false;
                    break;

                default:
                    break;
            }
        }

        camera.Update(dt);

        // DRAW

        window.Begin().Clear();

            camera.Begin(window);

                skybox.Draw();
                gl::DrawGrid(window, 10, 10);

            camera.End();

        window.End();

        clock.End();
    }

    return 0;
}