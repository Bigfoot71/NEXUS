#ifndef NEXUS_SHAPE_3D_CYLINDER_HPP
#define NEXUS_SHAPE_3D_CYLINDER_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape3D {

    /**
     * @brief Represents a 3D cylinder shape.
     */
    struct NEXUS_API Cylinder : CollisionShape
    {
        math::Vec3 center;  ///< The center point of the cylinder.
        float radius;       ///< The radius of the cylinder.
        float height;       ///< The height of the cylinder.

        /**
         * @brief Default constructor initializing center, radius, and height to zero.
         */
        Cylinder() : center(), radius(), height()
        { }

        /**
         * @brief Constructs a cylinder with the specified center, radius, and height.
         * @param center The center point of the cylinder.
         * @param radius The radius of the cylinder.
         * @param height The height of the cylinder.
         */
        Cylinder(const math::Vec3& center, float radius, float height)
        : center(center), radius(radius), height(height)
        { }

        /**
         * @brief Computes the axis-aligned bounding box (AABB) of the cylinder.
         * @return The AABB of the cylinder.
         */
        AABB GetAABB() const;

        /**
         * @brief Checks for collision with a point.
         * @param point The point to check for collision.
         * @return True if the cylinder collides with the point, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec3& point) const override;

        /**
         * @brief Checks for collision with an axis-aligned bounding box (AABB).
         * @param aabb The AABB to check for collision.
         * @return True if the cylinder collides with the AABB, false otherwise.
         */
        bool CollisionAABB(const nexus::shape3D::AABB& aabb) const override;

        /**
         * @brief Checks for collision with a cube.
         * @param cube The cube to check for collision.
         * @return True if the cylinder collides with the cube, false otherwise.
         */
        bool CollisionCube(const nexus::shape3D::Cube& cube) const override;

        /**
         * @brief Checks for collision with a plane.
         * @param plane The plane to check for collision.
         * @return True if the cylinder collides with the plane, false otherwise.
         */
        bool CollisionPlane(const nexus::shape3D::Plane& plane) const override;

        /**
         * @brief Checks for collision with a sphere.
         * @param sphere The sphere to check for collision.
         * @return True if the cylinder collides with the sphere, false otherwise.
         */
        bool CollisionSphere(const nexus::shape3D::Sphere& sphere) const override;

        /**
         * @brief Checks for collision with a capsule.
         * @param capsule The capsule to check for collision.
         * @return True if the cylinder collides with the capsule, false otherwise.
         */
        bool CollisionCapsule(const nexus::shape3D::Capsule& capsule) const override;

        /**
         * @brief Checks for collision with another cylinder.
         * @param cylinder The cylinder to check for collision.
         * @return True if the cylinder collides with the other cylinder, false otherwise.
         */
        bool CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const override;

        /**
         * @brief Checks for collision with a ray.
         * @param ray The ray to check for collision.
         * @return Information about the collision between the cylinder and the ray.
         */
        RayCollision CollisionRay(nexus::shape3D::Ray& ray) const override;

        /**
         * @brief Checks for collision with a line.
         * @param line The line to check for collision.
         * @return Information about the collision between the cylinder and the line.
         */
        RayCollision CollisionLine(const nexus::shape3D::Line& line) const override;

        /**
         * @brief Checks for collision with a mesh transformed by a matrix.
         * @param mesh The mesh to check for collision.
         * @param transform The transformation matrix to apply to the mesh.
         * @return True if the cylinder collides with the transformed mesh, false otherwise.
         */
        bool CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const override;
    };

}}

#endif // NEXUS_SHAPE_3D_CYLINDER_HPP
