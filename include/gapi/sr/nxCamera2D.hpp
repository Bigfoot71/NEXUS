#ifndef NEXUS_SR_CAMERA_2D_HPP
#define NEXUS_SR_CAMERA_2D_HPP

#include "../cmn_impl/nxCamera2D.hpp"
#include "../../math/nxMat4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxContext.hpp"

namespace nexus { namespace sr {

    class NEXUS_API Camera2D : public _gapi_impl::Camera2D
    {
      private:
        sr::Context *ctx = nullptr;

      public:
        using _gapi_impl::Camera2D::Camera2D;

        bool IsActive() const
        {
            return ctx != nullptr;
        }

        void Begin(sr::Context& ctx)
        {
            this->ctx = &ctx;
            ctx.LoadIdentity();
            ctx.MultMatrix(GetCameraMatrix2D());
        }

        void End()
        {
            ctx->LoadIdentity();
            ctx = nullptr;
        }
    };

}}

#endif //NEXUS_SR_CAMERA_2D_HPP
