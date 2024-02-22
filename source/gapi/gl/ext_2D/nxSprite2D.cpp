#include "gapi/gl/ext_2D/nxSprite2D.hpp"

using namespace nexus;

void gl::Sprite2D::Draw(const math::Vec2& pos, gfx::Color tint, const Instance * const instance) const
{
    texture->Draw(instance->frameRec, shape2D::RectangleF(
        pos.x, pos.y, frameSize.x, frameSize.y), frameCenter, 0.0f);
}

void gl::Sprite2D::Draw(const math::Vec2& pos, float scale, float rotation, const math::Vec2& uvOrigin, gfx::Color tint, const Instance * const instance) const
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

void gl::Sprite2D::Draw(const math::Vec2& pos, float sx, float sy, float rotation, const math::Vec2& uvOrigin, gfx::Color tint, const Instance * const instance) const
{
    shape2D::Rectangle srcFrameRect = instance->frameRec;
    const math::Vec2 scaledSize = static_cast<math::Vec2>(frameSize) * math::Vec2(sx, sy).Abs();

    if (sx < 0) srcFrameRect.w *= -1;
    if (sy < 0) srcFrameRect.h *= -1;

    texture->Draw(srcFrameRect, shape2D::RectangleF(
        pos.x, pos.y, scaledSize.x, scaledSize.y
    ), scaledSize * uvOrigin, rotation);
}

void gl::Sprite2D::Draw(shape2D::RectangleF dest, const math::Vec2& origin, float rot, gfx::Color tint, const Instance * const instance) const
{
    texture->Draw(instance->frameRec, dest, origin, rot);
}

void gl::Sprite2D::Draw(const nexus::math::Vec2& pos, gfx::Color tint, const std::string& keyInstance) const
{
    Draw(pos, tint, GetInstance(keyInstance));
}

void gl::Sprite2D::Draw(const nexus::math::Vec2& pos, float scale, float rotation, const nexus::math::Vec2& uvOrigin, gfx::Color tint, const std::string& keyInstance) const
{
    Draw(pos, scale, rotation, uvOrigin, tint, GetInstance(keyInstance));
}

void gl::Sprite2D::Draw(const nexus::math::Vec2& pos, float sx, float sy, float rotation, const nexus::math::Vec2& uvOrigin, gfx::Color tint, const std::string& keyInstance) const
{
    Draw(pos, sx, sy, rotation, uvOrigin, tint, GetInstance(keyInstance));
}

void gl::Sprite2D::Draw(const nexus::shape2D::Rectangle& dest, const nexus::math::Vec2& origin, float rot, gfx::Color tint, const std::string& keyInstance) const
{
    Draw(dest, origin, rot, tint, GetInstance(keyInstance));
}
