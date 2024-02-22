#ifndef NEXUS_EXT_GFX_VERTEX_RENDERER_HPP
#define NEXUS_EXT_GFX_VERTEX_RENDERER_HPP

#include "../../shape/2D/nxMesh.hpp"
#include "../../core/nxText.hpp"
#include "../../math/nxVec2.hpp"
#include "../../math/nxMat3.hpp"
#include "./nxRenderer.hpp"
#include "SDL_render.h"
#include <SDL_stdinc.h>
#include <array>

namespace nexus { namespace gfx {

    /**
     * @brief Enumeration for specifying drawing modes.
     */
    enum NEXUS_API DrawMode : Uint8
    {
        Lines       = 2,    ///< Draw lines
        Triangles   = 3,    ///< Draw triangles
        Quads       = 4     ///< Draw quads
    };

    /**
     * @brief Class for rendering geometric shapes using vertices.
     */
    class NEXUS_API VertexRenderer
    {
      private:
        std::array<math::Mat3, 8>       matrixStack;                        ///< Stack for storing transformation matrices
        std::array<shape2D::Vertex, 4>  quadStack;                          ///< Temporary storage for quad vertices to decompose into triangles
        math::Mat3                      transform = math::Mat3::Identity(); ///< Transformation matrix
        std::vector<shape2D::Vertex>    vertices;                           ///< Vertices to render to the Renderer in the End() function
        Renderer                        &renderer;                          ///< Target renderer for vertex drawings
        SDL_Texture                     *texture = nullptr;                 ///< Texture to render within geometries
        math::Vec2                      texcoord = { 0, 0 };                ///< Texture coordinates
        gfx::Color                      color = gfx::White;                 ///< Color for rendering
        Uint8                           matrixStackCounter = 0;             ///< Counter for the matrix stack
        Uint8                           quadStackCounter = 0;               ///< Counter for the quad stack
        bool                            transformRequired;                  ///< Flag indicating if transformation is required
        DrawMode                        mode;                               ///< Current drawing mode

      public:
        /**
         * @brief Constructor for VertexRenderer.
         * @param renderer Reference to the renderer object.
         * @param size Initial size for vertices vector.
         */
        VertexRenderer(Renderer& renderer, std::size_t size = 0);

        /**
         * @brief Begins rendering with specified drawing mode.
         * @param mode Drawing mode (Lines, Triangles, or Quads).
         */
        void Begin(DrawMode mode);

        /**
         * @brief Ends rendering and flushes the accumulated vertices to the renderer.
         */
        void End();

        /**
         * @brief Pushes the current transformation matrix onto the matrix stack.
         */
        void PushMatrix();

        /**
         * @brief Pops the top transformation matrix from the matrix stack.
         */
        void PopMatrix();

        /**
         * @brief Resets the transformation matrix to identity.
         */
        void LoadIdentity();

        /**
         * @brief Translates the transformation matrix.
         * @param x Translation in the x-direction.
         * @param y Translation in the y-direction.
         */
        void Translate(float x, float y);

        /**
         * @brief Translates the transformation matrix.
         * @param v Vector specifying translation in x and y directions.
         */
        void Translate(const math::Vec2& v);

        /**
         * @brief Rotates the transformation matrix.
         * @param angle Rotation angle in degrees.
         */
        void Rotate(float angle);

        /**
         * @brief Scales the transformation matrix.
         * @param sx Scaling factor in the x-direction.
         * @param sy Scaling factor in the y-direction.
         */
        void Scale(float sx, float sy);

        /**
         * @brief Scales the transformation matrix.
         * @param v Vector specifying scaling factors in x and y directions.
         */
        void Scale(const math::Vec3& v);

        /**
         * @brief Multiplies the current transformation matrix with another matrix.
         * @param mat Matrix to multiply with.
         */
        void MultMatrix(const math::Mat3& mat);

        /**
         * @brief Specifies a vertex position.
         * @param vertex Vertex position as a Vec2.
         */
        void Vertex(math::Vec2 vertex);

        /**
         * @brief Specifies a vertex position.
         * @param x X-coordinate of the vertex position.
         * @param y Y-coordinate of the vertex position.
         */
        void Vertex(float x, float y);

        /**
         * @brief Specifies texture coordinates.
         * @param texcoord Texture coordinates as a Vec2.
         */
        void TexCoord(const math::Vec2& texcoord);

        /**
         * @brief Specifies texture coordinates.
         * @param u U-coordinate of the texture.
         * @param v V-coordinate of the texture.
         */
        void TexCoord(float u, float v);

        /**
         * @brief Specifies vertex color.
         * @param color Vertex color.
         */
        void Color(const gfx::Color& color);

        /**
         * @brief Specifies vertex color.
         * @param r Red component of the color.
         * @param g Green component of the color.
         * @param b Blue component of the color.
         * @param a Alpha component of the color.
         */
        void Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

        /**
         * @brief Sets the texture to be rendered within triangles or quads using specified texture coordinates.
         * @details The texture will be rendered within triangles or quads according to the previously set drawing mode (DrawMode::Triangles or DrawMode::Quads).
         *          If a nullptr is provided, no texture will be rendered.
         * @param texture Pointer to the SDL_Texture to be rendered. nullptr to render no texture.
         */
        void SetTexture(SDL_Texture* texture);
    };

}}

#endif //NEXUS_EXT_GFX_VERTEX_RENDERER_HPP
