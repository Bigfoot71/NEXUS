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

#ifndef NEXUS_EXT_3D_GL_PARTICLES_HPP
#define NEXUS_EXT_3D_GL_PARTICLES_HPP

#include "../../../gfx/cmn_ext_2D_ext_3D_impl/nxParticles.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../nxPrimitives2D.hpp"
#include "../nxContext.hpp"

namespace nexus { namespace gl {

    struct NEXUS_API Particle3D : _gfx_impl::Particle<Texture, math::Vec3>
    {
        using _gfx_impl::Particle<Texture, math::Vec3>::Particle;

        void Draw(const Camera3D& camera, const Texture& texture) const
        {
            gfx::Color color; float scale;
            this->GetRenderData(color, scale);
            texture->DrawBillboard(camera, position, scale * texture->GetWidthF(), rotation, color);
        }
    };

    class NEXUS_API ParticleSystem3D : public _gfx_impl::ParticleSystem<Particle3D, Context>
    {
      public:
        using _gfx_impl::ParticleSystem<Particle3D, Context>::ParticleSystem;

        void Draw(const Camera3D& camera) const
        {
            for (auto& particle : particles)
            {
                particle.Draw(camera, this->texture);
            }
        }
    };

}}

#endif //NEXUS_EXT_3D_GL_PARTICLES_HPP
