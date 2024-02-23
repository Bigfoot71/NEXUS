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

#ifndef NEXUS_CORE_GL_CONTEXT_HPP
#define NEXUS_CORE_GL_CONTEXT_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../cmn_impl/nxContext.hpp"
#include "../../math/nxMat4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "../../gfx/nxColor.hpp"
#include "./nxRenderBatch.hpp"
#include "./nxEnums.hpp"

#include <SDL_video.h>
#include <cstddef>
#include <memory>

namespace _gl_impl {

    /**
     * @brief This class enables shared management of SDL_GLContext.
     */
    class CXX_SDL_GLContext
    {
      protected:
        std::shared_ptr<SDL_GLContext> glCtx = nullptr;   ///< We use a shared_ptr so that we can share OpenGL contexts between multiple gl::Context.

      protected:
        /**
        * @brief Default constructor.
        */
        CXX_SDL_GLContext() = default;

        /**
        * @brief Constructor.
        * @param window Pointer to the SDL window.
        * @param _glCtx Shared pointer to an existing SDL_GLContext.
        */
        CXX_SDL_GLContext(SDL_Window* window, const std::shared_ptr<SDL_GLContext>& _glCtx = nullptr);

        /**
        * @brief Destructor.
        */
        ~CXX_SDL_GLContext();
    };

}

namespace nexus { namespace gl {

    class Window; ///< Forward declaration of gl::Window

    /**
     * @struct Attributes
     * @brief Configuration structure for OpenGL context.
     *
     * This structure encapsulates various attributes and settings for configuring an
     * OpenGL context. Use it to set specific parameters before creating an OpenGL context.
     */
    struct NEXUS_API Attributes
    {
        int redSize                             = 8;        ///< The size of the red color buffer in bits.
        int greenSize                           = 8;        ///< The size of the green color buffer in bits.
        int blueSize                            = 8;        ///< The size of the blue color buffer in bits.
        int alphaSize                           = 8;        ///< The size of the alpha channel in bits.
        int bufferSize                          = 24;       ///< Total color buffer size (sum of red, green, blue, and alpha sizes).
        bool doubleBuffer                       = true;     ///< Enable double buffering.
        int depthSize                           = 24;       ///< The size of the depth buffer in bits.
        int stencilSize                         = 8;        ///< The size of the stencil buffer in bits.
        int accumRedSize                        = 0;        ///< The size of the red accumulation buffer in bits.
        int accumGreenSize                      = 0;        ///< The size of the green accumulation buffer in bits.
        int accumBlueSize                       = 0;        ///< The size of the blue accumulation buffer in bits.
        int accumAlphaSize                      = 0;        ///< The size of the alpha accumulation buffer in bits.
        bool stereo                             = false;    ///< Enable stereo rendering.
        int multiSampleBuffers                  = 0;        ///< The number of multi-sample buffers.
        int multiSampleSamples                  = 0;        ///< The number of samples per multi-sample buffer.
        bool acceleratedVisual                  = true;     ///< Request an accelerated visual.
        bool retainedBacking                    = false;    ///< Retain the framebuffer after swapping.
        int contextFlags                        = 0;        ///< Context creation flags.
        int contextProfileMask                  = 0;        ///< Context profile mask.
        SDL_Window* shareWithCurrentContext     = nullptr;  ///< Share resources with another context.
        bool frameBufferSRGBCapable             = false;    ///< Indicates if the framebuffer is sRGB-capable.
        int contextReleaseBehavior              = 0;        ///< Context release behavior.
        int contextResetNotification            = 0;        ///< Context reset notification strategy.
        bool contextNoError                     = false;    ///< Enable context creation without generating errors.
        bool floatBuffers                       = false;    ///< Use floating-point framebuffers.

        Attributes() = default;

        /**
         * @brief Set the OpenGL context attributes.
         *
         * This function sets the OpenGL context attributes based on the provided configuration.
         * It should be called before creating the OpenGL context.
         */
        void Apply() const;
    };

    /**
     * @class Context
     * @brief Class containing an OpenGL context with an RLGL context.
     *
     * The Context class represents an OpenGL context with an associated internal State.
     *
     * @note: The gl::Context class consists of an OpenGL context, which can be shared, and a global
     *       state that is unique to each gl::Context created and cannot be shared. When a gl::Context is
     *       instantiated, it will always retain the same OpenGL context, as its internal state depends on it.
     */
    class NEXUS_API Context : public gapi::Context/*<Context>*/, public _gl_impl::CXX_SDL_GLContext
    {
      public:
        struct State
        {
            math::Vec2 texcoord;                                                ///< Current active texture coordinate (added on glVertex*())
            math::Vec3 normal;                                                  ///< Current active normal (added on glVertex*())
            gfx::Color color;                                                   ///< Current active color (added on glVertex*())

            gl::MatrixMode currentMatrixMode;                                   ///< Current matrix mode
            math::Mat4 *currentMatrix;                                          ///< Current matrix pointer
            math::Mat4 modelview;                                               ///< Default modelview matrix
            math::Mat4 projection;                                              ///< Default projection matrix
            math::Mat4 transform;                                               ///< Transform matrix to be used with rlTranslate, rlRotate, rlScale
            bool transformRequired;                                             ///< Require transform matrix application to current draw-call vertex (if required)
            math::Mat4 stack[GL_MAX_MATRIX_STACK_SIZE];                         ///< Matrix stack for push/pop
            int stackCounter;                                                   ///< Matrix stack counter

            Uint32 defaultTextureId;                                            ///< Default texture used on shapes/poly drawing (required by shader)
            Uint32 activeTextureId[GL_DEFAULT_BATCH_MAX_TEXTURE_UNITS];         ///< Active texture ids to be enabled on batch drawing (0 active by default)
            Uint32 defaultVShaderId;                                            ///< Default vertex shader id (used by default shader program)
            Uint32 defaultFShaderId;                                            ///< Default fragment shader id (used by default shader program)
            Uint32 defaultShaderId;                                             ///< Default shader program id, supports vertex color and diffuse texture
            int *defaultShaderLocs;                                             ///< Default shader locations pointer to be used on rendering
            Uint32 currentShaderId;                                             ///< Current shader id to be used on rendering (by default, defaultShaderId)
            const int *currentShaderLocs;                                       ///< Current shader locations pointer to be used on rendering (by default, defaultShaderLocs)

            bool stereoRender;                                                  ///< Stereo rendering flag
            math::Mat4 projectionStereo[2];                                     ///< VR stereo rendering eyes projection matrices
            math::Mat4 viewOffsetStereo[2];                                     ///< VR stereo rendering eyes view offset matrices

            // Blending variables
            BlendMode currentBlendMode;                                         ///< Blending mode active
            int glBlendSrcFactor;                                               ///< Blending source factor
            int glBlendDstFactor;                                               ///< Blending destination factor
            int glBlendEquation;                                                ///< Blending equation
            int glBlendSrcFactorRGB;                                            ///< Blending source RGB factor
            int glBlendDestFactorRGB;                                           ///< Blending destination RGB factor
            int glBlendSrcFactorAlpha;                                          ///< Blending source alpha factor
            int glBlendDestFactorAlpha;                                         ///< Blending destination alpha factor
            int glBlendEquationRGB;                                             ///< Blending equation for RGB
            int glBlendEquationAlpha;                                           ///< Blending equation for alpha
            bool glCustomBlendModeModified;                                     ///< Custom blending factor and equation modification status

            int framebufferWidth;                                               ///< Current framebuffer width
            int framebufferHeight;                                              ///< Current framebuffer height
        };

      private:
        State state;                                    ///< Renderer state
#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
        std::unique_ptr<RenderBatch> defaultBatch;      ///< Default internal render batch
        RenderBatch *currentBatch;                      ///< Pointer to the current render batch
#   endif

      public:
        /**
         * @brief Default constructor for Context.
         *
         * This constructor initializes Context with default SDL and RLGL contexts.
         */
        Context() : CXX_SDL_GLContext() { }

        /**
         * @brief Constructor for Context with an existing SDL window and optional context inheritance.
         *
         * This constructor creates a Context associated with an existing SDL window. Optionally, it can inherit
         * the OpenGL context from another Context. It may throw core::NexusException if the OpenGL context cannot
         * be created.
         *
         * @param window The gl::Window instance associated with the Context.
         * @param other An optional Context to inherit the OpenGL context from.
         */
        Context(Window& window, Context* other = nullptr);

        // Destructor
        ~Context();

        // Deleted copy constructor and assignment operator to prevent unintended copying
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        /**
         * @brief Conversion operator to SDL_GLContext.
         *
         * This conversion operator allows Context to be used as an SDL_GLContext.
         *
         * @return The SDL_GLContext associated with this Context.
         */
        operator SDL_GLContext() { return *glCtx; }

        /**
         * @brief Conversion operator to a shared pointer of SDL_GLContext.
         *
         * This conversion operator allows Context to be used as a shared pointer to SDL_GLContext.
         *
         * @return A shared pointer to the SDL_GLContext associated with this Context.
         */
        operator const std::shared_ptr<SDL_GLContext>&() { return glCtx; }

        /**
         * @brief Check if this Context is the current OpenGL rendering context.
         *
         * This function checks if the OpenGL context associated with this Context is the currently active rendering context.
         *
         * @return True if this Context is the current OpenGL rendering context, false otherwise.
         */
        bool IsCurrentContext() const
        {
            return glCtx.get() == SDL_GL_GetCurrentContext();
        }

        /**
         * @brief Choose the current matrix to be transformed.
         *
         * This function selects the current matrix to be transformed. It can be one of the matrix modes
         * defined in the `MatrixMode` enumeration.
         *
         * @param mode The matrix mode to set.
         */
        void MatrixMode(gl::MatrixMode mode) override;

        /**
         * @brief Push the current matrix to the stack.
         *
         * This function pushes the current matrix to a stack, allowing you to save and restore matrix states.
         */
        void PushMatrix() override;

        /**
         * @brief Pop the latest inserted matrix from the stack.
         *
         * This function pops the latest inserted matrix from the stack, restoring the previous matrix state.
         */
        void PopMatrix() override;

        /**
         * @brief Reset the current matrix to an identity matrix.
         *
         * This function sets the current matrix to an identity matrix, effectively resetting it to its default state.
         */
        void LoadIdentity() override;

        /**
         * @brief Multiply the current matrix by a translation matrix.
         *
         * This function multiplies the current matrix by a translation matrix, effectively translating
         * the current matrix's transformation.
         *
         * @param x The translation in the X-axis.
         * @param y The translation in the Y-axis.
         * @param z The translation in the Z-axis.
         */
        void Translate(float x, float y, float z) override;

        /**
         * @brief Multiply the current matrix by a translation matrix.
         *
         * This function multiplies the current matrix by a translation matrix, effectively translating
         * the current matrix's transformation. This version of the function is used for 2D translation,
         * with the Z-axis translation set to zero.
         *
         * @param x The translation in the X-axis.
         * @param y The translation in the Y-axis.
         */
        void Translate(float x, float y) override
        {
            this->Translate(x, y, 0.0f);
        }

        /**
         * @brief Multiply the current matrix by a translation matrix using a 3D vector.
         *
         * This function multiplies the current matrix by a translation matrix based on the values of
         * a 3D vector, effectively translating the current matrix's transformation.
         *
         * @param v The translation vector containing x, y, and z components.
         */
        void Translate(const math::Vec3& v) override
        {
            this->Translate(v.x, v.y, v.z);
        }

        /**
         * @brief Multiply the current matrix by a translation matrix using a 2D vector.
         *
         * This function multiplies the current matrix by a translation matrix based on the values of
         * a 2D vector, effectively translating the current matrix's transformation.
         *
         * @param v The translation vector containing x and y components.
         */
        void Translate(const math::Vec2& v) override
        {
            this->Translate(v.x, v.y, 0.0f);
        }

        /**
         * @brief Multiply the current matrix by a rotation matrix.
         *
         * This function multiplies the current matrix by a rotation matrix, effectively rotating the current
         * matrix's transformation by the specified angle and axis.
         *
         * @param angle The rotation angle in degrees.
         * @param x The X-component of the rotation axis.
         * @param y The Y-component of the rotation axis.
         * @param z The Z-component of the rotation axis.
         */
        void Rotate(float angle, float x, float y, float z) override;

        /**
         * @brief Multiply the current matrix by a rotation matrix using a 3D vector.
         *
         * This function multiplies the current matrix by a rotation matrix based on the values of
         * a 3D vector, effectively rotating the current matrix's transformation by the specified angle and axis.
         *
         * @param angle The rotation angle in degrees.
         * @param v The rotation axis vector containing x, y, and z components.
         */
        void Rotate(float angle, const math::Vec3& v) override
        {
            this->Rotate(angle, v.x, v.y, v.z);
        }

        /**
         * @brief Multiply the current matrix by a rotation matrix around the Z-axis.
         *
         * This function multiplies the current matrix by a rotation matrix around the Z-axis,
         * effectively rotating the current matrix's transformation by the specified angle.
         *
         * @param angle The rotation angle in degrees.
         */
        void Rotate(float angle) override
        {
            this->Rotate(angle, 0.0f, 0.0f, 1.0f);
        }

        /**
         * @brief Multiply the current matrix by a scaling matrix.
         *
         * This function multiplies the current matrix by a scaling matrix, effectively scaling the current matrix's transformation.
         *
         * @param x The scaling factor in the X-axis.
         * @param y The scaling factor in the Y-axis.
         * @param z The scaling factor in the Z-axis.
         */
        void Scale(float x, float y, float z) override;

        /**
         * @brief Multiply the current matrix by a scaling matrix.
         *
         * This function multiplies the current matrix by a scaling matrix, effectively scaling
         * the current matrix's transformation. This version of the function is used for 2D scaling,
         * with the Z-axis scaling factor set to 1.0.
         *
         * @param x The scaling factor in the X-axis.
         * @param y The scaling factor in the Y-axis.
         */
        void Scale(float x, float y) override
        {
            this->Scale(x, y, 1.0f);
        }

        /**
         * @brief Multiply the current matrix by a scaling matrix using a 3D vector.
         *
         * This function multiplies the current matrix by a scaling matrix based on the values of
         * a 3D vector, effectively scaling the current matrix's transformation.
         *
         * @param v The scaling vector containing x, y, and z components.
         */
        void Scale(const math::Vec3& v) override
        {
            this->Scale(v.x, v.y, v.z);
        }

        /**
         * @brief Multiply the current matrix by a scaling matrix using a 2D vector.
         *
         * This function multiplies the current matrix by a scaling matrix based on the values of
         * a 2D vector, effectively scaling the current matrix's transformation.
         *
         * @param v The scaling vector containing x and y components.
         */
        void Scale(const math::Vec2& v) override
        {
            this->Scale(v.x, v.y, 1.0f);
        }

        /**
         * @brief Multiply the current matrix by another matrix.
         *
         * This function multiplies the current matrix by another matrix provided as an array of float values.
         *
         * @param matf A pointer to the float array representing the matrix to multiply with the current matrix.
         */
        void MultMatrix(const math::Mat4& mat) override;

        /**
         * @brief Set up a frustum perspective projection.
         *
         * This function sets up a perspective projection using a frustum defined by the provided parameters.
         *
         * @param left The left clipping plane coordinate.
         * @param right The right clipping plane coordinate.
         * @param bottom The bottom clipping plane coordinate.
         * @param top The top clipping plane coordinate.
         * @param znear The near clipping plane distance.
         * @param zfar The far clipping plane distance.
         */
        void Frustum(double left, double right, double bottom, double top, double znear, double zfar) override;

        /**
         * @brief Set up an orthographic projection.
         *
         * This function sets up an orthographic projection using the provided parameters.
         *
         * @param left The left clipping plane coordinate.
         * @param right The right clipping plane coordinate.
         * @param bottom The bottom clipping plane coordinate.
         * @param top The top clipping plane coordinate.
         * @param znear The near clipping plane distance.
         * @param zfar The far clipping plane distance.
         */
        void Ortho(double left, double right, double bottom, double top, double znear, double zfar) override;

        /**
         * @brief Set the viewport area.
         *
         * This function sets the viewport area to the specified dimensions and position.
         * @note: Just makes a call to glViewport without modifying the matrices.
         *
         * @param x The X-coordinate of the viewport area.
         * @param y The Y-coordinate of the viewport area.
         * @param width The width of the viewport area.
         * @param height The height of the viewport area.
         */
        void Viewport(int x, int y, int width, int height) const
        {
            glViewport(x, y, width, height);
        }

        /**
         * @brief Set the viewport area using a Rectangle object.
         *
         * This function sets the viewport area based on the dimensions and position
         * specified in the provided Rectangle object.
         * @note: Just makes a call to glViewport without modifying the matrices.
         *
         * @param vp The Rectangle object representing the viewport area.
         */
        void Viewport(const shape2D::Rectangle& vp) const
        {
            glViewport(vp.x, vp.y, vp.w, vp.h);
        }

        /**
         * @brief Set the viewport area and configure projection and modelview matrices.
         *
         * This function sets the viewport area to the specified dimensions and position,
         * and configures the projection and modelview matrices accordingly.
         *
         * @param x The X-coordinate of the viewport area.
         * @param y The Y-coordinate of the viewport area.
         * @param width The width of the viewport area.
         * @param height The height of the viewport area.
         */
        void SetViewport(int x, int y, int width, int height) override;

        /**
         * @brief Set the viewport area using a Rectangle object and configure matrices.
         *
         * This function sets the viewport area based on the dimensions and position
         * specified in the provided Rectangle object and configures the projection
         * and modelview matrices accordingly.
         *
         * @param vp A Rectangle object representing the viewport area.
         */
        void SetViewport(const shape2D::Rectangle& vp) override
        {
            SetViewport(vp.x, vp.y, vp.w, vp.h);
        }

        /**
         * @brief Get the current viewport area.
         *
         * This function retrieves the current dimensions and position of the viewport area.
         *
         * @return A shape2D::Rectangle representing the viewport area, where:
         *         - x: The X-coordinate of the viewport area.
         *         - y: The Y-coordinate of the viewport area.
         *         - width: The width of the viewport area.
         *         - height: The height of the viewport area.
         */
        shape2D::Rectangle GetViewport() const override;

        //------------------------------------------------------------------------------------
        // Functions Declaration - Vertex level operations
        //------------------------------------------------------------------------------------

        /**
         * @brief Initialize drawing mode (how to organize vertex).
         *
         * This function initializes the drawing mode, specifying how vertices should be organized.
         *
         * @param mode The drawing mode to set.
         */
        void Begin(DrawMode mode) override;

        /**
         * @brief Finish providing vertices.
         *
         * This function marks the end of vertex providing, finalizing the vertex data for rendering.
         */
        void End() override;

        /**
         * @brief Define one vertex with 3D position coordinates.
         *
         * @note: Vertex position data is the basic information required for drawing.
         *
         * @param x The X-coordinate of the vertex position.
         * @param y The Y-coordinate of the vertex position.
         * @param z The Z-coordinate of the vertex position.
         */
        void Vertex(float x, float y, float z) override;

        /**
         * @brief Define one vertex with 2D position coordinates.
         *
         * @note: Vertex position data is the basic information required for drawing.
         *
         * @param x The X-coordinate of the vertex position.
         * @param y The Y-coordinate of the vertex position.
         */
        void Vertex(float x, float y) override;

        /**
         * @brief Define one vertex with 3D position coordinates using a 3D vector.
         *
         * @note: Vertex position data is the basic information required for drawing.
         *
         * @param vertex The 3D vector representing the vertex position.
         */
        void Vertex(const math::Vec3& vertex) override;

        /**
         * @brief Define one vertex with 2D position coordinates using a 2D vector.
         *
         * @note: Vertex position data is the basic information required for drawing.
         *
         * @param vertex The 2D vector representing the vertex position.
         */
        void Vertex(const math::Vec2& vertex) override;

        /**
         * @brief Define one vertex with 2D texture coordinates.
         *
         * @note: Texture coordinates are limited to QUADS only.
         *
         * @param x The X-coordinate of the texture coordinate.
         * @param y The Y-coordinate of the texture coordinate.
         */
        void TexCoord(float x, float y) override;

        /**
         * @brief Define one vertex with 2D texture coordinates using a 2D vector.
         *
         * @note: Texture coordinates are limited to QUADS only.
         *
         * @param texcoord The 2D vector representing the texture coordinate.
         */
        void TexCoord(const math::Vec2& texcoord) override;

        /**
         * @brief Define one vertex with 3D normal coordinates.
         *
         * @note: Normals are limited to TRIANGLES only.
         *
         * @param x The X-coordinate of the normal.
         * @param y The Y-coordinate of the normal.
         * @param z The Z-coordinate of the normal.
         */
        void Normal(float x, float y, float z) override;

        /**
         * @brief Define one vertex with 3D normal coordinates using a 3D vector.
         *
         * @note: Normals are limited to TRIANGLES only.
         *
         * @param normal The 3D vector representing the normal.
         */
        void Normal(const math::Vec3& normal) override;

        /**
         * @brief Define the color of the vertex using RGBA values.
         *
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         * @param a The alpha (transparency) component of the color (default is 255).
         */
        void Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) override;

        /**
         * @brief Define the color of the vertex using normalized RGBA values (0.0f to 1.0f).
         *
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         * @param a The alpha (transparency) component of the color (default is 1.0f).
         */
        void Color(float r, float g, float b, float a = 1.0f) override;

        /**
         * @brief Define the color of the vertex using a Color structure.
         *
         * @param color The color information encapsulated in a gfx::Color structure.
         */
        void Color(const gfx::Color& color) override;

        /**
         * @brief Define the color of the vertex using a 4D vector representing RGBA values.
         *
         * @param color The 4D vector representing the RGBA values of the color.
         */
        void Color(const math::Vec4& color) override;

        //------------------------------------------------------------------------------------
        // Functions Declaration - OpenGL style functions (common to 1.1, 3.3+, ES2)
        //------------------------------------------------------------------------------------

        /* Vertex buffers state */

        /**
         * @brief Enable a vertex array (VAO) if supported.
         *
         * This function enables a vertex array object (VAO) if it is supported by the underlying graphics system.
         *
         * @param vaoId The ID of the vertex array to enable.
         * @return True if the vertex array is successfully enabled; otherwise, false.
         */
        bool EnableVertexArray(Uint32 vaoId);

        /**
         * @brief Disable the currently enabled vertex array (VAO).
         *
         * This function disables the currently enabled vertex array object (VAO), if any.
         */
        void DisableVertexArray();

        /**
         * @brief Enable a vertex buffer (VBO).
         *
         * This function enables a vertex buffer object (VBO) with the specified ID.
         *
         * @param id The ID of the vertex buffer to enable.
         */
        void EnableVertexBuffer(Uint32 id);

        /**
         * @brief Disable the currently enabled vertex buffer (VBO).
         *
         * This function disables the currently enabled vertex buffer object (VBO).
         */
        void DisableVertexBuffer();

        /**
         * @brief Enable a vertex buffer element (VBO element).
         *
         * This function enables a specific element of a vertex buffer object (VBO) with the specified ID.
         *
         * @param id The ID of the vertex buffer element to enable.
         */
        void EnableVertexBufferElement(Uint32 id);

        /**
         * @brief Disable the currently enabled vertex buffer element (VBO element).
         *
         * This function disables the currently enabled vertex buffer element (VBO element).
         */
        void DisableVertexBufferElement();

        /**
         * @brief Enable a vertex attribute index.
         *
         * This function enables a specific vertex attribute index for rendering.
         *
         * @param index The index of the vertex attribute to enable.
         */
        void EnableVertexAttribute(Uint32 index);

        /**
         * @brief Disable a vertex attribute index.
         *
         * This function disables a specific vertex attribute index for rendering.
         *
         * @param index The index of the vertex attribute to disable.
         */
        void DisableVertexAttribute(Uint32 index);

#       if defined(GRAPHICS_API_OPENGL_11)

        /**
         * @brief Enable an attribute state pointer for rendering.
         *
         * This function enables an attribute state pointer for rendering.
         *
         * @param vertexAttribType The type of the vertex attribute.
         * @param buffer A pointer to the attribute data buffer.
         */
        void EnableStatePointer(int vertexAttribType, void *buffer);

        /**
         * @brief Disable an attribute state pointer for rendering.
         *
         * This function disables an attribute state pointer for rendering.
         *
         * @param vertexAttribType The type of the vertex attribute.
         */
        void DisableStatePointer(int vertexAttribType);

#       endif

        /* Textures state */

        /**
         * @brief Select and activate a texture slot for rendering.
         *
         * This function selects and activates a specific texture slot for rendering.
         *
         * @param slot The texture slot to activate.
         */
        void ActiveTextureSlot(int slot);

        /**
         * @brief Enable a specific texture for rendering.
         *
         * This function enables a specific texture for rendering.
         *
         * @param id The ID of the texture to enable.
         */
        void EnableTexture(Uint32 id);

        /**
         * @brief Disable the currently enabled texture.
         *
         * This function disables the currently enabled texture.
         */
        void DisableTexture();

        /**
         * @brief Enable a specific cubemap texture for rendering.
         *
         * This function enables a specific cubemap texture for rendering.
         *
         * @param id The ID of the cubemap texture to enable.
         */
        void EnableTextureCubemap(Uint32 id);

        /**
         * @brief Disable the currently enabled cubemap texture.
         *
         * This function disables the currently enabled cubemap texture.
         */
        void DisableTextureCubemap();

        /**
         * @brief Set texture parameters for a specific texture (wrap mode).
         *
         * This function sets the texture parameters for a specific texture, including the wrap mode.
         *
         * @param id The ID of the texture.
         * @param param The texture parameter to set.
         * @param wrap The texture wrap mode to set.
         */
        void TextureParameters(Uint32 id, TextureParam param, TextureWrap wrap);

        /**
         * @brief Set texture parameters for a specific texture (filter mode).
         *
         * This function sets the texture parameters for a specific texture, including the filter mode.
         *
         * @param id The ID of the texture.
         * @param param The texture parameter to set.
         * @param filter The texture filter mode to set.
         */
        void TextureParameters(Uint32 id, TextureParam param, TextureFilter filter);

        /**
         * @brief Set texture parameters for a specific texture.
         *
         * This function sets various texture parameters for a specific texture.
         *
         * @param id The ID of the texture.
         * @param param The texture parameter to set.
         * @param value The value to set for the parameter.
         */
        void TextureParameters(Uint32 id, TextureParam param, float value);

        /**
         * @brief Set cubemap parameters for a specific cubemap texture (wrap mode).
         *
         * This function sets the cubemap parameters for a specific cubemap texture, including the wrap mode.
         *
         * @param id The ID of the cubemap texture.
         * @param param The cubemap parameter to set.
         * @param wrap The cubemap wrap mode to set.
         */
        void CubemapParameters(Uint32 id, TextureParam param, TextureWrap wrap);

        /**
         * @brief Set cubemap parameters for a specific cubemap texture (filter mode).
         *
         * This function sets the cubemap parameters for a specific cubemap texture, including the filter mode.
         *
         * @param id The ID of the cubemap texture.
         * @param param The cubemap parameter to set.
         * @param filter The cubemap filter mode to set.
         */
        void CubemapParameters(Uint32 id, TextureParam param, TextureFilter filter);

        /**
         * @brief Set cubemap parameters for a specific cubemap texture.
         *
         * This function sets various parameters for a specific cubemap texture.
         *
         * @param id The ID of the cubemap texture.
         * @param param The cubemap parameter to set.
         * @param value The value to set for the parameter.
         */
        void CubemapParameters(Uint32 id, TextureParam param, float value);

        /* Shader state */

        /**
         * @brief Enable a shader program for rendering.
         *
         * This function enables a shader program for rendering.
         *
         * @param id The ID of the shader program to enable.
         */
        void EnableShader(Uint32 id);

        /**
         * @brief Disable the currently enabled shader program.
         *
         * This function disables the currently enabled shader program.
         */
        void DisableShader();

        /* Framebuffer state */

        /**
         * @brief Enable a render texture (FBO) for rendering.
         *
         * This function enables a specific render texture (framebuffer object or FBO) for rendering.
         *
         * @param id The ID of the render texture (FBO) to enable.
         */
        void EnableFramebuffer(Uint32 id);

        /**
         * @brief Disable the currently enabled render texture (FBO).
         *
         * This function disables the currently enabled render texture (FBO) and returns to the default framebuffer.
         */
        void DisableFramebuffer();

        /**
         * @brief Activate multiple draw color buffers for rendering.
         *
         * This function activates multiple draw color buffers for rendering.
         *
         * @param count The number of color buffers to activate.
         */
        void ActiveDrawBuffers(int count);

        /**
         * @brief Blit the active framebuffer to the main framebuffer.
         *
         * This function blits (copies) the active framebuffer to the main framebuffer.
         *
         * @param srcX The X-coordinate of the source region to blit.
         * @param srcY The Y-coordinate of the source region to blit.
         * @param srcWidth The width of the source region to blit.
         * @param srcHeight The height of the source region to blit.
         * @param dstX The X-coordinate of the destination region to blit.
         * @param dstY The Y-coordinate of the destination region to blit.
         * @param dstWidth The width of the destination region to blit.
         * @param dstHeight The height of the destination region to blit.
         * @param bufferMask The buffer mask specifying the buffers to blit.
         */
        void BlitFramebuffer(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, int bufferMask);

        /**
         * @brief Blit a specified region from the active framebuffer to another region in the main framebuffer.
         *
         * This overloaded function blits (copies) a specified region from the active framebuffer to another region in the main framebuffer.
         *
         * @param src The Rectangle representing the source region to blit.
         * @param dst The Rectangle representing the destination region to blit.
         * @param bufferMask The buffer mask specifying the buffers to blit.
         */
        void BlitFramebuffer(const shape2D::Rectangle& src, const shape2D::Rectangle& dst, int bufferMask);

        /* General render state */

        /**
         * @brief Enable color blending for rendering.
         *
         * This function enables color blending for rendering.
         */
        void EnableColorBlend();

        /**
         * @brief Disable color blending for rendering.
         *
         * This function disables color blending for rendering.
         */
        void DisableColorBlend();

        /**
         * @brief Enable depth test for rendering.
         *
         * This function enables depth testing for rendering.
         */
        void EnableDepthTest() override;

        /**
         * @brief Disable depth test for rendering.
         *
         * This function disables depth testing for rendering.
         */
        void DisableDepthTest() override;

        /**
         * @brief Enable depth write for rendering.
         *
         * This function enables writing to the depth buffer.
         */
        void EnableDepthMask();

        /**
         * @brief Disable depth write for rendering.
         *
         * This function disables writing to the depth buffer.
         */
        void DisableDepthMask();

        /**
         * @brief Enable backface culling for rendering.
         *
         * This function enables backface culling for rendering.
         */
        void EnableBackfaceCulling();

        /**
         * @brief Disable backface culling for rendering.
         *
         * This function disables backface culling for rendering.
         */
        void DisableBackfaceCulling();

        /**
         * @brief Set the face culling mode.
         *
         * This function sets the face culling mode for rendering.
         *
         * @param mode The face culling mode to set.
         */
        void SetCullFace(CullMode mode);

        /**
         * @brief Enable scissor test for rendering.
         *
         * This function enables scissor testing for rendering.
         */
        void EnableScissorTest();

        /**
         * @brief Disable scissor test for rendering.
         *
         * This function disables scissor testing for rendering.
         */
        void DisableScissorTest();

        /**
         * @brief Set the scissor test region for rendering.
         *
         * This function sets the scissor test region for rendering.
         *
         * @param x The X-coordinate of the scissor region.
         * @param y The Y-coordinate of the scissor region.
         * @param width The width of the scissor region.
         * @param height The height of the scissor region.
         */
        void Scissor(int x, int y, int width, int height);

        /**
         * @brief Enable wireframe rendering mode.
         *
         * This function enables wireframe rendering mode.
         */
        void EnableWireMode() override;

        /**
         * @brief Enable point rendering mode.
         *
         * This function enables point rendering mode.
         */
        void EnablePointMode();

        /**
         * @brief Disable wireframe and point rendering modes.
         *
         * This function disables wireframe and point rendering modes.
         */
        void DisableWireMode() override;

        /**
         * @brief Set the line drawing width.
         *
         * This function sets the width for line drawing.
         *
         * @param width The width of lines to be drawn.
         */
        void SetLineWidth(float width);

        /**
         * @brief Get the line drawing width.
         *
         * This function retrieves the current line drawing width.
         *
         * @return The current line drawing width.
         */
        float GetLineWidth() const;

        /**
         * @brief Enable anti-aliasing for lines.
         *
         * This function enables anti-aliasing for line rendering.
         */
        void EnableSmoothLines();

        /**
         * @brief Disable anti-aliasing for lines.
         *
         * This function disables anti-aliasing for line rendering.
         */
        void DisableSmoothLines();

        /**
         * @brief Enable stereo rendering mode.
         *
         * This function enables stereo rendering mode.
         */
        void EnableStereoRender();

        /**
         * @brief Disable stereo rendering mode.
         *
         * This function disables stereo rendering mode.
         */
        void DisableStereoRender();

        /**
         * @brief Check if stereo rendering mode is enabled.
         *
         * This function checks if stereo rendering mode is enabled.
         *
         * @return true if stereo render is enabled; false otherwise.
         */
        bool IsStereoRenderEnabled();

        /**
         * @brief Clear the color buffer with a specified color.
         *
         * This function clears the color buffer with the specified color.
         *
         * @param r The red channel value (0-255).
         * @param g The green channel value (0-255).
         * @param b The blue channel value (0-255).
         * @param a The alpha channel value (0-255).
         */
        void ClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

        /**
         * @brief Clear the color buffer with a specified color using a Color structure.
         *
         * This function clears the color buffer with the specified color.
         *
         * @param color The color information encapsulated in a gfx::Color structure.
         */
        void ClearColor(const gfx::Color& color);

        /**
         * @brief Clear the color buffer with a specified color using a 4D vector.
         *
         * This function clears the color buffer with the specified color.
         *
         * @param color The 4D vector representing the normalized RGBA values of the color.
         */
        void ClearColor(const math::Vec4& color);

        /**
         * @brief Clear all used screen buffers (color and depth).
         *
         * This function clears all the screen buffers (color and depth) currently in use.
         */
        void ClearScreenBuffers();

        /**
         * @brief Check for and log OpenGL error codes.
         *
         * This function checks for OpenGL error codes and logs them for debugging purposes.
         */
        void CheckErrors();

        /**
         * @brief Set the blending mode.
         *
         * This function sets the blending mode for rendering.
         *
         * @param mode The blending mode to set.
         */
        void SetBlendMode(BlendMode mode);

        /**
         * @brief Set the blending mode factors and equation using OpenGL factors.
         *
         * This function sets the blending mode factors and equation using OpenGL factors.
         *
         * @param glSrcFactor The source blending factor.
         * @param glDstFactor The destination blending factor.
         * @param glEquation The blending equation.
         */
        void SetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation);

        /**
         * @brief Set the blending mode factors and equations separately using OpenGL factors.
         *
         * This function sets the blending mode factors and equations separately using OpenGL factors.
         *
         * @param glSrcRGB The source RGB blending factor.
         * @param glDstRGB The destination RGB blending factor.
         * @param glSrcAlpha The source alpha blending factor.
         * @param glDstAlpha The destination alpha blending factor.
         * @param glEqRGB The RGB blending equation.
         * @param glEqAlpha The alpha blending equation.
         */
        void SetBlendFactorsSeparate(int glSrcRGB, int glDstRGB, int glSrcAlpha, int glDstAlpha, int glEqRGB, int glEqAlpha);

        //------------------------------------------------------------------------------------
        // Functions Declaration - rlgl functionality
        //------------------------------------------------------------------------------------

        /**
         * @brief Set the current framebuffer width.
         *
         * This function sets the width of the current framebuffer.
         *
         * @param width The width of the framebuffer.
         */
        void SetFramebufferWidth(int width);

        /**
         * @brief Set the current framebuffer height.
         *
         * This function sets the height of the current framebuffer.
         *
         * @param height The height of the framebuffer.
         */
        void SetFramebufferHeight(int height);

        /**
         * @brief Get the default framebuffer width.
         *
         * This function retrieves the width of the default framebuffer.
         *
         * @return The width of the default framebuffer.
         */
        int GetFramebufferWidth() const;

        /**
         * @brief Get the default framebuffer height.
         *
         * This function retrieves the height of the default framebuffer.
         *
         * @return The height of the default framebuffer.
         */
        int GetFramebufferHeight() const;

        /**
         * @brief Get the default texture ID.
         *
         * This function retrieves the ID of the default texture.
         *
         * @return The ID of the default texture.
         */
        Uint32 GetTextureIdDefault() const;

        /**
         * @brief Get the default shader ID.
         *
         * This function retrieves the ID of the default shader.
         *
         * @return The ID of the default shader.
         */
        Uint32 GetShaderIdDefault() const;

        /**
         * @brief Get the default shader locations.
         *
         * This function retrieves the default shader locations.
         *
         * @return A pointer to the default shader locations array.
         */
        const int *GetShaderLocsDefault() const;

        /**
         * @brief Draw render batch data.
         *
         * This function draws the data stored in the specified render batch, which includes
         * updating, drawing, and resetting the batch.
         *
         * @param batch The render batch to draw.
         */
        void DrawRenderBatch(RenderBatch* batch);

        /**
         * @brief Set the active render batch for rendering.
         *
         * This function sets the specified render batch as the active render batch for rendering.
         * Setting it to NULL will use the default internal render batch.
         *
         * @param batch The render batch to set as active (NULL for the default internal batch).
         */
        void SetRenderBatchActive(RenderBatch* batch = nullptr);

        /**
         * @brief Update and draw the internal active render batch.
         *
         * This function updates and draws the internal active render batch.
         */
        void DrawRenderBatchActive();

        /**
         * @brief Check for internal buffer overflow for a given number of vertices.
         *
         * This function checks for buffer overflow in the internal batch for a given number of vertices.
         *
         * @param vCount The number of vertices to check for overflow.
         * @return true if there is an overflow, false otherwise.
         */
        bool CheckRenderBatchLimit(int vCount);

        /**
         * @brief Set the current texture for the render batch and check buffer limits.
         *
         * This function sets the current texture for the render batch and checks the buffer limits
         * for rendering.
         *
         * @param id The ID of the texture to set. You can provide 0 to unset the texture.
         */
        void SetTexture(Uint32 id);

        /**
         * @brief Set the current texture for the render batch using a Texture object and check buffer limits.
         *
         * This function sets the current texture for the render batch using a Texture object and checks the buffer limits
         * for rendering.
         *
         * @param texture The Texture object to set.
         */
        void SetTexture(const class Texture& texture);

        /**
         * @brief Sets the current texture for the render batch using a Texture pointer and checks buffer limits.
         *
         * This function sets the current texture for the render batch using a Texture pointer and checks the buffer limits
         * for rendering.
         *
         * @param texture A pointer to the Texture object to set. You can provide nullptr to unset the texture.
         */
        void SetTexture(const class Texture* texture);

        void SetDefaultTexture() override
        {
            this->SetTexture(this->GetTextureIdDefault());
        }

        void UnsetTexture() override
        {
            this->SetTexture(0u);
        }

        //------------------------------------------------------------------------------------------------------------------------

        /* Vertex buffers management */

        /**
         * @brief Load a vertex array (VAO) if supported.
         *
         * This function loads a vertex array (VAO) if the system supports it.
         *
         * @return The ID of the loaded vertex array (VAO).
         */
        Uint32 LoadVertexArray();

        /**
         * @brief Load a vertex buffer attribute.
         *
         * This function loads a vertex buffer attribute with the provided data. The buffer can
         * be marked as dynamic if it is expected to change frequently.
         *
         * @param buffer The data buffer to load.
         * @param size The size of the buffer.
         * @param dynamic Set to true for a dynamic buffer, false for a static one.
         * @return The ID of the loaded vertex buffer.
         */
        Uint32 LoadVertexBuffer(const void *buffer, int size, bool dynamic);

        /**
         * @brief Load a new attributes element buffer.
         *
         * This function loads a new attributes element buffer with the provided data. The buffer
         * can be marked as dynamic if it is expected to change frequently.
         *
         * @param buffer The data buffer to load.
         * @param size The size of the buffer.
         * @param dynamic Set to true for a dynamic buffer, false for a static one.
         * @return The ID of the loaded vertex buffer element.
         */
        Uint32 LoadVertexBufferElement(const void *buffer, int size, bool dynamic);

        /**
         * @brief Update a GPU buffer with new data.
         *
         * This function updates a GPU buffer with new data, including the data itself, its size, and an offset.
         *
         * @param bufferId The ID of the buffer to update.
         * @param data The new data to update the buffer with.
         * @param dataSize The size of the data.
         * @param offset The offset within the buffer to update.
         */
        void UpdateVertexBuffer(Uint32 bufferId, const void *data, int dataSize, int offset);

        /**
         * @brief Update vertex buffer elements with new data.
         *
         * This function updates vertex buffer elements with new data, including the data itself, its size, and an offset.
         *
         * @param id The ID of the vertex buffer element to update.
         * @param data The new data to update the buffer element with.
         * @param dataSize The size of the data.
         * @param offset The offset within the buffer element to update.
         */
        void UpdateVertexBufferElements(Uint32 id, const void *data, int dataSize, int offset);

        /**
         * @brief Unload a vertex array (VAO).
         *
         * This function unloads a vertex array (VAO) with the specified ID.
         *
         * @param vaoId The ID of the vertex array (VAO) to unload.
         */
        void UnloadVertexArray(Uint32 vaoId);

        /**
         * @brief Unload a vertex buffer (VBO).
         *
         * This function unloads a vertex buffer (VBO) with the specified ID.
         *
         * @param vboId The ID of the vertex buffer (VBO) to unload.
         */
        void UnloadVertexBuffer(Uint32 vboId);

        /**
         * @brief Set vertex attribute data.
         *
         * This function sets vertex attribute data for the specified attribute index.
         *
         * @param index The index of the vertex attribute.
         * @param compSize The number of components in the attribute (1, 2, 3, or 4).
         * @param type The data type of the attribute.
         * @param normalized A boolean indicating whether the data should be normalized.
         * @param stride The stride (byte offset) between consecutive elements.
         * @param pointer A pointer to the attribute data.
         */
        void SetVertexAttribute(Uint32 index, int compSize, DataType type, bool normalized, int stride, const void *pointer);

        /**
         * @brief Set the vertex attribute divisor.
         *
         * This function sets the vertex attribute divisor for a specified attribute index.
         *
         * @param index The index of the vertex attribute.
         * @param divisor The divisor value for instanced rendering.
         */
        void SetVertexAttributeDivisor(Uint32 index, int divisor);

        /**
         * @brief Set the default value for a vertex attribute.
         *
         * This function sets the default value for a vertex attribute with the specified location index.
         *
         * @param locIndex The location index of the vertex attribute.
         * @param value A pointer to the default value data.
         * @param attribType The type of the attribute.
         * @param count The number of values to set.
         */
        void SetVertexAttributeDefault(int locIndex, const void *value, ShaderAttributeType attribType, int count);

        /**
         * @brief Draw vertex arrays.
         *
         * This function draws vertex arrays using a specified offset and vertex count.
         *
         * @param offset The offset within the vertex arrays.
         * @param count The number of vertices to draw.
         */
        void DrawVertexArray(int offset, int count);

        /**
         * @brief Draw elements from vertex arrays.
         *
         * This function draws elements from vertex arrays using a specified offset, element count, and element buffer.
         *
         * @param offset The offset within the vertex arrays.
         * @param count The number of elements to draw.
         * @param buffer A pointer to the element buffer data.
         */
        void DrawVertexArrayElements(int offset, int count, const void *buffer);

        /**
         * @brief Draw instanced vertex arrays.
         *
         * This function draws instanced vertex arrays using a specified offset, vertex count, and number of instances.
         *
         * @param offset The offset within the vertex arrays.
         * @param count The number of vertices to draw.
         * @param instances The number of instances to draw.
         */
        void DrawVertexArrayInstanced(int offset, int count, int instances);

        /**
         * @brief Draw instanced elements from vertex arrays.
         *
         * This function draws instanced elements from vertex arrays using a specified offset, element count,
         * element buffer, and number of instances.
         *
         * @param offset The offset within the vertex arrays.
         * @param count The number of elements to draw.
         * @param buffer A pointer to the element buffer data.
         * @param instances The number of instances to draw.
         */
        void DrawVertexArrayElementsInstanced(int offset, int count, const void *buffer, int instances);

        /* Textures management */

        /**
         * @brief Load a texture into GPU memory.
         *
         * This function loads a texture into GPU memory with the specified data, width, height,
         * pixel format, and mipmap count.
         *
         * @param data A pointer to the texture data.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param format The pixel format of the texture.
         * @param mipmapCount The number of mipmaps for the texture.
         *
         * @return The ID of the loaded texture.
         */
        Uint32 LoadTexture(const void *data, int width, int height, TextureFormat format, int mipmapCount);

        /**
         * @brief Load a depth texture or renderbuffer into GPU memory.
         *
         * This function loads a depth texture or renderbuffer into GPU memory with the specified width
         * and height. The useRenderBuffer parameter determines whether a renderbuffer is used.
         *
         * @param width The width of the depth texture/renderbuffer.
         * @param height The height of the depth texture/renderbuffer.
         * @param useRenderBuffer A flag to indicate whether to use a renderbuffer instead of a texture.
         *
         * @return The ID of the loaded depth texture/renderbuffer.
         */
        Uint32 LoadTextureDepth(int width, int height, bool useRenderBuffer);

        /**
         * @brief Load a cubemap texture into GPU memory.
         *
         * This function loads a cubemap texture into GPU memory with the specified data, size, and pixel format.
         *
         * @param data A pointer to the cubemap texture data.
         * @param size The size (width and height) of the cubemap faces.
         * @param format The pixel format of the cubemap texture.
         *
         * @return The ID of the loaded cubemap texture.
         */
        Uint32 LoadTextureCubemap(const void *data, int size, TextureFormat format);

        /**
         * @brief Update a GPU texture with new data.
         *
         * This function updates a GPU texture with new data at the specified offset, size, format, and data.
         *
         * @param id The ID of the texture to update.
         * @param offsetX The X-axis offset in the texture.
         * @param offsetY The Y-axis offset in the texture.
         * @param width The width of the data to update.
         * @param height The height of the data to update.
         * @param format The pixel format of the data.
         * @param data A pointer to the new data.
         */
        void UpdateTexture(Uint32 id, int offsetX, int offsetY, int width, int height, TextureFormat format, const void *data);

        /**
         * @brief Unload a texture from GPU memory.
         *
         * This function unloads a texture with the specified ID from GPU memory.
         *
         * @param id The ID of the texture to unload.
         */
        void UnloadTexture(Uint32 id);

        /**
         * @brief Generate mipmap data for a selected texture.
         *
         * This function generates mipmap data for a texture with the specified ID, width, height, pixel format,
         * and stores the mipmap count in the provided pointer.
         *
         * @param id The ID of the texture.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param format The pixel format of the texture.
         * @param mipmaps A reference to store the mipmap count.
         */
        void GenTextureMipmaps(Uint32 id, int width, int height, TextureFormat format, int& mipmaps);

        /**
         * @brief Read pixel data from a texture.
         *
         * This function reads pixel data from a texture with the specified ID, width, height, and pixel format.
         *
         * @param id The ID of the texture.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param format The pixel format of the texture.
         *
         * @return A vector containing the pixel data.
         */
        std::vector<Uint8> ReadTexturePixels(Uint32 id, int width, int height, TextureFormat format);

        /**
         * @brief Read pixel data from the screen buffer.
         *
         * This function reads pixel data from the screen buffer with the specified width and height.
         *
         * @param width The width of the screen buffer.
         * @param height The height of the screen buffer.
         *
         * @return A vector containing the pixel data.
         */
        std::vector<Uint8> ReadScreenPixels(int width, int height);

        /* Framebuffer management (FBO) */

        /**
         * @brief Load an empty framebuffer.
         *
         * This function loads an empty framebuffer with the specified width and height into GPU memory.
         *
         * @param width The width of the framebuffer.
         * @param height The height of the framebuffer.
         *
         * @return The ID of the loaded framebuffer.
         */
        Uint32 LoadFramebuffer(int width, int height);

        /**
         * @brief Attach a texture or renderbuffer to a framebuffer.
         *
         * This function attaches a texture or renderbuffer with the specified ID to a framebuffer with the
         * specified attachment type, texture type, and mip level.
         *
         * @param fboId The ID of the framebuffer.
         * @param texId The ID of the texture or renderbuffer to attach.
         * @param attachType The attachment type (Color, Depth, or Stencil).
         * @param texType The texture type (2D or CubeMap).
         * @param mipLevel The mipmap level.
         */
        void FramebufferAttach(Uint32 fboId, Uint32 texId, FramebufferAttachType attachType, FramebufferAttachTextureType texType, int mipLevel);

        /**
         * @brief Verify the completeness of a framebuffer.
         *
         * This function verifies if a framebuffer with the specified ID is complete and returns true if complete,
         * or false if incomplete.
         *
         * @param id The ID of the framebuffer to verify.
         *
         * @return True if the framebuffer is complete; otherwise, false.
         */
        bool FramebufferComplete(Uint32 id);

        /**
         * @brief Unload a framebuffer from GPU memory.
         *
         * This function unloads a framebuffer with the specified ID from GPU memory.
         *
         * @param id The ID of the framebuffer to unload.
         */
        void UnloadFramebuffer(Uint32 id);

        /* Shaders management */

        /**
         * @brief Load a shader from code strings.
         *
         * This function loads a shader program from code strings for the vertex and fragment shaders.
         * It returns the ID of the loaded shader program.
         *
         * @param vsCode The code string for the vertex shader.
         * @param fsCode The code string for the fragment shader.
         *
         * @return The ID of the loaded shader program.
         */
        Uint32 LoadShaderCode(const char *vsCode, const char *fsCode);

        /**
         * @brief Compile a custom shader and return the shader ID.
         *
         * This function compiles a custom shader from the provided code and shader type and returns the shader ID.
         *
         * @param shaderCode The code string of the shader to compile.
         * @param type The type of the shader (RL_VERTEX_SHADER, RL_FRAGMENT_SHADER, RL_COMPUTE_SHADER).
         *
         * @return The ID of the compiled shader.
         */
        Uint32 CompileShader(const char *shaderCode, int type);

        /**
         * @brief Load a custom shader program.
         *
         * This function loads a custom shader program with the provided vertex and fragment shader IDs
         * and returns the program ID.
         *
         * @param vShaderId The ID of the vertex shader.
         * @param fShaderId The ID of the fragment shader.
         *
         * @return The ID of the loaded shader program.
         */
        Uint32 LoadShaderProgram(Uint32 vShaderId, Uint32 fShaderId);

        /**
         * @brief Unload a shader program from GPU memory.
         *
         * This function unloads a shader program with the specified ID from GPU memory.
         *
         * @param id The ID of the shader program to unload.
         */
        void UnloadShaderProgram(Uint32 id);

        /**
         * @brief Get the location of a uniform variable in a shader program.
         *
         * This function returns the location of a uniform variable with the specified name in the shader program.
         *
         * @param shaderId The ID of the shader program.
         * @param uniformName The name of the uniform variable.
         *
         * @return The location of the uniform variable or -1 if not found.
         */
        int GetLocationUniform(Uint32 shaderId, const std::string& uniformName) const;

        /**
         * @brief Get the location of an attribute variable in a shader program.
         *
         * This function returns the location of an attribute variable with the specified name in the shader program.
         *
         * @param shaderId The ID of the shader program.
         * @param attribName The name of the attribute variable.
         *
         * @return The location of the attribute variable or -1 if not found.
         */
        int GetLocationAttrib(Uint32 shaderId, const std::string& attribName) const;

        /**
         * @brief Set a shader uniform variable with a specific value.
         *
         * This function sets a shader uniform variable with the specified location index to the provided value.
         *
         * @param locIndex The location index of the uniform variable in the shader.
         * @param value A pointer to the value to set.
         * @param uniformType The type of the uniform variable (ShaderUniformType).
         * @param count The number of elements in the value (e.g., 1 for scalars, 16 for 4x4 matrices).
         */
        void SetUniform(int locIndex, const void *value, ShaderUniformType uniformType, int count);

        /**
         * @brief Set a shader uniform matrix variable.
         *
         * This function sets a shader uniform matrix variable with the specified location index to the provided matrix.
         *
         * @param locIndex The location index of the uniform matrix in the shader.
         * @param mat The matrix to set.
         */
        void SetUniformMatrix(int locIndex, const math::Mat4& mat);

        /**
         * @brief Set a shader uniform sampler variable with a texture ID.
         *
         * This function sets a shader uniform sampler variable with the specified location index to the provided texture ID.
         *
         * @param locIndex The location index of the uniform sampler in the shader.
         * @param textureId The ID of the texture to set.
         */
        void SetUniformSampler(int locIndex, Uint32 textureId);

        /**
         * @brief Set a shader program as the currently active shader.
         *
         * This function sets a shader program as the currently active shader with the specified ID and associated locations.
         *
         * @param id The ID of the shader program to activate.
         * @param locs An array of shader locations to bind with the shader program.
         */
        void SetShader(Uint32 id, const int *locs);

        /* Compute shader management */

        /**
         * @brief Load a compute shader program.
         *
         * This function loads a compute shader program with the specified shader ID.
         *
         * @param shaderId The ID of the compute shader program to load.
         * @return The ID of the loaded compute shader program.
         */
        Uint32 LoadComputeShaderProgram(Uint32 shaderId);

        /**
         * @brief Dispatch a compute shader.
         *
         * This function dispatches a compute shader with the specified group dimensions.
         *
         * @param groupX The number of work groups in the X dimension.
         * @param groupY The number of work groups in the Y dimension.
         * @param groupZ The number of work groups in the Z dimension.
         */
        void ComputeShaderDispatch(Uint32 groupX, Uint32 groupY, Uint32 groupZ);

        /* Shader buffer storage object management (SSBO) */

        /**
         * @brief Load a shader storage buffer object (SSBO).
         *
         * This function loads a shader storage buffer object (SSBO) with the specified size, data, and usage hint.
         *
         * @param size The size of the SSBO.
         * @param data The data to initialize the SSBO with.
         * @param usageHint The usage hint for the SSBO.
         * @return The ID of the loaded SSBO.
         */
        Uint32 LoadShaderBuffer(Uint32 size, const void *data, BufferUsage usageHint);

        /**
         * @brief Unload a shader storage buffer object (SSBO).
         *
         * This function unloads a shader storage buffer object (SSBO) from GPU memory.
         *
         * @param ssboId The ID of the SSBO to unload.
         */
        void UnloadShaderBuffer(Uint32 ssboId);

        /**
         * @brief Update the data of a shader storage buffer object (SSBO).
         *
         * This function updates the data of a shader storage buffer object (SSBO) with new data.
         *
         * @param id The ID of the SSBO to update.
         * @param data The new data to write to the SSBO.
         * @param dataSize The size of the data to update.
         * @param offset The offset within the SSBO to update.
         */
        void UpdateShaderBuffer(Uint32 id, const void *data, Uint32 dataSize, Uint32 offset);

        /**
         * @brief Bind a shader storage buffer object (SSBO).
         *
         * This function binds a shader storage buffer object (SSBO) to a specified index.
         *
         * @param id The ID of the SSBO to bind.
         * @param index The index to bind the SSBO to.
         */
        void BindShaderBuffer(Uint32 id, Uint32 index);

        /**
         * @brief Read data from a shader storage buffer object (SSBO).
         *
         * This function reads data from a shader storage buffer object (SSBO) into CPU memory.
         *
         * @param id The ID of the SSBO to read from.
         * @param dest The destination buffer in CPU memory.
         * @param count The number of elements to read.
         * @param offset The offset within the SSBO to start reading from.
         */
        void ReadShaderBuffer(Uint32 id, void *dest, Uint32 count, Uint32 offset);

        /**
         * @brief Copy data between shader storage buffer objects (SSBOs).
         *
         * This function copies data from one SSBO to another.
         *
         * @param destId The ID of the destination SSBO.
         * @param srcId The ID of the source SSBO.
         * @param destOffset The offset within the destination SSBO.
         * @param srcOffset The offset within the source SSBO.
         * @param count The number of elements to copy.
         */
        void CopyShaderBuffer(Uint32 destId, Uint32 srcId, Uint32 destOffset, Uint32 srcOffset, Uint32 count);

        /**
         * @brief Get the size of a shader storage buffer object (SSBO).
         *
         * This function returns the size of a shader storage buffer object (SSBO) in bytes.
         *
         * @param id The ID of the SSBO.
         * @return The size of the SSBO in bytes.
         */
        Uint32 GetShaderBufferSize(Uint32 id) const;

        /* Buffer management */

        /**
         * @brief Bind an image texture to an image unit.
         *
         * This function binds an image texture to the specified image unit with the given format and read-only mode.
         *
         * @param id The ID of the image texture to bind.
         * @param index The index of the image unit to bind the image texture to.
         * @param format The format of the image texture.
         * @param readonly Set to `true` to make the image texture read-only, `false` otherwise.
         */
        void BindImageTexture(Uint32 id, Uint32 index, int format, bool readonly);

        /* Matrix state management */

        /**
         * @brief Get the internal modelview matrix.
         *
         * This function returns the internal modelview matrix used by the context.
         *
         * @return The internal modelview matrix.
         */
        math::Mat4 GetMatrixModelview() const;

        /**
         * @brief Get the internal projection matrix.
         *
         * This function returns the internal projection matrix used by the context.
         *
         * @return The internal projection matrix.
         */
        math::Mat4 GetMatrixProjection() const;

        /**
         * @brief Get the internal accumulated transform matrix.
         *
         * This function returns the internal accumulated transform matrix used by the context.
         *
         * @return The internal accumulated transform matrix.
         */
        math::Mat4 GetMatrixTransform() const;

        /**
         * @brief Get the internal projection matrix for stereo rendering.
         *
         * This function returns the internal projection matrix used for stereo rendering for the selected eye.
         *
         * @param eye The selected eye for stereo rendering (0 for the right eye, 1 for the left eye).
         * @return The internal projection matrix for stereo rendering.
         */
        math::Mat4 GetMatrixProjectionStereo(int eye) const;

        /**
         * @brief Get the internal view offset matrix for stereo rendering.
         *
         * This function returns the internal view offset matrix used for stereo rendering for the selected eye.
         *
         * @param eye The selected eye for stereo rendering (0 for the right eye, 1 for the left eye).
         * @return The internal view offset matrix for stereo rendering.
         */
        math::Mat4 GetMatrixViewOffsetStereo(int eye) const;

        /**
         * @brief Set a custom projection matrix.
         *
         * This function replaces the internal projection matrix with a custom projection matrix.
         *
         * @param proj The new custom projection matrix.
         */
        void SetMatrixProjection(const math::Mat4& proj);

        /**
         * @brief Set a custom modelview matrix.
         *
         * This function replaces the internal modelview matrix with a custom modelview matrix.
         *
         * @param view The new custom modelview matrix.
         */
        void SetMatrixModelview(const math::Mat4& view);

        /**
         * @brief Set projection matrices for stereo rendering.
         *
         * This function replaces the internal projection matrices for stereo rendering for both eyes.
         *
         * @param right The projection matrix for the right eye.
         * @param left The projection matrix for the left eye.
         */
        void SetMatrixProjectionStereo(const math::Mat4& right, const math::Mat4& left);

        /**
         * @brief Set view offset matrices for stereo rendering.
         *
         * This function replaces the internal view offset matrices for stereo rendering for both eyes.
         *
         * @param right The view offset matrix for the right eye.
         * @param left The view offset matrix for the left eye.
         */
        void SetMatrixViewOffsetStereo(const math::Mat4& right, const math::Mat4& left);

        /* Quick and dirty cube/quad buffers load->draw->unloa */

        /**
         * @brief Load and draw a cube.
         *
         * This function loads a cube into the context and draws it.
         * The cube is drawn with the currently active transformations (modelview and view).
         */
        void LoadDrawCube();

        /**
         * @brief Load and draw a quad.
         *
         * This function loads a quad (a rectangle) into the context and draws it.
         * The quad is drawn with the currently active transformations (modelview and view).
         */
        void LoadDrawQuad();

      private:
#     if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
        void LoadShaderDefault();      // Load default shader
        void UnloadShaderDefault();    // Unload default shader
#     endif  // GRAPHICS_API_OPENGL_33 || GRAPHICS_API_OPENGL_ES2

      public:
        /**
         * @brief Retrieves a constant reference to the internal state of the gl::Context.
         *
         * This function allows access to the internal state of the gl::Context by providing
         * a constant reference. The state is not modified by this operation, making it a
         * read-only access method.
         *
         * @return A constant reference to the internal state of the gl::Context.
         */
        const State& GetState() const
        {
            return state;
        }
    };

}}

#endif //NEXUS_CORE_GL_CONTEXT_HPP
