#include "./common.hpp"

using namespace nexus;

struct World
{
    std::vector<Object>             objects;
    std::unique_ptr<phys3D::World>  physics;
    std::unique_ptr<gl::Skybox>     skybox;
    std::unique_ptr<gl::Lights3D>   lights;

    World(gl::App& app, const gl::Camera3D& camera, const math::Vec3& size)
    {
        physics = std::make_unique<phys3D::World>();

        objects.emplace_back(gl::Model(app, gl::Mesh::Heightmap(app, gfx::Surface(RESOURCES_PATH "images/ground_height.png"), size)),
            physics.get(), math::Vec3(-size.x * 0.5f, 0, -size.z * 0.5f), math::Quaternion(0, 0, 0, 1), phys3D::Shape::Mesh, 0.0f, gfx::White);

        objects.back().model->meshes[0]->GenTangents(); // Generation of tangents for calculating normals with the normalMap (grass_normal)
        objects.back().model->materials[0]->maps[gl::Material::MapType::Diffuse].texture = gl::Texture(app, RESOURCES_PATH "images/ground_diffuse.png");
        objects.back().model->materials[0]->maps[gl::Material::MapType::Normal].texture = gl::Texture(app, RESOURCES_PATH "images/ground_normal.png");

        skybox = std::make_unique<gl::Skybox>(app, RESOURCES_PATH "images/skybox.png");

        lights = std::make_unique<gl::Lights3D>(app, gfx::DarkGray, 1, 1024);
        lights->AddShadowLight(gl::Camera3D({ -size.x * 0.5f, 32, size.z * 0.5f }, {}));
    }

    void AddCube(gl::Context& ctx, const math::Vec3& position, const math::Quaternion& rotation, const math::Vec3& size, float mass, gfx::Color color)
    {
        objects.emplace_back(ctx, physics.get(), position, rotation, size, mass, color);
    }

    void AddSphere(gl::Context& ctx, const math::Vec3& position, const math::Quaternion& rotation, float radius, float mass, gfx::Color color)
    {
        objects.emplace_back(ctx, physics.get(), position, rotation, radius, mass, color);
    }

    void Update(gl::Camera3D& camera)
    {
        physics->Step(1.0f/60.0f, 10);
        lights->Update(camera);

        for (auto& object : objects)
        {
            lights->ShadowCastModel(object.model,
                object.physicalObject.GetPosition(),
                object.physicalObject.GetRotationAxis(),
                object.physicalObject.GetAngle());
        }
    }

    void Draw()
    {
        skybox->Draw();

        for (auto& object : objects)
        {
            lights->DrawModel(object.model,
                object.physicalObject.GetPosition(),
                object.physicalObject.GetRotationAxis(),
                object.physicalObject.GetAngle(),
                { 1, 1, 1 }, object.color);
        }
    }
};

struct Player
{
    gl::Camera3D        camera;
    gl::Model           hitBox;
    phys3D::RigidSphere &obj;

    Player(gl::App& app, World* world)
    : camera({ 0.0f, 16.0f, -32.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 60.0f, app.window.GetAspect())
    , hitBox(app, gl::Mesh::Sphere(app, 8.0f, 24, 24))
    , obj(world->physics->AddObject<phys3D::RigidSphere>(camera.position, math::Vec4(0, 0, 0, 1), 8.0f, 1.0f))
    { }

    void Update(float dt)
    {
        const math::Vec3 newPos = obj.GetPosition();
        camera.target += newPos - camera.position;
        camera.position = newPos;

        const math::Vec2 mouseDelta = input::GetMouseDelta();
        camera.Yaw(-mouseDelta.x * camera.mouseMoveSensitivity, false);
        camera.Pitch(-mouseDelta.y * camera.mouseMoveSensitivity, false, false, false);

        const math::Vec3 targetDir = camera.GetDirection();
        const math::Vec3 forward = camera.GetForward();
        const math::Vec3 right = camera.GetRight();

        math::Vec3 velocity = { 0, 0, 0 };

        if (input::IsKeyPressed(input::Scancode::W)) velocity += forward + targetDir;
        if (input::IsKeyPressed(input::Scancode::S)) velocity -= forward + targetDir;
        if (input::IsKeyPressed(input::Scancode::D)) velocity += right;
        if (input::IsKeyPressed(input::Scancode::A)) velocity -= right;

        obj.ApplyCentralForce(velocity.Normalized() * 20.0f);
    }

    void DrawHitBox(gl::Context& ctx)
    {
        //gl::DrawSphereWires(ctx, obj.GetPosition(), 8.0f, gfx::Green);

        ctx.SetCullFace(gl::CullMode::FaceFront);
            hitBox->DrawWires(obj.GetPosition(), obj.GetRotationAxis(), obj.GetAngle(), { 1.0f, 1.0f, 1.0f }, gfx::Green);
        ctx.SetCullFace(gl::CullMode::FaceBack);
    }
};

class Demo : public gl::State
{
  private:
    std::unique_ptr<World> world;
    std::unique_ptr<Player> player;

  public:
    using gl::State::State;

    void Enter() override
    {
        world = std::make_unique<World>(app, player->camera, math::Vec3{ 64, 1, 64 });
        player = std::make_unique<Player>(app, world.get());

        core::RandomGenerator gen;

        for (int i = 0; i < 10; i++)
        {
            if (gen.Random<uint8_t>(0, 1))
            {
                world->AddCube(app,
                    gen.RandomVec3({ -5, 10, -5 }, { 5, 15, 5 }),
                    gen.RandomVec4({ -1, -1, -1, -1 }, { 1, 1, 1, 1 }).Normalized(),
                    gen.RandomVec3({ 1, 1, 1 }, { 3, 3, 3 }),
                    1.0f, gen.RandomColor());
            }
            else
            {
                world->AddSphere(app,
                    gen.RandomVec3({ -5, 10, -5 }, { 5, 15, 5 }),
                    gen.RandomVec4({ -1, -1, -1, -1 }, { 1, 1, 1, 1 }).Normalized(),
                    gen.Random<float>(1, 3), 1.0f, gen.RandomColor()
                );
            }
        }

        input::CaptureMouse();
    }

    void Exit() override
    {
        input::ReleaseMouse();
        world = nullptr;
    }

    void Update(float dt) override
    {
        world->Update(player->camera);
        player->Update(app.clock.GetDelta());
    }

    void Draw() override
    {
        app.window.Clear();

        player->camera.Begin(app);

            world->Draw();

            if (input::IsMouseButtonPressed())
            {
                player->DrawHitBox(app);
            }

        player->camera.End();
    }

    /* Event callbacks */

    void OnKeyDown(const core::Event::Keyboard& event) override
    {
        if (event.keysym.scancode == input::Scancode::Escape)
        {
            app.Finish();
        }
    }
};

int main(int argv, char** args)
{
    gl::App app("Physics 3D - World", 800, 600);
    app.AddState<Demo>("demo");
    return app.Run("demo");
}