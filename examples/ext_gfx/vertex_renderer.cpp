#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Basic GFX - Triangle", 800, 600);

    gfx::Renderer renderer(window);
    gfx::VertexRenderer vertexRenderer(renderer);

    core::Clock clock;
    core::Event event;

    float angle = 0;
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

        angle += 45.0f * clock.GetDelta();

        // DRAW
        renderer.Clear();

            vertexRenderer.PushMatrix();

                vertexRenderer.Translate(400, 300);
                vertexRenderer.Rotate(angle);
                vertexRenderer.Translate(-400, -300);

                vertexRenderer.Begin(gfx::DrawMode::Quads);

                    vertexRenderer.Color(gfx::Red);
                    vertexRenderer.Vertex(100, 100);

                    vertexRenderer.Color(gfx::Green);
                    vertexRenderer.Vertex(700, 100);

                    vertexRenderer.Color(gfx::Blue);
                    vertexRenderer.Vertex(700, 500);

                    vertexRenderer.Color(gfx::Yellow);
                    vertexRenderer.Vertex(100, 500);

                vertexRenderer.End();

                vertexRenderer.Begin(gfx::DrawMode::Lines);

                    vertexRenderer.Color(gfx::White);

                    vertexRenderer.Vertex(100, 100);
                    vertexRenderer.Vertex(700, 100);

                    vertexRenderer.Vertex(700, 500);
                    vertexRenderer.Vertex(100, 500);

                    vertexRenderer.Vertex(100, 100);
                    vertexRenderer.Vertex(100, 500);

                    vertexRenderer.Vertex(700, 100);
                    vertexRenderer.Vertex(700, 500);

                vertexRenderer.End();

            vertexRenderer.PopMatrix();

        renderer.Present();

        clock.End();
    }

    return 0;
}