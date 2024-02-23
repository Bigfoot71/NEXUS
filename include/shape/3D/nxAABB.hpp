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

#ifndef NEXUS_SHAPE_3D_AABB_HPP
#define NEXUS_SHAPE_3D_AABB_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape3D {

    /**
     * @brief Axis-Aligned Bounding Box (AABB) in 3D space.
     *
     * This struct represents an Axis-Aligned Bounding Box (AABB) in 3D space.
     * An AABB is defined by two opposite corner points: the minimum point (min)
     * and the maximum point (max), forming a box aligned with the coordinate axes.
     */
    struct NEXUS_API AABB : CollisionShape
    {
        math::Vec3 min; ///< The minimum point of the AABB.
        math::Vec3 max; ///< The maximum point of the AABB.

        /**
         * @brief Default constructor for AABB.
         * Initializes the AABB with zero vectors for min and max.
         */
        AABB() : min(), max() { }

        /**
         * @brief Constructs an AABB with specified minimum and maximum points.
         * @param _min The minimum point of the AABB.
         * @param _max The maximum point of the AABB.
         */
        AABB(const math::Vec3& _min, const math::Vec3& _max)
        : min(_min), max(_max)
        { }

        /**
         * @brief Checks if a point is inside the AABB.
         * @param point The point to check.
         * @return True if the point is inside the AABB, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec3& point) const override;

        /**
         * @brief Checks collision between two AABBs.
         * @param aabb The AABB to check collision with.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionAABB(const nexus::shape3D::AABB& aabb) const override;

        /**
         * @brief Checks collision between the AABB and a Cube.
         * @param cube The Cube to check collision with.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionCube(const nexus::shape3D::Cube& cube) const override;

        /**
         * @brief Checks collision between the AABB and a Plane.
         * @param plane The Plane to check collision with.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionPlane(const nexus::shape3D::Plane& plane) const override;

        /**
         * @brief Checks collision between the AABB and a Sphere.
         * @param sphere The Sphere to check collision with.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionSphere(const nexus::shape3D::Sphere& sphere) const override;

        /**
         * @brief Checks collision between the AABB and a Capsule.
         * @param capsule The Capsule to check collision with.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionCapsule(const nexus::shape3D::Capsule& capsule) const override;

        /**
         * @brief Checks collision between the AABB and a Cylinder.
         * @param cylinder The Cylinder to check collision with.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const override;

        /**
         * @brief Checks collision between the AABB and a Ray.
         * @param ray The Ray to check collision with.
         * @return A RayCollision object containing collision information.
         */
        RayCollision CollisionRay(nexus::shape3D::Ray& ray) const override;

        /**
         * @brief Checks collision between the AABB and a Line.
         * @param line The Line to check collision with.
         * @return A RayCollision object containing collision information.
         */
        RayCollision CollisionLine(const nexus::shape3D::Line& line) const override;

        /**
         * @brief Checks collision between the AABB and a Mesh with transformation.
         * @param mesh The Mesh to check collision with.
         * @param transform The transformation matrix applied to the Mesh.
         * @return True if there is a collision, false otherwise.
         */
        bool CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const override;
    };

}}

#endif //NEXUS_SHAPE_3D_AABB_HPP
