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
