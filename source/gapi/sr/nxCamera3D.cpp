#include "gapi/sr/nxCamera3D.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

void sr::Camera3D::Begin(sr::Context& ctx)
{
    this->ctx = &ctx;

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

void sr::Camera3D::End()
{
    ctx->MatrixMode(MatrixMode::Projection);                     // Switch to projection matrix
    ctx->PopMatrix();                                            // Restore previous matrix (projection) from matrix stack

    ctx->MatrixMode(MatrixMode::ModelView);                      // Switch back to modelview matrix
    ctx->LoadIdentity();                                         // Reset current matrix (modelview)

    //ctx->MultMatrixf(MatrixToFloat(CORE.Window.screenScale));  // Apply screen scaling if required

    ctx->DisableDepthTest();                                     // Disable DEPTH_TEST for 2D

    ctx = nullptr;
}
