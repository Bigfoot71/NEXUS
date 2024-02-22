#include "phys/2D/nxDrawing.hpp"

using namespace nexus;


#ifdef EXTENSION_GFX

#   include "gfx/ext_gfx/nxRenderer.hpp"

    void phys2D::DrawShape(gfx::Renderer& ctx, const Shape* shape, const Transform& transform)
    {
        switch (shape->GetType())
        {
            case Shape::e_circle:
            {
                auto circleShape = static_cast<const CircleShape*>(shape);
                ctx.DrawCircleLines(transform.p + circleShape->m_p, circleShape->m_radius);
            } break;

            case Shape::e_chain:
            {
                auto chainShape = static_cast<const ChainShape*>(shape);
                const int vertexCount = chainShape->m_count;

                for (int i = 1; i < vertexCount; i++)
                {
                    const math::Vec2 pA = transform.p + chainShape->m_vertices[i - 1];
                    const math::Vec2 pB = transform.p + chainShape->m_vertices[i];
                    ctx.DrawLine(pA, pB);
                }
            } break;

            case Shape::e_edge:
            {
                const auto edgeShape = static_cast<const EdgeShape*>(shape);
                const math::Vec2 pA = transform.p + edgeShape->m_vertex1;
                const math::Vec2 pB = transform.p + edgeShape->m_vertex2;
                ctx.DrawLine(pA, pB);
            } break;

            case Shape::e_polygon:
            {
                const auto polygonShape = static_cast<const PolygonShape*>(shape);
                const float s = transform.q.s, c = transform.q.c;
                const int vertexCount = polygonShape->m_count;

                for (int i = 0; i < vertexCount; i++)
                {
                    int j = (i + 1) < vertexCount ? (i + 1) : 0; // next vertice

                    const math::Vec2& vertexA = polygonShape->m_vertices[i];
                    const math::Vec2& vertexB = polygonShape->m_vertices[j];

                    const math::Vec2 pA = {
                        transform.p.x + (vertexA.x * c - vertexA.y * s),
                        transform.p.y + (vertexA.x * s + vertexA.y * c)
                    };

                    const math::Vec2 pB = {
                        transform.p.x + (vertexB.x * c - vertexB.y * s),
                        transform.p.y + (vertexB.x * s + vertexB.y * c)
                    };

                    ctx.DrawLine(pA, pB);
                }
            } break;

            default: break;
        }
    }

    void phys2D::DrawBody(gfx::Renderer& ctx, const phys2D::Body* body)
    {
        const gfx::Color prevColor = ctx.GetColor();

        // Green: when the body is awake
        // Yellow: when the body is asleep
        // Transparent: when the body is deactivated

        Transform transform = body->GetTransform();

        ctx.SetColor(gfx::Purple.Alpha(body->IsEnabled() ? 1.0f : 0.5f));
        ctx.DrawCircle(transform.p, 2);

        ctx.SetColor((body->IsAwake() ? gfx::Green : gfx::Yellow)
            .Alpha(body->IsEnabled() ? 1.0f : 0.5f));

        for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            DrawShape(ctx, fixture->GetShape(), transform);
        }

        ctx.SetColor(prevColor);
    }

    void phys2D::DrawWorld(gfx::Renderer& ctx, const phys2D::World* world)
    {
        for (auto body = world->GetBodyList(); body; body = body->GetNext())
        {
            DrawBody(ctx, body);
        }
    }

#endif


#ifdef SUPPORT_OPENGL

#   include "gapi/gl/nxPrimitives2D.hpp"

    void phys2D::DrawShape(gl::Context& ctx, const Shape* shape, const Transform& transform, const gfx::Color& color)
    {
        switch (shape->GetType())
        {
            case Shape::e_circle:
            {
                auto circleShape = static_cast<const CircleShape*>(shape);
                gl::DrawCircleLines(ctx, transform.p + circleShape->m_p, circleShape->m_radius, color);
            } break;

            case Shape::e_chain:
            {
                auto chainShape = static_cast<const ChainShape*>(shape);
                const int vertexCount = chainShape->m_count;

                for (int i = 1; i < vertexCount; i++)
                {
                    const math::Vec2 pA = transform.p + chainShape->m_vertices[i - 1];
                    const math::Vec2 pB = transform.p + chainShape->m_vertices[i];
                    gl::DrawLine(ctx, pA, pB, color);
                }
            } break;

            case Shape::e_edge:
            {
                const auto edgeShape = static_cast<const EdgeShape*>(shape);
                const math::Vec2 pA = transform.p + edgeShape->m_vertex1;
                const math::Vec2 pB = transform.p + edgeShape->m_vertex2;
                gl::DrawLine(ctx, pA, pB, color);
            } break;

            case Shape::e_polygon:
            {
                const auto polygonShape = static_cast<const PolygonShape*>(shape);
                const float s = transform.q.s, c = transform.q.c;
                const int vertexCount = polygonShape->m_count;

                for (int i = 0; i < vertexCount; i++)
                {
                    int j = (i + 1) < vertexCount ? (i + 1) : 0; // next vertice

                    const math::Vec2& vertexA = polygonShape->m_vertices[i];
                    const math::Vec2& vertexB = polygonShape->m_vertices[j];

                    const math::Vec2 pA = {
                        transform.p.x + (vertexA.x * c - vertexA.y * s),
                        transform.p.y + (vertexA.x * s + vertexA.y * c)
                    };

                    const math::Vec2 pB = {
                        transform.p.x + (vertexB.x * c - vertexB.y * s),
                        transform.p.y + (vertexB.x * s + vertexB.y * c)
                    };

                    gl::DrawLine(ctx, pA, pB, color);
                }
            } break;

            default: break;
        }
    }

    void phys2D::DrawBody(gl::Context& ctx, const phys2D::Body* body)
    {
        // Green: when the body is awake
        // Yellow: when the body is asleep
        // Transparent: when the body is deactivated

        Transform transform = body->GetTransform();

        gl::DrawCircle(ctx, transform.p.x, transform.p.y, 2,
            gfx::Purple.Alpha(body->IsEnabled() ? 1.0f : 0.5f));

        const gfx::Color color = (body->IsAwake() ? gfx::Green : gfx::Yellow)
            .Alpha(body->IsEnabled() ? 1.0f : 0.5f);

        for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            DrawShape(ctx, fixture->GetShape(), transform, color);
        }
    }

    void phys2D::DrawWorld(gl::Context& ctx, const phys2D::World* world)
    {
        for (auto body = world->GetBodyList(); body; body = body->GetNext())
        {
            DrawBody(ctx, body);
        }
    }

#endif


#ifdef SUPPORT_SOFTWARE_RASTERIZER

#   include "gapi/sr/nxPrimitives2D.hpp"

    void phys2D::DrawShape(sr::Context& ctx, const Shape* shape, const Transform& transform, const gfx::Color& color)
    {
        switch (shape->GetType())
        {
            case Shape::e_circle:
            {
                auto circleShape = static_cast<const CircleShape*>(shape);
                sr::DrawCircleLines(ctx, transform.p + circleShape->m_p, circleShape->m_radius, color);
            } break;

            case Shape::e_chain:
            {
                auto chainShape = static_cast<const ChainShape*>(shape);
                const int vertexCount = chainShape->m_count;

                for (int i = 1; i < vertexCount; i++)
                {
                    const math::Vec2 pA = transform.p + chainShape->m_vertices[i - 1];
                    const math::Vec2 pB = transform.p + chainShape->m_vertices[i];
                    sr::DrawLine(ctx, pA, pB, color);
                }
            } break;

            case Shape::e_edge:
            {
                const auto edgeShape = static_cast<const EdgeShape*>(shape);
                const math::Vec2 pA = transform.p + edgeShape->m_vertex1;
                const math::Vec2 pB = transform.p + edgeShape->m_vertex2;
                sr::DrawLine(ctx, pA, pB, color);
            } break;

            case Shape::e_polygon:
            {
                const auto polygonShape = static_cast<const PolygonShape*>(shape);
                const float s = transform.q.s, c = transform.q.c;
                const int vertexCount = polygonShape->m_count;

                for (int i = 0; i < vertexCount; i++)
                {
                    int j = (i + 1) < vertexCount ? (i + 1) : 0; // next vertice

                    const math::Vec2& vertexA = polygonShape->m_vertices[i];
                    const math::Vec2& vertexB = polygonShape->m_vertices[j];

                    const math::Vec2 pA = {
                        transform.p.x + (vertexA.x * c - vertexA.y * s),
                        transform.p.y + (vertexA.x * s + vertexA.y * c)
                    };

                    const math::Vec2 pB = {
                        transform.p.x + (vertexB.x * c - vertexB.y * s),
                        transform.p.y + (vertexB.x * s + vertexB.y * c)
                    };

                    sr::DrawLine(ctx, pA, pB, color);
                }
            } break;

            default: break;
        }
    }

    void phys2D::DrawBody(sr::Context& ctx, const phys2D::Body* body)
    {
        // Green: when the body is awake
        // Yellow: when the body is asleep
        // Transparent: when the body is deactivated

        Transform transform = body->GetTransform();

        sr::DrawCircle(ctx, transform.p.x, transform.p.y, 2,
            gfx::Purple.Alpha(body->IsEnabled() ? 1.0f : 0.5f));

        const gfx::Color color = (body->IsAwake() ? gfx::Green : gfx::Yellow)
            .Alpha(body->IsEnabled() ? 1.0f : 0.5f);

        for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            DrawShape(ctx, fixture->GetShape(), transform, color);
        }
    }

    void phys2D::DrawWorld(sr::Context& ctx, const phys2D::World* world)
    {
        for (auto body = world->GetBodyList(); body; body = body->GetNext())
        {
            DrawBody(ctx, body);
        }
    }

#endif
