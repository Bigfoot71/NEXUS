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

#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_SPHERE_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_SPHERE_HPP

#include "./nxRigidObject.hpp"

namespace nexus { namespace phys3D {

    /**
     * @brief Struct representing a sphere-shaped 3D physics object.
     */
    class NEXUS_API RigidSphere : public RigidObject
    {
      public:
        /**
         * @brief Constructor for the RigidSphere class.
         * @param position Initial position of the sphere.
         * @param rotation Initial rotation of the sphere.
         * @param radius Radius of the sphere.
         * @param mass Mass of the sphere (0.0f for static spheres).
         */
        RigidSphere(const nexus::math::Vec3& position,
                    const nexus::math::Quaternion& rotation,
                    float radius, float mass = Static);

        /**
         * @brief Constructor for the RigidSphere class using an AABB bounding box.
         * @param bb AABB bounding box defining the sphere's dimensions.
         * @param rotation Initial rotation of the sphere.
         * @param mass Mass of the sphere (0.0f for static spheres).
         */
        RigidSphere(const nexus::shape3D::AABB bb,
                    const nexus::math::Quaternion& rotation = { 0, 0, 0, 1 },
                    float mass = Static);

        /**
         * @brief Constructor for the RigidSphere class using a sphere shape.
         * @param sphere Sphere shape defining the sphere's dimensions.
         * @param rotation Initial rotation of the sphere.
         * @param mass Mass of the sphere (0.0f for static spheres).
         */
        RigidSphere(const nexus::shape3D::Sphere& sphere,
                    const nexus::math::Quaternion& rotation = { 0, 0, 0, 1 },
                    float mass = Static);
    };

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_SPHERE_HPP
