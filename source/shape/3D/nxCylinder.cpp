#include "shape/3D/nxCylinder.hpp"

#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxAABB.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxRay.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

shape3D::AABB shape3D::Cylinder::GetAABB() const
{
    math::Vec3 minPoint(center.x - radius, center.y - height * 0.5f, center.z - radius);
    math::Vec3 maxPoint(center.x + radius, center.y + height * 0.5f, center.z + radius);
    return AABB(minPoint, maxPoint);
}

bool shape3D::Cylinder::CollisionPoint(const math::Vec3& point) const
{
    bool insideHeight = std::abs(point.y - center.y) <= height * 0.5f;
    bool insideRadius = (point - center).LengthSqr() <= radius * radius;
    return insideHeight && insideRadius;
}

bool shape3D::Cylinder::CollisionAABB(const shape3D::AABB& aabb) const
{
    return aabb.CollisionCylinder(*this);
}

bool shape3D::Cylinder::CollisionCube(const shape3D::Cube& cube) const
{
    return cube.CollisionCylinder(*this);
}

bool shape3D::Cylinder::CollisionPlane(const shape3D::Plane& plane) const
{
    return plane.CollisionCylinder(*this);
}

bool shape3D::Cylinder::CollisionSphere(const shape3D::Sphere& sphere) const
{
    return sphere.CollisionCylinder(*this);
}

bool shape3D::Cylinder::CollisionCapsule(const shape3D::Capsule& capsule) const
{
    return capsule.CollisionCylinder(*this);
}

bool shape3D::Cylinder::CollisionCylinder(const shape3D::Cylinder& cylinder) const
{
    // Calculate the distance between the centers of the two cylinders
    float distanceSquared = (center - cylinder.center).LengthSqr();
    float radiusSum = radius + cylinder.radius;

    // Check if the distance between the centers is less than or equal to the sum of the radii
    if (distanceSquared > radiusSum * radiusSum) return false; // The cylinders do not touch

    // Check the projection on the height axis
    float halfHeightSum = height * 0.5f + cylinder.height * 0.5f;
    float heightDiff = std::abs(center.y - cylinder.center.y);

    // The cylinders do not touch on the height axis
    if (heightDiff > halfHeightSum) return false;

    // Check lateral collision (projection of base circles onto a plane perpendicular to the height axis)
    float horizontalDistanceSquared = (center - cylinder.center).LengthSqr() - heightDiff * heightDiff;

    // The projections of the base circles do not touch
    if (horizontalDistanceSquared > radiusSum * radiusSum) return false;

    // The cylinders touch
    return true;
}

shape3D::RayCollision shape3D::Cylinder::CollisionRay(shape3D::Ray& ray) const
{
    return ray.CollisionCylinder(*this);
}

shape3D::RayCollision shape3D::Cylinder::CollisionLine(const shape3D::Line& line) const
{
    return line.CollisionCylinder(*this);
}

bool shape3D::Cylinder::CollisionMesh(const shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    return mesh.CollisionCylinder(*this, transform);
}
