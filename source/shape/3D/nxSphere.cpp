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

#include "shape/3D/nxSphere.hpp"

#include "shape/3D/nxCylinder.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxAABB.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxRay.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

shape3D::AABB shape3D::Sphere::GetAABB() const
{
    math::Vec3 minPoint(center.x - radius, center.y - radius, center.z - radius);
    math::Vec3 maxPoint(center.x + radius, center.y + radius, center.z + radius);
    return AABB(minPoint, maxPoint);
}

bool shape3D::Sphere::CollisionPoint(const math::Vec3& point) const
{
    return (point - center).LengthSqr() <= radius * radius;
}

bool shape3D::Sphere::CollisionAABB(const shape3D::AABB& aabb) const
{
    return aabb.CollisionSphere(*this);
}

bool shape3D::Sphere::CollisionCube(const shape3D::Cube& cube) const
{
    return cube.CollisionSphere(*this);
}

bool shape3D::Sphere::CollisionPlane(const shape3D::Plane& plane) const
{
    return plane.CollisionSphere(*this);
}

bool shape3D::Sphere::CollisionSphere(const shape3D::Sphere& sphere) const
{
    return sphere.CollisionSphere(*this);
}

bool shape3D::Sphere::CollisionCapsule(const shape3D::Capsule& capsule) const
{
    return capsule.CollisionSphere(*this);
}

bool shape3D::Sphere::CollisionCylinder(const shape3D::Cylinder& cylinder) const
{
    return cylinder.CollisionSphere(*this);
}

shape3D::RayCollision shape3D::Sphere::CollisionRay(shape3D::Ray& ray) const
{
    return ray.CollisionSphere(*this);
}

shape3D::RayCollision shape3D::Sphere::CollisionLine(const shape3D::Line& line) const
{
    return line.CollisionSphere(*this);
}

bool shape3D::Sphere::CollisionMesh(const shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    return mesh.CollisionSphere(*this, transform);
}
