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
