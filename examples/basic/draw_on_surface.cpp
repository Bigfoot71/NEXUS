#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic - Draw on Surface", 800, 600);
    core::Clock clock;
    core::Event event;

    gfx::Surface winCanvas(window.GetSurface());

    shape2D::Polygon concave({
        { 0, 0 }, { 400, 25 }, { 800, 0 },
        { 700, 300 }, { 400, 50 }, { 100, 300 }
    });

    shape2D::Polygon hexagon;
    hexagon.vertices.reserve(6);
    for (float a = -math::Pi; a < math::Pi; a += math::Tau / 6)
    {
        hexagon.vertices.push_back({ 400 + 128 * std::cos(a), 300 + 128 * std::sin(a) });
    }

    shape2D::AABB bounds = hexagon.GetAABB();

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

        winCanvas.Lock();

            winCanvas.Fill(gfx::Black);

            winCanvas.DrawRectangle(bounds.GetRectangle(), gfx::Blue);
            winCanvas.DrawRectangleLines(bounds.GetRectangle(), 1, gfx::Green);

            winCanvas.DrawPolygon(hexagon, gfx::Red);
            winCanvas.DrawPolygonLines(hexagon, gfx::Green);

            winCanvas.DrawPolygon(concave, gfx::Red);
            winCanvas.DrawPolygonLines(concave, gfx::Green);

        winCanvas.Unlock();

        window.UpdateSurface();

        clock.End();
    }

    return 0;
}