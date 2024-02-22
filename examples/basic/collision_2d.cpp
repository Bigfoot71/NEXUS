#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic - Collision 2D", 800, 600);
    core::Clock clock;
    core::Event event;

    gfx::Surface winCanvas(window.GetSurface());

    shape2D::Circle circle(0, 0, 20);

    shape2D::Polygon poly({
        { 400, 300 - 100 },
        { 400 - 100, 300 + 100 },
        { 400, 300 },
        { 400 + 100, 300 + 100 },
    });

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

                case core::Event::Type::MouseMotion:
                    circle.center.x = event->motion.x;
                    circle.center.y = event->motion.y;

                default:
                    break;
            }
        }

        bool collision = circle.CollisionPolygon(poly);

        // DRAW

        winCanvas.Lock();
            winCanvas.Fill(gfx::Black);
            winCanvas.DrawPolygonLines(poly, collision ? gfx::Yellow : gfx::Green);
            winCanvas.DrawCircleLines(circle, collision ? gfx::Yellow : gfx::Green);
        winCanvas.Unlock();

        window.UpdateSurface();

        clock.End();
    }

    return 0;
}