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
