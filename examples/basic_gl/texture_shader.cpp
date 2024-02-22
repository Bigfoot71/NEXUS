#include <nexus.hpp>

using namespace nexus;

const std::string vertex(R"(
    #version 330

    in vec3 vertexPosition;
    in vec2 vertexTexCoord;
    in vec3 vertexNormal;
    in vec4 vertexColor;

    uniform mat4 mvp;

    out vec2 fragTexCoord;
    out vec4 fragColor;

    void main()
    {
        fragTexCoord = vertexTexCoord;
        fragColor = vertexColor;
        gl_Position = mvp*vec4(vertexPosition, 1.0);
    }
)");

const std::string fragment(R"(
    #version 330

    in vec2 fragTexCoord;
    in vec4 fragColor;

    uniform sampler2D texture0;
    uniform vec4 colDiffuse;
    uniform float timer;

    out vec4 finalColor;

    void main()
    {
        vec4 texelColor = texture(texture0, fragTexCoord + vec2(0.0, sin(fragTexCoord.x * 4.0 + timer) * 0.075));
        finalColor = texelColor*colDiffuse;
    }
)");

int main(int argv, char** args)
{
    gl::Window window("GL - Texture & Shader", 800, 600);

    core::Clock clock;
    core::Event event;

    gfx::Surface screen(800, 600);
    screen.DrawImage(gfx::Surface(RESOURCES_PATH "images/heart-256x256.png"),
        { 0, 0, 254, 254 }, { (800-254)/2, (600-254)/2, 254, 254 });

    gl::Texture texture(window, screen);
    gl::Shader shader(window, vertex, fragment);

    int locTimer = shader->GetLocation("timer");
    float timer = 0;

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

        timer += 4 * clock.GetDelta();
        shader->SendValue(locTimer, &timer, gl::ShaderUniformType::Float);

        // DRAW
        window.Begin();
        window.Clear(gfx::White);

            shader->Begin();
                texture->Draw(texture->GetRectSize(), { (window.GetSize() - texture->GetSize()) / 2, texture->GetSize() });
            shader->End();

        window.End();

        clock.End();
    }

    return 0;
}