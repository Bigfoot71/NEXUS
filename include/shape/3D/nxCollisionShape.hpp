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

#ifndef NEXUS_SHAPE_3D_COLLISION_SHAPE_HPP
#define NEXUS_SHAPE_3D_COLLISION_SHAPE_HPP

#include "../../math/nxVec3.hpp"

// TODO: Add Cone shape

namespace nexus { namespace shape3D {

    // Shapes forward declarations

    class Ray;
    class Line;
    class Mesh;
    class AABB;
    class Cube;
    class Plane;
    class Sphere;
    class Capsule;
    class Cylinder;

    // Collisions class result definitions

    struct NEXUS_API RayCollision
    {
        math::Vec3 point;       // Point of the nearest hit
        math::Vec3 normal;      // Surface normal of hit
        float distance;         // Distance to the nearest hit
        bool hit;               // Did the ray hit something?
    };

    struct NEXUS_API CollisionShape
    {
        virtual bool CollisionPoint(const math::Vec3& point)                        const = 0;
        virtual bool CollisionAABB(const AABB& aabb)                                const = 0;
        virtual bool CollisionCube(const Cube& cube)                                const = 0;
        virtual bool CollisionPlane(const Plane& plane)                             const = 0;
        virtual bool CollisionSphere(const Sphere& sphere)                          const = 0;
        virtual bool CollisionCapsule(const Capsule& circle)                        const = 0;
        virtual bool CollisionCylinder(const Cylinder& cylinder)                    const = 0;

        virtual RayCollision CollisionRay(Ray& ray)                                 const = 0;
        virtual RayCollision CollisionLine(const Line& line)                        const = 0;

        virtual bool CollisionMesh(const Mesh& mesh, const math::Mat4& transform)   const = 0;
    };

}}

#endif //NEXUS_SHAPE_3D_COLLISION_SHAPE_HPP
