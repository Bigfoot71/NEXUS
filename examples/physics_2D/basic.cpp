#include <nexus.hpp>

using namespace nexus;

class Box
{
  private:
    shape2D::RectangleF rect;
    math::Vec2 rectOrigin;

  public:
    phys2D::Body *body;

    Box(phys2D::World* world, const shape2D::RectangleF& _rect)
    : rect(_rect), rectOrigin(rect.GetSize() * 0.5f)
    {
        phys2D::BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(rect.x, rect.y);
        body = world->CreateBody(&bodyDef);

        phys2D::PolygonShape boxShape;
        boxShape.SetAsBox(rectOrigin.x, rectOrigin.y);

        // Define the coefficient of restitution
        phys2D::FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 10.0f;
        fixtureDef.restitution = 0.5f;
        body->CreateFixture(&fixtureDef);
    }

    inline void Update()
    {
        rect.SetPosition(body->GetPosition());
    }

    inline void Draw(gl::Context& ctx, const gfx::Color& color) const
    {
        gl::DrawRectangle(ctx, rect, rectOrigin, math::RadToDeg(body->GetAngle()), color);
    }
};

class Rect
{
  public:
    phys2D::Body *body;
    shape2D::Rectangle rect;

    Rect(phys2D::World* world, float x, float y, float width, float height)
    : rect(x - width * 0.5f, y - height * 0.5f, width, height)
    {
        phys2D::BodyDef bodyDef;
        bodyDef.position.Set(x, y);
        body = world->CreateBody(&bodyDef);

        phys2D::PolygonShape wallShape;
        wallShape.SetAsBox(width, height);
        body->CreateFixture(&wallShape, 0.0f);
    }

    void Draw(gl::Context& ctx, const gfx::Color& color)
    {
        gl::DrawRectangleLines(ctx, rect, 1, color);
    }
};

class Mouse
{
  private:
    phys2D::Body *body;
    float radius;

  public:
    Mouse(phys2D::World* world, float _radius) : radius(_radius)
    {
        phys2D::BodyDef circleBodyDef;
        circleBodyDef.type = b2_kinematicBody;
        circleBodyDef.position.Set(0, 0);
        body = world->CreateBody(&circleBodyDef);

        phys2D::CircleShape circleShape;
        circleShape.m_radius = radius;
        circleShape.m_p.Set(0, 0);

        phys2D::FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 1.0f;
        fixtureDef.restitution = 10.0f;
        body->CreateFixture(&fixtureDef);

        body->SetEnabled(false);
    }

    void OnMouseButtonDown()
    {
        body->SetEnabled(true);
    }

    void OnMouseButtonUp()
    {
        body->SetEnabled(false);
    }

    void Update()
    {
        if (body->IsEnabled())
        {
            math::Vec2 mousePosition = input::GetMousePosition();
            body->SetTransform(mousePosition, body->GetAngle());
        }
    }
};

class Demo : public gl::State
{
  private:
    static constexpr float gravity = 9.81f;
    static constexpr float timeStep = 1.0f / 60.0f;

    std::unique_ptr<phys2D::World> world;

    std::unique_ptr<Mouse> circleMouse;
    std::vector<Box> boxes;

    std::unique_ptr<Rect> groundBody;
    std::unique_ptr<Rect> ceilingBody;
    std::unique_ptr<Rect> leftWallBody;
    std::unique_ptr<Rect> rightWallBody;

  public:
    using gl::State::State;

    void Enter() override
    {
        // Initalize the world
        world = std::make_unique<phys2D::World>(math::Vec2(0, gravity));

        // Create static walls
        const float sw = app.window.GetWidth();
        const float sh = app.window.GetHeight();

        groundBody = std::make_unique<Rect>(world.get(), sw * 0.5f, sh - 5.0f, sw, 10.0f);
        ceilingBody = std::make_unique<Rect>(world.get(), sw * 0.5f, 5.0f, sw, 10.0f);
        leftWallBody = std::make_unique<Rect>(world.get(), 5.0f, sh * 0.5f, 10.0f, sh);
        rightWallBody = std::make_unique<Rect>(world.get(), sw - 5.0f, sh * 0.5f, 10.0f, sh);

        // Create some dynamics boxes from image
        {
            gfx::Surface heart(RESOURCES_PATH "images/heart-13x12.png");

            for (int y = 0; y < heart.GetHeight(); y++)
            {
                for (int x = 0; x < heart.GetWidth(); x++)
                {
                    float px = (app.window.GetWidth() - heart.GetHeight()) * 0.5f + x * 20.0f;
                    float py = (app.window.GetHeight() - heart.GetWidth()) * 0.5f + y * 20.0f;
                    boxes.push_back(Box(world.get(), { px, py, 20, 20 }));
                }
            }
        }

        // Creates the mouse interaction body (dynamic circle)
        circleMouse = std::make_unique<Mouse>(world.get(), 20.0f);
    }

    void Exit() override
    {
        world = nullptr;
        boxes.clear();
    }

    void OnMouseButtonDown(const core::Event::MouseButton& event) override
    {
        circleMouse->OnMouseButtonDown();
    }

    void OnMouseButtonUp(const core::Event::MouseButton& event) override
    {
        circleMouse->OnMouseButtonUp();
    }

    void Update(float dt) override
    {
        world->Step(timeStep, 6, 2);
        for (Box& box : boxes) box.Update();
        circleMouse->Update();
    }

    void Draw() override
    {
        app.window.Clear();
        for (Box& box : boxes) box.Draw(app, gfx::Red);
        phys2D::DrawWorld(app, world.get());
    }
};

int main(int argv, char** args)
{
    gl::App app("Physics 2D - Basic", 800, 600);

    app.AddState<Demo>("demo");
    return app.Run("demo");
}