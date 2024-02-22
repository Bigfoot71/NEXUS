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
