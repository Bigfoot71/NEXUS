#include "./common.hpp"

using namespace nexus;

class Demo : public gl::State
{
  private:
    static constexpr float gravity = 9.81f;
    static constexpr float timeStep = 1.0f / 60.0f;

    gl::Camera3D camera;
    phys3D::World *world;
    std::vector<Object*> objects;

  public:
    using gl::State::State;

    void Enter() override
    {
        camera = gl::Camera3D(
            { 10.0f, 5.0f, 10.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            60.0f, app.window.GetAspect());

        world = new phys3D::World();

        // Add static floor object
        objects.push_back(
            new Object(app, world,
                math::Vec3(0, 0, 0),
                math::Quaternion(0, 0, 0, 1),
                math::Vec3(8.0f, 0.5f, 8.0f),
                0.0f, gfx::Gray));

        core::RandomGenerator gen;
        gfx::Color color = gen.RandomColor();

        for (size_t i = 0; i < 30; i++)
        {
            if (gen.Random<Uint8>(0, 1))
            {
                objects.push_back(
                    new Object(app, world,
                        gen.RandomVec3({ -5, 10, -5 }, { 5, 15, 5 }),
                        gen.RandomVec4({ -1, -1, -1, -1 }, { 1, 1, 1, 1 }).Normalized(),
                        gen.RandomVec3({ 1, 1, 1 }, { 3, 3, 3 }),
                        1.0f, gen.RandomColor()));
            }
            else
            {
                objects.push_back(
                    new Object(app, world,
                        gen.RandomVec3({ -5, 10, -5 }, { 5, 15, 5 }),
                        gen.RandomVec4({ -1, -1, -1, -1 }, { 1, 1, 1, 1 }).Normalized(),
                        gen.Random<float>(1, 3), 1.0f, gen.RandomColor()));
            }
        }
    }

    void Exit() override
    {
        for (auto obj : objects) delete obj;
        delete world;
    }

    void Update(float dt) override
    {
        camera.Update(dt, gl::Camera3D::Mode::Orbital);
        world->Step(timeStep, 10); // Update physics
    }

    void Draw() override
    {
        app.window.Clear();

        camera.Begin(app);
            gl::DrawGrid(app, 10, 1.0f);
            for (auto obj : objects) obj->Draw();
        camera.End();
    }
};

int main(int argv, char** args)
{
    gl::App app("Physics 3D - Basic", 800, 600);
    app.AddState<Demo>("demo");
    return app.Run("demo");
}
