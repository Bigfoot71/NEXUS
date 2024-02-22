#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic GFX - Font", 800, 600);
    gfx::Renderer renderer(window);

    core::Clock clock;
    core::Event event;

    gfx::Font font(renderer, RESOURCES_PATH "fonts/Futurette.otf", 32);
    font->SetStyle(sr::Font::Style::Bold);

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

        // DRAW

        renderer.Clear();
            font->DrawCentered("Hello, World!", 400, 300);
        renderer.Present();

        clock.End();
    }

    return 0;

}
