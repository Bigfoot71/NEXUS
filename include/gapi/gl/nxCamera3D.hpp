#ifndef NEXUS_GL_CAMERA_3D_HPP
#define NEXUS_GL_CAMERA_3D_HPP

#include "../cmn_impl/nxCamera3D.hpp"
#include "./nxContext.hpp"

namespace nexus { namespace gl {

    class NEXUS_API Camera3D : public _gapi_impl::Camera3D
    {
      private:
        gl::Context *ctx = nullptr;

      public:
        using _gapi_impl::Camera3D::Camera3D;

        bool IsActive() const
        {
            return ctx != nullptr;
        }

        void Begin(gl::Context& ctx);
        void End();
    };

}}

#endif //NEXUS_GL_CAMERA_3D_HPP
