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

shape3D::AABB shape3D::Plane::GetAABB() const
{
    return AABB(center - size * 0.5f, center + size * 0.5f);
}

bool shape3D::Plane::CollisionPoint(const nexus::math::Vec3& point) const
{
    return this->GetAABB().CollisionPoint(point);
}

bool shape3D::Plane::CollisionAABB(const nexus::shape3D::AABB& aabb) const
{
    return this->GetAABB().CollisionAABB(aabb);
}

bool shape3D::Plane::CollisionCube(const nexus::shape3D::Cube& cube) const
{
    return this->GetAABB().CollisionCube(cube);
}

bool shape3D::Plane::CollisionPlane(const nexus::shape3D::Plane& plane) const
{
    return this->GetAABB().CollisionPlane(plane);
}

bool shape3D::Plane::CollisionSphere(const nexus::shape3D::Sphere& sphere) const
{
    return this->GetAABB().CollisionSphere(sphere);
}

bool shape3D::Plane::CollisionCapsule(const nexus::shape3D::Capsule& capsule) const
{
    return this->GetAABB().CollisionCapsule(capsule);
}

bool shape3D::Plane::CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const
{
    return this->GetAABB().CollisionCylinder(cylinder);
}

shape3D::RayCollision shape3D::Plane::CollisionRay(nexus::shape3D::Ray& ray) const
{
    return this->GetAABB().CollisionRay(ray);
}

shape3D::RayCollision shape3D::Plane::CollisionLine(const nexus::shape3D::Line& line) const
{
    return this->GetAABB().CollisionLine(line);
}

bool shape3D::Plane::CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    return this->GetAABB().CollisionMesh(mesh, transform);
}
