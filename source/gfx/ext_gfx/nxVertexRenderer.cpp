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

#include "gfx/ext_gfx/nxVertexRenderer.hpp"

using namespace nexus;

gfx::VertexRenderer::VertexRenderer(Renderer& renderer, std::size_t size)
: renderer(renderer)
{
    if (size) vertices.reserve(size);
}

void gfx::VertexRenderer::Begin(DrawMode mode)
{
    this->mode = mode;
}

void gfx::VertexRenderer::End()
{
    switch (mode)
    {
        case Lines:
        {
            gfx::Color pCol = renderer.GetColor();
            for (int i = 0; i < vertices.size(); i += 2)
            {
                // WARNING: No color interpolation for line drawing
                renderer.SetColor(vertices[i].color * vertices[i + 1].color);
                renderer.DrawLine(vertices[i].position, vertices[i + 1].position);
            }
            renderer.SetColor(pCol);
        }
        break;

        case Quads:
        case Triangles:
            renderer.DrawGeometry(vertices, texture);
            break;
    }

    vertices.clear();
}

void gfx::VertexRenderer::PushMatrix()
{
    if (matrixStackCounter >= matrixStack.size())
    {
        throw core::NexusException("sr::Context::PushMatrix", core::TextFormat("Matrix stack overflow (MAX=%i)", matrixStack.size()));
    }

    matrixStack[matrixStackCounter++] = transform;
    transformRequired = true;
}

void gfx::VertexRenderer::PopMatrix()
{
    if (matrixStackCounter > 0) transform = matrixStack[--matrixStackCounter];
    if (matrixStackCounter == 0) transformRequired = false;
}

void gfx::VertexRenderer::LoadIdentity()
{
    transform = math::Mat3::Identity();
}

void gfx::VertexRenderer::Translate(float x, float y)
{
    // NOTE: We transpose matrix with multiplication order
    transform = math::Mat3::Translate(x, y) * transform;
}

void gfx::VertexRenderer::Translate(const math::Vec2& v)
{
    this->Translate(v.x, v.y);
}

void gfx::VertexRenderer::Rotate(float angle)
{
    // NOTE: We transpose matrix with multiplication order
    transform = math::Mat3::Rotate(angle * math::Deg2Rad) * transform;
}

void gfx::VertexRenderer::Scale(float sx, float sy)
{
    // NOTE: We transpose matrix with multiplication order
    transform = math::Mat3::Scale(sx, sy) * transform;
}

void gfx::VertexRenderer::Scale(const math::Vec3& v)
{
    this->Scale(v.x, v.y);
}

void gfx::VertexRenderer::MultMatrix(const math::Mat3& mat)
{
    transform *= mat;
}

void gfx::VertexRenderer::Vertex(math::Vec2 vertex)
{
    if (transform) vertex.Transform(transform);

    if (mode == DrawMode::Quads)
    {
        quadStack[quadStackCounter++] = { vertex, color, texcoord };

        if (quadStackCounter == static_cast<int>(DrawMode::Quads))
        {
            vertices.push_back(quadStack[0]);
            vertices.push_back(quadStack[1]);
            vertices.push_back(quadStack[2]);

            vertices.push_back(quadStack[0]);
            vertices.push_back(quadStack[2]);
            vertices.push_back(quadStack[3]);

            quadStackCounter = 0;
        }
    }
    else
    {
        vertices.emplace_back(vertex, color, texcoord);
    }
}

void gfx::VertexRenderer::Vertex(float x, float y)
{
    this->Vertex({ x, y });
}

void gfx::VertexRenderer::TexCoord(const math::Vec2& texcoord)
{
    this->texcoord = texcoord;
}

void gfx::VertexRenderer::TexCoord(float u, float v)
{
    this->texcoord = { u, v };
}

void gfx::VertexRenderer::Color(const gfx::Color& color)
{
    this->color = color;
}

void gfx::VertexRenderer::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    this->color = { r, g, b, a };
}

void gfx::VertexRenderer::SetTexture(SDL_Texture* texture)
{
    this->texture = texture;
}
