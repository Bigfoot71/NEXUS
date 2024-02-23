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

#include "gapi/gl/nxCamera3D.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

void gl::Camera3D::Begin(gl::Context& ctx)
{
    this->ctx = &ctx;

    ctx.DrawRenderBatchActive();      // Update and draw internal render batch

    ctx.MatrixMode(MatrixMode::Projection);             // Switch to projection matrix
    ctx.PushMatrix();                                   // Save previous matrix, which contains the settings for the 2d ortho projection
    ctx.LoadIdentity();                                 // Reset current matrix (projection)

    if (orthographic)
    {
        // Setup orthographic projection
        double top = fovy/2.0;
        double right = top*aspect;

        ctx.Ortho(-right, right, -top, top, near, far);
    }
    else
    {
        // Setup perspective projection
        double top = near*std::tan(fovy*0.5*math::Deg2Rad);
        double right = top*aspect;

        ctx.Frustum(-right, right, -top, top, near, far);
    }

    ctx.MatrixMode(MatrixMode::ModelView);              // Switch back to modelview matrix
    ctx.LoadIdentity();                                 // Reset current matrix (modelview)

    // Setup Camera view
    ctx.MultMatrix(GetViewMatrix());      // Multiply modelview matrix by view matrix (camera)

    ctx.EnableDepthTest();            // Enable depth test for 3D
}

void gl::Camera3D::End()
{
    ctx->DrawRenderBatchActive();                                // Update and draw internal render batch

    ctx->MatrixMode(MatrixMode::Projection);                     // Switch to projection matrix
    ctx->PopMatrix();                                            // Restore previous matrix (projection) from matrix stack

    ctx->MatrixMode(MatrixMode::ModelView);                      // Switch back to modelview matrix
    ctx->LoadIdentity();                                         // Reset current matrix (modelview)

    //ctx->MultMatrixf(MatrixToFloat(CORE.Window.screenScale));  // Apply screen scaling if required

    ctx->DisableDepthTest();                                     // Disable DEPTH_TEST for 2D

    ctx = nullptr;
}
