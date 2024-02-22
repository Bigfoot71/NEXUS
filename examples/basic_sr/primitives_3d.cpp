#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Primitives 3D", 800, 600);
    input::CaptureMouse();

    core::Clock clock;
    core::Event event;

    sr::Font font(window, RESOURCES_PATH "fonts/Futurette.otf", 32);
    font->SetStyle(sr::Font::Style::Bold);

    std::string strFPS{};

    sr::Camera3D camera(
        { 0, 10, -20 },
        { 0, 0, 0 },
        { 0, 1, 0 },
        60.0f,
        window.GetAspect());

    bool isRunning = true;

    while (isRunning)
    {
        clock.Begin();

        core::TextFormatTo(strFPS, "FPS: %.0f", clock.GetAverageFPS());

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::KeyDown:
                    isRunning = !(event->key.keysym.scancode == input::Scancode::Escape);

                default:
                    break;
            }
        }

        camera.Update(clock.GetDelta(), sr::Camera3D::Mode::Free);

        window.Begin();
        window.Clear();

            camera.Begin(window);

                sr::DrawCubeWires(window, { -10, 5, 0 }, { 5, 10, 5 }, gfx::Green);
                sr::DrawSphereWires(window, { 10, 4, 0 }, 4.0f, 8, 8, gfx::Green);
                sr::DrawCapsuleWires(window, { 0, 4 }, { 0, 10 }, 4.0f, 8, 8, gfx::Green);
                sr::DrawCylinderWires(window, { -20, 0, 0 }, 2.5f, 2.5f, 10, 36, gfx::Green);
                sr::DrawCylinderWires(window, { 20, 0, 0 }, 0, 2.5f, 10, 36, gfx::Green);

                if (input::IsMouseButtonPressed(input::MouseButton::Left))
                {
                    sr::DrawSphere(window, { 10, 4, 0 }, 4.0f, 8, 8, gfx::Gray);
                    sr::DrawCapsule(window, { 0, 4 }, { 0, 10 }, 4.0f, 8, 8, gfx::Gray);
                    sr::DrawCube(window, { -10, 5, 0 }, { 5, 10, 5 }, gfx::Gray);
                    sr::DrawCylinder(window, { -20, 0, 0 }, 2.5f, 2.5f, 10, 36, gfx::Gray);
                    sr::DrawCylinder(window, { 20, 0, 0 }, 0, 2.5f, 10, 36, gfx::Gray);
                }

                sr::DrawGrid(window, 10, 10);

            camera.End();

            font->Draw(strFPS, 10, 10);

        window.End();

        clock.End();
    }

    return 0;
}