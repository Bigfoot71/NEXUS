#ifndef NEXUS_GAPI_IMPL_CONTEXT_HPP
#define NEXUS_GAPI_IMPL_CONTEXT_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../math/nxMat4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "../../gfx/nxColor.hpp"
#include "./nxEnums.hpp"

namespace nexus { namespace gapi {

    // This pure virtual class serves as the parent for all other gapi::Context classes.
    // It contains all the minimal functions that will be mandatory, for example,
    // for the common implementation of primitive drawing, thus avoiding duplicating the code
    // among different implementations of graphic API utilities.

    //template <typename T_Derived>
    struct Context
    {
        virtual void MatrixMode(gapi::MatrixMode mode) = 0;

        virtual void PushMatrix() = 0;
        virtual void PopMatrix() = 0;

        virtual void LoadIdentity() = 0;

        virtual void Translate(float x, float y, float z) = 0;
        virtual void Translate(float x, float y) = 0;

        virtual void Translate(const nexus::math::Vec3& v) = 0;
        virtual void Translate(const nexus::math::Vec2& v) = 0;

        virtual void Rotate(float angle, float x, float y, float z) = 0;
        virtual void Rotate(float angle, const nexus::math::Vec3& v) = 0;
        virtual void Rotate(float angle) = 0;

        virtual void Scale(float x, float y, float z) = 0;
        virtual void Scale(float x, float y) = 0;

        virtual void Scale(const nexus::math::Vec3& v) = 0;
        virtual void Scale(const nexus::math::Vec2& v) = 0;

        virtual void MultMatrix(const nexus::math::Mat4& mat) = 0;

        virtual void Frustum(double left, double right, double bottom, double top, double znear, double zfar) = 0;
        virtual void Ortho(double left, double right, double bottom, double top, double znear, double zfar) = 0;

        virtual void SetViewport(int x, int y, int width, int height) = 0;
        virtual void SetViewport(const nexus::shape2D::Rectangle& vp) = 0;
        virtual nexus::shape2D::Rectangle GetViewport() const = 0;

        virtual void EnableDepthTest() = 0;
        virtual void DisableDepthTest() = 0;

        virtual void EnableWireMode() = 0;
        virtual void DisableWireMode() = 0;

        virtual void Begin(gapi::DrawMode mode) = 0;
        virtual void End() = 0;

        virtual void Vertex(const nexus::math::Vec3& vertex) = 0;
        virtual void Vertex(const nexus::math::Vec2& vertex) = 0;
        virtual void Vertex(float x, float y, float z) = 0;
        virtual void Vertex(float x, float y) = 0;

        virtual void Normal(const nexus::math::Vec3& normal) = 0;
        virtual void Normal(float x, float y, float z) = 0;

        virtual void TexCoord(const nexus::math::Vec2& texcoord) = 0;
        virtual void TexCoord(float u, float v) = 0;

        virtual void Color(const nexus::gfx::Color& color) = 0;
        virtual void Color(const nexus::math::Vec4& color) = 0;
        virtual void Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) = 0;
        virtual void Color(float x, float y, float z, float w) = 0;

        // These two functions are mainly used by primitive
        // drawing functions, it was necessary to avoid
        // templates to allow correct substitution
        // from App and Window to gapi::Context
        virtual void SetDefaultTexture() = 0;
        virtual void UnsetTexture() = 0;
    };

}}

#endif //NEXUS_GAPI_IMPL_CONTEXT_HPP
