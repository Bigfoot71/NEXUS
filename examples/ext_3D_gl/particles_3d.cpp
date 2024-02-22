#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 3D - Particles", 800, 600);

    gl::Camera3D camera(
        { -35, 35, -35 },
        { 0, 0, 0 },
        { 0, 1, 0 },
        60.0f,
        window.GetAspect());

    gl::Font font(window, RESOURCES_PATH "fonts/Futurette.otf", 24);
    core::RandomGenerator gen;

    gl::Shader discardAlpha(window, nullptr, R"(
        #version 330
        in vec2 fragTexCoord;
        in vec4 fragColor;
        uniform sampler2D texture0;
        out vec4 finalColor;
        void main()
        {
            vec4 texelColor = texture(texture0, fragTexCoord);
            if (texelColor.a == 0) discard;

            finalColor = texelColor * fragColor;
        }
    )");

    gl::ParticleSystem3D psys(window, 512);

    psys.SetVelocity({ -20.0f, 40.0f, -20.0f }, { 20.0f, 80.0f, 20.0f });
    psys.SetGravity({ 0, -120.0f, 0 });
    psys.SetColor(gen.RandomColor());
    psys.SetSize(0.25f, 1.0f);

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
                case core::Event::Type::Quit:
                    isRunning = false;
                    break;

                case core::Event::Type::MouseButtonUp:
                    psys.SetColor(gen.RandomColor());

                case core::Event::Type::KeyDown:
                    if (event->key.keysym.scancode == input::Scancode::C) psys.SetColorVariation(!psys.HasColorVariation());
                    if (event->key.keysym.scancode == input::Scancode::S) psys.SetSizeVariation(!psys.HasSizeVariation());
                    break;

                default:
                    break;
            }
        }

        camera.Update(clock.GetDelta(), camera.Orbital);

        psys.Emit(); // Emit one particle
        psys.Update(clock.GetDelta());

        window.Begin();
        window.Clear();

            camera.Begin(window);

                gl::DrawGrid(window, 10, 10);

                discardAlpha->Begin();
                    psys.Draw(camera);
                discardAlpha->End();

            camera.End();

            font->Draw(core::TextFormat("[C] - Color variation: %s", psys.HasColorVariation() ? "Enabled" : "Disabled"), 10, 10);
            font->Draw(core::TextFormat("[S] - Size variation: %s", psys.HasSizeVariation() ? "Enabled" : "Disabled"), 10, 2 * 10 + font->GetHeight());

        window.End();

        clock.End();
    }

    return 0;
}