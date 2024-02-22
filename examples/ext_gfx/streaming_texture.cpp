#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic GFX - Streaming Texture", 800, 600);

    gfx::Renderer renderer(window);
    core::RandomGenerator gen;
    core::Clock clock;
    core::Event event;

    gfx::Surface image(RESOURCES_PATH "images/heart-256x256.png");
    gfx::StreamingTexture st(renderer, 800, 600);

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
                    if (event->button.button == input::MouseButton::Middle)
                    {
                        st->Lock();
                            st->Fill(gen.RandomColor());
                        st->Unlock();
                    }
                    break;

                default:
                    break;
            }
        }

        math::IVec2 pos = input::GetMousePosition();

        // DRAW

        st->Lock();

            if (input::IsMouseButtonPressed(input::MouseButton::Left))
            {
                st->DrawRectangle(shape2D::Rectangle{ pos - math::IVec2(16, 16), { 32, 32 } }, gfx::Yellow);
            }
            if (input::IsMouseButtonPressed(input::MouseButton::Right))
            {
                st->DrawImageScaled(image, pos, { 0.25f, 0.25f }, image.GetSize() / 8);
            }

        st->Unlock();

        renderer.Clear(gfx::White);
            st->Draw(0, 0);
        renderer.Present();

        clock.End();
    }

    return 0;

}
