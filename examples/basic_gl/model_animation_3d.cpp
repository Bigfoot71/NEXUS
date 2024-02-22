#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Model Animation 3D", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Camera3D camera({ 0, 25, 50 }, { 0, 0, 0 },
        { 0, 1, 0 }, 60.0f, window.GetAspect());

    gl::Model model(window, RESOURCES_PATH "models/robot.glb");
    model->LoadAnimations(RESOURCES_PATH "models/robot.glb", "Robot_Dance");

    input::CaptureMouse();

    bool isRunning = true;
    float animTime = 0;

    while (isRunning)
    {
        clock.Begin();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::KeyDown:
                    isRunning = !(event->key.keysym.scancode == input::Scancode::Escape);
                    break;

                default:
                    break;
            }
        }

        camera.Update(clock.GetDelta());
        model->UpdateAnimation("Robot_Dance", animTime);

        animTime += clock.GetDelta();

        window.Begin();
        window.Clear();

            camera.Begin(window);
                model->Draw({ 0, 0, 0 }, 5.0f, gfx::White);
                gl::DrawGrid(window, 10, 10);
            camera.End();

        window.End();

        clock.End();
    }

    return 0;
}