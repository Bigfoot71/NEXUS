#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 2D - Sprite", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Sprite2D sprite(window, RESOURCES_PATH "images/scarfy.png", 1, 6, 0.085f);

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

        sprite.Update(clock.GetDelta());

        // DRAW

        window.Begin().Clear(gfx::White);
            sprite.Draw(window.GetSize() / 2, 3.0f, 0.0f);
        window.End();

        clock.End();
    }

    return 0;
}