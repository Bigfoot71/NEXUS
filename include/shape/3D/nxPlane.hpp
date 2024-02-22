#ifndef NEXUS_SHAPE_3D_PLANE_HPP
#define NEXUS_SHAPE_3D_PLANE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape3D {

    /**
     * @brief Represents a 3D plane.
     */
    struct NEXUS_API Plane : CollisionShape
    {
        math::Vec3 center;  ///< The center of the plane.
        math::Vec2 size;    ///< The size of the plane in the X and Z axes.

        /**
         * @brief Default constructor initializing the center and size to zero vectors.
         */
        Plane() : center(), size() { }

        /**
         * @brief Constructor that initializes the plane with a specified center and size.
         * @param center The center of the plane.
         * @param size The size of the plane in the X and Z axes.
         */
        Plane(const math::Vec3& center, const math::Vec2& size)
        : center(center), size(size)
        { }

        /**
         * @brief Computes the axis-aligned bounding box (AABB) of the plane.
         * @return The AABB of the plane.
         */
        AABB GetAABB() const;

        /**
         * @brief Checks for collision with a point.
         * @param point The point to check for collision.
         * @return True if the plane collides with the point, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec3& point) const override;

        /**
         * @brief Checks for collision with an axis-aligned bounding box (AABB).
         * @param aabb The AABB to check for collision.
         * @return True if the plane collides with the AABB, false otherwise.
         */
        bool CollisionAABB(const nexus::shape3D::AABB& aabb) const override;

        /**
         * @brief Checks for collision with a cube.
         * @param cube The cube to check for collision.
         * @return True if the plane collides with the cube, false otherwise.
         */
        bool CollisionCube(const nexus::shape3D::Cube& cube) const override;

        /**
         * @brief Checks for collision with another plane.
         * @param plane The plane to check for collision.
         * @return True if the plane collides with the other plane, false otherwise.
         */
        bool CollisionPlane(const nexus::shape3D::Plane& plane) const override;

        /**
         * @brief Checks for collision with a sphere.
         * @param sphere The sphere to check for collision.
         * @return True if the plane collides with the sphere, false otherwise.
         */
        bool CollisionSphere(const nexus::shape3D::Sphere& sphere) const override;

        /**
         * @brief Checks for collision with a capsule.
         * @param capsule The capsule to check for collision.
         * @return True if the plane collides with the capsule, false otherwise.
         */
        bool CollisionCapsule(const nexus::shape3D::Capsule& capsule) const override;

        /**
         * @brief Checks for collision with a cylinder.
         * @param cylinder The cylinder to check for collision.
         * @return True if the plane collides with the cylinder, false otherwise.
         */
        bool CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const override;

        /**
         * @brief Checks for collision with a ray.
         * @param ray The ray to check for collision.
         * @return Information about the collision between the plane and the ray.
         */
        RayCollision CollisionRay(nexus::shape3D::Ray& ray) const override;

        /**
         * @brief Checks for collision with a line.
         * @param line The line to check for collision.
         * @return Information about the collision between the plane and the line.
         */
        RayCollision CollisionLine(const nexus::shape3D::Line& line) const override;

        /**
         * @brief Checks for collision with a mesh transformed by a matrix.
         * @param mesh The mesh to check for collision.
         * @param transform The transformation matrix to apply to the mesh.
         * @return True if the plane collides with the transformed mesh, false otherwise.
         */
        bool CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const override;
    };

}}

#endif //NEXUS_SHAPE_3D_PLANE_HPP
