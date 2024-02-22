#ifndef NEXUS_GL_CAMERA_2D_HPP
#define NEXUS_GL_CAMERA_2D_HPP

#include "../cmn_impl/nxCamera2D.hpp"
#include "../../math/nxMat4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxContext.hpp"

namespace nexus { namespace gl {

    class NEXUS_API Camera2D : public _gapi_impl::Camera2D
    {
      private:
        gl::Context *ctx = nullptr;

      public:
        using _gapi_impl::Camera2D::Camera2D;

        bool IsActive() const
        {
            return ctx != nullptr;
        }

        void Begin(gl::Context& ctx)
        {
            this->ctx = &ctx;

            ctx.DrawRenderBatchActive();
            ctx.LoadIdentity();
            ctx.MultMatrix(GetCameraMatrix2D());
        }

        void End()
        {
            ctx->DrawRenderBatchActive();
            ctx->LoadIdentity();
            ctx = nullptr;
        }
    };

}}

#endif //NEXUS_GL_CAMERA_2D_HPP
