#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Software Rasterizer - Renderer", 800, 600);

    gfx::Renderer renderer(window);
    core::Clock clock;
    core::Event event;

    gfx::Texture texture(renderer, RESOURCES_PATH "images/heart-256x256.png");

    bool isRunning = true;
    float texRotation = 0;

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

        texRotation += 90.0f * clock.GetDelta();

        // DRAW

        renderer.Clear(gfx::White);
            texture->Draw((window.GetSize() - texture->GetSize()) / 2, texRotation, texture->GetSize() / 2);
        renderer.Present();

        clock.End();
    }

    return 0;

}
