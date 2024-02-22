#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic - Surface Generation", 800, 600);
    core::Clock clock;
    core::Event event;

    gfx::Surface winCanvas(window.GetSurface());

    gfx::Surface images[] = {
        gfx::Surface::NewGradientLinear(128, 128, 0, gfx::Red, gfx::Blue),
        gfx::Surface::NewGradientRadial(128, 128, 0.0f, gfx::Red, gfx::Blue),
        gfx::Surface::NewGradientLinear(128, 128, 45, gfx::Red, gfx::Blue),
        gfx::Surface::NewGradientSquare(128, 128, 0.0f, gfx::Red, gfx::Blue),
        gfx::Surface::NewGradientLinear(128, 128, 90, gfx::Red, gfx::Blue),
        gfx::Surface::NewChecked(128, 128, 16, 16, gfx::Red, gfx::Blue),
        gfx::Surface::NewWhiteNoise(128, 128, 0.5f),
        gfx::Surface::NewCellular(128, 128, 16)
    };

    bool isRunning = true;
    float gradientDir = 0.0f;
    float gradientDen = 0.0f;

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

        gradientDir += 45 * clock.GetDelta();

        gradientDen += clock.GetDelta();
        if (gradientDen >= 1.0f) gradientDen = - 1.0f;

        // DRAW

        winCanvas.Lock();
            winCanvas.Fill(gfx::Black);

            int i = 0;
            for (int y = 16; y < window.GetHeight(); y += 128+32)
            {
                for (int x = 16; x < window.GetWidth(); x += 128+32)
                {
                    winCanvas.DrawImage(images[i++], math::Vec2(x, y));
                    if (i == 8) break;
                }
                if (i == 8) break;
            }

        winCanvas.Unlock();

        window.UpdateSurface();

        // Update surfaces
        images[0].DrawGradientLinear(images[6].GetRectSize(), gradientDir, gfx::Red, gfx::Blue);
        images[1].DrawGradientRadial(images[6].GetRectSize(), gradientDen, gfx::Red, gfx::Blue);
        images[2].DrawGradientLinear(images[6].GetRectSize(), gradientDir + 45, gfx::Red, gfx::Blue);
        images[3].DrawGradientSquare(images[6].GetRectSize(), gradientDen, gfx::Red, gfx::Blue);
        images[4].DrawGradientLinear(images[6].GetRectSize(), gradientDir + 90, gfx::Red, gfx::Blue);
        images[6].DrawWhiteNoise(images[6].GetRectSize(), 0.5f);

        clock.End();
    }

    return 0;
}