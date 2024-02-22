#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic GFX - Triangle", 800, 600);
    gfx::Renderer renderer(window);

    core::Clock clock;
    core::Event event;

    shape2D::Vertex triangle[3] = {
        { { 400.0f, 100.0f }, gfx::Red, {} },
        { { 133.333333333f, 500.0f }, gfx::Green, {} },
        { { 666.666666667f, 500.0f }, gfx::Blue, {} }
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

        for (int i = 0; i < 3; i++)
        {
            triangle[i].color.FromHSV(triangle[i].color.ToHSV() += math::Vec3(45.0f * clock.GetDelta(), 0.0f, 0.0f));
        }

        // DRAW
        renderer.Clear();
            //renderer.DrawMesh(mesh);
            renderer.DrawTriangle(triangle[0], triangle[1], triangle[2]);
        renderer.Present();

        clock.End();
    }

    return 0;
}