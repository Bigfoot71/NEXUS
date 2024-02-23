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
