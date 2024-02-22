#include "gfx/ext_gfx/nxTargetTexture.hpp"

using namespace nexus;

_gfx_impl::TargetTexture::TargetTexture(nexus::gfx::Renderer& ctx, int w, int h, nexus::gfx::PixelFormat format)
: Texture(ctx, w, h, format, nexus::gfx::TextureAccess::Target)
{ }

_gfx_impl::TargetTexture::~TargetTexture()
{
    if (active) End();
}

bool _gfx_impl::TargetTexture::IsActive() const
{
    return active;
}

_gfx_impl::TargetTexture& _gfx_impl::TargetTexture::Begin()
{
    ctx.SetTarget(data);
    active = true;
    return *this;
}

_gfx_impl::TargetTexture& _gfx_impl::TargetTexture::End()
{
    ctx.SetTarget(nullptr);
    active = false;
    return *this;
}

void _gfx_impl::TargetTexture::Clear()
{
    if (active)
    {
        ctx.Clear();
    }
    else
    {
        ctx.SetTarget(data);
        ctx.Clear();
        ctx.SetTarget(nullptr);
    }
}
