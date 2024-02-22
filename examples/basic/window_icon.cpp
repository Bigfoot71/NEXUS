#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic - Window Icon", 800, 600);
    gfx::Surface winCanvas(window.GetSurface());

    core::Clock clock;
    core::Event event;

    gfx::Surface icon(RESOURCES_PATH "images/icon.png");
    window.SetIcon(icon);

    gfx::Surface text = gfx::BasicFont(RESOURCES_PATH "fonts/Futurette.otf", 42)
        .RenderTextBlended("The window icon should be the image below", gfx::White);

    bool isRunning = true;
    bool keyPressed = false;

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

        winCanvas.Lock();
            winCanvas.Fill(gfx::Black);
            winCanvas.DrawImage(icon, { 0, 256, 1024, 512 }, { (800-512)/2, (600-256)/2, 512, 256 });
            winCanvas.DrawRectangleLines((800-512)/2, (600-256)/2, 512, 256, 1, gfx::White);
            winCanvas.DrawImage(text, (window.GetWidth() - text.GetWidth()) / 2, 32);
        winCanvas.Unlock();

        window.UpdateSurface();

        clock.End();
    }

    return 0;

}
