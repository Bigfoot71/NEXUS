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

#include "shape/3D/nxMesh.hpp"

#include "shape/3D/nxCylinder.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxAABB.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxRay.hpp"
#include "math/nxMath.hpp"
#include "math/nxVec3.hpp"

using namespace nexus;


/* Public Implementation Mesh */

shape3D::Mesh::Mesh(const Mesh& other)
: positions(other.positions)
, texcoords(other.texcoords)
, normals(other.normals)
, colors(other.colors)
, tangents(other.tangents)
, texcoords2(other.texcoords2)
, indices(other.indices)
, numVertices(other.numVertices)
, numFaces(other.numFaces)
{ }

shape3D::Mesh::Mesh(Mesh&& other) noexcept
: positions(std::move(other.positions))
, texcoords(std::move(other.texcoords))
, normals(std::move(other.normals))
, colors(std::move(other.colors))
, tangents(std::move(other.tangents))
, texcoords2(std::move(other.texcoords2))
, indices(std::move(other.indices))
, numVertices(std::exchange(other.numVertices, 0))
, numFaces(std::exchange(other.numFaces, 0))
{ }

shape3D::Mesh& shape3D::Mesh::operator=(const Mesh& other)
{
    if (this != &other)
    {
        numVertices   = other.numVertices;
        numFaces      = other.numFaces;
        positions     = other.positions;
        texcoords     = other.texcoords;
        normals       = other.normals;
        colors        = other.colors;
        tangents      = other.tangents;
        texcoords2    = other.texcoords2;
        indices       = other.indices;
    }
    return *this;
}

shape3D::Mesh& shape3D::Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        numVertices   = std::exchange(other.numVertices, 0);
        numFaces      = std::exchange(other.numFaces, 0);
        positions     = std::move(other.positions);
        texcoords     = std::move(other.texcoords);
        normals       = std::move(other.normals);
        colors        = std::move(other.colors);
        tangents      = std::move(other.tangents);
        texcoords2    = std::move(other.texcoords2);
        indices       = std::move(other.indices);
    }
    return *this;
}

shape3D::AABB shape3D::Mesh::GetAABB() const
{
    math::Vec3 minVertex(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    math::Vec3 maxVertex(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for (const auto &vertex : positions)
    {
        minVertex = minVertex.Min(vertex);
        maxVertex = maxVertex.Max(vertex);
    }

    return shape3D::AABB(minVertex, maxVertex);
}

void shape3D::Mesh::GenTangents()
{
    if (positions.empty() || texcoords.empty())
    {
        throw core::NexusException("Mesh", "Tangents generation requires texcoord vertex attribute data.");
        return;
    }

    if (!tangents.empty()) tangents.clear();
    tangents.reserve(numVertices);

    std::vector<math::Vec3> tan1; tan1.reserve(numVertices);
    std::vector<math::Vec3> tan2; tan2.reserve(numVertices);

    if (indices.empty())
    {
        for (int i = 0; i < numVertices; i += 3)
        {
            const int i1 = i, i2 = i + 1, i3 = i + 2;

            math::Vec3 v21 = positions[i2] - positions[i1];
            math::Vec3 v31 = positions[i3] - positions[i1];
            math::Vec2 uv21 = texcoords[i2] - texcoords[i1];
            math::Vec2 uv31 = texcoords[i3] - texcoords[i1];

            float div = uv21.x * uv31.y - uv31.x * uv21.y;
            float r = (div == 0.0f) ? 0.0f : 1.0f / div;

            tan1.insert(tan1.end(), 3, {
                (uv31.y * v21.x - uv21.y * v31.x) * r,
                (uv31.y * v21.y - uv21.y * v31.y) * r,
                (uv31.y * v21.z - uv21.y * v31.z) * r
            });

            tan2.insert(tan2.end(), 3, {
                (uv21.x * v31.x - uv31.x * v21.x) * r,
                (uv21.x * v31.y - uv31.x * v21.y) * r,
                (uv21.x * v31.z - uv31.x * v21.z) * r
            });
        }
    }
    else
    {
        for (int i = 0; i < numVertices; i += 3)
        {
            const int i1 = indices[i], i2 = indices[i + 1], i3 = indices[i + 2];

            math::Vec3 v21 = positions[i2] - positions[i1];
            math::Vec3 v31 = positions[i3] - positions[i1];
            math::Vec2 uv21 = texcoords[i2] - texcoords[i1];
            math::Vec2 uv31 = texcoords[i3] - texcoords[i1];

            float div = uv21.x * uv31.y - uv31.x * uv21.y;
            float r = (div == 0.0f) ? 0.0f : 1.0f / div;

            tan1.insert(tan1.end(), 3, {
                (uv31.y * v21.x - uv21.y * v31.x) * r,
                (uv31.y * v21.y - uv21.y * v31.y) * r,
                (uv31.y * v21.z - uv21.y * v31.z) * r
            });

            tan2.insert(tan2.end(), 3, {
                (uv21.x * v31.x - uv31.x * v21.x) * r,
                (uv21.x * v31.y - uv31.x * v21.y) * r,
                (uv21.x * v31.z - uv31.x * v21.z) * r
            });
        }
    }

    for (int i = 0; i < positions.size(); i++)
    {
        const math::Vec3 &normal = normals[i], &tangent = tan1[i];
        tangents[i] = math::Vec4((tangent - normal * normal.Dot(tangent)).Normalized(),
            normal.Cross(tangent).Dot(tan2[i]) < 0.0f ? -1.0f : 1.0f);
    }
}


/* Public Implementation Mesh Generation */

shape3D::Mesh shape3D::Mesh::Polygon(int sides, float radius)
{
    Mesh mesh;

    sides = std::max(sides, 3);
    mesh.numVertices = sides * 3;
    mesh.numFaces = sides;

    // Vertices definition
    mesh.positions.resize(mesh.numVertices);

    float d = 0.0f, dStep = math::DegToRad(360.0f)/sides;
    for (int i = 0; i < mesh.numVertices - 2; i += 3)
    {
        mesh.positions[i]    = { 0.0f, 0.0f, 0.0f };
        mesh.positions[i+1]  = { std::sin(d)*radius, 0.0f, std::cos(d)*radius };
        mesh.positions[i+2]  = { std::sin(d+dStep)*radius, 0.0f, std::cos(d+dStep)*radius };
        d += dStep;
    }

    mesh.normals.resize(mesh.numVertices, math::Vec3(0, 1, 0));
    mesh.texcoords.resize(mesh.numVertices, math::Vec2(0, 0));

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Plane(float width, float length, int resX, int resZ)
{
    Mesh mesh;

    resX++, resZ++;
    mesh.numVertices = resX * resZ;

    mesh.positions.resize(mesh.numVertices);
    for (int z = 0; z < resZ; z++)
    {
        // [-length/2, length/2]
        float zPos = (static_cast<float>(z) / (resZ - 1) - 0.5f) * length;
        for (int x = 0; x < resX; x++)
        {
            // [-width/2, width/2]
            float xPos = (static_cast<float>(x) / (resX - 1) - 0.5f) * width;
            mesh.positions[x + z * resX] = { xPos, 0.0f, zPos };
        }
    }

    // Normals definition
    mesh.normals.resize(mesh.numVertices, math::Vec3(0, 1, 0));

    // TexCoords definition
    mesh.texcoords.resize(mesh.numVertices);
    for (int v = 0; v < resZ; v++)
    {
        for (int u = 0; u < resX; u++)
        {
            mesh.texcoords[u + v * resX] = {
                static_cast<float>(u) / (resX - 1),
                static_cast<float>(v) / (resZ - 1)
            };
        }
    }

    int numFaces = (resX - 1) * (resZ - 1);
    mesh.numFaces = 2 * numFaces;

    // Triangles definition (indices)
    mesh.indices.resize(6 * numFaces);
    for (int face = 0, t = 0; face < numFaces; face++)
    {
        // Retrieve lower left corner from face ind
        int i = face + face / (resX - 1);

        mesh.indices[t++] = i + resX;
        mesh.indices[t++] = i + 1;
        mesh.indices[t++] = i;

        mesh.indices[t++] = i + resX;
        mesh.indices[t++] = i + resX + 1;
        mesh.indices[t++] = i + 1;
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Cube(float width, float height, float length)
{
    Mesh mesh;

    mesh.numVertices = 24;
    mesh.numFaces = 12;

    mesh.positions = {
        { -width/2, -height/2, length/2 }, { width/2, -height/2, length/2 }, { width/2, height/2, length/2 },
        { -width/2, height/2, length/2 }, { -width/2, -height/2, -length/2 }, { -width/2, height/2, -length/2 },
        { width/2, height/2, -length/2 }, { width/2, -height/2, -length/2 }, { -width/2, height/2, -length/2 },
        { -width/2, height/2, length/2 }, { width/2, height/2, length/2 }, { width/2, height/2, -length/2 },
        { -width/2, -height/2, -length/2 }, { width/2, -height/2, -length/2 }, { width/2, -height/2, length/2 },
        { -width/2, -height/2, length/2 }, { width/2, -height/2, -length/2 }, { width/2, height/2, -length/2 },
        { width/2, height/2, length/2 }, { width/2, -height/2, length/2 }, { -width/2, -height/2, -length/2 },
        { -width/2, -height/2, length/2 }, { -width/2, height/2, length/2 }, { -width/2, height/2, -length/2 }
    };

    mesh.texcoords = {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
        { 1.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    mesh.normals = {
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f },
        { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f },
        { 0.0f,-1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }
    };

    // Indices can be initialized right now
    mesh.indices.reserve(36);
    for (int i = 0, j = 0; i < 36; i += 6, j++)
    {
        mesh.indices.push_back(4 * j);
        mesh.indices.push_back(4 * j + 1);
        mesh.indices.push_back(4 * j + 2);
        mesh.indices.push_back(4 * j);
        mesh.indices.push_back(4 * j + 2);
        mesh.indices.push_back(4 * j + 3);
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Sphere(float radius, int rings, int slices)
{
    Mesh mesh;

    rings = std::max(rings, 3);
    slices = std::max(slices, 3);

    mesh.numVertices = rings * slices;
    mesh.numFaces = 2 * (rings - 1) * slices;

    mesh.positions.reserve(mesh.numVertices);
    mesh.normals.reserve(mesh.numVertices);
    mesh.texcoords.reserve(mesh.numVertices);

    // Generate vertices
    for (int ring = 0; ring < rings; ring++)
    {
        const float theta = static_cast<float>(ring) / (rings - 1) * math::Pi;

        for (int slice = 0; slice < slices; slice++)
        {
            const float phi = static_cast<float>(slice) / (slices - 1) * math::Tau;

            const float nx = std::sin(theta) * std::cos(phi);
            const float nz = std::sin(theta) * std::sin(phi);
            const float ny = std::cos(theta);

            mesh.normals.emplace_back(nx, ny, nz);
            mesh.positions.push_back(math::Vec3(nx, ny, nz) * radius);
            mesh.texcoords.emplace_back(static_cast<float>(slice) / (slices - 1), static_cast<float>(ring) / (rings - 1));
        }
    }

    // Generate indices
    mesh.indices.reserve(mesh.numFaces * 3);
    for (int ring = 0; ring < rings - 1; ring++)
    {
        for (int slice = 0; slice < slices - 1; slice++)
        {
            const int current = ring * slices + slice;
            const int next = current + slices;

            // First triangle
            mesh.indices.push_back(current);
            mesh.indices.push_back(next + 1);
            mesh.indices.push_back(next);

            // Second triangle
            mesh.indices.push_back(current);
            mesh.indices.push_back(current + 1);
            mesh.indices.push_back(next + 1);
        }
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::HemiSphere(float radius, int rings, int slices)
{
    Mesh mesh;

    rings = std::max(rings, 3);
    slices = std::max(slices, 3);

    mesh.numVertices = (rings / 2 + 1) * slices; // Half of the rings, plus the top pole
    mesh.numFaces = 2 * (rings / 2) * slices;

    mesh.positions.reserve(mesh.numVertices);
    mesh.normals.reserve(mesh.numVertices);
    mesh.texcoords.reserve(mesh.numVertices);

    // Generate vertices
    for (int ring = 0; ring <= rings / 2; ring++)
    {
        const float theta = static_cast<float>(ring) / (rings - 1) * math::Pi;

        for (int slice = 0; slice < slices; slice++)
        {
            const float phi = static_cast<float>(slice) / (slices - 1) * math::DegToRad(360.0f);

            const float nx = std::sin(theta) * std::cos(phi);
            const float nz = std::sin(theta) * std::sin(phi);
            const float ny = std::cos(theta);

            mesh.normals.emplace_back(nx, ny, nz);
            mesh.positions.push_back(math::Vec3(nx, ny, nz) * radius);
            mesh.texcoords.emplace_back(static_cast<float>(slice) / (slices - 1), static_cast<float>(ring) / (rings - 1));
        }
    }

    // Generate indices
    mesh.indices.reserve(mesh.numFaces * 3);
    for (int ring = 0; ring < rings / 2; ring++)
    {
        for (int slice = 0; slice < slices - 1; slice++)
        {
            const int current = ring * slices + slice;
            const int next = current + slices;

            // First triangle
            mesh.indices.push_back(current);
            mesh.indices.push_back(next + 1);
            mesh.indices.push_back(next);

            // Second triangle
            mesh.indices.push_back(current);
            mesh.indices.push_back(current + 1);
            mesh.indices.push_back(next + 1);
        }
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Cylinder(float radius, float height, int slices)
{
    Mesh mesh;

    slices = std::max(slices, 3);

    mesh.numVertices = 2 * slices; // Top and bottom circles
    mesh.numFaces = 2 * slices + slices * 2; // Two circles and connecting rectangles

    mesh.positions.resize(mesh.numVertices);
    mesh.normals.resize(mesh.numVertices);
    mesh.texcoords.resize(mesh.numVertices);

    // Generate vertices for the top circle
    for (int i = 0; i < slices; ++i)
    {
        float theta = static_cast<float>(i) / (slices - 1) * math::DegToRad(360.0f);
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);

        mesh.positions[i] = { x, height / 2, z };
        mesh.normals[i] = { 0.0f, 1.0f, 0.0f };
        mesh.texcoords[i] = { x / (2.0f * radius) + 0.5f, z / (2.0f * radius) + 0.5f };
    }

    // Generate vertices for the bottom circle
    for (int i = 0; i < slices; ++i)
    {
        float theta = static_cast<float>(i) / (slices - 1) * math::DegToRad(360.0f);
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);

        mesh.positions[slices + i] = { x, -height / 2, z };
        mesh.normals[slices + i] = { 0.0f, -1.0f, 0.0f };
        mesh.texcoords[slices + i] = { x / (2.0f * radius) + 0.5f, z / (2.0f * radius) + 0.5f };
    }

    // Generate indices for the top and bottom circles
    mesh.indices.resize(mesh.numFaces * 3);
    for (int i = 0; i < slices; ++i)
    {
        // Top circle
        mesh.indices[i * 3] = i;
        mesh.indices[i * 3 + 1] = (i + 1) % slices;
        mesh.indices[i * 3 + 2] = slices;

        // Bottom circle
        mesh.indices[slices * 3 + i * 3] = slices + i;
        mesh.indices[slices * 3 + i * 3 + 1] = slices + (i + 1) % slices;
        mesh.indices[slices * 3 + i * 3 + 2] = slices * 2;
    }

    // Generate vertices and indices for the side rectangles
    for (int i = 0; i < slices; ++i)
    {
        int index = 2 * slices + i * 2 * 3;
        int top1 = i;
        int top2 = (i + 1) % slices;
        int bottom1 = slices + i;
        int bottom2 = slices + (i + 1) % slices;

        // First rectangle
        mesh.indices[index] = top1;
        mesh.indices[index + 1] = bottom1;
        mesh.indices[index + 2] = top2;

        // Second rectangle
        mesh.indices[index + 3] = top2;
        mesh.indices[index + 4] = bottom1;
        mesh.indices[index + 5] = bottom2;
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Cone(float radius, float height, int slices)
{
    Mesh mesh;

    slices = std::max(slices, 3);

    mesh.numVertices = slices + 1; // Base circle + top point
    mesh.numFaces = slices * 2 + slices; // Base triangles + side triangles

    mesh.positions.resize(mesh.numVertices);
    mesh.normals.resize(mesh.numVertices);
    mesh.texcoords.resize(mesh.numVertices);

    // Generate vertices for the base circle
    for (int i = 0; i < slices; ++i)
    {
        float theta = static_cast<float>(i) / (slices - 1) * math::DegToRad(360.0f);
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);

        mesh.positions[i] = { x, -height / 2, z };
        mesh.normals[i] = { 0.0f, -1.0f, 0.0f };
        mesh.texcoords[i] = { x / (2.0f * radius) + 0.5f, z / (2.0f * radius) + 0.5f };
    }

    // Generate vertex for the top point
    mesh.positions[slices] = { 0.0f, height / 2, 0.0f };
    mesh.normals[slices] = { 0.0f, 1.0f, 0.0f };
    mesh.texcoords[slices] = { 0.5f, 0.5f };

    // Generate indices for the base triangles
    mesh.indices.resize(mesh.numFaces * 3);
    for (int i = 0; i < slices; ++i)
    {
        mesh.indices[i * 3] = i;
        mesh.indices[i * 3 + 1] = (i + 1) % slices;
        mesh.indices[i * 3 + 2] = slices;
    }

    // Generate indices for the side triangles
    for (int i = 0; i < slices; ++i)
    {
        mesh.indices[slices * 3 + i * 3] = i;
        mesh.indices[slices * 3 + i * 3 + 1] = slices;
        mesh.indices[slices * 3 + i * 3 + 2] = (i + 1) % slices;
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Torus(float radius, float size, int radSeg, int sides)
{
    Mesh mesh;

    sides = std::max(sides, 3);
    radSeg = std::max(radSeg, 3);

    mesh.numVertices = radSeg * sides;
    mesh.numFaces = 2 * radSeg * sides;

    mesh.positions.resize(mesh.numVertices);
    mesh.normals.resize(mesh.numVertices);
    mesh.texcoords.resize(mesh.numVertices);

    // Generate vertices
    for (int i = 0; i < radSeg; ++i)
    {
        float theta = static_cast<float>(i) / radSeg * math::DegToRad(360.0f);
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);

        for (int j = 0; j < sides; ++j)
        {
            float phi = static_cast<float>(j) / sides * math::DegToRad(360.0f);
            float cosPhi = std::cos(phi);
            float sinPhi = std::sin(phi);

            float x = (radius + size * cosPhi) * cosTheta;
            float y = size * sinPhi;
            float z = (radius + size * cosPhi) * sinTheta;

            int index = i * sides + j;
            mesh.positions[index] = { x, y, z };
            mesh.normals[index] = math::Vec3(cosTheta * cosPhi, sinPhi, sinTheta * cosPhi).Normalized();
            mesh.texcoords[index] = { static_cast<float>(i) / radSeg, static_cast<float>(j) / sides };
        }
    }

    // Generate indices
    mesh.indices.resize(mesh.numFaces * 3);
    for (int i = 0, index = 0; i < radSeg; ++i)
    {
        for (int j = 0; j < sides; ++j)
        {
            int nextI = (i + 1) % radSeg;
            int nextJ = (j + 1) % sides;

            // First triangle
            mesh.indices[index++] = i * sides + j;
            mesh.indices[index++] = nextI * sides + j;
            mesh.indices[index++] = i * sides + nextJ;

            // Second triangle
            mesh.indices[index++] = i * sides + nextJ;
            mesh.indices[index++] = nextI * sides + j;
            mesh.indices[index++] = nextI * sides + nextJ;
        }
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::TrefoilKnot(float radius, float size, int radSeg, int sides)
{
    Mesh mesh;

    sides = std::max(sides, 3);
    radSeg = std::max(radSeg, 3);

    mesh.numVertices = radSeg * sides;
    mesh.numFaces = 2 * radSeg * sides;

    mesh.positions.resize(mesh.numVertices);
    mesh.normals.resize(mesh.numVertices);
    mesh.texcoords.resize(mesh.numVertices);

    // Generate trefoil knot using parametric equations
    for (int i = 0; i < radSeg; ++i)
    {
        float u = static_cast<float>(i) / radSeg * math::DegToRad(360.0f);

        for (int j = 0; j < sides; ++j)
        {
            float v = static_cast<float>(j) / sides * math::DegToRad(360.0f);

            float x = (radius + size * std::cos(3.0f * u)) * std::cos(2.0f * u) + std::cos(v) * size * std::cos(2.0f * math::DegToRad(360.0f) * u);
            float y = (radius + size * std::cos(3.0f * u)) * std::sin(2.0f * u) + std::cos(v) * size * std::sin(2.0f * math::DegToRad(360.0f) * u);
            float z = size * std::sin(3.0f * u) + std::sin(v) * size * std::sin(2.0f * math::DegToRad(360.0f) * u);

            int index = i * sides + j;
            mesh.positions[index] = { x, y, z };

            mesh.normals[index] = math::Vec3(
                -3.0f * std::sin(3.0f * u) * std::cos(2.0f * u) - 2.0f * std::cos(3.0f * u) * std::sin(2.0f * u) + std::cos(v) * std::cos(2.0f * math::DegToRad(360.0f) * u),
                -3.0f * std::sin(3.0f * u) * std::sin(2.0f * u) + 2.0f * std::cos(3.0f * u) * std::cos(2.0f * u) + std::cos(v) * std::sin(2.0f * math::DegToRad(360.0f) * u),
                3.0f * std::cos(3.0f * u) - std::sin(v) * std::sin(2.0f * math::DegToRad(360.0f) * u));

            mesh.texcoords[index] = {
                u / static_cast<float>(math::DegToRad(360.0f)),
                v / static_cast<float>(math::DegToRad(360.0f))
            };
        }
    }

    // Generate indices
    mesh.indices.resize(mesh.numFaces * 3);
    for (int i = 0, index = 0; i < radSeg; ++i)
    {
        for (int j = 0; j < sides; ++j)
        {
            int nextI = (i + 1) % radSeg;
            int nextJ = (j + 1) % sides;

            // First triangle
            mesh.indices[index++] = i * sides + j;
            mesh.indices[index++] = nextI * sides + j;
            mesh.indices[index++] = i * sides + nextJ;

            // Second triangle
            mesh.indices[index++] = i * sides + nextJ;
            mesh.indices[index++] = nextI * sides + j;
            mesh.indices[index++] = nextI * sides + nextJ;
        }
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Heightmap(const gfx::Surface& heightmap, const math::Vec3& size)
{
    constexpr auto getGray = [](const gfx::Surface& heightmap, int x, int y) -> float
    {
        const gfx::Color c = heightmap.GetPixel(x, y);
        return static_cast<float>(c.r + c.g + c.b) / 3.0f;
    };

    Mesh mesh;

    const float mapX = heightmap.GetWidth() - 1;
    const float mapZ = heightmap.GetHeight() - 1;

    const float invMapX = 1.0f / mapX;
    const float invMapZ = 1.0f / mapZ;

    mesh.numFaces = mapX * mapZ * 2;        // One quad every four pixels
    mesh.numVertices = mesh.numFaces * 3;   // One vertex per pixel

    mesh.positions.reserve(mesh.numVertices);
    mesh.normals.reserve(mesh.numVertices);
    mesh.texcoords.reserve(mesh.numVertices);

    int vCounter = 0;

    const math::Vec3 scaleFactor(
        size.x * invMapX,
        size.y / 255.0f,
        size.z * invMapZ
    );

    for (int z = 0; z < mapZ; z++)
    {
        for (int x = 0; x < mapX; x++)
        {
            // Fill vertices vector with data
            mesh.positions.emplace_back(x * scaleFactor.x, getGray(heightmap, x, z) * scaleFactor.y, z * scaleFactor.z);
            mesh.positions.emplace_back(x * scaleFactor.x, getGray(heightmap, x, z + 1) * scaleFactor.y, (z + 1) * scaleFactor.z);
            mesh.positions.emplace_back((x + 1) * scaleFactor.x, getGray(heightmap, x + 1, z) * scaleFactor.y, z * scaleFactor.z);

            mesh.positions.push_back(mesh.positions[vCounter + 2]);
            mesh.positions.push_back(mesh.positions[vCounter + 1]);
            mesh.positions.emplace_back((x + 1) * scaleFactor.x, getGray(heightmap, x + 1, z + 1) * scaleFactor.y, (z + 1) * scaleFactor.z);

            // Fill texcoords vector with data
            mesh.texcoords.emplace_back(x * invMapX, z * invMapZ);
            mesh.texcoords.emplace_back(x * invMapX, (z + 1) * invMapZ);
            mesh.texcoords.emplace_back((x + 1) * invMapX, z * invMapZ);

            mesh.texcoords.push_back(mesh.texcoords[vCounter + 2]);
            mesh.texcoords.push_back(mesh.texcoords[vCounter + 1]);
            mesh.texcoords.emplace_back((x + 1) * invMapX, (z + 1) * invMapZ);

            // Fill normals vector with data
            for (int i = 0; i < 6; i += 3)
            {
                const math::Vec3 vA = mesh.positions[vCounter + i];
                const math::Vec3 vB = mesh.positions[vCounter + i + 1];
                const math::Vec3 vC = mesh.positions[vCounter + i + 2];

                const math::Vec3 vN = (vB - vA).Cross(vC - vA).Normalized();

                mesh.normals.push_back(vN);
                mesh.normals.push_back(vN);
                mesh.normals.push_back(vN);
            }

            vCounter += 6;
        }
    }

    return mesh;
}

shape3D::Mesh shape3D::Mesh::Cubicmap(const gfx::Surface& cubicmap, const math::Vec3& cubeSize)
{
    Mesh mesh;

    // NOTE: Max possible number of triangles numCubes*(12 triangles by cube)
    const int maxTriangles = cubicmap.GetWidth() * cubicmap.GetHeight() * 12;
    const float w = cubeSize.x, h = cubeSize.z, h2 = cubeSize.y;

    mesh.positions.reserve(3 * maxTriangles);
    mesh.texcoords.reserve(3 * maxTriangles);
    mesh.normals.reserve(3 * maxTriangles);

    // Define the 6 normals of the cube, we will combine them accordingly later...
    constexpr math::Vec3 n1{ 1.0f, 0.0f, 0.0f };
    constexpr math::Vec3 n2{ -1.0f, 0.0f, 0.0f };
    constexpr math::Vec3 n3{ 0.0f, 1.0f, 0.0f };
    constexpr math::Vec3 n4{ 0.0f, -1.0f, 0.0f };
    constexpr math::Vec3 n5{ 0.0f, 0.0f, -1.0f };
    constexpr math::Vec3 n6{ 0.0f, 0.0f, 1.0f };

    constexpr shape2D::RectangleF rightTexUV  { 0.0f, 0.0f, 0.5f, 0.5f };
    constexpr shape2D::RectangleF leftTexUV   { 0.5f, 0.0f, 0.5f, 0.5f };
    constexpr shape2D::RectangleF frontTexUV  { 0.0f, 0.0f, 0.5f, 0.5f };
    constexpr shape2D::RectangleF backTexUV   { 0.5f, 0.0f, 0.5f, 0.5f };
    constexpr shape2D::RectangleF topTexUV    { 0.0f, 0.5f, 0.5f, 0.5f };
    constexpr shape2D::RectangleF bottomTexUV { 0.5f, 0.5f, 0.5f, 0.5f };

    for (int z = 0; z < cubicmap.GetHeight(); z++)
    {
        for (int x = 0; x < cubicmap.GetWidth(); x++)
        {
            // Define the 8 vertex of the cube, we will combine them accordingly later...
            const math::Vec3 v1{ w * (x - 0.5f), h2, h * (z - 0.5f) };
            const math::Vec3 v2{ w * (x - 0.5f), h2, h * (z + 0.5f) };
            const math::Vec3 v3{ w * (x + 0.5f), h2, h * (z + 0.5f) };
            const math::Vec3 v4{ w * (x + 0.5f), h2, h * (z - 0.5f) };
            const math::Vec3 v5{ w * (x + 0.5f), 0, h * (z - 0.5f) };
            const math::Vec3 v6{ w * (x - 0.5f), 0, h * (z - 0.5f) };
            const math::Vec3 v7{ w * (x - 0.5f), 0, h * (z + 0.5f) };
            const math::Vec3 v8{ w * (x + 0.5f), 0, h * (z + 0.5f) };

            // We check pixel color to be WHITE -> draw full cube
            if (cubicmap.GetPixel(x, z) == gfx::White)
            {
                // Define triangles and checking collateral cubes
                //------------------------------------------------

                // Define top triangles (2 tris, 6 vertex --> v1-v2-v3, v1-v3-v4)
                // WARNING: Not required for a WHITE cubes, created to allow seeing the map from outside
                mesh.positions.push_back(v1);
                mesh.positions.push_back(v2);
                mesh.positions.push_back(v3);
                mesh.positions.push_back(v1);
                mesh.positions.push_back(v3);
                mesh.positions.push_back(v4);

                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);

                mesh.texcoords.emplace_back(topTexUV.x, topTexUV.y);
                mesh.texcoords.emplace_back(topTexUV.x, topTexUV.y + topTexUV.h);
                mesh.texcoords.emplace_back(topTexUV.x + topTexUV.w, topTexUV.y + topTexUV.h);
                mesh.texcoords.emplace_back(topTexUV.x, topTexUV.y);
                mesh.texcoords.emplace_back(topTexUV.x + topTexUV.w, topTexUV.y + topTexUV.h);
                mesh.texcoords.emplace_back(topTexUV.x + topTexUV.w, topTexUV.y);

                // Define bottom triangles (2 tris, 6 vertex --> v6-v8-v7, v6-v5-v8)
                mesh.positions.push_back(v6);
                mesh.positions.push_back(v8);
                mesh.positions.push_back(v7);
                mesh.positions.push_back(v6);
                mesh.positions.push_back(v5);
                mesh.positions.push_back(v8);

                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);

                mesh.texcoords.emplace_back(bottomTexUV.x + bottomTexUV.w, bottomTexUV.y);
                mesh.texcoords.emplace_back(bottomTexUV.x, bottomTexUV.y + bottomTexUV.h);
                mesh.texcoords.emplace_back(bottomTexUV.x + bottomTexUV.w, bottomTexUV.y + bottomTexUV.h);
                mesh.texcoords.emplace_back(bottomTexUV.x + bottomTexUV.w, bottomTexUV.y);
                mesh.texcoords.emplace_back(bottomTexUV.x, bottomTexUV.y);
                mesh.texcoords.emplace_back(bottomTexUV.x, bottomTexUV.y + bottomTexUV.h);

                // Checking cube on bottom of current cube
                if ((z < cubicmap.GetHeight() - 1 && cubicmap.GetPixel(x, z+1) == gfx::Black) || (z == cubicmap.GetHeight() - 1))
                {
                    // Define front triangles (2 tris, 6 vertex) --> v2 v7 v3, v3 v7 v8
                    // NOTE: Collateral occluded faces are not generated
                    mesh.positions.push_back(v2);
                    mesh.positions.push_back(v7);
                    mesh.positions.push_back(v3);
                    mesh.positions.push_back(v3);
                    mesh.positions.push_back(v7);
                    mesh.positions.push_back(v8);

                    mesh.normals.push_back(n6);
                    mesh.normals.push_back(n6);
                    mesh.normals.push_back(n6);
                    mesh.normals.push_back(n6);
                    mesh.normals.push_back(n6);
                    mesh.normals.push_back(n6);

                    mesh.texcoords.emplace_back(frontTexUV.x, frontTexUV.y);
                    mesh.texcoords.emplace_back(frontTexUV.x, frontTexUV.y + frontTexUV.h);
                    mesh.texcoords.emplace_back(frontTexUV.x + frontTexUV.w, frontTexUV.y);
                    mesh.texcoords.emplace_back(frontTexUV.x + frontTexUV.w, frontTexUV.y);
                    mesh.texcoords.emplace_back(frontTexUV.x, frontTexUV.y + frontTexUV.h);
                    mesh.texcoords.emplace_back(frontTexUV.x + frontTexUV.w, frontTexUV.y + frontTexUV.h);
                }

                // Checking cube on top of current cube
                if ((z > 0 && cubicmap.GetPixel(x, z-1) == gfx::Black) || (z == 0))
                {
                    // Define back triangles (2 tris, 6 vertex) --> v1 v5 v6, v1 v4 v5
                    // NOTE: Collateral occluded faces are not generated
                    mesh.positions.push_back(v1);
                    mesh.positions.push_back(v5);
                    mesh.positions.push_back(v6);
                    mesh.positions.push_back(v1);
                    mesh.positions.push_back(v4);
                    mesh.positions.push_back(v5);

                    mesh.normals.push_back(n5);
                    mesh.normals.push_back(n5);
                    mesh.normals.push_back(n5);
                    mesh.normals.push_back(n5);
                    mesh.normals.push_back(n5);
                    mesh.normals.push_back(n5);

                    mesh.texcoords.emplace_back(backTexUV.x + backTexUV.w, backTexUV.y);
                    mesh.texcoords.emplace_back(backTexUV.x, backTexUV.y + backTexUV.h);
                    mesh.texcoords.emplace_back(backTexUV.x + backTexUV.w, backTexUV.y + backTexUV.h);
                    mesh.texcoords.emplace_back(backTexUV.x + backTexUV.w, backTexUV.y);
                    mesh.texcoords.emplace_back(backTexUV.x, backTexUV.y);
                    mesh.texcoords.emplace_back(backTexUV.x, backTexUV.y + backTexUV.h);
                }

                // Checking cube on right of current cube
                if ( (x < cubicmap.GetWidth()-1 && cubicmap.GetPixel(x+1, z) == gfx::Black) || (x == cubicmap.GetWidth() - 1) )
                {
                    // Define right triangles (2 tris, 6 vertex) --> v3 v8 v4, v4 v8 v5
                    // NOTE: Collateral occluded faces are not generated
                    mesh.positions.push_back(v3);
                    mesh.positions.push_back(v8);
                    mesh.positions.push_back(v4);
                    mesh.positions.push_back(v4);
                    mesh.positions.push_back(v8);
                    mesh.positions.push_back(v5);

                    mesh.normals.push_back(n1);
                    mesh.normals.push_back(n1);
                    mesh.normals.push_back(n1);
                    mesh.normals.push_back(n1);
                    mesh.normals.push_back(n1);
                    mesh.normals.push_back(n1);

                    mesh.texcoords.emplace_back(rightTexUV.x, rightTexUV.y);
                    mesh.texcoords.emplace_back(rightTexUV.x, rightTexUV.y + rightTexUV.h);
                    mesh.texcoords.emplace_back(rightTexUV.x + rightTexUV.w, rightTexUV.y);
                    mesh.texcoords.emplace_back(rightTexUV.x + rightTexUV.w, rightTexUV.y);
                    mesh.texcoords.emplace_back(rightTexUV.x, rightTexUV.y + rightTexUV.h);
                    mesh.texcoords.emplace_back(rightTexUV.x + rightTexUV.w, rightTexUV.y + rightTexUV.h);
                }

                // Checking cube on left of current cube
                if ( (x > 0 && cubicmap.GetPixel(x-1, z) == gfx::Black) || (x == 0) )
                {
                    // Define left triangles (2 tris, 6 vertex) --> v1 v7 v2, v1 v6 v7
                    // NOTE: Collateral occluded faces are not generated
                    mesh.positions.push_back(v1);
                    mesh.positions.push_back(v7);
                    mesh.positions.push_back(v2);
                    mesh.positions.push_back(v1);
                    mesh.positions.push_back(v6);
                    mesh.positions.push_back(v7);

                    mesh.normals.push_back(n2);
                    mesh.normals.push_back(n2);
                    mesh.normals.push_back(n2);
                    mesh.normals.push_back(n2);
                    mesh.normals.push_back(n2);
                    mesh.normals.push_back(n2);

                    mesh.texcoords.emplace_back(leftTexUV.x, leftTexUV.y);
                    mesh.texcoords.emplace_back(leftTexUV.x + leftTexUV.w, leftTexUV.y + leftTexUV.h);
                    mesh.texcoords.emplace_back(leftTexUV.x + leftTexUV.w, leftTexUV.y);
                    mesh.texcoords.emplace_back(leftTexUV.x, leftTexUV.y);
                    mesh.texcoords.emplace_back(leftTexUV.x, leftTexUV.y + leftTexUV.h);
                    mesh.texcoords.emplace_back(leftTexUV.x + leftTexUV.w, leftTexUV.y + leftTexUV.h);
                }
            }
            else // If the current pixel color is not white, we will only draw the floor and roof
            {
                // Define top triangles (2 tris, 6 vertex --> v1-v2-v3, v1-v3-v4)
                mesh.positions.push_back(v1);
                mesh.positions.push_back(v3);
                mesh.positions.push_back(v2);
                mesh.positions.push_back(v1);
                mesh.positions.push_back(v4);
                mesh.positions.push_back(v3);

                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);
                mesh.normals.push_back(n4);

                mesh.texcoords.emplace_back(topTexUV.x, topTexUV.y);
                mesh.texcoords.emplace_back(topTexUV.x + topTexUV.w, topTexUV.y + topTexUV.h);
                mesh.texcoords.emplace_back(topTexUV.x, topTexUV.y + topTexUV.h);
                mesh.texcoords.emplace_back(topTexUV.x, topTexUV.y);
                mesh.texcoords.emplace_back(topTexUV.x + topTexUV.w, topTexUV.y);
                mesh.texcoords.emplace_back(topTexUV.x + topTexUV.w, topTexUV.y + topTexUV.h);

                // Define bottom triangles (2 tris, 6 vertex --> v6-v8-v7, v6-v5-v8)
                mesh.positions.push_back(v6);
                mesh.positions.push_back(v7);
                mesh.positions.push_back(v8);
                mesh.positions.push_back(v6);
                mesh.positions.push_back(v8);
                mesh.positions.push_back(v5);

                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);
                mesh.normals.push_back(n3);

                mesh.texcoords.emplace_back(bottomTexUV.x + bottomTexUV.w, bottomTexUV.y);
                mesh.texcoords.emplace_back(bottomTexUV.x + bottomTexUV.w, bottomTexUV.y + bottomTexUV.h);
                mesh.texcoords.emplace_back(bottomTexUV.x, bottomTexUV.y + bottomTexUV.h);
                mesh.texcoords.emplace_back(bottomTexUV.x + bottomTexUV.w, bottomTexUV.y);
                mesh.texcoords.emplace_back(bottomTexUV.x, bottomTexUV.y + bottomTexUV.h);
                mesh.texcoords.emplace_back(bottomTexUV.x, bottomTexUV.y);
            }
        }
    }

    // Adjusting unused vector size
    mesh.positions.shrink_to_fit();
    mesh.texcoords.shrink_to_fit();
    mesh.normals.shrink_to_fit();

    // Move data from mapVertices temp arrays to vertices float array
    mesh.numVertices = mesh.positions.size();
    mesh.numFaces = mesh.positions.size()/3;

    return mesh;
}


/* Collision Implementation */

bool shape3D::Mesh::CollisionPoint(const nexus::math::Vec3& point, const math::Mat4& meshTransform) const
{
    const AABB aabb = this->GetAABB();
    const Ray ray(point, point.Direction((aabb.min + aabb.max) * 0.5f));

    Uint32 intersectionCount = 0;
    for (Uint32 i = 0; i < positions.size(); i += 3)
    {
        math::Vec3 v0, v1, v2;

        if (!indices.empty())
        {
            v0 = positions[indices[i]].Transformed(meshTransform);
            v1 = positions[indices[i + 1]].Transformed(meshTransform);
            v2 = positions[indices[i + 2]].Transformed(meshTransform);
        }
        else
        {
            v0 = positions[i].Transformed(meshTransform);
            v1 = positions[i + 1].Transformed(meshTransform);
            v2 = positions[i + 2].Transformed(meshTransform);
        }

        if (ray.CollisionTriangle(v0, v1, v2).hit)
        {
            intersectionCount++;
        }
    }

    return intersectionCount % 2;
}

bool shape3D::Mesh::CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& meshTransform) const
{
    for (size_t i = 0; i < positions.size(); i += 3)
    {
        math::Vec3 v0, v1, v2;

        if (!indices.empty())
        {
            v0 = positions[indices[i]].Transformed(meshTransform);
            v1 = positions[indices[i + 1]].Transformed(meshTransform);
            v2 = positions[indices[i + 2]].Transformed(meshTransform);
        }
        else
        {
            v0 = positions[i].Transformed(meshTransform);
            v1 = positions[i + 1].Transformed(meshTransform);
            v2 = positions[i + 2].Transformed(meshTransform);
        }

        for (size_t j = 0; j < mesh.positions.size(); j += 3)
        {
            math::Vec3 u0, u1, u2;

            if (!mesh.indices.empty())
            {
                u0 = mesh.positions[mesh.indices[j]].Transformed(meshTransform);
                u1 = mesh.positions[mesh.indices[j + 1]].Transformed(meshTransform);
                u2 = mesh.positions[mesh.indices[j + 2]].Transformed(meshTransform);
            }
            else
            {
                u0 = mesh.positions[j].Transformed(meshTransform);
                u1 = mesh.positions[j + 1].Transformed(meshTransform);
                u2 = mesh.positions[j + 2].Transformed(meshTransform);
            }

            // Calculate the plane equation for the first triangle
            math::Vec3 normal = (v1 - v0).Cross(v2 - v0).Normalized();
            float d = -normal.Dot(v0);

            // Check which side of the plane the vertices of the second triangle are on
            bool allPointsOnSameSide = true;
            for (const auto& vertex : {u0, u1, u2})
            {
                if (normal.Dot(vertex) + d < 0)
                {
                    allPointsOnSameSide = false;
                    break;
                }
            }

            // If all points are on the same side of the plane, there is no intersection
            if (allPointsOnSameSide) continue;

            // Calculate the plane equation for the second triangle
            normal = (u1 - u0).Cross(u2 - u0).Normalized();
            d = -normal.Dot(u0);

            // Check which side of the plane the vertices of the first triangle are on
            for (const auto& vertex : {v0, v1, v2})
            {
                if (normal.Dot(vertex) + d < 0)
                {
                    allPointsOnSameSide = false;
                    break;
                }
            }

            // If at least one point of each triangle is on the other side of the other triangle's plane, there is an intersection
            if (!allPointsOnSameSide) return true;
        }
    }

    return false; // No intersection found
}

bool shape3D::Mesh::CollisionAABB(const nexus::shape3D::AABB& aabb, const math::Mat4& meshTransform) const
{
    return aabb.CollisionMesh(*this, meshTransform);
}

bool shape3D::Mesh::CollisionCube(const nexus::shape3D::Cube& cube, const math::Mat4& meshTransform) const
{
    return cube.CollisionMesh(*this, meshTransform);
}

bool shape3D::Mesh::CollisionPlane(const nexus::shape3D::Plane& plane, const math::Mat4& meshTransform) const
{
    return plane.CollisionMesh(*this, meshTransform);
}

bool shape3D::Mesh::CollisionSphere(const nexus::shape3D::Sphere& sphere, const math::Mat4& meshTransform) const
{
    // Transform the center of the sphere
    nexus::math::Vec3 sphereCenter = sphere.center.Transformed(meshTransform);

    // Iterate over each triangle of the mesh
    for (size_t i = 0; i < positions.size(); i += 3)
    {
        nexus::math::Vec3 v0, v1, v2;

        if (!indices.empty())
        {
            v0 = positions[indices[i]].Transformed(meshTransform);
            v1 = positions[indices[i + 1]].Transformed(meshTransform);
            v2 = positions[indices[i + 2]].Transformed(meshTransform);
        }
        else
        {
            v0 = positions[i].Transformed(meshTransform);
            v1 = positions[i + 1].Transformed(meshTransform);
            v2 = positions[i + 2].Transformed(meshTransform);
        }

        // Create a ray from the sphere center in the direction of the triangle normal
        nexus::shape3D::Ray ray(sphereCenter, (v1 - v0).Cross(v2 - v0).Normalized());

        // Check if the ray intersects the triangle
        nexus::shape3D::RayCollision collision = ray.CollisionTriangle(v0, v1, v2);

        // If there is a collision and the distance to the collision point is within the sphere radius, return true
        if (collision.hit && collision.distance <= sphere.radius)
        {
            return true;
        }
    }

    return false;
}

bool shape3D::Mesh::CollisionCapsule(const nexus::shape3D::Capsule& capsule, const math::Mat4& meshTransform) const
{
    // Divides the capsule into two spheres and a cylinder
    nexus::shape3D::Sphere startSphere(capsule.start, capsule.radius);
    nexus::shape3D::Sphere endSphere(capsule.end, capsule.radius);
    nexus::shape3D::Cylinder cylinder(capsule.GetCenter(), capsule.GetHeight(false), capsule.radius);

    if (this->CollisionSphere(startSphere, meshTransform))  return true;
    if (this->CollisionSphere(endSphere, meshTransform))    return true;
    if (this->CollisionCylinder(cylinder, meshTransform))   return true;

    return false;
}

bool shape3D::Mesh::CollisionCylinder(const nexus::shape3D::Cylinder& cylinder, const math::Mat4& meshTransform) const
{
    constexpr auto projectPointOntoCylinder = [](const nexus::math::Vec3& point, const nexus::shape3D::Cylinder& cylinder)
    {
        // Calculate the direction of the cylinder
        constexpr nexus::math::Vec3 cylinderDirection(0.0f, 1.0f, 0.0f); // Assume the cylinder is aligned vertically

        // Calculates the vector of the point projected on the axis of the cylinder
        nexus::math::Vec3 projectedVector = point - cylinder.center;
        projectedVector.y = 0.0f; // Ignore the y component

        // Normalizes the projected vector to bring it back to the surface of the cylinder
        projectedVector.Normalize();

        // Multiply the projected vector by the radius of the cylinder to extend it to the surface of the cylinder
        return cylinder.center + projectedVector * cylinder.radius;
    };

    // Iterate over each triangle of the mesh
    for (size_t i = 0; i < positions.size(); i += 3)
    {
        nexus::math::Vec3 v0, v1, v2;

        if (!indices.empty())
        {
            v0 = positions[indices[i]].Transformed(meshTransform);
            v1 = positions[indices[i + 1]].Transformed(meshTransform);
            v2 = positions[indices[i + 2]].Transformed(meshTransform);
        }
        else
        {
            v0 = positions[i].Transformed(meshTransform);
            v1 = positions[i + 1].Transformed(meshTransform);
            v2 = positions[i + 2].Transformed(meshTransform);
        }

        // Calculate the plane of the triangle
        nexus::math::Vec3 normal = (v1 - v0).Cross(v2 - v0).Normalized();
        float d = -normal.Dot(v0);

        // Calculates the signed distances from the vertices of the triangle to the plane of the cylinder
        float distance0 = normal.Dot(v0) + d;
        float distance1 = normal.Dot(v1) + d;
        float distance2 = normal.Dot(v2) + d;

        // Checks if the vertices of the triangle are all on the same side of the plane of the cylinder
        bool allPointsOnSameSide = (distance0 > 0 && distance1 > 0 && distance2 > 0) ||
                                   (distance0 < 0 && distance1 < 0 && distance2 < 0);

        // If all the points of the triangle are on the same side of the plane of the cylinder, there is no intersection
        if (allPointsOnSameSide) continue;

        // Calculate the projections of the vertices of the triangle onto the cylinder
        nexus::math::Vec3 projectedV0 = projectPointOntoCylinder(v0, cylinder);
        nexus::math::Vec3 projectedV1 = projectPointOntoCylinder(v1, cylinder);
        nexus::math::Vec3 projectedV2 = projectPointOntoCylinder(v2, cylinder);

        // If at least one vertex of the triangle is inside the cylinder, there is an intersection
        if (cylinder.CollisionPoint(projectedV0) || cylinder.CollisionPoint(projectedV1) || cylinder.CollisionPoint(projectedV2))
        {
            return true;
        } 
    }

    return false; // No intersection found
}

shape3D::RayCollision shape3D::Mesh::CollisionRay(nexus::shape3D::Ray& ray, const math::Mat4& meshTransform) const
{
    return ray.CollisionMesh(*this, meshTransform);
}

shape3D::RayCollision shape3D::Mesh::CollisionLine(const nexus::shape3D::Line& line, const math::Mat4& meshTransform) const
{
    return line.CollisionMesh(*this, meshTransform);
}
