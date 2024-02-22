#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Rasterized Texture", 800, 600);

    core::Clock clock;
    core::Event event;

    sr::Texture texture(window, RESOURCES_PATH "images/heart-256x256.png");

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
        window.Begin().Clear(gfx::White);
            texture->Draw((window.GetSize() - texture->GetSize()) / 2);
        window.End();

        clock.End();
    }

    return 0;
}
