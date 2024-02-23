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

#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_CAPSULE_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_CAPSULE_HPP

#include "./nxRigidObject.hpp"
#include "../../../shape/3D/nxCapsule.hpp"

namespace nexus { namespace phys3D {

    /**
     * @brief Struct representing a capsule-shaped 3D physics object.
     */
    class NEXUS_API RigidCapsule : public RigidObject
    {
      public:
        /**
         * @brief Constructor for the RigidCapsule class.
         * @param position Initial position of the capsule.
         * @param rotation Initial rotation of the capsule.
         * @param radius Radius of the capsule.
         * @param height Height of the capsule.
         * @param mass Mass of the capsule (0.0f for static capsules).
         */
        RigidCapsule(const math::Vec3& position,
                     const math::Quaternion& rotation,
                     float radius, float height,
                     float mass = Static);

        /**
         * @brief Constructor for the RigidCapsule class using an AABB bounding box.
         * @param bb AABB bounding box defining the capsule's dimensions.
         * @param rotation Initial rotation of the capsule.
         * @param mass Mass of the capsule (0.0f for static capsules).
         */
        RigidCapsule(const shape3D::AABB bb,
                     const math::Quaternion& rotation = { 0, 0, 0, 1 },
                     float mass = Static);

        /**
         * @brief Constructor for the RigidCapsule class using a capsule shape.
         * @param capsule Capsule shape defining the capsule's dimensions.
         * @param rotation Initial rotation of the capsule.
         * @param mass Mass of the capsule (0.0f for static capsules).
         */
        RigidCapsule(const shape3D::Capsule& capsule,
                     const math::Quaternion& rotation = { 0, 0, 0, 1 },
                     float mass = Static);
    };

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_CAPSULE_HPP
