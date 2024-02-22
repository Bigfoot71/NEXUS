#include "gapi/sr/nxTargetTexture.hpp"
#include "gapi/sr/nxTexture.hpp"

using namespace nexus;

_sr_impl::TargetTexture& _sr_impl::TargetTexture::Begin()
{
    prevViewport = ctx.GetViewport();       // Get current viewport area before change it
    ctx.EnableFramebuffer(framebuffer);     // Enable render target
    framebuffer.Begin();                    // Lock surface if necessary

    // Set viewport and gl::Context internal framebuffer size
    ctx.SetViewport(framebuffer.GetRectSize());

    active = true;
    return *this;
}

_sr_impl::TargetTexture& _sr_impl::TargetTexture::End()
{
    ctx.DisableFramebuffer();           // Disable render target (fbo)
    ctx.SetViewport(prevViewport);      // Set viewport to previous framebuffer size
    framebuffer.End();                  // Unlock surface if necessary

    active = false;
    return *this;
}

void _sr_impl::TargetTexture::Clear(const gfx::Color& color)
{
    if (!active)
    {
        framebuffer.Begin();
    }

    framebuffer.Clear(color);

    if (!active)
    {
        framebuffer.End();
    }
}

void _sr_impl::TargetTexture::Draw(shape2D::RectangleF src, const shape2D::RectangleF& dst, const math::Vec2& origin, float rotation, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).Draw(src, dst, origin, rotation, tint);
}

void _sr_impl::TargetTexture::Draw(float x, float y, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).Draw(x, y, tint);
}

void _sr_impl::TargetTexture::Draw(const math::Vec2& position, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).Draw(position, tint);
}

void _sr_impl::TargetTexture::Draw(const math::Vec2& position, float rotation, float scale, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).Draw(position, rotation, scale, tint);
}

void _sr_impl::TargetTexture::Draw(const math::Vec2& position, float rotation, const math::Vec2& origin, const math::Vec2& scale, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).Draw(position, rotation, origin, scale, tint);
}

void _sr_impl::TargetTexture::Draw(const shape2D::RectangleF& src, const math::Vec2& position, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).Draw(src, position, tint);
}

void _sr_impl::TargetTexture::DrawBillboard(const sr::Camera3D& camera, shape2D::Rectangle source, const math::Vec3& position, const math::Vec3& up, const math::Vec2& size, const math::Vec2& origin, float rotation, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).DrawBillboard(camera, source, position, up, size, origin, rotation, tint);
}

void _sr_impl::TargetTexture::DrawBillboard(const sr::Camera3D& camera, const math::Vec3& position, float size, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).DrawBillboard(camera, position, size, tint);
}

void _sr_impl::TargetTexture::DrawBillboard(const sr::Camera3D& camera, const shape2D::Rectangle& source, const math::Vec3& position, const math::Vec2& size, const gfx::Color& tint) const
{
    _sr_impl::Texture(ctx, framebuffer, false).DrawBillboard(camera, source, position, size, tint);
}
