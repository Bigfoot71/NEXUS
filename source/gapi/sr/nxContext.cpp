#include "gapi/sr/nxContext.hpp"

#include "gapi/sr/nxTexture.hpp"
#include "gapi/sr/sp_model/nxMaterial.hpp"
#include "gapi/sr/sp_model/nxMesh.hpp"
#include "gfx/nxSurface.hpp"
#include "shape/2D/nxRectangle.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "math/nxMath.hpp"
#include "math/nxVec2.hpp"

using namespace nexus;

/* Public Implementation Context */

void sr::Context::EnableDepthTest()
{
    state.depthTesting = true;
}

void sr::Context::DisableDepthTest()
{
    state.depthTesting = false;
}

void sr::Context::MatrixMode(sr::MatrixMode mode)
{
    switch (mode)
    {
        case MatrixMode::Projection:
            state.currentMatrix = &state.projection;
            break;

        case MatrixMode::ModelView:
            state.currentMatrix = &state.modelview;
            break;

        case MatrixMode::Texture:
            // Not supported
            break;
    }

    state.currentMatrixMode = mode;
}

void sr::Context::PushMatrix()
{
    if (state.stackCounter >= state.stack.size())
    {
        throw core::NexusException("sr::Context::PushMatrix", core::TextFormat("Matrix stack overflow (MAX=%i)", state.stack.size()));
    }

    if (state.currentMatrixMode == MatrixMode::ModelView)
    {
        state.transformRequired = true;
        state.currentMatrix = &state.transform;
    }

    state.stack[state.stackCounter] = *state.currentMatrix;
    state.stackCounter++;
}

void sr::Context::PopMatrix()
{
    if (state.stackCounter > 0)
    {
        *state.currentMatrix = state.stack[--state.stackCounter];
    }

    if (state.stackCounter == 0 && state.currentMatrixMode == MatrixMode::ModelView)
    {
        state.currentMatrix = &state.modelview;
        state.transformRequired = false;
    }
}

void sr::Context::LoadIdentity()
{
    *state.currentMatrix = math::Mat4::Identity();
}

void sr::Context::Translate(float x, float y, float z)
{
    // NOTE: We transpose matrix with multiplication order
    *state.currentMatrix = math::Mat4::Translate(x, y, z) * (*state.currentMatrix);
}

void sr::Context::Translate(float x, float y)
{
    this->Translate(x, y, 0);
}

void sr::Context::Translate(const math::Vec3& v)
{
    this->Translate(v.x, v.y, v.z);
}

void sr::Context::Translate(const math::Vec2& v)
{
    this->Translate(v.x, v.y, 0);
}

void sr::Context::Rotate(float angle, float x, float y, float z)
{
    // NOTE: We transpose matrix with multiplication order
    *state.currentMatrix = math::Mat4::Rotate(x, y, z, angle * math::Deg2Rad) * (*state.currentMatrix);
}

void sr::Context::Rotate(float angle, const math::Vec3& v)
{
    this->Rotate(angle, v.x, v.y, v.z);
}

void sr::Context::Rotate(float angle)
{
    this->Rotate(angle, 0.0f, 0.0f, 1.0f);
}

void sr::Context::Scale(float x, float y, float z)
{
    // NOTE: We transpose matrix with multiplication order
    *state.currentMatrix = math::Mat4::Scale(x, y, z) * (*state.currentMatrix);
}

void sr::Context::Scale(float x, float y)
{
    this->Scale(x, y, 1.0f);
}

void sr::Context::Scale(const math::Vec3& v)
{
    this->Scale(v.x, v.y, v.z);
}

void sr::Context::Scale(const math::Vec2& v)
{
    this->Scale(v.x, v.y, 1.0f);
}

void sr::Context::MultMatrix(const math::Mat4& mat)
{
    *state.currentMatrix = (*state.currentMatrix) * mat;
}

void sr::Context::Frustum(double left, double right, double bottom, double top, double znear, double zfar)
{
    *state.currentMatrix = (*state.currentMatrix) * math::Mat4::Frustum(left, right, bottom, top, znear, zfar);
}

void sr::Context::Ortho(double left, double right, double bottom, double top, double znear, double zfar)
{
    *state.currentMatrix = (*state.currentMatrix) * math::Mat4::Ortho(left, right, bottom, top, znear, zfar);
}

void sr::Context::SetViewport(int x, int y, int width, int height)
{
    this->SetViewport({ x, y, width, height });
}

void sr::Context::SetViewport(const shape2D::Rectangle& vp)
{
    state.viewport = vp;

    MatrixMode(sr::MatrixMode::Projection);     // Switch to projection matrix
    LoadIdentity();                             // Reset current matrix (projection)

    // Set orthographic projection to the current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    Ortho(vp.x, vp.w, vp.h, vp.y, 0.0f, 1.0f);

    MatrixMode(sr::MatrixMode::ModelView);      // Switch back to modelview matrix
    LoadIdentity();                             // Reset current matrix (modelview)
}

shape2D::Rectangle sr::Context::GetViewport() const
{
    return state.viewport;
}

void sr::Context::EnableFramebuffer(Framebuffer& framebuffer)
{
    state.currentFramebuffer = &framebuffer;
}

void sr::Context::DisableFramebuffer()
{
    state.currentFramebuffer = &state.winFramebuffer;
}

void sr::Context::EnableWireMode()
{
    state.wireMode = true;
}

void sr::Context::DisableWireMode()
{
    state.wireMode = false;
}

void sr::Context::DrawVertexArray(const _sr_impl::Mesh& mesh, sr::Material& material, const math::Mat4& transform)
{
#   define GET_VERTEX_TEXCOORD(i) (mesh.texcoords.empty() ? math::Vec2() : mesh.texcoords[i])
#   define GET_VERTEX_COLOR(i) (mesh.colors.empty() ? gfx::White : mesh.colors[i])

    const math::Mat4 mvp = state.modelview * state.projection;
    const shape2D::Rectangle viewport = { state.viewport.x, state.viewport.y, state.viewport.w - 1, state.viewport.h - 1 };

    const auto &vertices = mesh.animVertices.empty() ? mesh.vertices : mesh.animVertices;
    const auto &normals = mesh.animNormals.empty() ? mesh.normals : mesh.animNormals;

    const gfx::Color colDiffuse = material->GetColor(Material::MapType::Diffuse);
    sr::Shader *matShader = &material->shader;
    const gfx::Surface *mapDiffuse = nullptr;
    {
        const sr::Texture *tmp = material->GetTexture(Material::MapType::Diffuse);
        mapDiffuse = tmp ? static_cast<const gfx::Surface*>(*tmp) : nullptr;
    }

    pipeline.Reset();

    if (state.wireMode)
    {
        for (int i = 0; i < mesh.numVertices; i += 2)
        {
            pipeline.AddVertex(DrawMode::Lines, vertices[i].Transformed(transform), normals[i], GET_VERTEX_TEXCOORD(i), GET_VERTEX_COLOR(i) * colDiffuse);
            pipeline.AddVertex(DrawMode::Lines, vertices[i + 1].Transformed(transform), normals[i + 1], GET_VERTEX_TEXCOORD(i + 1), GET_VERTEX_COLOR(i + 1) * colDiffuse);
            pipeline.ProcessAndRender(*state.currentFramebuffer, mvp, viewport, matShader, mapDiffuse, state.depthTesting);
        }

        pipeline.AddVertex(DrawMode::Lines, vertices.back().Transformed(transform), normals.back(), GET_VERTEX_TEXCOORD(mesh.numVertices - 1), GET_VERTEX_COLOR(mesh.numVertices - 1) * colDiffuse);
        pipeline.AddVertex(DrawMode::Lines, vertices.front().Transformed(transform), normals.front(), GET_VERTEX_TEXCOORD(0), GET_VERTEX_COLOR(0) * colDiffuse);
        pipeline.ProcessAndRender(*state.currentFramebuffer, mvp, viewport, matShader, mapDiffuse, state.depthTesting);
    }
    else if (!mesh.indices.empty())
    {
        for (int i = 0; i < mesh.indices.size(); i += 3)
        {
            pipeline.AddVertex(DrawMode::Triangles, vertices[mesh.indices[i]].Transformed(transform), normals[mesh.indices[i]], GET_VERTEX_TEXCOORD(mesh.indices[i]), GET_VERTEX_COLOR(mesh.indices[i]) * colDiffuse);
            pipeline.AddVertex(DrawMode::Triangles, vertices[mesh.indices[i + 1]].Transformed(transform), normals[mesh.indices[i + 1]], GET_VERTEX_TEXCOORD(mesh.indices[i + 1]), GET_VERTEX_COLOR(mesh.indices[i + 1]) * colDiffuse);
            pipeline.AddVertex(DrawMode::Triangles, vertices[mesh.indices[i + 2]].Transformed(transform), normals[mesh.indices[i + 2]], GET_VERTEX_TEXCOORD(mesh.indices[i + 2]), GET_VERTEX_COLOR(mesh.indices[i + 2]) * colDiffuse);
            pipeline.ProcessAndRender(*state.currentFramebuffer, mvp, viewport, matShader, mapDiffuse, state.depthTesting);
        }
    }
    else
    {
        for (int i = 0; i < mesh.numVertices; i += 3)
        {
            pipeline.AddVertex(DrawMode::Triangles, vertices[i].Transformed(transform), normals[i], GET_VERTEX_TEXCOORD(i), GET_VERTEX_COLOR(i) * colDiffuse);
            pipeline.AddVertex(DrawMode::Triangles, vertices[i + 1].Transformed(transform), normals[i + 1], GET_VERTEX_TEXCOORD(i + 1), GET_VERTEX_COLOR(i + 1) * colDiffuse);
            pipeline.AddVertex(DrawMode::Triangles, vertices[i + 2].Transformed(transform), normals[i + 2], GET_VERTEX_TEXCOORD(i + 2), GET_VERTEX_COLOR(i + 2) * colDiffuse);
            pipeline.ProcessAndRender(*state.currentFramebuffer, mvp, viewport, matShader, mapDiffuse, state.depthTesting);
        }
    }
}

void sr::Context::Begin(DrawMode mode)
{
    state.currentDrawMode = mode;
    state.renderBeginned = true;
}

void sr::Context::End()
{
    state.renderBeginned = false;
}

void sr::Context::SetTexture(const gfx::Surface& texture)
{
    state.image = &texture;
}

void sr::Context::SetTexture(const gfx::Surface* texture)
{
    state.image = texture;
}

void sr::Context::SetDefaultTexture()
{
    this->SetTexture(nullptr);
}

void sr::Context::UnsetTexture()
{
    this->SetTexture(nullptr);
}

void sr::Context::SetShader(sr::Shader* shader)
{
    if (shader == nullptr)
    {
        shader = &state.defaultShader;
    }

    state.currentShader = shader;
}

void sr::Context::Vertex(const math::Vec3& vertex)
{
    bool readyToRender = false;

    if (state.transformRequired)
    {
        readyToRender = pipeline.AddVertex(state.currentDrawMode, vertex.Transformed(state.transform), state.normal, state.texcoord, state.color);
    }
    else
    {
        readyToRender = pipeline.AddVertex(state.currentDrawMode, vertex, state.normal, state.texcoord, state.color);
    }

    if (readyToRender)
    {
        pipeline.ProcessAndRender(*state.currentFramebuffer, state.modelview * state.projection, { state.viewport.x, state.viewport.y, state.viewport.w - 1, state.viewport.h -1  }, state.currentShader, state.image, state.depthTesting);
    }
}

void sr::Context::Vertex(const math::Vec2& vertex)
{
    this->Vertex(math::Vec3(vertex));
}

void sr::Context::Vertex(float x, float y, float z)
{
    this->Vertex(math::Vec3(x, y, z));
}

void sr::Context::Vertex(float x, float y)
{
    this->Vertex(math::Vec3(x, y, 0));
}

void sr::Context::Normal(const math::Vec3& normal)
{
    state.normal = normal;
}

void sr::Context::Normal(float x, float y, float z)
{
    state.normal.x = x;
    state.normal.y = y;
    state.normal.z = z;
}

void sr::Context::TexCoord(const math::Vec2& texcoord)
{
    state.texcoord = texcoord;
}

void sr::Context::TexCoord(float u, float v)
{
    state.texcoord.x = u;
    state.texcoord.y = v;
}

void sr::Context::Color(const gfx::Color& color)
{
    state.color = color;
}

void sr::Context::Color(const math::Vec4& color)
{
    state.color.r = static_cast<Uint8>(color.x * 255);
    state.color.g = static_cast<Uint8>(color.y * 255);
    state.color.b = static_cast<Uint8>(color.z * 255);
    state.color.a = static_cast<Uint8>(color.w * 255);
}

void sr::Context::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    state.color.r = r;
    state.color.g = g;
    state.color.b = b;
    state.color.a = a;
}

void sr::Context::Color(float x, float y, float z, float w)
{
    state.color.r = static_cast<Uint8>(x * 255);
    state.color.g = static_cast<Uint8>(y * 255);
    state.color.b = static_cast<Uint8>(z * 255);
    state.color.a = static_cast<Uint8>(w * 255);
}