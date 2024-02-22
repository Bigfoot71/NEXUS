#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("GFX - Extension 2D - Particles", 800, 600);
    gfx::Renderer renderer(window);

    core::Clock clock;
    core::Event event;

    gfx::Font font(renderer, RESOURCES_PATH "fonts/Futurette.otf", 32);

    gfx::ParticleSystem psys(renderer, 512);
    psys.SetVelocity({ -200, -200 }, { 200, 200 });
    psys.SetSize(4, 8);

    core::RandomGenerator gen;

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

                case core::Event::Type::MouseButtonUp:
                    psys.SetColor(gen.RandomColor());

                case core::Event::Type::KeyDown:
                    if (event->key.keysym.scancode == input::Scancode::C) psys.SetColorVariation(!psys.HasColorVariation());
                    if (event->key.keysym.scancode == input::Scancode::S) psys.SetSizeVariation(!psys.HasSizeVariation());
                    break;

                default:
                    break;
            }
        }

        if (input::IsMouseButtonPressed(input::MouseButton::Left))
        {
            psys.SetPosition(input::GetMousePosition());
            psys.Emit(8);
        }

        if (input::IsMouseButtonPressed(input::MouseButton::Right))
        {
            psys.SetPosition(input::GetMousePosition());

            for (int i = 0; i < 8; i++)
            {
                psys.SetColor(gen.RandomColor());
                psys.Emit();
            }
        }

        psys.Update(clock.GetDelta());

        // DRAW

        renderer.Clear();

            psys.Draw();

            font->Draw(core::TextFormat("[C] - Color variation: %s", psys.HasColorVariation() ? "Enabled" : "Disabled"), 10, 10);
            font->Draw(core::TextFormat("[S] - Size variation: %s", psys.HasSizeVariation() ? "Enabled" : "Disabled"), 10, 2 * 10 + font->GetHeight());

        renderer.Present();

        clock.End();
    }

    return 0;
}