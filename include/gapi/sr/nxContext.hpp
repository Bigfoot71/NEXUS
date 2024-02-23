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

#ifndef NEXUS_SR_CONTEXT_HPP
#define NEXUS_SR_CONTEXT_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../shape/2D/nxAABB.hpp"
#include "../cmn_impl/nxContext.hpp"
#include "../../core/nxWindow.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "../../core/nxText.hpp"
#include "../../gfx/nxPixel.hpp"
#include "./nxFramebuffer.hpp"
#include "./nxPipeline.hpp"
#include "./nxShader.hpp"
#include "./nxEnums.hpp"
#include <limits>
#include <array>

namespace nexus { namespace sr {
    class Material;     ///< Forward declaration of sr::Material
}}

namespace _sr_impl {
    class Mesh;         ///< Forward declaration of _sr_impl::Mesh
}

namespace nexus { namespace sr {

    /**
     * @brief The Context class provides a simple software rasterizer with an API similar to OpenGL 1.1.
     */
    class NEXUS_API Context : public gapi::Context //<Context>
    {
      public:
        /**
         * @brief The State struct holds the current state of the rasterizer, including matrices, viewport, and other rendering parameters.
         */
        struct State
        {
            std::array<math::Mat4, 32>  stack;                          ///< Matrix stack for push/pop
            math::Mat4                  modelview;                      ///< Default modelview matrix
            math::Mat4                  projection;                     ///< Default projection matrix
            math::Mat4                  transform;                      ///< Transform matrix to be used with rlTranslate, rlRotate, rlScale
            shape2D::Rectangle          viewport;                       ///< Viewport
            Shader                      defaultShader;                  ///< Default shader
            math::Vec3                  normal;                         ///< Current active normal
            math::Vec2                  texcoord;                       ///< Current active texture coordinate
            gfx::Color                  color;                          ///< Current active color
            const gfx::Surface          *image;                         ///< Current active image for rendering
            Framebuffer                 &winFramebuffer;                ///< Window to which the rasterizer is linked
            Framebuffer                 *currentFramebuffer;
            math::Mat4                  *currentMatrix;                 ///< Current matrix pointer
            Shader                      *currentShader;                 ///< Current shader used for rendering
            int                         stackCounter;                   ///< Matrix stack counter
            sr::MatrixMode              currentMatrixMode;              ///< Current matrix mode
            sr::DrawMode                currentDrawMode;
            bool                        transformRequired;              ///< Require transform matrix application to current draw-call vertex (if required)
            bool                        renderBeginned;                 ///< Indicates if `Begin()` has been called but not yet `End()`
            bool                        depthTesting;                   ///< Indicates if a depth test will be necessary for next vertices added to the batch
            bool                        wireMode;                       ///< Indicates whether the next rendered meshes should be rendered in wireframe

            /**
             * @brief Constructs a State object with the specified window, and dimensions.
             * @param winFramebuffer The window framebuffer to which the context is linked.
             */
            State(Framebuffer& winFramebuffer)
            : winFramebuffer(winFramebuffer)
            {
                currentFramebuffer = &winFramebuffer;
                currentShader = &defaultShader;

                // Init default vertex values
                normal = { 0, 0, 1 };
                texcoord = { 0, 0 };
                color = 0xFFFFFFFF;
                image = nullptr;

                // Init internal matrices
                transform = math::Mat4::Identity();
                projection = math::Mat4::Identity();
                modelview = math::Mat4::Identity();
                currentMatrix = &modelview;
                transformRequired = false;
                stackCounter = 0;

                // Init various bool values
                renderBeginned = false;
                depthTesting = false;
                wireMode = false;
            }
        };

      private:
        State state;
        Pipeline pipeline;

      public:
        using DrawMode = sr::DrawMode;                  ///< Used by primitives drawing template functions

      public:
        /**
         * @brief Constructs a Context object with the specified window, and dimensions.
         * @param winFramebuffer The window framebuffer to which the context is linked.
         */
        Context(Framebuffer& winFramebuffer)
        : state(winFramebuffer)
        {
            state.defaultShader.ctx = this;
        }

        /**
         * @brief Enables depth testing for subsequent rendering operations.
         */
        void EnableDepthTest() override;

        /**
         * @brief Disables depth testing for subsequent rendering operations.
         */
        void DisableDepthTest() override;

        /**
         * @brief Sets the current matrix mode for subsequent matrix operations.
         * @param mode The matrix mode to set.
         */
        void MatrixMode(sr::MatrixMode mode) override;

        /**
         * @brief Pushes the current matrix onto the matrix stack.
         */
        void PushMatrix() override;

        /**
         * @brief Pops the top matrix from the matrix stack.
         */
        void PopMatrix() override;

        /**
         * @brief Loads the identity matrix into the current matrix.
         */
        void LoadIdentity() override;

        /**
         * @brief Translates the current matrix by the specified translation vector.
         * @param x The x-component of the translation vector.
         * @param y The y-component of the translation vector.
         * @param z The z-component of the translation vector.
         */
        void Translate(float x, float y, float z) override;

        /**
         * @brief Translates the current matrix by the specified translation vector in 2D space.
         * @param x The x-component of the translation vector.
         * @param y The y-component of the translation vector.
         */
        void Translate(float x, float y) override;

        /**
         * @brief Translates the current matrix by the specified translation vector.
         * @param v The translation vector.
         */
        void Translate(const math::Vec3& v) override;

        /**
         * @brief Translates the current matrix by the specified translation vector in 2D space.
         * @param v The translation vector containing x and y components.
         */
        void Translate(const math::Vec2& v) override;

        /**
         * @brief Rotates the current matrix by the specified angle around the specified axis.
         * @param angle The rotation angle in degrees.
         * @param x The x-component of the rotation axis.
         * @param y The y-component of the rotation axis.
         * @param z The z-component of the rotation axis.
         */
        void Rotate(float angle, float x, float y, float z) override;

        /**
         * @brief Rotates the current matrix by the specified angle around the specified axis.
         * @param angle The rotation angle in degrees.
         * @param v The rotation axis.
         */
        void Rotate(float angle, const math::Vec3& v) override;

        /**
         * @brief Rotates the current matrix by the specified angle around the z-axis.
         * @param angle The rotation angle in degrees.
         */
        void Rotate(float angle) override;

        /**
         * @brief Scales the current matrix by the specified scaling factors.
         * @param x The scaling factor along the x-axis.
         * @param y The scaling factor along the y-axis.
         * @param z The scaling factor along the z-axis.
         */
        void Scale(float x, float y, float z) override;

        /**
         * @brief Scales the current matrix by the specified scaling factors in 2D space.
         * @param x The scaling factor along the x-axis.
         * @param y The scaling factor along the y-axis.
         */
        void Scale(float x, float y) override;

        /**
         * @brief Scales the current matrix by the specified scaling factors.
         * @param v The scaling factors along each axis.
         */
        void Scale(const math::Vec3& v) override;

        /**
         * @brief Scales the current matrix by the specified scaling factors in 2D space.
         * @param v The scaling factors along each axis.
         */
        void Scale(const math::Vec2& v) override;

        /**
         * @brief Multiplies the current matrix by the specified matrix.
         * @param mat The matrix to multiply with the current matrix.
         */
        void MultMatrix(const math::Mat4& mat) override;

        /**
         * @brief Sets a perspective projection frustum for the current matrix.
         * @param left The coordinate for the left vertical clipping plane.
         * @param right The coordinate for the right vertical clipping plane.
         * @param bottom The coordinate for the bottom horizontal clipping plane.
         * @param top The coordinate for the top horizontal clipping plane.
         * @param znear The distance to the near clipping plane.
         * @param zfar The distance to the far clipping plane.
         */
        void Frustum(double left, double right, double bottom, double top, double znear, double zfar) override;

        /**
         * @brief Sets an orthographic projection for the current matrix.
         * @param left The coordinate for the left vertical clipping plane.
         * @param right The coordinate for the right vertical clipping plane.
         * @param bottom The coordinate for the bottom horizontal clipping plane.
         * @param top The coordinate for the top horizontal clipping plane.
         * @param znear The distance to the near clipping plane.
         * @param zfar The distance to the far clipping plane.
         */
        void Ortho(double left, double right, double bottom, double top, double znear, double zfar) override;

        /**
         * @brief Sets the viewport for rendering and defines the projection and modelview matrices for 2D rendering within the viewport.
         * @param x The x-coordinate of the viewport.
         * @param y The y-coordinate of the viewport.
         * @param width The width of the viewport.
         * @param height The height of the viewport.
         */
        void SetViewport(int x, int y, int width, int height) override;

        /**
         * @brief Sets the viewport for rendering and defines the projection and modelview matrices for 2D rendering within the specified rectangle.
         * @param vp The rectangle defining the viewport.
         */
        void SetViewport(const shape2D::Rectangle& vp) override;

        /**
         * @brief Retrieves the current viewport rectangle.
         * @return The current viewport rectangle.
         */
        shape2D::Rectangle GetViewport() const override;

        /**
         * @brief Enables the specified framebuffer for rendering.
         * 
         * This function sets the specified framebuffer as the active framebuffer for rendering.
         * The rendered output will be directed to this framebuffer until it is disabled.
         * 
         * @param framebuffer The framebuffer to be enabled for rendering.
         */
        void EnableFramebuffer(Framebuffer& framebuffer);

        /**
         * @brief Disables the current active framebuffer and switches rendering to the default window framebuffer.
         * 
         * This function disables the currently active framebuffer, if any, and switches the rendering
         * output back to the default window framebuffer.
         */
        void DisableFramebuffer();

        /**
         * @brief Enables wireframe rendering mode for subsequent mesh drawing.
         */
        void EnableWireMode() override;

        /**
         * @brief Disables wireframe rendering mode for subsequent mesh drawing.
         */
        void DisableWireMode() override;

        /**
         * @brief Draws a batch of vertices with the specified mesh, material and transformation.
         *
         * @param mesh The mesh containing vertex information.
         * @param material The material to be used for rendering the mesh.
         * @param transform The transformation matrix to apply to the vertices.
         */
        void DrawVertexArray(const _sr_impl::Mesh& mesh, sr::Material& material, const math::Mat4& transform);

        /**
         * @brief Begins a new drawing sequence with the specified drawing mode.
         * @param mode The drawing mode (e.g., points, lines, triangles).
         */
        void Begin(DrawMode mode) override;

        /**
         * @brief Ends the current drawing sequence.
         */
        void End() override;

        /**
         * @brief Sets the texture for rendering. Subsequent draw calls will use this texture.
         * @param texture The 'texture' surface. You can provide nullptr to unset the texture.
         */
        void SetTexture(const gfx::Surface& texture);

        /**
         * @brief Sets the texture for rendering. Subsequent draw calls will use this texture.
         * @param texture The 'texture' surface pointer. You can provide nullptr to unset the texture.
         */
        void SetTexture(const gfx::Surface* texture);

        /**
         * @brief Sets the default texture, effectively unsetting the current texture.
         * @note: For this context, this will simply unset the texture.
         */
        void SetDefaultTexture() override;

        /**
         * @brief Unsets the current texture, so subsequent draw calls won't use any texture.
         */
        void UnsetTexture() override;

        /**
         * @brief Sets the shader for rendering. Subsequent draw calls will use this shader.
         * @param shader The shader.
         */
        void SetShader(sr::Shader* shader);

        /**
         * @brief Specifies a vertex for rendering.
         * @param vertex The vertex coordinates.
         */
        void Vertex(const math::Vec3& vertex) override;

        /**
         * @brief Specifies a 2D vertex for rendering.
         * @param vertex The 2D vertex coordinates.
         */
        void Vertex(const math::Vec2& vertex) override;

        /**
         * @brief Specifies a vertex for rendering.
         * @param x The x-coordinate of the vertex.
         * @param y The y-coordinate of the vertex.
         * @param z The z-coordinate of the vertex.
         */
        void Vertex(float x, float y, float z) override;

        /**
         * @brief Specifies a 2D vertex for rendering.
         * @param x The x-coordinate of the 2D vertex.
         * @param y The y-coordinate of the 2D vertex.
         */
        void Vertex(float x, float y) override;

        /**
         * @brief Specifies the normal vector for lighting calculations.
         * @param normal The normal vector.
         */
        void Normal(const math::Vec3& normal) override;

        /**
         * @brief Specifies the normal vector for lighting calculations.
         * @param x The x-component of the normal vector.
         * @param y The y-component of the normal vector.
         * @param z The z-component of the normal vector.
         */
        void Normal(float x, float y, float z) override;

        /**
         * @brief Specifies the texture coordinates for rendering.
         * @param texcoord The texture coordinates.
         */
        void TexCoord(const math::Vec2& texcoord) override;

        /**
         * @brief Specifies the texture coordinates for rendering.
         * @param u The u-coordinate of the texture.
         * @param v The v-coordinate of the texture.
         */
        void TexCoord(float u, float v) override;

        /**
         * @brief Specifies the color for rendering.
         * @param color The color to set.
         */
        void Color(const gfx::Color& color) override;

        /**
         * @brief Specifies the color for rendering using RGBA values.
         * @param color The color to set, represented as a 4D vector (RGBA).
         */
        void Color(const math::Vec4& color) override;

        /**
         * @brief Specifies the color for rendering.
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         * @param a The alpha component of the color. Default value is 255.
         */
        void Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) override;

        /**
         * @brief Specifies the color for rendering.
         * @param x The x-component of the color (normalized).
         * @param y The y-component of the color (normalized).
         * @param z The z-component of the color (normalized).
         * @param w The w-component of the color (normalized). Default value is 1.0f.
         */
        void Color(float x, float y, float z, float w = 1.0f) override;

        /**
         * @brief Retrieves a constant reference to the internal state of the sr::Context.
         *
         * This function allows access to the internal state of the sr::Context context by providing
         * a constant reference. The state is not modified by this operation, making it a
         * read-only access method.
         *
         * @return A constant reference to the internal state of the sr::Context.
         */
        const State& GetState() const
        {
            return state;
        }
    };

}}

#endif //NEXUS_SR_CONTEXT_HPP