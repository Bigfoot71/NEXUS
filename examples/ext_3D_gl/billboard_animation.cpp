#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 3D - Sprite", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Sprite3D sprite(window, RESOURCES_PATH "images/scarfy.png", 1, 6, 0.085f);

    gl::Camera3D camera = {
        { 0, 25, -50 },
        { 0, 0, 0 },
        { 0, 1, 0 },
        60.0f, window.GetAspect()
    };

    bool isRunning = true;

    while (isRunning)
    {
        // UPDATE
        clock.Begin();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::Quit:
                    isRunning = false;
                    break;

                default:
                    break;
            }
        }

        camera.Update(clock.GetDelta(), gl::Camera3D::Mode::Orbital);
        sprite.Update(clock.GetDelta());

        // DRAW
        window.Begin().Clear(gfx::Black);

            camera.Begin(window);
                gl::DrawGrid(window, 10, 10);
                sprite.DrawBillboard(camera, { 0, 7.5f, 0 }, { 15, 15 }, gfx::White);
            camera.End();

        window.End();

        clock.End();
    }

    return 0;
}