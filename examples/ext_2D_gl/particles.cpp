#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 2D - Particles", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Font font(window, RESOURCES_PATH "fonts/Futurette.otf", 32);

    gl::ParticleSystem2D psys(window, 512);
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

        window.Begin().Clear();

            psys.Draw();

            font->Draw(core::TextFormat("[C] - Color variation: %s", psys.HasColorVariation() ? "Enabled" : "Disabled"), 10, 10);
            font->Draw(core::TextFormat("[S] - Size variation: %s", psys.HasSizeVariation() ? "Enabled" : "Disabled"), 10, 2 * 10 + font->GetHeight());

        window.End();

        clock.End();
    }

    return 0;
}