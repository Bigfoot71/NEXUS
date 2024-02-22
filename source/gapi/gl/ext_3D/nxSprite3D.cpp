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
