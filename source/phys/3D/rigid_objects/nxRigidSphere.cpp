#include "phys/3D/rigid_objects/nxRigidSphere.hpp"
#include "shape/3D/nxSphere.hpp"

using namespace nexus;

phys3D::RigidSphere::RigidSphere(const math::Vec3& position, const math::Quaternion& rotation, float radius, float mass)
: RigidObject(Shape::Sphere)
{
    this->colliderShape = new btSphereShape(radius);
    this->body = this->CreateBody(position, rotation, mass);
}

phys3D::RigidSphere::RigidSphere(const shape3D::AABB bb, const math::Quaternion& rotation, float mass)
: RigidObject(Shape::Sphere)
{
    const math::Vec3 size = bb.max - bb.min;
    const float radius = std::max({ size.x, size.y, size.z });

    this->colliderShape = new btSphereShape(radius);
    this->body = this->CreateBody((bb.min + bb.max) * 0.5f, rotation, mass);
}

phys3D::RigidSphere::RigidSphere(const shape3D::Sphere& sphere, const math::Quaternion& rotation, float mass)
: RigidObject(Shape::Sphere)
{
    this->colliderShape = new btSphereShape(sphere.radius);
    this->body = this->CreateBody(sphere.center, rotation, mass);
}
