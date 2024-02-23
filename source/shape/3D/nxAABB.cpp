#include "shape/3D/nxAABB.hpp"

#include "math/nxVec3.hpp"
#include "shape/3D/nxCylinder.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxRay.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

bool shape3D::AABB::CollisionPoint(const nexus::math::Vec3& point) const
{
    return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z);
}

bool shape3D::AABB::CollisionAABB(const nexus::shape3D::AABB& aabb) const
{
    return (max.x >= aabb.min.x && aabb.max.x >= min.x) &&
           (max.y >= aabb.min.y && aabb.max.y >= min.y) &&
           (max.z >= aabb.min.z && aabb.max.z >= min.z);
}

bool shape3D::AABB::CollisionCube(const nexus::shape3D::Cube& cube) const
{
    return this->CollisionAABB(cube.GetAABB());
}

bool shape3D::AABB::CollisionPlane(const nexus::shape3D::Plane& plane) const
{
    return this->CollisionAABB(plane.GetAABB());
}

bool shape3D::AABB::CollisionSphere(const nexus::shape3D::Sphere& sphere) const
{
    const nexus::math::Vec3 closestPoint = sphere.center.Clamp(min, max);
    return (closestPoint - sphere.center).LengthSqr() <= sphere.radius * sphere.radius;
}

bool shape3D::AABB::CollisionCapsule(const nexus::shape3D::Capsule& capsule) const
{
    // Check the intersection with the capsule spheres
    nexus::shape3D::Sphere sphere1(capsule.start, capsule.radius);
    nexus::shape3D::Sphere sphere2(capsule.end, capsule.radius);

    if (!this->CollisionSphere(sphere1) && !this->CollisionSphere(sphere2))
    {
        return false;
    }

    // Check the intersection with the capsule cylinder
    float minX = std::min(min.x, max.x);
    float maxX = std::max(min.x, max.x);
    float minZ = std::min(min.z, max.z);
    float maxZ = std::max(min.z, max.z);

    float cylinderMinX = std::min(capsule.start.x, capsule.end.x) - capsule.radius;
    float cylinderMaxX = std::max(capsule.start.x, capsule.end.x) + capsule.radius;
    float cylinderMinZ = std::min(capsule.start.z, capsule.end.z) - capsule.radius;
    float cylinderMaxZ = std::max(capsule.start.z, capsule.end.z) + capsule.radius;

    if (maxX < cylinderMinX || minX > cylinderMaxX || maxZ < cylinderMinZ || minZ > cylinderMaxZ)
    {
        return false;
    }

    return true;
}

bool shape3D::AABB::CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const
{
    float minX = std::min(min.x, max.x);
    float maxX = std::max(min.x, max.x);
    float minZ = std::min(min.z, max.z);
    float maxZ = std::max(min.z, max.z);

    float cylinderMinX = cylinder.center.x - cylinder.radius;
    float cylinderMaxX = cylinder.center.x + cylinder.radius;
    float cylinderMinZ = cylinder.center.z - cylinder.radius;
    float cylinderMaxZ = cylinder.center.z + cylinder.radius;

    if (maxX < cylinderMinX || minX > cylinderMaxX || maxZ < cylinderMinZ || minZ > cylinderMaxZ)
    {
        return false;
    }

    float minY = std::min(min.y, max.y);
    float maxY = std::max(min.y, max.y);

    float cylinderMinY = cylinder.center.y - (cylinder.height * 0.5f);
    float cylinderMaxY = cylinder.center.y + (cylinder.height * 0.5f);

    if (cylinderMinY > maxY || cylinderMaxY < minY)
    {
        return false;
    }

    return true;
}

shape3D::RayCollision shape3D::AABB::CollisionRay(nexus::shape3D::Ray& ray) const
{
    return ray.CollisionAABB(*this);
}

shape3D::RayCollision shape3D::AABB::CollisionLine(const nexus::shape3D::Line& line) const
{
    return line.CollisionAABB(*this);
}

bool shape3D::AABB::CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    // Calculation of half lengths of AABB
    const math::Vec3 halfLengths = (max - min) * 0.5f;

    for (Uint32 i = 0; i < mesh.vertices.size(); i += 3)
    {
        math::Vec3 v0, v1, v2;

        if (!mesh.indices.empty())
        {
            v0 = mesh.vertices[mesh.indices[i]].Transformed(transform);
            v1 = mesh.vertices[mesh.indices[i + 1]].Transformed(transform);
            v2 = mesh.vertices[mesh.indices[i + 2]].Transformed(transform);
        }
        else
        {
            v0 = mesh.vertices[i].Transformed(transform);
            v1 = mesh.vertices[i + 1].Transformed(transform);
            v2 = mesh.vertices[i + 2].Transformed(transform);
        }

        // Loop on the different separation axes
        for (Uint8 axis = 0; axis < 3; axis++)
        {
            // Calculation of the "radius" projected on the axis
            const float r = halfLengths[axis] * std::abs(v0[axis]);

            // Projection of triangles on the axis
            const float p0 = v0[(axis + 1) % 3] * v1[(axis + 2) % 3] - v0[(axis + 2) % 3] * v1[(axis + 1) % 3];
            const float p2 = (v1[(axis + 1) % 3] - v0[(axis + 1) % 3]) * v2[(axis + 2) % 3] - (v1[(axis + 2) % 3] - v0[(axis + 2) % 3]) * v2[(axis + 1) % 3];

            // On-axis separation test
            if (std::min(p0, p2) > r || std::max(p0, p2) < -r)
            {
                return false;
            }
        }
    }

    return true;
}
