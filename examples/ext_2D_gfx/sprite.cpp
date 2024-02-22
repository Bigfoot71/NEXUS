#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Extension GFX - Sprite", 800, 600);
    gfx::Renderer renderer(window);

    core::Clock clock;
    core::Event event;

    gfx::Sprite sprite(renderer, RESOURCES_PATH "images/scarfy.png", 1, 6, 0.085f);

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

        renderer.Clear(gfx::White);
            sprite.Draw(window.GetSize() / 2, 3.0f, 0.0f);
        renderer.Present();

        clock.End();
    }

    return 0;
}