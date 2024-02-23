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

#ifndef NEXUS_GL_RENDER_BATCH_HPP
#define NEXUS_GL_RENDER_BATCH_HPP

#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxConfig.hpp"
#include "./nxEnums.hpp"
#include "./nxUtils.hpp"
#include <SDL_stdinc.h>
#include <queue>
#include <array>

namespace _gl_impl {

    struct Vertex
    {
        nexus::math::Vec3 vertex;      ///< Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
        nexus::math::Vec2 texcoord;    ///< Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
        nexus::gfx::Color color;       ///< Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    };

    // Dynamic vertex buffers (position + texcoords + colors + indices arrays)

    struct VertexBuffer
    {
        std::array<Uint32, 2> vboId{};          ///< OpenGL Vertex Buffer Objects id (vertices and indices)
        Vertex *vertices            = nullptr;  ///< Unique vertices array contains Vertex/Texcoord/Color
#   if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)
        Uint32 *indices             = nullptr;  ///< Vertex indices (in case vertex data comes indexed) (6 indices per quad)
#   endif
#   if defined(GRAPHICS_API_OPENGL_ES2)
        Uint16 *indices             = nullptr;  ///< Vertex indices (in case vertex data comes indexed) (6 indices per quad)
#   endif
        Uint32 vaoId                = 0;        ///< OpenGL Vertex Array Object id
        Uint32 maxVertices          = 0;        ///< Max number of vertices in the buffer
        Uint32 vertexCounter        = 0;        ///< Current number of new vertices added to the buffer (RAM)
        Uint32 gpuVertexCount       = 0;        ///< Current number of vertices in the GPU buffer (VRAM)
        bool verticesChanges        = false;    ///< Indicates if there are new vertices that have been added to RAM since the last update

        /**
         * @brief Default constructor of VertexBuffer.
         *
         * Default constructor of the VertexBuffer class.
         * Initializes members with their default values.
         */
        VertexBuffer() = default;

        /**
         * @brief Constructor of VertexBuffer with specified shader locations.
         *
         * Constructor of the VertexBuffer class taking shader locations and buffer elements as parameters.
         *
         * @param shaderLocs Array of shader locations for vertex attributes.
         * @param bufferElements Number of buffer elements.
         */
        VertexBuffer(const int *shaderLocs, int bufferElements);

        /**
         * @brief Destructor of VertexBuffer.
         *
         * Destructor of the VertexBuffer class.
         * Releases dynamically allocated memory for vertex data.
         */
        ~VertexBuffer();

        /**
         * @brief Deleted copy constructor.
         *
         * Deleted copy constructor to prevent copying of VertexBuffer objects.
         */
        VertexBuffer(const VertexBuffer&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         *
         * Deleted copy assignment operator to prevent copying of VertexBuffer objects.
         */
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        /**
         * @brief Move constructor of VertexBuffer.
         *
         * Move constructor of the VertexBuffer class.
         * Transfers resources from one VertexBuffer object to another through move semantics.
         *
         * @param other The other VertexBuffer object to move from.
         */
        VertexBuffer(VertexBuffer&& other) noexcept;

        /**
         * @brief Move assignment operator of VertexBuffer.
         *
         * Move assignment operator of the VertexBuffer class.
         * Transfers resources from one VertexBuffer object to another through move semantics.
         *
         * @param other The other VertexBuffer object to move from.
         */
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        /**
         * @brief Indicates whether the vertex buffer is full.
         *
         * Method to check whether the vertex buffer is full.
         *
         * @return true if the vertex buffer is full, otherwise false.
         */
        bool IsFull() const;

        /**
         * @brief Indicates whether there is enough space for the specified number of new vertices.
         *
         * Method to check if there is enough space in the buffer
         * for the specified number of new vertices.
         *
         * @param requiredVertices The number of new vertices to add.
         * @return true if there is enough space, otherwise false.
         */
        bool CanAddVertices(int requiredVertices) const;

        /**
         * @brief Adds vertices to RAM until they are sent to the GPU via the VertexBuffer::Update function.
         *
         * Method to add vertices to RAM until they are
         * sent to the GPU via the VertexBuffer::Update function.
         *
         * @warning: No overflow checking is done, this is done in `RenderBatch::AddVertex`.
         *
         * @param vertex The position of the vertex.
         * @param texcoord The texture coordinates of the vertex.
         * @param color The color of the vertex.
         */
        void AddVertex(const nexus::math::Vec3& vertex, const nexus::math::Vec2& texcoord, const nexus::gfx::Color& color);

        /**
         * @brief Updates the vertex data in the Vertex Buffer Object (VBO).
         *
         * This function is responsible for updating the vertex data in the VBO associated
         * with the VertexBuffer instance. It assumes that the vertex data is organized
         * into three main buffers: vertex positions, texture coordinates, and colors.
         *
         * @param vertexCounter The number of vertices to update in the VBO.
         *
         * The function performs the following steps:
         * 1. Activates the Vertex Array Object (VAO) if supported.
         * 2. Updates the vertex positions buffer in the VBO.
         * 3. Updates the texture coordinates buffer in the VBO.
         * 4. Updates the colors buffer in the VBO.
         * 5. Unbinds the current VAO if supported.
         *
         * Note: This function assumes that OpenGL is being used, and it should be
         * called within a valid OpenGL rendering context.
         *
         * @see VertexBuffer
         * @see GetExtensions()
         * @see glBindVertexArray
         * @see glBindBuffer
         * @see glBufferSubData
         * @see glVertexAttribPointer
         * @see glEnableVertexAttribArray
         */
        void Update();

        /**
         * @brief Binds the Vertex Buffer Object (VBO) and Vertex Array Object (VAO) for rendering.
         *
         * This function is responsible for binding the VBO and VAO associated with the
         * VertexBuffer instance, setting up vertex attribute pointers for position, texture
         * coordinates, and colors. If VAO is supported, it directly binds the VAO; otherwise,
         * it manually configures the vertex attribute pointers and element array buffer.
         *
         * @param currentShaderLocs An array of shader locations for vertex attributes.
         *                          The order should match the shader's attribute layout.
         *
         * The function performs the following steps:
         * 1. Binds the VAO directly if supported.
         * 2. Manually configures and binds vertex attribute pointers for position, texcoord, and color.
         * 3. Binds the element array buffer for indexed rendering.
         *
         * Note: This function assumes that OpenGL is being used, and it should be called
         * within a valid OpenGL rendering context.
         *
         * @see VertexBuffer
         * @see GetExtensions()
         * @see glBindVertexArray
         * @see glBindBuffer
         * @see glVertexAttribPointer
         * @see glEnableVertexAttribArray
         */
        void Bind(const int *currentShaderLocs) const;
    };

    /**
     * @brief Represents a draw call.
     *
     * This struct defines a draw call with specific parameters such as drawing mode, number of vertices,
     * vertex alignment, and texture ID. Changes in texture ID trigger a new draw call. Other state-change-related
     * elements like vertex array object ID (vaoId), shader ID (shaderId), and matrices (projection and modelview)
     * are not used at this moment. Instead, gl::RenderBatch forces a batch draw call if any of those state
     * changes occur (this behavior is handled by gl::RenderBatch).
     */
    struct DrawCall
    {
        nexus::gl::DrawMode mode = nexus::gl::DrawMode::Quads;    ///< Drawing mode: LINES, TRIANGLES, QUADS
        int numVertices = 0;                                ///< Number of vertices for the draw
        int vertexAlignment = 0;                            ///< Vertex alignment required for index alignment (LINES, TRIANGLES)
        //Uint32 vaoId = 0;                                 ///< Vertex array ID to be used for the draw -> Using Context.state.currentBatch->vertexBuffer.vaoId
        //Uint32 shaderId = 0;                              ///< Shader ID to be used for the draw -> Using Context.state.currentShaderId
        Uint32 textureId = 0;                               ///< Texture ID to be used for the draw. Changes in texture ID trigger a new draw call.

        //Matrix projection = Matrix::Identity;             ///< Projection matrix for this draw -> Using Context.state.projection by default
        //Matrix modelview = Matrix::Identity;              ///< Modelview matrix for this draw -> Using Context.state.modelview by default

        /**
        * @brief Default constructor.
        */
        DrawCall() = default;

        /**
        * @brief Constructor specifying the texture ID.
        * @param textureId The texture ID to be used for the draw.
        */
        DrawCall(Uint32 textureId)
            : textureId(textureId)
        { }

        /**
        * @brief Renders the draw call.
        * @param vertexOffset The vertex offset.
        */
        void Render(int& vertexOffset);
    };

}

namespace nexus { namespace gl {

    /**
     * @brief Manages render batches.
     *
     * The RenderBatch class is responsible for managing a batch of draw calls.
     * It handles vertex data buffering, draw call queuing, and rendering.
     *
     * @note: gl::Context provides a default render batch to behave like OpenGL 1.1 immediate mode,
     *       but this RenderBatch API is exposed in case custom batches are required.
     */
    class NEXUS_API RenderBatch
    {
      private:
        class Context *ctx = nullptr;                       ///< Pointer to the rendering context

      private:
        std::vector<_gl_impl::VertexBuffer> vertexBuffer;   ///< Dynamic buffer(s) for vertex data
        int currentBuffer;                                  ///< Index tracking the current buffer in case of multi-buffering

        std::queue<_gl_impl::DrawCall> drawQueue;           ///< Queue of draw calls, organized by textureId
        int drawQueueLimit;                                 ///< Maximum number of draw calls allowed in the queue
        float currentDepth;                                 ///< Current depth value for next draw

      private:
        /**
         * @brief Creates a new draw call in the batch.
         *
         * This method creates a new draw call in the batch with the specified default texture ID.
         *
         * @param defaultTextureId The default texture ID for the draw call.
         * @return Pointer to the newly created draw call.
         */
        _gl_impl::DrawCall* NewDrawCall(Uint32 defaultTextureId)
        {
            drawQueue.emplace(defaultTextureId);
            return &drawQueue.back();
        }

      public:
        /**
         * @brief Constructs a RenderBatch object.
         *
         * Constructs a RenderBatch object with the specified rendering context,
         * number of buffers, buffer elements (quads), and draw calls limit.
         *
         * @param ctx The rendering context.
         * @param numBuffers Number of vertex buffers.
         * @param bufferElements Number of elements per buffer (quads).
         * @param drawCallsLimit Maximum number of draw calls allowed in the batch.
         */
        RenderBatch(class Context& ctx,
            int numBuffers = GL_DEFAULT_BATCH_BUFFERS,
            int bufferElements = GL_DEFAULT_BATCH_BUFFER_ELEMENTS,
            int drawCallsLimit = GL_DEFAULT_BATCH_DRAWCALLS);

        /**
         * @brief Destructor for RenderBatch.
         *
         * Destructor for the RenderBatch class.
         * Cleans up resources and memory allocated for the batch.
         */
        ~RenderBatch();

        RenderBatch(const RenderBatch&) = delete;               ///< Deleted copy constructor
        RenderBatch& operator=(const RenderBatch&) = delete;    ///< Deleted copy assignment operator

        RenderBatch(RenderBatch&& other) noexcept;              ///< Move constructor
        RenderBatch& operator=(RenderBatch&& other) noexcept;   ///< Move assignment operator

        /**
         * @brief Gets the rendering context associated with the batch.
         *
         * Returns a pointer to the rendering context associated with the batch.
         *
         * @return Pointer to the rendering context.
         */
        Context* GetContext() { return ctx; }

        /**
         * @brief Gets the current depth value for next draw call.
         *
         * Returns the current depth value for the next draw call in the batch.
         *
         * @return The current depth value.
         */
        float GetCurrentDepth() const { return currentDepth; }

        /**
         * @brief Checks if the vertex limit of the batch has been reached.
         *
         * Checks if the vertex limit of the batch has been reached.
         * Also handles the case where the limit is reached by rendering the batch.
         *
         * @param requiredVertices The number of vertices required to be added.
         * @return true if the vertex limit has been reached, otherwise false.
         */
        bool CheckLimit(int requiredVertices);

        /**
         * @brief Adds a vertex to the batch.
         *
         * Adds a vertex with the specified position, texture coordinates, and color
         * to the current vertex buffer of the batch.
         *
         * @param vertex The position of the vertex.
         * @param texcoord The texture coordinates of the vertex.
         * @param color The color of the vertex.
         */
        void AddVertex(const math::Vec3& vertex, const math::Vec2& texcoord, const gfx::Color& color);

        /**
         * @brief Sets the current texture ID for subsequent draw calls.
         *
         * Sets the current texture ID to be used for subsequent draw calls in the batch.
         *
         * @param id The texture ID to set.
         */
        void SetTexture(Uint32 id);

        /**
         * @brief Begins a new draw operation with the specified draw mode.
         *
         * Begins a new draw operation with the specified draw mode,
         * preparing the batch for drawing geometry.
         *
         * @param mode The draw mode to use (e.g., GL_TRIANGLES, GL_LINES, etc.).
         */
        void Begin(gl::DrawMode mode);

        /**
         * @brief Ends the current draw operation.
         *
         * Ends the current draw operation, finalizing the batch for rendering.
         */
        void End();

        /**
         * @brief Renders the batch of draw calls.
         *
         * Renders the batch of draw calls, flushing the queued draw calls
         * to the rendering context for rendering.
         */
        void Draw();
    };

}}

#endif //NEXUS_GL_RENDER_BATCH_HPP
