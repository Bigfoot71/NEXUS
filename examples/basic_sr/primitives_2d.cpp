#include <nexus.hpp>

using namespace nexus;

struct CircleSector
{
    bool expense =  true;
    float angle = 0;

    void Update(float dt)
    {
        if (expense)
        {
            angle += 90 * dt;
            if (angle > 360)
            {
                angle = 360;
                expense = false;
            }
        }
        else
        {
            angle -= 90 * dt;
            if (angle < 0)
            {
                angle = 0;
                expense = true;
            }
        }
    }

    void Draw(sr::Window& win, const shape2D::Circle& circle)
    {
        sr::DrawCircleSector(win, circle.center, circle.radius * 0.75f, 0, angle, 36, gfx::Red);
        sr::DrawCircleSectorLines(win, circle, 0, angle, 36, gfx::Red);
    }
};

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Primitives 2D", 800, 600);

    core::Clock clock;
    core::Event event;

    CircleSector circleSector;

    constexpr float s = 80;
    constexpr float h = 40;
    constexpr float q = 20;

    std::function<void(float, float)> draws[] = {

        [&](float x, float y)
        {
            sr::DrawRectangleLines(window, x - h, y - h, s, s, gfx::Red);
            sr::DrawRectangleGradient(window, { x - q, y - q, h, h }, gfx::Red, gfx::Green, gfx::Red, gfx::Blue);
        },

        [&](float x, float y)
        {
            sr::DrawRectangleRoundedLines(window, { x - h, y - h, s, s }, 0.5f, 8, 1,  gfx::Red);
            sr::DrawRectangleRounded(window, { x - q, y - q, h, h }, 0.5f, 8, gfx::Red);
        },

        [&](float x, float y)
        {
            sr::DrawPolygonLines(window, { x, y }, 8, h, 0, gfx::Red);
            sr::DrawPolygon(window, { x, y }, 6, q, 0, gfx::Red);
        },

        [&](float x, float y)
        {
            circleSector.Draw(window, { x, y, h });
        },

        [&](float x, float y)
        {
            sr::DrawPolygonLines(window, { x, y }, 3, h, 90.0f, gfx::Red);
            sr::DrawPolygon(window, { x, y }, 3, h, 270.0f, gfx::Red);
        },

        [&](float x, float y)
        {
            sr::DrawEllipseLines(window, { x, y }, h, q, gfx::Red);
            sr::DrawEllipse(window, { x, y }, q, q*0.5f, gfx::Red);
        },
    };

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

        circleSector.Update(clock.GetDelta());

        // DRAW
        window.Begin();
        window.Clear(gfx::Black);

            int i = 0;

            for (float y = h + 10; y < window.GetHeight() - h; y += s)
            {
                for (float x = h + 10; y < window.GetWidth() - h; x += s)
                {
                    if (i == 6) break;
                    draws[i++](x, y);
                }
                if (i == 6) break;
            }

            sr::DrawLineBezier(window, { 10, 590 }, { 790, 550 }, 3, gfx::Red);

        window.End();

        clock.End();
    }

    return 0;
}
