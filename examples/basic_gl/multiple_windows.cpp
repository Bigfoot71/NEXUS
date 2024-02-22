#include <nexus.hpp>

using namespace nexus;

void UpdateContext(gl::Window& window, const gfx::Color& color)
{
    window.MakeCurrent();
    window.Begin();
    window.Clear();

        window.ctx->Begin(gl::DrawMode::Triangles);

            window.ctx->Color(color.r, color.g, color.b, color.a);

            window.ctx->Vertex(400.0f, 100.0f);
            window.ctx->Vertex(133.333333333f, 500.0f);
            window.ctx->Vertex(666.666666667f, 500.0f);

        window.ctx->End();

    window.End();
}

int main(int argv, char** args)
{
    gl::Window win1("GL - Window 1", 800, 600);
    gl::Window win2("GL - Window 2", win1); // Shares the same GL context as with the first window

    core::Clock clock;
    core::Event event;

    bool isRunning = true;

    while (isRunning)
    {
        clock.Begin();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::WindowEvent:
                    isRunning = !(event->window.event == core::WindowEvent::Close);
                    break;

                default:
                    break;
            }
        }

        UpdateContext(win1, gfx::Red);
        UpdateContext(win2, gfx::Blue);

        clock.End();
    }

    return 0;
}