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