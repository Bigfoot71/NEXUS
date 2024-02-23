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

#include "gfx/ext_gfx/ext_2D/nxSprite.hpp"

using namespace nexus;

void gfx::Sprite::Draw(const math::Vec2& pos, const Instance * const instance)
{
    texture->Draw(instance->frameRec, shape2D::RectangleF(
        pos.x, pos.y, frameSize.x, frameSize.y), frameCenter, 0.0f);
}

void gfx::Sprite::Draw(const math::Vec2& pos, float scale, float rotation, const math::Vec2& uvOrigin, const Instance * const instance)
{
    shape2D::Rectangle srcFrameRect = instance->frameRec;
    const math::Vec2 scaledSize = static_cast<math::Vec2>(frameSize) * std::abs(scale);

    if (scale < 0)
    {
        srcFrameRect.x += srcFrameRect.w, srcFrameRect.w *= -1;
        srcFrameRect.y += srcFrameRect.h, srcFrameRect.h *= -1;
    }

    texture->Draw(srcFrameRect, shape2D::RectangleF(
        pos.x, pos.y, scaledSize.x, scaledSize.y
    ), scaledSize * uvOrigin, rotation);
}

void gfx::Sprite::Draw(const math::Vec2& pos, float sx, float sy, float rotation, const math::Vec2& uvOrigin, const Instance * const instance)
{
    shape2D::Rectangle srcFrameRect = instance->frameRec;
    const math::Vec2 scaledSize = static_cast<math::Vec2>(frameSize) * math::Vec2(sx, sy).Abs();

    if (sx < 0) srcFrameRect.w *= -1;
    if (sy < 0) srcFrameRect.h *= -1;

    texture->Draw(srcFrameRect, shape2D::RectangleF(
        pos.x, pos.y, scaledSize.x, scaledSize.y
    ), scaledSize * uvOrigin, rotation);
}

void gfx::Sprite::Draw(shape2D::RectangleF dest, const math::Vec2& origin, float rot, const Instance * const instance)
{
    texture->Draw(instance->frameRec, dest, origin, rot);
}

void gfx::Sprite::Draw(const nexus::math::Vec2& pos, const std::string& keyInstance)
{
    Draw(pos, GetInstance(keyInstance));
}

void gfx::Sprite::Draw(const nexus::math::Vec2& pos, float scale, float rotation, const nexus::math::Vec2& uvOrigin, const std::string& keyInstance)
{
    Draw(pos, scale, rotation, uvOrigin, GetInstance(keyInstance));
}

void gfx::Sprite::Draw(const nexus::math::Vec2& pos, float sx, float sy, float rotation, const nexus::math::Vec2& uvOrigin, const std::string& keyInstance)
{
    Draw(pos, sx, sy, rotation, uvOrigin, GetInstance(keyInstance));
}

void gfx::Sprite::Draw(const nexus::shape2D::Rectangle& dest, const nexus::math::Vec2& origin, float rot, const std::string& keyInstance)
{
    Draw(dest, origin, rot, GetInstance(keyInstance));
}
