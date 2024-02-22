#include <nexus.hpp>

using namespace nexus;

class Heart
{
  private:
    math::Vec2 position;
    math::Vec2 velocity;
    float rotation;
    float scale;
    Uint8 alpha;

  public:
    Heart() = default;

    Heart(const math::Vec2& pos)
    {
        core::RandomGenerator rng;
        position = pos;
        velocity = rng.RandomVec2({ -200, -200 }, { 200, 200 });
        rotation = rng.Random<float>(0, math::Tau);
        scale = rng.Random<float>(0.25f, 0.5f);
        alpha = rng.Random<Uint8>(100, 255);
    }

    void Update(float dt, const core::Window& win)
    {
        position += velocity * dt;
        rotation += (velocity.x + velocity.y) * 0.05f * dt;

        if (position.x < 0 || position.x > win.GetWidth())
        {
            position.x = std::clamp(position.x, 0.0f, static_cast<float>(win.GetWidth()));
            velocity.x = -velocity.x;
        }

        if (position.y < 0 || position.y > win.GetHeight())
        {
            position.y = std::clamp(position.y, 0.0f, static_cast<float>(win.GetHeight()));
            velocity.y = -velocity.y;
        }
    }

    void Draw(sr::Texture& tex)
    {
        tex->Draw(position, rotation, { 127*scale, 127*scale }, { scale, scale }, { 255, 255, 255, alpha });
    }
};

int main(int argv, char** args)
{
    sr::Window window("Software Rasterizer - Sprite Batch", 800, 600);

    core::Clock clock;
    core::Event event;

    sr::Font font(window, RESOURCES_PATH "fonts/Futurette.otf", 32);
    font->SetStyle(sr::Font::Style::Bold);

    sr::Texture texture(window, RESOURCES_PATH "images/heart-256x256.png");

    bool isRunning = true;

    std::string strFPS{};
    std::string strCount{};

    Heart hearts[50000];
    int spriteCount = 0;

    while (isRunning)
    {
        // UPDATE
        clock.Begin();

        core::TextFormatTo(strFPS, "FPS: %.0f", clock.GetAverageFPS());
        core::TextFormatTo(strCount, "Sprites: %i", spriteCount);

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

        if (input::IsMouseButtonPressed() && spriteCount < 50000)
        {
            if (input::IsMouseButtonPressed(input::MouseButton::Left))
            {
                hearts[spriteCount++] = Heart(input::GetMousePosition());
            }
            else
            {
                for (int i = 0; i < 100; i++)
                {
                    hearts[spriteCount++] = Heart(input::GetMousePosition());
                    if (spriteCount == 50000) break;
                }
            }
        }

        // DRAW
        window.Begin();
        window.Clear(gfx::Blue);

            for (int i = 0; i < spriteCount; i++)
            {
                hearts[i].Update(clock.GetDelta(), window);
                hearts[i].Draw(texture);
            }

            font->Draw(strFPS, 10, 10);
            font->Draw(strCount, 10, 50);

        window.End();

        clock.End();
    }

    return 0;
}