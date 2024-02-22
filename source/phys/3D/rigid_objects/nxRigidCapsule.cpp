#include "phys/3D/rigid_objects/nxRigidCapsule.hpp"

using namespace nexus;

phys3D::RigidCapsule::RigidCapsule(const math::Vec3& position, const math::Quaternion& rotation, float radius, float height, float mass)
: RigidObject(Shape::Capsule)
{
    this->colliderShape = new btCapsuleShape(radius, height);
    this->body = this->CreateBody(position, rotation, mass);
}

phys3D::RigidCapsule::RigidCapsule(const shape3D::AABB bb, const math::Quaternion& rotation, float mass)
: RigidObject(Shape::Capsule)
{
    const math::Vec3 size = bb.max - bb.min;
    const float radius = (size.x > size.z ? size.x : size.z) * 0.5f;
    this->colliderShape = new btCapsuleShape(radius, size.z + 2 * radius);
    this->body = this->CreateBody((bb.min + bb.max) * 0.5f, rotation, mass);
}

phys3D::RigidCapsule::RigidCapsule(const shape3D::Capsule& capsule, const math::Quaternion& rotation, float mass)
: RigidObject(Shape::Capsule)
{
    this->colliderShape = new btCapsuleShape(capsule.radius, capsule.start.Distance(capsule.end) + 2 * capsule.radius);
    this->body = this->CreateBody((capsule.start + capsule.end) * 0.5f, rotation, mass);
}
