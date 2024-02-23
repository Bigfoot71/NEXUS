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

#include "gapi/cmn_impl/nxPrimitives3D.hpp"

using namespace nexus;

void _gapi_primitives_3d::DrawLine3D(gapi::Context& ctx, const math::Vec3& startPos, const math::Vec3& endPos, const gfx::Color& color)
{
    ctx.Begin(gapi::DrawMode::Lines);
        ctx.Color(color);
        ctx.Vertex(startPos.x, startPos.y, startPos.z);
        ctx.Vertex(endPos.x, endPos.y, endPos.z);
    ctx.End();
}

void _gapi_primitives_3d::DrawPoint3D(gapi::Context& ctx, const math::Vec3& position, const gfx::Color& color)
{
    ctx.PushMatrix();
        ctx.Translate(position.x, position.y, position.z);
        ctx.Begin(gapi::DrawMode::Lines);
            ctx.Color(color);
            ctx.Vertex(0.0f, 0.0f, 0.0f);
            ctx.Vertex(0.0f, 0.0f, 0.1f);
        ctx.End();
    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawCircle3D(gapi::Context& ctx, const math::Vec3& center, float radius, const math::Vec3& rotationAxis, float rotationAngle, const gfx::Color& color)
{
    ctx.PushMatrix();

        ctx.Translate(center.x, center.y, center.z);
        ctx.Rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

        ctx.Begin(gapi::DrawMode::Lines);
            for (float i = 0; i < math::Tau; i += CircleSegStepLen)
            {
                ctx.Color(color);

                ctx.Vertex(std::sin(i) * radius, std::cos(i) * radius, 0.0f);
                ctx.Vertex(std::sin(i + CircleSegStepLen) * radius, std::cos(i + CircleSegStepLen) * radius, 0.0f);
            }
        ctx.End();

    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawEllipse3D(gapi::Context& ctx, const math::Vec3& center, float rx, float ry, const math::Vec3& rotationAxis, float rotationAngle, const gfx::Color& color)
{
    ctx.PushMatrix();

        ctx.Translate(center.x, center.y, center.z);
        ctx.Rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

        ctx.Begin(gapi::DrawMode::Lines);
            for (float i = 0; i < math::Tau; i += CircleSegStepLen)
            {
                ctx.Color(color);

                ctx.Vertex(std::sin(i) * rx, std::cos(i) * ry, 0.0f);
                ctx.Vertex(std::sin(i + CircleSegStepLen) * rx, std::cos(i + CircleSegStepLen) * ry, 0.0f);
            }
        ctx.End();

    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawTriangle3D(gapi::Context& ctx, const math::Vec3& v1, const math::Vec3& v2, const math::Vec3& v3, const gfx::Color& color)
{
    ctx.Begin(gapi::DrawMode::Triangles);
        ctx.Color(color);
        ctx.Vertex(v1.x, v1.y, v1.z);
        ctx.Vertex(v2.x, v2.y, v2.z);
        ctx.Vertex(v3.x, v3.y, v3.z);
    ctx.End();
}

void _gapi_primitives_3d::DrawTriangleStrip3D(gapi::Context& ctx, const std::vector<math::Vec3>& points, const gfx::Color& color)
{
    if (points.size() < 3) return;

    ctx.Begin(gapi::DrawMode::Triangles);
        ctx.Color(color);

        for (int i = 2; i < points.size(); i++)
        {
            if (i % 2 == 0)
            {
                ctx.Vertex(points[i].x, points[i].y, points[i].z);
                ctx.Vertex(points[i - 2].x, points[i - 2].y, points[i - 2].z);
                ctx.Vertex(points[i - 1].x, points[i - 1].y, points[i - 1].z);
            }
            else
            {
                ctx.Vertex(points[i].x, points[i].y, points[i].z);
                ctx.Vertex(points[i - 1].x, points[i - 1].y, points[i - 1].z);
                ctx.Vertex(points[i - 2].x, points[i - 2].y, points[i - 2].z);
            }
        }
    ctx.End();
}

void _gapi_primitives_3d::DrawCube(gapi::Context& ctx, const math::Vec3& position, float width, float height, float length, const gfx::Color& color)
{
    const float hw = width * 0.5f, hh = height * 0.5f, hl = length * 0.5f;

    ctx.Begin(gapi::DrawMode::Triangles);

        ctx.Color(color);

        /* --- Front face --- */

        ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom Left
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left

        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top Right
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right

        /* --- Back face --- */

        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom Left
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left
        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right

        ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top Right
        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left

        /* --- Top face --- */

        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Bottom Left
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Bottom Right

        ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top Right
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Bottom Right

        /* --- Bottom face --- */

        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Top Left
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right
        ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom Left

        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Top Right
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right
        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Top Left

        /* --- Right face --- */

        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right
        ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top Right
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top Left

        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Left
        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top Left

        /* --- Left face --- */

        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom Right
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Right

        ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom Left
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left
        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom Right

    ctx.End();
}

void _gapi_primitives_3d::DrawCubeWires(gapi::Context& ctx, const math::Vec3& position, float width, float height, float length, const gfx::Color& color)
{
    const float hw = width * 0.5f, hh = height * 0.5f, hl = length * 0.5f;

    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        /* --- Front face --- */

        // Bottom line
        ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom left
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom right

        // Left line
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom right
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top right

        // Top line
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top right
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top left

        // Right line
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top left
        ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom left

        /* --- Back face --- */

        // Bottom line
        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom left
        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom right

        // Left line
        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom right
        ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top right

        // Top line
        ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top right
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top left

        // Right line
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top left
        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom left

        /* --- Top face --- */

        // Left line
        ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top left front
        ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top left back

        // Right line
        ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top right front
        ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top right back

        /* --- Bottom face --- */

        // Left line
        ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Top left front
        ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Top left back

        // Right line
        ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Top right front
        ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Top right back

    ctx.End();
}

void _gapi_primitives_3d::DrawSphere(gapi::Context& ctx, const math::Vec3& centerPos, float radius, int rings, int slices, const gfx::Color& color)
{
    ctx.PushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> translate)
        ctx.Translate(centerPos.x, centerPos.y, centerPos.z);
        ctx.Scale(radius, radius, radius);

        ctx.Begin(gapi::DrawMode::Triangles);
            ctx.Color(color);

            for (int i = 0; i < (rings + 2); i++)
            {
                for (int j = 0; j < slices; j++)
                {
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::cos(math::Tau * j / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * (j + 1) / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * (j + 1) / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * j / slices));

                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::cos(math::Tau * j / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i)) * std::sin(math::Tau * (j + 1) / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i)) * std::cos(math::Tau * (j + 1) / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * (j + 1) / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * (j + 1) / slices));
                }
            }
        ctx.End();
    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawSphereWires(gapi::Context& ctx, const math::Vec3& centerPos, float radius, int rings, int slices, const gfx::Color& color)
{
    ctx.PushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> translate)
        ctx.Translate(centerPos.x, centerPos.y, centerPos.z);
        ctx.Scale(radius, radius, radius);

        ctx.Begin(gapi::DrawMode::Lines);
            ctx.Color(color);

            for (int i = 0; i < (rings + 2); i++)
            {
                for (int j = 0; j < slices; j++)
                {
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::cos(math::Tau * j / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * (j + 1) / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * (j + 1) / slices));

                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * (j + 1) / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * (j + 1) / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * j / slices));

                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * (i + 1)) * std::cos(math::Tau * j / slices));
                    ctx.Vertex(std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::sin(math::Tau * j / slices),
                                std::sin((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i),
                                std::cos((math::Pi + math::Pi / 2) + (math::Pi / (rings + 1)) * i) * std::cos(math::Tau * j / slices));
                }
            }
        ctx.End();
    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawCylinder(gapi::Context& ctx, const math::Vec3& position, float radiusTop, float radiusBottom, float height, int sides, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    ctx.PushMatrix();
        ctx.Translate(position.x, position.y, position.z);

        ctx.Begin(gapi::DrawMode::Triangles);
            ctx.Color(color);

            if (radiusTop > 0)
            {
                // Draw Body
                for (float i = 0; i < math::Tau; i += math::Tau / sides)
                {
                    ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom); //Bottom Left
                    ctx.Vertex(std::sin(i + math::Tau / sides) * radiusBottom, 0, std::cos(i + math::Tau / sides) * radiusBottom); //Bottom Right
                    ctx.Vertex(std::sin(i + math::Tau / sides) * radiusTop, height, std::cos(i + math::Tau / sides) * radiusTop); //Top Right

                    ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop); //Top Left
                    ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom); //Bottom Left
                    ctx.Vertex(std::sin(i + math::Tau / sides) * radiusTop, height, std::cos(i + math::Tau / sides) * radiusTop); //Top Right
                }

                // Draw Cap
                for (float i = 0; i < math::Tau; i += math::Tau / sides)
                {
                    ctx.Vertex(0, height, 0);
                    ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop);
                    ctx.Vertex(std::sin(i + math::Tau / sides) * radiusTop, height, std::cos(i + math::Tau / sides) * radiusTop);
                }
            }
            else
            {
                // Draw Cone
                for (float i = 0; i < math::Tau; i += math::Tau / sides)
                {
                    ctx.Vertex(0, height, 0);
                    ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
                    ctx.Vertex(std::sin(i + math::Tau / sides) * radiusBottom, 0, std::cos(i + math::Tau / sides) * radiusBottom);
                }
            }

            // Draw Base
            for (float i = 0; i < math::Tau; i += math::Tau / sides)
            {
                ctx.Vertex(0, 0, 0);
                ctx.Vertex(std::sin(i + math::Tau / sides) * radiusBottom, 0, std::cos(i + math::Tau / sides) * radiusBottom);
                ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
            }
        ctx.End();
    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawCylinder(gapi::Context& ctx, const math::Vec3& startPos, const math::Vec3& endPos, float startRadius, float endRadius, int sides, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };
    if ((direction.x == 0) && (direction.y == 0) && (direction.z == 0)) return;

    // Construct a basis of the base and the top face:
    math::Vec3 b1 = math::Vec3::Perpendicular(direction).Normalized();
    math::Vec3 b2 = b1.Cross(direction).Normalized();

    const float baseAngle = math::Tau / sides;

    ctx.Begin(gapi::DrawMode::Triangles);
        ctx.Color(color);

        for (int i = 0; i < sides; i++)
        {
            // Compute the four vertices
            float s1 = std::sin(baseAngle * i) * startRadius;
            float c1 = std::cos(baseAngle * i) * startRadius;
            math::Vec3 w1 = { startPos.x + s1 * b1.x + c1 * b2.x, startPos.y + s1 * b1.y + c1 * b2.y, startPos.z + s1 * b1.z + c1 * b2.z };

            float s2 = std::sin(baseAngle * (i + 1)) * startRadius;
            float c2 = std::cos(baseAngle * (i + 1)) * startRadius;
            math::Vec3 w2 = { startPos.x + s2 * b1.x + c2 * b2.x, startPos.y + s2 * b1.y + c2 * b2.y, startPos.z + s2 * b1.z + c2 * b2.z };

            float s3 = std::sin(baseAngle * i) * endRadius;
            float c3 = std::cos(baseAngle * i) * endRadius;
            math::Vec3 w3 = { endPos.x + s3 * b1.x + c3 * b2.x, endPos.y + s3 * b1.y + c3 * b2.y, endPos.z + s3 * b1.z + c3 * b2.z };

            float s4 = std::sin(baseAngle * (i + 1)) * endRadius;
            float c4 = std::cos(baseAngle * (i + 1)) * endRadius;
            math::Vec3 w4 = { endPos.x + s4 * b1.x + c4 * b2.x, endPos.y + s4 * b1.y + c4 * b2.y, endPos.z + s4 * b1.z + c4 * b2.z };

            if (startRadius > 0)
            {
                ctx.Vertex(startPos.x, startPos.y, startPos.z);
                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w1.x, w1.y, w1.z);
            }

            ctx.Vertex(w1.x, w1.y, w1.z);
            ctx.Vertex(w2.x, w2.y, w2.z);
            ctx.Vertex(w3.x, w3.y, w3.z);

            ctx.Vertex(w2.x, w2.y, w2.z);
            ctx.Vertex(w4.x, w4.y, w4.z);
            ctx.Vertex(w3.x, w3.y, w3.z);

            if (endRadius > 0)
            {
                ctx.Vertex(endPos.x, endPos.y, endPos.z);
                ctx.Vertex(w3.x, w3.y, w3.z);
                ctx.Vertex(w4.x, w4.y, w4.z);
            }
        }
    ctx.End();
}

void _gapi_primitives_3d::DrawCylinderWires(gapi::Context& ctx, const math::Vec3& position, float radiusTop, float radiusBottom, float height, int sides, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    ctx.PushMatrix();
        ctx.Translate(position.x, position.y, position.z);

        ctx.Begin(gapi::DrawMode::Lines);
            ctx.Color(color);

            for (float i = 0; i < math::Tau; i += math::Tau / sides)
            {
                ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
                ctx.Vertex(std::sin(i + math::Tau / sides) * radiusBottom, 0, std::cos(i + math::Tau / sides) * radiusBottom);

                ctx.Vertex(std::sin(i + math::Tau / sides) * radiusBottom, 0, std::cos(i + math::Tau / sides) * radiusBottom);
                ctx.Vertex(std::sin(i + math::Tau / sides) * radiusTop, height, std::cos(i + math::Tau / sides) * radiusTop);

                ctx.Vertex(std::sin(i + math::Tau / sides) * radiusTop, height, std::cos(i + math::Tau / sides) * radiusTop);
                ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop);

                ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop);
                ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
            }
        ctx.End();
    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawCylinderWires(gapi::Context& ctx, const math::Vec3& startPos, const math::Vec3& endPos, float startRadius, float endRadius, int sides, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };
    if ((direction.x == 0) && (direction.y == 0) && (direction.z == 0))return;

    // Construct a basis of the base and the top face:
    math::Vec3 b1 = math::Vec3::Perpendicular(direction).Normalized();
    math::Vec3 b2 = b1.Cross(direction).Normalized();

    const float baseAngle = math::Tau / sides;

    ctx.Begin(gapi::DrawMode::Lines);
        ctx.Color(color);

        for (int i = 0; i < sides; i++)
        {
            // compute the four vertices
            float s1 = std::sin(baseAngle * i) * startRadius;
            float c1 = std::cos(baseAngle * i) * startRadius;
            math::Vec3 w1 = { startPos.x + s1 * b1.x + c1 * b2.x, startPos.y + s1 * b1.y + c1 * b2.y, startPos.z + s1 * b1.z + c1 * b2.z };

            float s2 = std::sin(baseAngle * (i + 1)) * startRadius;
            float c2 = std::cos(baseAngle * (i + 1)) * startRadius;
            math::Vec3 w2 = { startPos.x + s2 * b1.x + c2 * b2.x, startPos.y + s2 * b1.y + c2 * b2.y, startPos.z + s2 * b1.z + c2 * b2.z };

            float s3 = std::sin(baseAngle * i) * endRadius;
            float c3 = std::cos(baseAngle * i) * endRadius;
            math::Vec3 w3 = { endPos.x + s3 * b1.x + c3 * b2.x, endPos.y + s3 * b1.y + c3 * b2.y, endPos.z + s3 * b1.z + c3 * b2.z };

            float s4 = std::sin(baseAngle * (i + 1)) * endRadius;
            float c4 = std::cos(baseAngle * (i + 1)) * endRadius;
            math::Vec3 w4 = { endPos.x + s4 * b1.x + c4 * b2.x, endPos.y + s4 * b1.y + c4 * b2.y, endPos.z + s4 * b1.z + c4 * b2.z };

            ctx.Vertex(w1.x, w1.y, w1.z);
            ctx.Vertex(w2.x, w2.y, w2.z);

            ctx.Vertex(w1.x, w1.y, w1.z);
            ctx.Vertex(w3.x, w3.y, w3.z);

            ctx.Vertex(w3.x, w3.y, w3.z);
            ctx.Vertex(w4.x, w4.y, w4.z);
        }
    ctx.End();
}

void _gapi_primitives_3d::DrawCapsule(gapi::Context& ctx, const math::Vec3& startPos, const math::Vec3& endPos, float radius, int slices, int rings, const gfx::Color& color)
{
    if (slices < 3) slices = 3;

    math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };

    // Draw a sphere if start and end points are the same
    bool sphereCase = (direction.x == 0) && (direction.y == 0) && (direction.z == 0);
    if (sphereCase) direction = (math::Vec3){0.0f, 1.0f, 0.0f};

    // Construct a basis of the base and the caps:
    math::Vec3 b0 = direction.Normalized();
    math::Vec3 b1 = math::Vec3::Perpendicular(direction).Normalized();
    math::Vec3 b2 = b1.Cross(direction).Normalized();
    math::Vec3 capCenter = endPos;

    float baseSliceAngle = math::Tau / slices;
    float baseRingAngle  = math::DegToRad(90.0f) / rings;

    ctx.Begin(gapi::DrawMode::Triangles);

        ctx.Color(color);

        // Render both caps
        for (int c = 0; c < 2; c++)
        {
            for (int i = 0; i < rings; i++)
            {
                for (int j = 0; j < slices; j++)
                {

                    // We build up the rings from capCenter in the direction of the 'direction' vector we computed earlier

                    // As we iterate through the rings they must be placed higher above the center, the height we need is sin(angle(i))
                    // As we iterate through the rings they must get smaller by the cos(angle(i))

                    // Compute the four vertices
                    float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                    float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                    math::Vec3 w1 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin1 * b1.x + ringCos1 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin1 * b1.y + ringCos1 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin1 * b1.z + ringCos1 * b2.z) * radius
                    };
                    float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                    float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                    math::Vec3 w2 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin2 * b1.x + ringCos2 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin2 * b1.y + ringCos2 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin2 * b1.z + ringCos2 * b2.z) * radius
                    };

                    float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                    float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                    math::Vec3 w3 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin3 * b1.x + ringCos3 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin3 * b1.y + ringCos3 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin3 * b1.z + ringCos3 * b2.z) * radius
                    };
                    float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                    float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                    math::Vec3 w4 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin4 * b1.x + ringCos4 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin4 * b1.y + ringCos4 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin4 * b1.z + ringCos4 * b2.z) * radius
                    };

                    // Make sure cap triangle normals are facing outwards
                    if(c == 0)
                    {
                        ctx.Vertex(w1.x, w1.y, w1.z);
                        ctx.Vertex(w2.x, w2.y, w2.z);
                        ctx.Vertex(w3.x, w3.y, w3.z);

                        ctx.Vertex(w2.x, w2.y, w2.z);
                        ctx.Vertex(w4.x, w4.y, w4.z);
                        ctx.Vertex(w3.x, w3.y, w3.z);
                    }
                    else
                    {
                        ctx.Vertex(w1.x, w1.y, w1.z);
                        ctx.Vertex(w3.x, w3.y, w3.z);
                        ctx.Vertex(w2.x, w2.y, w2.z);

                        ctx.Vertex(w2.x, w2.y, w2.z);
                        ctx.Vertex(w3.x, w3.y, w3.z);
                        ctx.Vertex(w4.x, w4.y, w4.z);
                    }
                }
            }
            capCenter = startPos;
            b0 *= -1.0f;
        }

        // Cender middle
        if (!sphereCase)
        {
            for (int j = 0; j < slices; j++)
            {
                // Compute the four vertices
                float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * radius;
                float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * radius;
                math::Vec3 w1 = {
                    startPos.x + ringSin1 * b1.x + ringCos1 * b2.x,
                    startPos.y + ringSin1 * b1.y + ringCos1 * b2.y,
                    startPos.z + ringSin1 * b1.z + ringCos1 * b2.z
                };
                float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * radius;
                float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * radius;
                math::Vec3 w2 = {
                    startPos.x + ringSin2 * b1.x + ringCos2 * b2.x,
                    startPos.y + ringSin2 * b1.y + ringCos2 * b2.y,
                    startPos.z + ringSin2 * b1.z + ringCos2 * b2.z
                };

                float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * radius;
                float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * radius;
                math::Vec3 w3 = {
                    endPos.x + ringSin3 * b1.x + ringCos3 * b2.x,
                    endPos.y + ringSin3 * b1.y + ringCos3 * b2.y,
                    endPos.z + ringSin3 * b1.z + ringCos3 * b2.z
                };
                float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * radius;
                float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * radius;
                math::Vec3 w4 = {
                    endPos.x + ringSin4 * b1.x + ringCos4 * b2.x,
                    endPos.y + ringSin4 * b1.y + ringCos4 * b2.y,
                    endPos.z + ringSin4 * b1.z + ringCos4 * b2.z
                };

                ctx.Vertex(w1.x, w1.y, w1.z);
                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w3.x, w3.y, w3.z);

                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w4.x, w4.y, w4.z);
                ctx.Vertex(w3.x, w3.y, w3.z);
            }
        }

    ctx.End();
}

void _gapi_primitives_3d::DrawCapsuleWires(gapi::Context& ctx, const math::Vec3& startPos, const math::Vec3& endPos, float radius, int slices, int rings, const gfx::Color& color)
{
    if (slices < 3) slices = 3;

    math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };

    // draw a sphere if start and end points are the same
    bool sphereCase = (direction.x == 0) && (direction.y == 0) && (direction.z == 0);
    if (sphereCase) direction = (math::Vec3){0.0f, 1.0f, 0.0f};

    // Construct a basis of the base and the caps:
    math::Vec3 b0 = direction.Normalized();
    math::Vec3 b1 = math::Vec3::Perpendicular(direction).Normalized();
    math::Vec3 b2 = b1.Cross(direction).Normalized();
    math::Vec3 capCenter = endPos;

    float baseSliceAngle = math::Tau / slices;
    float baseRingAngle  = math::DegToRad(90.0f) / rings;

    ctx.Begin(gapi::DrawMode::Lines);
        ctx.Color(color);

        // render both caps
        for (int c = 0; c < 2; c++)
        {
            for (int i = 0; i < rings; i++)
            {
                for (int j = 0; j < slices; j++)
                {

                    // we build up the rings from capCenter in the direction of the 'direction' vector we computed earlier

                    // as we iterate through the rings they must be placed higher above the center, the height we need is sin(angle(i))
                    // as we iterate through the rings they must get smaller by the cos(angle(i))

                    // compute the four vertices
                    float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                    float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                    math::Vec3 w1 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin1 * b1.x + ringCos1 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin1 * b1.y + ringCos1 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin1 * b1.z + ringCos1 * b2.z) * radius
                    };
                    float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                    float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                    math::Vec3 w2 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin2 * b1.x + ringCos2 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin2 * b1.y + ringCos2 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin2 * b1.z + ringCos2 * b2.z) * radius
                    };

                    float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                    float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                    math::Vec3 w3 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin3 * b1.x + ringCos3 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin3 * b1.y + ringCos3 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin3 * b1.z + ringCos3 * b2.z) * radius
                    };
                    float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                    float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                    math::Vec3 w4 = (math::Vec3){
                        capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin4 * b1.x + ringCos4 * b2.x) * radius,
                        capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin4 * b1.y + ringCos4 * b2.y) * radius,
                        capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin4 * b1.z + ringCos4 * b2.z) * radius
                    };

                    ctx.Vertex(w1.x, w1.y, w1.z);
                    ctx.Vertex(w2.x, w2.y, w2.z);

                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);

                    ctx.Vertex(w1.x, w1.y, w1.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);

                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w4.x, w4.y, w4.z);

                    ctx.Vertex(w3.x, w3.y, w3.z);
                    ctx.Vertex(w4.x, w4.y, w4.z);
                }
            }
            capCenter = startPos;
            b0 *= -1.0f;
        }
        // render middle
        if (!sphereCase)
        {
            for (int j = 0; j < slices; j++)
            {
                // compute the four vertices
                float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * radius;
                float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * radius;
                math::Vec3 w1 = {
                    startPos.x + ringSin1 * b1.x + ringCos1 * b2.x,
                    startPos.y + ringSin1 * b1.y + ringCos1 * b2.y,
                    startPos.z + ringSin1 * b1.z + ringCos1 * b2.z
                };
                float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * radius;
                float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * radius;
                math::Vec3 w2 = {
                    startPos.x + ringSin2 * b1.x + ringCos2 * b2.x,
                    startPos.y + ringSin2 * b1.y + ringCos2 * b2.y,
                    startPos.z + ringSin2 * b1.z + ringCos2 * b2.z
                };

                float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * radius;
                float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * radius;
                math::Vec3 w3 = {
                    endPos.x + ringSin3 * b1.x + ringCos3 * b2.x,
                    endPos.y + ringSin3 * b1.y + ringCos3 * b2.y,
                    endPos.z + ringSin3 * b1.z + ringCos3 * b2.z
                };
                float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * radius;
                float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * radius;
                math::Vec3 w4 = {
                    endPos.x + ringSin4 * b1.x + ringCos4 * b2.x,
                    endPos.y + ringSin4 * b1.y + ringCos4 * b2.y,
                    endPos.z + ringSin4 * b1.z + ringCos4 * b2.z
                };

                ctx.Vertex(w1.x, w1.y, w1.z);
                ctx.Vertex(w3.x, w3.y, w3.z);

                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w4.x, w4.y, w4.z);

                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w3.x, w3.y, w3.z);
            }
        }
    ctx.End();
}

void _gapi_primitives_3d::DrawPlane(gapi::Context& ctx, const math::Vec3& centerPos, const math::Vec2& size, const gfx::Color& color)
{
    // NOTE: Plane is always created on XZ ground
    ctx.PushMatrix();
        ctx.Translate(centerPos.x, centerPos.y, centerPos.z);
        ctx.Scale(size.x, 1.0f, size.y);

        ctx.Begin(gapi::DrawMode::Quads);
            ctx.Color(color);
            ctx.Normal(0.0f, 1.0f, 0.0f);

            ctx.Vertex(-0.5f, 0.0f, -0.5f);
            ctx.Vertex(-0.5f, 0.0f, 0.5f);
            ctx.Vertex(0.5f, 0.0f, 0.5f);
            ctx.Vertex(0.5f, 0.0f, -0.5f);
        ctx.End();
    ctx.PopMatrix();
}

void _gapi_primitives_3d::DrawRay(gapi::Context& ctx, const shape3D::Ray& ray, const gfx::Color& color)
{
    constexpr float scale = 10000;

    ctx.Begin(gapi::DrawMode::Lines);
        ctx.Color(color);
        ctx.Vertex(ray.position);
        ctx.Vertex(ray.position + ray.direction * scale);
    ctx.End();
}

void _gapi_primitives_3d::DrawGrid(gapi::Context& ctx, int slices, float spacing)
{
    const int halfSlices = slices / 2;

    ctx.Begin(gapi::DrawMode::Lines);
        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            if (i == 0)
            {
                ctx.Color(0.5f, 0.5f, 0.5f, 1.0f);
                ctx.Color(0.5f, 0.5f, 0.5f, 1.0f);
                ctx.Color(0.5f, 0.5f, 0.5f, 1.0f);
                ctx.Color(0.5f, 0.5f, 0.5f, 1.0f);
            }
            else
            {
                ctx.Color(0.75f, 0.75f, 0.75f, 1.0f);
                ctx.Color(0.75f, 0.75f, 0.75f, 1.0f);
                ctx.Color(0.75f, 0.75f, 0.75f, 1.0f);
                ctx.Color(0.75f, 0.75f, 0.75f, 1.0f);
            }

            ctx.Vertex(i * spacing, 0.0f, -halfSlices * spacing);
            ctx.Vertex(i * spacing, 0.0f, halfSlices * spacing);

            ctx.Vertex(-halfSlices * spacing, 0.0f, i * spacing);
            ctx.Vertex(halfSlices * spacing, 0.0f, i * spacing);
        }
    ctx.End();
}
