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

#include "shape/3D/nxLine.hpp"

#include "shape/3D/nxCylinder.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxAABB.hpp"
#include "shape/3D/nxRay.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

shape3D::RayCollision shape3D::Line::CollisionPoint(const nexus::math::Vec3& point) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionPoint(point);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionAABB(const nexus::shape3D::AABB& aabb) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionAABB(aabb);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionCube(const nexus::shape3D::Cube& cube) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionCube(cube);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionPlane(const nexus::shape3D::Plane& plane) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionPlane(plane);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionSphere(const nexus::shape3D::Sphere& sphere) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionSphere(sphere);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionCapsule(const nexus::shape3D::Capsule& capsule) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionCapsule(capsule);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionCylinder(cylinder);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionRay(nexus::shape3D::Ray& ray) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionRay(ray);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionLine(const nexus::shape3D::Line& line) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionLine(line);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionMesh(mesh, transform);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionTriangle(const math::Vec3& v0, const math::Vec3& v1, const math::Vec3& v2) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionTriangle(v0, v1, v2);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Line::CollisionQuad(const math::Vec3& v0, const math::Vec3& v1, const math::Vec3& v2, const math::Vec3& v3) const
{
    RayCollision collision = Ray(start, start.Direction(end)).CollisionQuad(v0, v1, v2, v3);
    if (collision.distance < start.Distance(end)) return collision;
    return { start, { 0, 0, 0 }, 0, false };
}
