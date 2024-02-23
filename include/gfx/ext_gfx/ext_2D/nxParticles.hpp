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

#ifndef NEXUS_EXT_2D_GFX_PARTICLES_HPP
#define NEXUS_EXT_2D_GFX_PARTICLES_HPP

#include "../../cmn_ext_2D_ext_3D_impl/nxParticles.hpp"
#include "../../nxColor.hpp"
#include "../nxRenderer.hpp"
#include "../nxTexture.hpp"

namespace nexus { namespace gfx {

    struct NEXUS_API Particle : _gfx_impl::Particle<Texture, math::Vec2>
    {
        using _gfx_impl::Particle<Texture, math::Vec2>::Particle;

        void Draw(Texture& texture)
        {
            gfx::Color color; float scale;
            this->GetRenderData(color, scale);

            texture->SetTint(color);
            texture->Draw(position, rotation, static_cast<math::Vec2>(texture->GetSize()) * scale * 0.5f, math::Vec2(scale));
        }
    };

    class NEXUS_API ParticleSystem : public _gfx_impl::ParticleSystem<Particle, Renderer>
    {
      public:
        using _gfx_impl::ParticleSystem<Particle, Renderer>::ParticleSystem;

        void Draw()
        {
            for (auto& particle : particles)
            {
                particle.Draw(this->texture);
            }
        }
    };

}}

#endif //NEXUS_EXT_2D_GFX_PARTICLES_HPP
