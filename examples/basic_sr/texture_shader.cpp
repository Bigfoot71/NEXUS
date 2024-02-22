#include <nexus.hpp>

using namespace nexus;

class MyShader : public sr::Shader
{
  public:
    float timer;

  public:
    using sr::Shader::Shader;

    gfx::Color Fragment(const gfx::Surface* image, const math::IVec2& fragCoord, const math::Vec2& texCoord, const math::Vec3 fragNormal, const gfx::Color& fragColor)
    {
        return image->GetFrag(texCoord + math::Vec2(0.0f, std::sin(texCoord.x * 4.0 + timer) * 0.075f)) * fragColor;
    }
};

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Texture & Shader", 800, 600);

    core::Clock clock;
    core::Event event;

    sr::Texture texture(window, 800, 600);
    texture->DrawImage(gfx::Surface(RESOURCES_PATH "images/heart-256x256.png"),
        { 0, 0, 254, 254 }, { (800-254)/2, (600-254)/2, 254, 254 });

    MyShader shader(window);

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

        shader.timer += 4 * clock.GetDelta();

        // DRAW
        window.Begin();
        window.Clear(gfx::White);

            shader.Begin();
                texture->Draw(texture->GetRectSize(), { (window.GetSize() - texture->GetSize()) / 2, texture->GetSize() });
            shader.End();

        window.End();

        clock.End();
    }

    return 0;
}