#include "core/nxRandom.hpp"
#include <nexus.hpp>

using namespace nexus;

struct Character
{
    gl::Sprite2D::Instance *inst;
    gl::Sprite2D &sprite;

    math::Vec2 position;
    float speed, speedAnim;
    float dirX;

    Character(gl::Sprite2D& sprite, const std::string& keyInstance, const std::string& keyAnimation, core::RandomGenerator& gen, int screenW, int screenH)
    : sprite(sprite)
    {
        inst = sprite.NewInstance(keyInstance, keyAnimation);

        speed = gen.Random(100, 200);
        speedAnim = speed / 100;

        position = math::Vec2(gen.Random<float>(32, screenW - 32), screenH - 32);
        dirX = gen.Random(0, 1) ? -1 : 1;
    }

    void Update(int screenW, float dt)
    {
        sprite.Update(speedAnim * dt, inst);
        position.x += dirX * speed * dt;

        if (dirX > 0 && position.x + 16 > screenW)
        {
            position.x = screenW - 16;
            dirX *= -1;
        }
        else if (dirX < 0 && position.x - 16 < 0)
        {
            position.x = 16;
            dirX *= -1;
        }
    }

    void Draw() const
    {
        sprite.Draw(position, 2 * dirX, 2, 0, math::Vec2(0.5f), gfx::White, inst);
    }
};

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 2D - Sprite", 800, 600);

    core::Clock clock;
    core::Event event;

    core::RandomGenerator gen;
    std::vector<Character> characters;
    gl::Sprite2D sprite(window, RESOURCES_PATH "images/spritesheet.png", 4, 4);

    sprite.NewAnimation("A", 0, 3, 0.1f, true);
    sprite.NewAnimation("B", 4, 7, 0.1f, true);
    sprite.NewAnimation("C", 8, 11, 0.1f, true);
    sprite.NewAnimation("D", 12, 15, 0.1f, true);

    for (char i = 1; i <= 32; i++)
    {
        characters.emplace_back(sprite,
            std::string(1, i),
            std::string(1, gen.RandomChar("ABCD")),
            gen, window.GetWidth(), window.GetHeight());
    }

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

        for (auto & character : characters)
        {
            character.Update(window.GetWidth(), clock.GetDelta());
        }

        // DRAW
        window.Begin().Clear(gfx::White);
            for (const auto & character : characters) character.Draw();
        window.End();

        clock.End();
    }

    return 0;
}
