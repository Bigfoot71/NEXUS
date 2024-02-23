/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

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