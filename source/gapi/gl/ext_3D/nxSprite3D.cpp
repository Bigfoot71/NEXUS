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

#include "gapi/gl/ext_3D/nxSprite3D.hpp"

using namespace nexus;

void gl::Sprite3D::DrawBillboard(Camera3D& camera, const math::Vec3& pos, const gfx::Color& tint, const Instance * const instance) const
{
    texture->DrawBillboard(camera, instance->frameRec, pos, { 0.0f, 1.0f, 0.0f }, { 1.f, 1.f }, {0,0}, 0.0f, tint);
}

void gl::Sprite3D::DrawBillboard(Camera3D& camera, const math::Vec3& pos, math::Vec2 size, const gfx::Color& tint, const Instance * const instance) const
{
    shape2D::Rectangle srcFrameRect = instance->frameRec;

    if (size.x < 0) srcFrameRect.w *= -1, size.x *= -1;
    if (size.y < 0) srcFrameRect.h *= -1, size.y *= -1;

    texture->DrawBillboard(camera,
        srcFrameRect, pos,
        {0.0f, 1.0f, 0.0f}, size,
        {}, 0.0f, tint);
}

void gl::Sprite3D::DrawBillboard(Camera3D& camera, const math::Vec3& pos, const gfx::Color& tint, const std::string& keyInstance) const
{
    DrawBillboard(camera, pos, tint, GetInstance(keyInstance));
}

void gl::Sprite3D::DrawBillboard(Camera3D& camera, const math::Vec3& pos, math::Vec2 size, const gfx::Color& tint, const std::string& keyInstance) const
{
    DrawBillboard(camera, pos, size, tint, GetInstance(keyInstance));
}
