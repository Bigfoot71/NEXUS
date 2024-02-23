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

#ifndef NEXUS_EXT_2D_GL_PARTICLES_HPP
#define NEXUS_EXT_2D_GL_PARTICLES_HPP

#include "../../../gfx/cmn_ext_2D_ext_3D_impl/nxParticles.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../nxPrimitives2D.hpp"
#include "../nxContext.hpp"

namespace nexus { namespace gl {

    struct NEXUS_API Particle2D : _gfx_impl::Particle<Texture, math::Vec2>
    {
        using _gfx_impl::Particle<Texture, math::Vec2>::Particle;

        void Draw(const Texture& texture) const
        {
            gfx::Color color; float scale;
            this->GetRenderData(color, scale);
            texture->Draw(position, rotation, static_cast<math::Vec2>(texture->GetSize()) * scale * 0.5f, math::Vec2(scale), color);
        }
    };

    class NEXUS_API ParticleSystem2D : public _gfx_impl::ParticleSystem<Particle2D, Context>
    {
      public:
        using _gfx_impl::ParticleSystem<Particle2D, Context>::ParticleSystem;

        void Draw() const
        {
            for (auto& particle : particles)
            {
                particle.Draw(this->texture);
            }
        }
    };

}}

#endif //NEXUS_EXT_2D_GL_PARTICLES_HPP