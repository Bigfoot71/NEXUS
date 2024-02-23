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

#include "shape/3D/nxPlane.hpp"

#include "shape/3D/nxCylinder.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxAABB.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxRay.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

math::Vec3 shape3D::Capsule::GetCenter() const
{
    return (start + end) * 0.5f;
}

float shape3D::Capsule::GetHeight(bool totalHeight) const
{
    return (std::max(start.y, end.y) - std::min(start.y, end.y)) + (totalHeight ? 2 * radius : 0);
}

shape3D::AABB shape3D::Capsule::GetAABB() const
{
    return shape3D::AABB {
        (start - radius).Min(end - radius),
        (start + radius).Max(end + radius)
    };
}

bool shape3D::Capsule::CollisionPoint(const math::Vec3& point) const
{
    // Calculate the vector representing the direction of the capsule
    math::Vec3 capsuleDirection = end - start;

    // Calculate the vector representing the direction from the point to the closest end of the capsule
    math::Vec3 pointToClosestEnd = point - start;

    // Calculate the dot product of the capsule direction vector with the direction vector from the point to the closest end
    float dotProduct = pointToClosestEnd.Dot(capsuleDirection);

    // If the dot product is less than 0, the point is closer to the starting end
    if (dotProduct < 0) return point.Distance(start) <= radius;

    // If the dot product is greater than the squared norm of the capsule direction vector, the point is closer to the ending end
    float capsuleDirectionLengthSquared = capsuleDirection.Dot(capsuleDirection);
    if (dotProduct > capsuleDirectionLengthSquared) return point.Distance(end) <= radius;

    // Otherwise, the point is inside the cylinder
    // Calculate the distance from the point to the line segment representing the capsule
    math::Vec3 closestPointOnLine = start + (capsuleDirection * (dotProduct / capsuleDirectionLengthSquared));
    return point.Distance(closestPointOnLine) <= radius;
}

bool shape3D::Capsule::CollisionAABB(const shape3D::AABB& aabb) const
{
    return aabb.CollisionCapsule(*this);
}

bool shape3D::Capsule::CollisionCube(const shape3D::Cube& cube) const
{
    return cube.CollisionCapsule(*this);
}

bool shape3D::Capsule::CollisionPlane(const shape3D::Plane& plane) const
{
    return plane.CollisionCapsule(*this);
}

bool shape3D::Capsule::CollisionSphere(const shape3D::Sphere& sphere) const
{
    return sphere.CollisionCapsule(*this);
}

bool shape3D::Capsule::CollisionCapsule(const shape3D::Capsule& capsule) const
{
    // Calculate the sum of the radii of the two capsules
    float totalRadius = radius + capsule.radius;

    // Check if the capsules are close enough for a collision to be possible
    if (start.Distance(capsule.start) > totalRadius) return false;

    // Calculate the direction vectors of the line segments for each capsule
    math::Vec3 thisCapsuleDirection = end - start;
    math::Vec3 otherCapsuleDirection = capsule.end - capsule.start;

    // Calculate the distance between the two line segments (their axes)
    math::Vec3 dist = capsule.start - start;
    float a = thisCapsuleDirection.Dot(thisCapsuleDirection);
    float b = thisCapsuleDirection.Dot(otherCapsuleDirection);
    float c = otherCapsuleDirection.Dot(otherCapsuleDirection);
    float d = thisCapsuleDirection.Dot(dist);
    float e = otherCapsuleDirection.Dot(dist);

    float det = a * c - b * b;
    float s, t;

    // Calculate the parameters of the intersection between the two line segments
    if (det < 1e-4f)
    {
        // Parallel segments
        s = 0.0f;
        t = (b > c ? d / b : e / c);
    }
    else
    {
        s = (b * e - c * d) / det;
        t = (a * e - b * d) / det;
    }

    // Check if the intersection lies between the line segments
    if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f)
    {
        // Calculate the intersection point
        math::Vec3 intersectionPoint = start + (thisCapsuleDirection * s);

        // Check if the intersection point is at a distance less than or equal to the sum of the radii
        return intersectionPoint.Distance(capsule.start + (otherCapsuleDirection * t)) <= totalRadius;
    }

    return false;
}

bool shape3D::Capsule::CollisionCylinder(const shape3D::Cylinder& cylinder) const
{
    return cylinder.CollisionCapsule(*this);
}

shape3D::RayCollision shape3D::Capsule::CollisionRay(shape3D::Ray& ray) const
{
    return ray.CollisionCapsule(*this);
}

shape3D::RayCollision shape3D::Capsule::CollisionLine(const shape3D::Line& line) const
{
    return line.CollisionCapsule(*this);
}

bool shape3D::Capsule::CollisionMesh(const shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    return mesh.CollisionCapsule(*this, transform);
}
