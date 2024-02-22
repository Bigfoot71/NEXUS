#include <nexus.hpp>

using namespace nexus;

shape2D::Polygon GenStar(float radius);
gfx::Surface GenFlag();

int main(int argv, char** args)
{
    core::Window window("Basic - Surface Manipulation", 800, 600);
    core::Clock clock;
    core::Event event;

    gfx::Surface winCanvas(window.GetSurface());
    gfx::Surface flag = GenFlag();

    bool isRunning = true;
    float p = 0;

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

        p += 4 * clock.GetDelta();

        winCanvas.Lock();
        winCanvas.Fill(gfx::Black);

            const int cx = (winCanvas.GetWidth() - flag.GetWidth()) / 2;
            const int cy = (winCanvas.GetHeight() - flag.GetHeight()) / 2;

            for (int y = cy; y < cy + flag.GetHeight(); y++)
            {
                for (int x = cx; x < cx + flag.GetWidth(); x++)
                {
                    const float s = std::sin(p - x / 24.0f);
                    winCanvas.SetPixelUnsafe(x, y + s * 4, flag.GetPixelUnsafe(x-cx, y-cy).Normalized() * (2.0f + s) * 0.5f);
                }
            }

        winCanvas.Unlock();
        window.UpdateSurface();

        clock.End();
    }

    return 0;
}

shape2D::Polygon GenStar(float radius)
{
    shape2D::Polygon starPoints;

    float angleIncrement = math::Tau / 5.0f;

    for (int i = 0; i < 5; i++)
    {
        float outerAngle = angleIncrement * i;
        float innerAngle = angleIncrement * (i + 0.5f);

        float outerX = radius * std::cos(outerAngle);
        float outerY = radius * std::sin(outerAngle);
        float innerX = (radius * 0.5f) * std::cos(innerAngle);
        float innerY = (radius * 0.5f) * std::sin(innerAngle);

        starPoints.vertices.emplace_back(outerX, outerY);
        starPoints.vertices.emplace_back(innerX, innerY);
    }

    return starPoints;
}

gfx::Surface GenFlag()
{
    constexpr int height = 299;
    constexpr int width = height * 1.9f;
    constexpr int bandH = 1.0f / 13.0f * height;
    constexpr int cantonW = 2 * width / 4.5f;         // correct is (2 * width / 5) but issues for pixel perfect
    constexpr int cantonH = 7.0f / 13.0f * height;
    constexpr int starGapX = cantonW / 12, starGapY = cantonH / 10;

    gfx::Surface flag(width, height, gfx::White);

    for (int y = 0; y < height; y += 2 * bandH)
    {
        flag.DrawRectangle(0, y, width, bandH, { 179, 25, 66, 255 });
    }

    flag.DrawRectangle(0, 0, cantonW, cantonH, { 10, 49, 97, 255 });

    shape2D::Polygon star = GenStar(std::min(starGapX, starGapY) * 0.5f);
    star.Rotate(-90, star.GetCenter());

    for (int y = starGapY; y < cantonH; y += 2 * starGapY)
    {
        for (int x = starGapX; x < cantonW; x += 2 * starGapX)
        {
            star.SetPosition(math::IVec2(x, y), star.GetCenter());
            flag.DrawPolygon(star, gfx::White);
        }
    }

    for (int y = 2 * starGapY; y < cantonH - starGapY; y += 2 * starGapY)
    {
        for (int x = 2 * starGapX; x < cantonW; x += 2 * starGapX)
        {
            star.SetPosition(math::IVec2(x, y), star.GetCenter());
            flag.DrawPolygon(star, gfx::White);
        }
    }

    return flag;
}
