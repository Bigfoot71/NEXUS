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
