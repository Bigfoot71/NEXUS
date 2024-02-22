#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Triangle", 800, 600);

    core::Clock clock;
    core::Event event;

    gfx::Color c1 = gfx::Red;
    gfx::Color c2 = gfx::Green;
    gfx::Color c3 = gfx::Blue;

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

        c1.FromHSV(c1.ToHSV() += math::Vec3(45.0f * clock.GetDelta(), 0.0f, 0.0f));
        c2.FromHSV(c2.ToHSV() += math::Vec3(45.0f * clock.GetDelta(), 0.0f, 0.0f));
        c3.FromHSV(c3.ToHSV() += math::Vec3(45.0f * clock.GetDelta(), 0.0f, 0.0f));

        // DRAW
        window.Begin();
        window.Clear();

            window.ctx->Begin(sr::DrawMode::Triangles);

                window.ctx->Color(c1.r, c1.g, c1.b, c1.a);
                window.ctx->Vertex(400.0f, 100.0f);

                window.ctx->Color(c2.r, c2.g, c2.b, c2.a);
                window.ctx->Vertex(133.333333333f, 500.0f);

                window.ctx->Color(c3.r, c3.g, c3.b, c3.a);
                window.ctx->Vertex(666.666666667f, 500.0f);

            window.ctx->End();

        window.End();

        clock.End();
    }

    return 0;
}