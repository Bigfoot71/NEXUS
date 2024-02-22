#include "input/nxKeyboard.hpp"
#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    gl::Window window("GL - Extension 3D - Basic Light", 800, 600);

    core::Clock clock;
    core::Event event;

    gl::Camera3D camera({ -5, 5, -5 }, { 0, 0, 0 }, { 0, 1, 0 }, 60.0f, window.GetAspect());
    gl::Camera3D lightCaster({ 25, 25, 25 }, { 0, 0, 0 });

    camera.orbitalSpeed *= 2.0f;
    lightCaster.orbitalSpeed *= 2.0f;

    gl::Lights3D lights(window, 0.1f, 1, 0);
    gl::Light3D *light = lights.AddLight(lightCaster);

    gl::Model plane(window, gl::Mesh::Plane(window, 10, 10, 1, 1));
    plane->meshes[0]->GenTangents();

    plane->materials[0]->SetTexture(gl::Material::MapType::Diffuse, gl::Texture(window, RESOURCES_PATH "images/metal_panel/Diffuse.jpg"));
    plane->materials[0]->SetTexture(gl::Material::MapType::Specular, gl::Texture(window, RESOURCES_PATH "images/metal_panel/Specular.jpg"));
    plane->materials[0]->SetTexture(gl::Material::MapType::Normal, gl::Texture(window, RESOURCES_PATH "images/metal_panel/Normal.jpg"));
    plane->materials[0]->SetTexture(gl::Material::MapType::Height, gl::Texture(window, RESOURCES_PATH "images/metal_panel/Height.png"));

    input::CaptureMouse();
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
                    break;

                case core::Event::Type::MouseButtonDown:
                    light->SetSpotlight(!light->IsSpotlight());

                default:
                    break;
            }
        }

        camera.Update(dt, camera.Free);

        lightCaster.Update(dt, camera.Orbital);
        light->SetCaster(lightCaster);

        lights.Update(camera);

        // DRAW
        window.Begin().Clear();
            camera.Begin(window);
                lights.DrawModel(plane, { 0, 0, 0 }, {}, 0, { 1, 1, 1 }, gfx::White);
            camera.End();
        window.End();

        clock.End();
    }

    return 0;
}
