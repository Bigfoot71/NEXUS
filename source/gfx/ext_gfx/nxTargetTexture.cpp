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
