#ifndef NEXUS_SR_CAMERA_3D_HPP
#define NEXUS_SR_CAMERA_3D_HPP

#include "../cmn_impl/nxCamera3D.hpp"
#include "./nxContext.hpp"

namespace nexus { namespace sr {

    class NEXUS_API Camera3D : public _gapi_impl::Camera3D
    {
      private:
        sr::Context *ctx = nullptr;

      public:
        using _gapi_impl::Camera3D::Camera3D;

        bool IsActive() const
        {
            return ctx != nullptr;
        }

        void Begin(sr::Context& ctx);
        void End();
    };

}}

#endif //NEXUS_SR_CAMERA_3D_HPP