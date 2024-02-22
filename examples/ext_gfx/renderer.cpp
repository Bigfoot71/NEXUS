#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic GFX - Renderer", 800, 600);
    gfx::Renderer renderer(window);

    core::Clock clock;
    core::Event event;

    gfx::Font font(renderer, RESOURCES_PATH "fonts/Futurette.otf", 32);
    font->SetStyle(sr::Font::Style::Bold);

    constexpr char text[] = "Renderer created with core::Window";
    const math::Vec2 size = font->GetSizeText(text);

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

        renderer.Clear(gfx::Black);
            renderer.SetColor(gfx::White);
            font->DrawCentered(text, 400, 300);
            renderer.DrawRectangleLines(shape2D::Rectangle{ (window.GetSize() - size) / 2, size });
        renderer.Present();

        clock.End();
    }

    return 0;

}
