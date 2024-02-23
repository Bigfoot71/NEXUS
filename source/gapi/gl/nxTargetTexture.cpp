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

#include "gapi/gl/nxTargetTexture.hpp"
#include "gapi/gl/nxEnums.hpp"

using namespace nexus;

/* Private TargetTexture Implementaiton */

_gl_impl::TargetTexture::TargetTexture(nexus::gl::Context& ctx, int w, int h, nexus::gl::TextureFormat format)
: Contextual(ctx), texture(ctx, w, h, format), depth(ctx, w, h, true), active(false)
{
    id = ctx.LoadFramebuffer(w, h);   // Load an empty framebuffer

    if (id == 0)
    {
        throw nexus::core::NexusException("gl::TargetTexture", "Unable to create framebuffer");
    }

    ctx.EnableFramebuffer(id);

    // Attach color texture and depth renderbuffer/texture to FBO
    ctx.FramebufferAttach(id, texture.GetID(), nexus::gl::FramebufferAttachType::ColorChannel0, nexus::gl::FramebufferAttachTextureType::Texture2D, 0);
    ctx.FramebufferAttach(id, depth.GetID(), nexus::gl::FramebufferAttachType::Depth, nexus::gl::FramebufferAttachTextureType::RenderBuffer, 0);

    // Check if fbo is complete with attachments (valid)
    if (!ctx.FramebufferComplete(id))
    {
        throw nexus::core::NexusException("gl::TargetTexture", "Unable to create framebuffer");
    }

    ctx.DisableFramebuffer();
}

_gl_impl::TargetTexture::~TargetTexture()
{
    if (active) this->End();
    ctx.UnloadFramebuffer(id);
}

_gl_impl::TargetTexture& _gl_impl::TargetTexture::Begin()
{
    prevViewport = ctx.GetViewport();   // Get current viewport area before change it
    ctx.DrawRenderBatchActive();        // Update and draw internal render batch
    ctx.EnableFramebuffer(id);          // Enable render target

    // Set viewport and gl::Context internal framebuffer size
    ctx.SetViewport(0, 0, texture.GetWidth(), texture.GetHeight());

    active = true;
    return *this;
}

_gl_impl::TargetTexture& _gl_impl::TargetTexture::End()
{
    ctx.DrawRenderBatchActive();      // Update and draw internal render batch
    ctx.DisableFramebuffer();         // Disable render target (fbo)

    // Set viewport to previous framebuffer size
    ctx.SetViewport(prevViewport);

    active = false;
    return *this;
}

void _gl_impl::TargetTexture::Clear(const gfx::Color& color = gfx::Black)
{
    if (!active)
    {
        prevViewport = ctx.GetViewport();
        ctx.DrawRenderBatchActive();
        ctx.EnableFramebuffer(id);

        ctx.Viewport(this->GetRectSize());
    }

    ctx.ClearColor(color);
    ctx.ClearScreenBuffers();

    if (!active)
    {
        ctx.DrawRenderBatchActive();
        ctx.DisableFramebuffer();

        ctx.SetViewport(prevViewport);
    }
}

void _gl_impl::TargetTexture::Draw(shape2D::RectangleF src, const shape2D::RectangleF& dst, const math::Vec2& origin, float rotation, const gfx::Color& tint)  const
{
    texture.Draw(src, dst, origin, rotation, tint);
}

void _gl_impl::TargetTexture::Draw(float x, float y, const gfx::Color& tint) const
{
    texture.Draw(x, y, tint);
}

void _gl_impl::TargetTexture::Draw(const math::Vec2& position, const gfx::Color& tint) const
{
    texture.Draw(position, tint);
}

void _gl_impl::TargetTexture::Draw(const math::Vec2& position, float rotation, float scale, const gfx::Color& tint) const
{
    texture.Draw(position, rotation, scale, tint);
}

void _gl_impl::TargetTexture::Draw(const math::Vec2& position, float rotation, const math::Vec2& origin, const math::Vec2& scale, const gfx::Color& tint) const
{
    texture.Draw(position, rotation, origin, scale, tint);
}

void _gl_impl::TargetTexture::Draw(const shape2D::RectangleF& src, const math::Vec2& position, const gfx::Color& tint) const
{
    texture.Draw(src, position, tint);
}

void _gl_impl::TargetTexture::DrawBillboard(const gl::Camera3D& camera, shape2D::Rectangle source, const math::Vec3& position, const math::Vec3& up, const math::Vec2& size, const math::Vec2& origin, float rotation, const gfx::Color& tint) const
{
    texture.DrawBillboard(camera, source, position, up, size, origin, rotation, tint);
}

void _gl_impl::TargetTexture::DrawBillboard(const gl::Camera3D& camera, const math::Vec3& position, float size, const gfx::Color& tint) const
{
    texture.DrawBillboard(camera, position, size, tint);
}

void _gl_impl::TargetTexture::DrawBillboard(const gl::Camera3D& camera, const shape2D::Rectangle& source, const math::Vec3& position, const math::Vec2& size, const gfx::Color& tint) const
{
    texture.DrawBillboard(camera, source, position, size, tint);
}
