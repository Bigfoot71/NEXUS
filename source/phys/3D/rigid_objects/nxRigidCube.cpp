#include "phys/3D/rigid_objects/nxRigidCube.hpp"
#include "shape/3D/nxCube.hpp"

using namespace nexus;

phys3D::RigidCube::RigidCube(const math::Vec3& position, const math::Quaternion& rotation, const math::Vec3& size, float mass)
: RigidObject(Shape::Cube)
{
    this->colliderShape = new btBoxShape(size * 0.5f);
    this->body = this->CreateBody(position, rotation, mass);
}

phys3D::RigidCube::RigidCube(const shape3D::AABB bb, const math::Quaternion& rotation, float mass)
: RigidObject(Shape::Cube)
{
    this->colliderShape = new btBoxShape((bb.max - bb.min) * 0.5f);
    this->body = this->CreateBody((bb.min + bb.max) * 0.5f, rotation, mass);
}

phys3D::RigidCube::RigidCube(const shape3D::Cube& cube, const math::Quaternion& rotation, float mass)
: RigidObject(Shape::Cube)
{
    this->colliderShape = new btBoxShape(cube.size * 0.5f);
    this->body = this->CreateBody(cube.center, rotation, mass);
}
