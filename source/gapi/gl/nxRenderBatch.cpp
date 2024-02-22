#include "gapi/gl/nxRenderBatch.hpp"
#include "gapi/gl/nxExtensions.hpp"
#include "gapi/gl/nxContext.hpp"
#include "core/nxException.hpp"
#include "gapi/gl/nxEnums.hpp"
#include <utility>

using namespace nexus;

/* Private Implementation VertexBuffer */

_gl_impl::VertexBuffer::VertexBuffer(const int *shaderLocs, int bufferElements)
: maxVertices(4 * bufferElements), vertexCounter(0)
{
    vertices = new Vertex[maxVertices]{};       ///< 4 vertices per quad

#if defined(GRAPHICS_API_OPENGL_33)
    indices = new Uint32[bufferElements * 6];   ///< 6 indices per quad
#endif

#if defined(GRAPHICS_API_OPENGL_ES2)
    indices = new Uint16[bufferElements * 6];   ///< 6 indices per quad
#endif

    // Indices can be initialized right now
    for (int j = 0, k = 0; j < (6 * bufferElements); j += 6, k++)
    {
        indices[j] = 4 * k;
        indices[j + 1] = 4 * k + 1;
        indices[j + 2] = 4 * k + 2;
        indices[j + 3] = 4 * k;
        indices[j + 4] = 4 * k + 2;
        indices[j + 5] = 4 * k + 3;
    }

    if (gl::GetExtensions().vao)
    {
        // Initialize Quads VAO
        glGenVertexArrays(1, &vaoId);
        glBindVertexArray(vaoId);
    }

    // Vertex buffer
    glGenBuffers(1, &vboId[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBufferData(GL_ARRAY_BUFFER, bufferElements * 4 * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);

    // Vertex position (shader-location = 0)
    glEnableVertexAttribArray(shaderLocs[gl::LocVertexPosition]);
    glVertexAttribPointer(shaderLocs[gl::LocVertexPosition], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex));

    // Vertex texcoord (shader-location = 2)
    glEnableVertexAttribArray(shaderLocs[gl::LocVertexTexCoord01]);
    glVertexAttribPointer(shaderLocs[gl::LocVertexTexCoord01], 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

    // Vertex color (shader-location = 5)
    glEnableVertexAttribArray(shaderLocs[gl::LocVertexColor]);
    glVertexAttribPointer(shaderLocs[gl::LocVertexColor], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // Index buffer
    glGenBuffers(1, &vboId[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);

#if defined(GRAPHICS_API_OPENGL_33)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferElements * 6 * sizeof(Uint32), indices, GL_STATIC_DRAW);
#endif

#if defined(GRAPHICS_API_OPENGL_ES2)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferElements * 6 * sizeof(Uint16), indices, GL_STATIC_DRAW);
#endif
}

_gl_impl::VertexBuffer::~VertexBuffer()
{
    // Unbind VAO attribs data
    if (gl::GetExtensions().vao)
    {
        glBindVertexArray(vaoId);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    // Delete VBOs from GPU (VRAM)
    glDeleteBuffers(2, vboId.data());

    // Delete VAO from GPU (VRAM)
    if (gl::GetExtensions().vao)
    {
        glDeleteVertexArrays(1, &vaoId);
    }

    // Free allocated memory CPU (RAM)
    delete[] indices;
    delete[] vertices;
}

_gl_impl::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
: vboId(std::move(other.vboId))
, vertices(std::exchange(other.vertices, nullptr))
, indices(std::exchange(other.indices, nullptr))
, vaoId(std::exchange(other.vaoId, 0))
, maxVertices(std::exchange(other.maxVertices, 0))
, vertexCounter(std::exchange(other.vertexCounter, 0))
, gpuVertexCount(std::exchange(other.gpuVertexCount, 0))
, verticesChanges(std::exchange(other.verticesChanges, false))
{ }

_gl_impl::VertexBuffer& _gl_impl::VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        verticesChanges = std::exchange(other.verticesChanges, false);
        gpuVertexCount = std::exchange(other.gpuVertexCount, 0);
        vertexCounter = std::exchange(other.vertexCounter, 0);
        maxVertices = std::exchange(other.maxVertices, 0);
        vertices = std::exchange(other.vertices, nullptr);
        indices = std::exchange(other.indices, nullptr);
        vaoId = std::exchange(other.vaoId, 0);
        vboId = std::move(other.vboId);
    }
    return *this;
}

bool _gl_impl::VertexBuffer::IsFull() const
{
    return vertexCounter >= maxVertices;
}

bool _gl_impl::VertexBuffer::CanAddVertices(int requiredVertices) const
{
    return vertexCounter + requiredVertices <= maxVertices;
}

void _gl_impl::VertexBuffer::AddVertex(const nexus::math::Vec3& vertex, const nexus::math::Vec2& texcoord, const nexus::gfx::Color& color)
{
    vertices[vertexCounter++] = { vertex, texcoord, color };
    verticesChanges = true;
}

void _gl_impl::VertexBuffer::Update()
{
    // Activate elements VAO
    if (gl::GetExtensions().vao) glBindVertexArray(vaoId);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCounter * sizeof(Vertex), vertices);

    // Unbind the current VAO
    if (gl::GetExtensions().vao) glBindVertexArray(0);

    // Reset the new vertex counter contained in RAM
    gpuVertexCount = std::exchange(vertexCounter, 0);
    verticesChanges = false;
}

void _gl_impl::VertexBuffer::Bind(const int *currentShaderLocs) const
{
    if (gl::GetExtensions().vao)
    {
        glBindVertexArray(vaoId);
        return;
    }

    // Bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);

    // Vertex position (shader-location = 0)
    glVertexAttribPointer(currentShaderLocs[gl::LocVertexPosition], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, vertex));
    glEnableVertexAttribArray(currentShaderLocs[gl::LocVertexPosition]);

    // Vertex texcoord (shader-location = 2)
    glVertexAttribPointer(currentShaderLocs[gl::LocVertexTexCoord01], 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(currentShaderLocs[gl::LocVertexTexCoord01]);

    // Vertex color (shader-location = 5)
    glVertexAttribPointer(currentShaderLocs[gl::LocVertexColor], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(currentShaderLocs[gl::LocVertexColor]);

    // Bind indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
}


/* Private Implementation DrawCall */

void _gl_impl::DrawCall::Render(int& vertexOffset)
{
    // Bind current draw call texture, activated as GL_TEXTURE0 and Bound to sampler2D texture0 by default
    glBindTexture(GL_TEXTURE_2D, textureId);

    switch (mode)
    {
        case gl::DrawMode::Lines:
            glDrawArrays(GL_LINES, vertexOffset, numVertices);
            break;

        case gl::DrawMode::Triangles:
            glDrawArrays(GL_TRIANGLES, vertexOffset, numVertices);
            break;

        case gl::DrawMode::Quads:
            // We need to define the number of indices to be processed: maxElements*6
            // NOTE: The final parameter tells the GPU the offset in bytes from the
            // start of the index buffer to the location of the first index to process
#           if defined(GRAPHICS_API_OPENGL_33)
                glDrawElements(GL_TRIANGLES, numVertices/4*6, GL_UNSIGNED_INT,
                    reinterpret_cast<const void*>(vertexOffset/4*6*sizeof(GLuint)));
#           elif defined(GRAPHICS_API_OPENGL_ES2)
                glDrawElements(GL_TRIANGLES, numVertices/4*6, GL_UNSIGNED_SHORT,
                    reinterpret_cast<const void*>(vertexOffset/4*6*sizeof(GLushort)));
#           endif
            break;
    }

    vertexOffset += (numVertices + vertexAlignment);
}


/* Public Implementation RenderBatch */

gl::RenderBatch::RenderBatch(Context& ctx, int numBuffers, int bufferElements, int drawCallsLimit)
: ctx(&ctx), currentBuffer(0), drawQueueLimit(drawCallsLimit), currentDepth(-1.0f)
{
    const Context::State &ctxState = ctx.GetState();

    // Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes)
    // And upload to GPU (VRAM) vertex data and initialize VAOs/VBOs
    //--------------------------------------------------------------------------------------------
    vertexBuffer.reserve(numBuffers);

    for (int i = 0; i < numBuffers; i++)
    {
        vertexBuffer.emplace_back(ctxState.currentShaderLocs, bufferElements);
    }

    NEXUS_LOG(Info) << "[gl::RenderBatch::RenderBatch] Vertex buffers loaded successfully in RAM (CPU) and VRAM (GPU).\n";

    // Unbind the current VAO
    if (GetExtensions().vao) glBindVertexArray(0);
    //--------------------------------------------------------------------------------------------

    // Reserve max possible space to drawQueue
    //drawQueue.reserve(drawCallsLimit);

    // Initializes the first DrawCall in the draw call queue
    drawQueue.push(_gl_impl::DrawCall(ctx.GetTextureIdDefault()));
}

gl::RenderBatch::~RenderBatch()
{
    // Unbind everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

gl::RenderBatch::RenderBatch(RenderBatch&& other) noexcept
: currentBuffer(other.currentBuffer)
, vertexBuffer(std::move(other.vertexBuffer))
, drawQueue(std::move(other.drawQueue))
, currentDepth(other.currentDepth)
{ }

gl::RenderBatch& gl::RenderBatch::operator=(RenderBatch&& other) noexcept
{
    if (this != &other)
    {
        currentBuffer = other.currentBuffer;
        vertexBuffer = std::move(other.vertexBuffer);
        drawQueue = std::move(other.drawQueue);
        currentDepth = other.currentDepth;
    }
    return *this;
}

// La fonction retourne vrai si la limite de sommets du lot a été atteinte, sinon retourne faux
// La fonction gère aussi le cas où la limite est atteinte en effectuant le rendu du lot
bool gl::RenderBatch::CheckLimit(int requiredVertices)
{
    if (vertexBuffer[currentBuffer].CanAddVertices(requiredVertices))
    {
        return false;
    }

    // Store current primitive drawing mode and texture id
    int currentTexture = drawQueue.back().textureId;
    DrawMode currentMode = drawQueue.back().mode;

    // Renders the current batch and resets everything
    this->Draw(); // NOTE: Stereo rendering is checked inside

    // Restore state of last batch so we can continue adding vertices
    drawQueue.back().textureId = currentTexture;
    drawQueue.back().mode = currentMode;

    return true;
}

void gl::RenderBatch::AddVertex(const math::Vec3& vertex, const math::Vec2& texcoord, const gfx::Color& color)
{
    auto *drawCall = &drawQueue.back();
    auto *curBuffer = &vertexBuffer[currentBuffer];

    // WARNING: We can't break primitives when launching a new batch.
    // DrawMode::Lines comes in pairs, DrawMode::Triangles come in groups of 3 vertices and DrawMode::Quads come in groups of 4 vertices.
    // We must check current draw.mode when a new vertex is required and finish the batch only if the draw.mode draw.numVertices is %2, %3 or %4

    // Check if the elements/quads count has reached the limit of the vertex buffer
    if (!curBuffer->CanAddVertices(4))
    {
        // Get the required vertices count for the current draw mode
        const Uint8 requiredVertices = static_cast<Uint8>(drawCall->mode);

        // Check if the current vertex count is a multiple of the required vertices for the draw mode
        // If the current vertex count is a multiple this means that the previous primitive is complete
        if (drawCall->numVertices % requiredVertices == 0)
        {
            // Launch a draw call, but keep the current state for the next vertices
            // NOTE: We add +1 vertex to the check for security (?)
            if (CheckLimit(requiredVertices + 1))
            {
                curBuffer = &vertexBuffer[currentBuffer];
                drawCall = &drawQueue.back();
            }
        }
    }

    // WARNING: By default VertexBuffer struct does not store normals
    curBuffer->AddVertex(vertex, texcoord, color);
    drawCall->numVertices++;
}

void gl::RenderBatch::SetTexture(Uint32 id)
{
    auto &curBuffer = vertexBuffer[currentBuffer];

    if (id == 0)
    {
        if (curBuffer.IsFull())
        {
            this->Draw();
        }
        return;
    }

    auto &drawCall = drawQueue.back();
    if (drawCall.textureId == id) return;

    if (drawCall.numVertices > 0)
    {
        // Make sure current currentBatch->draws[i].numVertices is aligned a multiple of 4,
        // that way, following QUADS drawing will keep aligned with index processing
        // It implies adding some extra alignment vertex at the end of the draw,
        // those vertex are not processed but they are considered as an additional offset
        // for the next set of vertex to be drawn
        if (drawCall.mode == DrawMode::Lines)
        {
            drawCall.vertexAlignment = (drawCall.numVertices < 4) ? drawCall.numVertices : drawCall.numVertices%4;
        }
        else if (drawCall.mode == DrawMode::Triangles)
        {
            drawCall.vertexAlignment = (drawCall.numVertices < 4) ? 1 : 4 - (drawCall.numVertices%4);
        }
        else
        {
            drawCall.vertexAlignment = 0;
        }

        if (!CheckLimit(drawCall.vertexAlignment))
        {
            vertexBuffer[currentBuffer].vertexCounter += drawCall.vertexAlignment;
            NewDrawCall(ctx->GetTextureIdDefault());
        }
    }

    if (drawQueue.size() >= drawQueueLimit)
    {
        this->Draw();
    }

    // We define the texture is the number of vertices at the last/new drawCall
    drawQueue.back().textureId = id;
    drawQueue.back().numVertices = 0;
}

void gl::RenderBatch::Begin(gl::DrawMode mode)
{
    auto &drawCall = drawQueue.back();

    // Draw mode can be DrawMode::Lines, DrawMode::Triangles and DrawMode::Quads
    // NOTE: In all three cases, vertex are accumulated over default internal vertex buffer
    if (drawCall.mode != mode)
    {
        if (drawCall.numVertices > 0)
        {
            // Make sure current currentBatch->draws[i].numVertices is aligned a multiple of 4,
            // that way, following QUADS drawing will keep aligned with index processing
            // It implies adding some extra alignment vertex at the end of the draw,
            // those vertex are not processed but they are considered as an additional offset
            // for the next set of vertex to be drawn
            if (drawCall.mode == DrawMode::Lines)
            {
                drawCall.vertexAlignment = (drawCall.numVertices < 4) ? drawCall.numVertices : drawCall.numVertices%4;
            }
            else if (drawCall.mode == DrawMode::Triangles)
            {
                drawCall.vertexAlignment = (drawCall.numVertices < 4) ? 1 : 4 - (drawCall.numVertices%4);
            }
            else
            {
                drawCall.vertexAlignment = 0;
            }

            if (!CheckLimit(drawCall.vertexAlignment))
            {
                vertexBuffer[currentBuffer].vertexCounter += drawCall.vertexAlignment;
                NewDrawCall(ctx->GetTextureIdDefault());
            }
        }

        if (drawQueue.size() >= drawQueueLimit)
        {
            this->Draw();
        }

        // Initialize the new drawCall
        drawQueue.back().mode = mode;
        drawQueue.back().numVertices = 0;
        drawQueue.back().textureId = ctx->GetTextureIdDefault();
    }
}

void gl::RenderBatch::End()
{
    // NOTE: Depth increment is dependant on Context::Ortho(): z-near and z-far values,
    // as well as depth buffer bit-depth (16bit or 24bit or 32bit)
    // Correct increment formula would be: depthInc = (zfar - znear)/pow(2, bits)
    currentDepth += 1.0f / 20000.0f;
}

void gl::RenderBatch::Draw()
{
    auto &curBuffer = vertexBuffer[currentBuffer];
    const Context::State &ctxState = ctx->GetState();

    // Update batch vertex buffers
    if (curBuffer.verticesChanges) curBuffer.Update();

    // Draw batch vertex buffers (considering VR stereo if required)
    math::Mat4 matProjection = ctx->GetMatrixProjection();
    math::Mat4 matModelView = ctx->GetMatrixModelview();

    const int eyeCount = ctxState.stereoRender ? 2 : 1;
    for (int eye = 0; eye < eyeCount; eye++)
    {
        if (eyeCount == 2)
        {
            ctx->Viewport(eye*ctx->GetFramebufferWidth()/2, 0, ctx->GetFramebufferWidth()/2, ctx->GetFramebufferHeight());      // Setup current eye viewport (half screen width)
            ctx->SetMatrixModelview(matModelView * ctxState.viewOffsetStereo[eye]);                                              // Set current eye view offset to modelview matrix
            ctx->SetMatrixProjection(ctxState.projectionStereo[eye]);                                                            // Set current eye projection matrix
        }

        // Draw buffers
        if (curBuffer.gpuVertexCount > 0)
        {
            // Set current shader and upload current MVP matrix
            glUseProgram(ctxState.currentShaderId);

            // Create modelview-projection matrix and upload to shader
            glUniformMatrix4fv(ctxState.currentShaderLocs[LocMatrixMVP], 1, false,
                (ctxState.modelview * ctxState.projection).m); // MVP

            // Binds VertexBuffer (position, texcoords, colors)
            curBuffer.Bind(ctxState.currentShaderLocs);

            // Setup some default shader values
            glUniform4f(ctxState.currentShaderLocs[LocColorDiffuse], 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1i(ctxState.currentShaderLocs[LocMapDiffuse], 0);  // Active default sampler2D: texture0

            // Activate additional sampler textures
            // Those additional textures will be common for all draw calls of the batch
            for (int i = 0; i < GL_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++)
            {
                if (ctxState.activeTextureId[i] > 0)
                {
                    glActiveTexture(GL_TEXTURE0 + 1 + i);
                    glBindTexture(GL_TEXTURE_2D, ctxState.activeTextureId[i]);
                }
            }

            // Activate default sampler2D texture0 (one texture is always active for default batch shader)
            // NOTE: Batch system accumulates calls by texture0 changes, additional textures are enabled for all the draw calls
            glActiveTexture(GL_TEXTURE0);

            for (int vertexOffset = 0; !drawQueue.empty();)
            {
                drawQueue.front().Render(vertexOffset);
                if (eye == eyeCount - 1) drawQueue.pop();
            }

            if (!GetExtensions().vao)
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            glBindTexture(GL_TEXTURE_2D, 0);    // Unbind textures
        }

        if (GetExtensions().vao) glBindVertexArray(0); // Unbind VAO

        glUseProgram(0);    // Unbind shader program
    }

    // Restore viewport to default measures
    if (eyeCount == 2) ctx->Viewport(0, 0, ctxState.framebufferWidth, ctxState.framebufferHeight);
    //------------------------------------------------------------------------------------------------------------

    // Reset depth for next draw
    currentDepth = -1.0f;

    // Restore projection/modelview matrices
    ctx->SetMatrixProjection(matProjection);
    ctx->SetMatrixModelview(matModelView);

    // If all drawCalls have been dequeued, we are resetting one
    if (drawQueue.size() == 0) drawQueue.push(_gl_impl::DrawCall(ctx->GetTextureIdDefault()));

    // Change to next buffer in the list (in case of multi-buffering)
    if ((++currentBuffer) >= vertexBuffer.size()) currentBuffer = 0;
}
