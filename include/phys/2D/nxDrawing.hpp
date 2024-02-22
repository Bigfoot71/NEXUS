#ifndef NEXUS_PHYSICS_2D_DRAWING_HPP
#define NEXUS_PHYSICS_2D_DRAWING_HPP

#ifdef EXTENSION_GFX
#   include "../../gfx/ext_gfx/nxRenderer.hpp"
#endif

#ifdef SUPPORT_OPENGL
#   include "../../gapi/gl/nxContext.hpp"
#endif

#ifdef SUPPORT_SOFTWARE_RASTERIZER
#   include "../../gapi/sr/nxContext.hpp"
#endif

#include "../../gfx/nxColor.hpp"
#include "./nxPhysics.hpp"

namespace nexus { namespace phys2D {

#ifdef EXTENSION_GFX
    NEXUS_API void DrawShape(gfx::Renderer& ctx, const Shape* shape, const Transform& transform);
    NEXUS_API void DrawBody(gfx::Renderer& ctx, const Body* body);
    NEXUS_API void DrawWorld(gfx::Renderer& ctx, const World* world);
#endif

#ifdef SUPPORT_OPENGL
    NEXUS_API void DrawShape(gl::Context& ctx, const Shape* shape, const Transform& transform, const gfx::Color& color = gfx::White);
    NEXUS_API void DrawBody(gl::Context& ctx, const Body* body);
    NEXUS_API void DrawWorld(gl::Context& ctx, const World* world);
#endif

#ifdef SUPPORT_SOFTWARE_RASTERIZER
    NEXUS_API void DrawShape(sr::Context& ctx, const Shape* shape, const Transform& transform, const gfx::Color& color = gfx::White);
    NEXUS_API void DrawBody(sr::Context& ctx, const Body* body);
    NEXUS_API void DrawWorld(sr::Context& ctx, const World* world);
#endif

}}

#endif //NEXUS_PHYSICS_2D_DRAWING_HPP