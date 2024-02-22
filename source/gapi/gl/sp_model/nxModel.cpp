#include "gapi/gl/sp_model/nxModel.hpp"

#include "core/nxException.hpp"
#include "math/nxMat4.hpp"
#include "core/nxLog.hpp"
#include <cassert>

using namespace nexus;

/* Private Implementation Model */

void _gl_impl::Model::Draw(const math::Vec3& position, const math::Vec3& rotationAxis, float rotationAngle, const math::Vec3& scale, const gfx::Color& tint)
{
    // Calculate transformation matrix from function parameters
    // Get transform matrix (rotation -> scale -> translation)
    math::Mat4 matTransform = math::Mat4::Scale(scale.x, scale.y, scale.z)
                            * math::Mat4::Rotate(rotationAxis, rotationAngle * math::Deg2Rad)
                            * math::Mat4::Translate(position.x, position.y, position.z);

    matTransform = transform * matTransform;

    for (int i = 0; i < meshes.size(); i++)
    {
        auto& material = materials[meshMaterial[i]];
        const gfx::Color color = material->maps[Material::MapType::Diffuse].color;

        material->maps[Material::MapType::Diffuse].color = color * tint;
        meshes[i]->Draw(material, matTransform);
        material->maps[Material::MapType::Diffuse].color = color;
    }
}

void _gl_impl::Model::Draw(const math::Vec3& position, float scale, const gfx::Color& tint)
{
    Draw(position, { 0.0f, 1.0f, 0.0f }, 0.0f, { scale, scale, scale }, tint);
}

void _gl_impl::Model::DrawWires(const math::Vec3& position, float scale, const gfx::Color& tint)
{
    ctx.EnableWireMode();
        DrawWires(position, scale, tint);
    ctx.DisableWireMode();
}

void _gl_impl::Model::DrawWires(const math::Vec3& position, const math::Vec3& rotationAxis, float rotationAngle, const math::Vec3& scale, const gfx::Color& tint)
{
    ctx.EnableWireMode();
        Draw(position, rotationAxis, rotationAngle, scale, tint);
    ctx.DisableWireMode();
}
