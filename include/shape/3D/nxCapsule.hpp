#ifndef NEXUS_SHAPE_3D_CAPSULE_HPP
#define NEXUS_SHAPE_3D_CAPSULE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape3D {

    /**
     * @brief Represents a capsule in 3D space.
     *
     * This structure defines a capsule, which is a cylindrical shape with hemispherical ends.
     */
    struct NEXUS_API Capsule : CollisionShape
    {
        math::Vec3 start;   ///< The starting point of the capsule.
        math::Vec3 end;     ///< The ending point of the capsule.
        float radius;       ///< The radius of the capsule.

        /**
         * @brief Default constructor for Capsule.
         */
        Capsule() : start(), end(), radius()
        { }

        /**
         * @brief Constructs a Capsule with specified start and end points and radius.
         * @param start The starting point of the capsule.
         * @param end The ending point of the capsule.
         * @param radius The radius of the capsule.
         */
        Capsule(const math::Vec3& start, const math::Vec3& end, float radius)
        : start(start), end(end), radius(radius)
        { }

        /**
         * @brief Constructs a Capsule with specified center, radius, and height.
         * @param center The center of the capsule.
         * @param radius The radius of the capsule.
         * @param height The height of the capsule.
         */
        Capsule(const math::Vec3& center, float radius, float height)
        : start(center), end(center), radius(radius)
        {
            const float d = height * 0.5f - radius;
            start.y -= d, end.y += d;
        }

        /**
         * @brief Calculates and returns the center of the capsule.
         * @return The center of the capsule as a 3D vector.
         */
        math::Vec3 GetCenter() const;

        /**
         * @brief Calculates and returns the height of the capsule.
         * @param totalHeight If true, returns the total height of the capsule including the hemispherical ends.
         *                    If false, returns the cylindrical height only.
         * @return The height of the capsule.
         */
        float GetHeight(bool totalHeight = true) const;

        /**
         * @brief Calculates and returns the axis-aligned bounding box (AABB) of the capsule.
         * @return The AABB of the capsule.
         */
        AABB GetAABB() const;

        /**
         * @brief Checks if a point is inside or on the surface of the capsule.
         * @param point The point to check.
         * @return True if the point is inside or on the surface of the capsule, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec3& point) const override;

        /**
         * @brief Checks for collision between the capsule and an axis-aligned bounding box (AABB).
         * @param aabb The AABB to check for collision with the capsule.
         * @return True if the capsule collides with the AABB, false otherwise.
         */
        bool CollisionAABB(const nexus::shape3D::AABB& aabb) const override;

        /**
         * @brief Checks for collision between the capsule and a cube.
         * @param cube The cube to check for collision with the capsule.
         * @return True if the capsule collides with the cube, false otherwise.
         */
        bool CollisionCube(const nexus::shape3D::Cube& cube) const override;

        /**
         * @brief Checks for collision between the capsule and a plane.
         * @param plane The plane to check for collision with the capsule.
         * @return True if the capsule collides with the plane, false otherwise.
         */
        bool CollisionPlane(const nexus::shape3D::Plane& plane) const override;

        /**
         * @brief Checks for collision between the capsule and a sphere.
         * @param sphere The sphere to check for collision with the capsule.
         * @return True if the capsule collides with the sphere, false otherwise.
         */
        bool CollisionSphere(const nexus::shape3D::Sphere& sphere) const override;

        /**
         * @brief Checks for collision between two capsules.
         * @param capsule The capsule to check for collision with the current capsule.
         * @return True if the two capsules collide, false otherwise.
         */
        bool CollisionCapsule(const nexus::shape3D::Capsule& capsule) const override;

        /**
         * @brief Checks for collision between the capsule and a cylinder.
         * @param cylinder The cylinder to check for collision with the capsule.
         * @return True if the capsule collides with the cylinder, false otherwise.
         */
        bool CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const override;

        /**
         * @brief Checks for collision between the capsule and a ray.
         * @param ray The ray to check for collision with the capsule.
         * @return Information about the collision between the ray and the capsule.
         */
        RayCollision CollisionRay(nexus::shape3D::Ray& ray) const override;

        /**
         * @brief Checks for collision between the capsule and a line segment.
         * @param line The line segment to check for collision with the capsule.
         * @return Information about the collision between the line segment and the capsule.
         */
        RayCollision CollisionLine(const nexus::shape3D::Line& line) const override;

        /**
         * @brief Checks for collision between the capsule and a 3D mesh transformed by a given transformation matrix.
         * @param mesh The mesh to check for collision with the capsule.
         * @param transform The transformation matrix to apply to the mesh vertices.
         * @return True if the capsule collides with the transformed mesh, false otherwise.
         */
        bool CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const override;
    };

}}

#endif //NEXUS_SHAPE_3D_CAPSULE_HPP
