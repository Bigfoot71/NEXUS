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
