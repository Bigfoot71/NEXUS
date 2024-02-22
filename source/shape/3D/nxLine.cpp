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
