#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 3D - Lights", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Camera3D camera({ 0, 25, 50 }, { 0, 0, 0 },
        { 0, 1, 0 }, 60.0f, window.GetAspect());

    camera.movementSpeed = 1.0f;

    gl::Lights3D lights(window, 0.1f, 2);

    gl::Light3D *l1 = lights.AddShadowLight({
        { 0, 30, 0 }, { 0, 0, 0 }, { 0, 1, 0 }
    });

    gl::Light3D *l2 = lights.AddShadowLight({
        { 0, 30, 0 }, { 0, 0, 0 }, { 0, 1, 0 }
    });

    math::Vec3 l2ColHSV = { 0, 1.0f, 1.0f };

    gl::Model robot(window, RESOURCES_PATH "models/robot.glb");
    gl::Model plane(window, gl::Mesh::Plane(window, 50, 50, 1, 1));

    robot->LoadAnimations(RESOURCES_PATH "models/robot.glb", "Robot_Dance");
    l2->SetActive(false);

    input::CaptureMouse();

    float animTime = 0.0f;
    bool isRunning = true;

    while (isRunning)
    {
        // UPDATE
        clock.Begin();
        const float dt = clock.GetDelta();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::KeyDown:
                    if (event->key.keysym.scancode == input::Scancode::Escape)
                        isRunning = false;
                    if (event->key.keysym.scancode == input::Scancode::E)
                        l1->SetSpotlight(!l1->IsSpotlight()),
                        l2->SetSpotlight(!l2->IsSpotlight());
                    break;

                case core::Event::Type::MouseButtonDown:
                    l2->SetActive(true);
                    break;

                case core::Event::Type::MouseButtonUp:
                    l2->SetActive(false);
                    break;

                default:
                    break;
            }
        }

        animTime += dt;

        l1->SetPosition({
            std::cos(animTime) * 30.0f,
            30.0f,
            std::sin(animTime) * 30.0f
        });

        if (l2->IsActive())
        {
            l2->SetPosition({
                std::cos(animTime) * -30.0f,
                30.0f,
                std::sin(animTime) * -30.0f
            });

            l2->SetColor(gfx::Color::NewFromHSV(l2ColHSV));
            l2ColHSV.x += 90 * dt;
        }

        camera.Update(dt);
        lights.Update(camera);

        robot->UpdateAnimation("Robot_Dance", animTime);

        lights.ShadowCastModel(plane, { 0, 0, 0 }, {}, 0, { 1, 1, 1 });
        lights.ShadowCastModel(robot, { 0, 0, 0 }, {}, 0, { 4, 4, 4 });

        // DRAW

        window.Begin().Clear();

            camera.Begin(window);
                lights.DrawModel(plane, { 0, 0, 0 }, {}, 0, { 1, 1, 1 }, gfx::Gray);
                lights.DrawModel(robot, { 0, 0, 0 }, {}, 0, { 4, 4, 4 }, gfx::White);
            camera.End();

            lights.DrawShadowMap({ 0, 0, 128, 128 }, 0.1f, 100.0f);

        window.End();

        clock.End();
    }

    return 0;
}