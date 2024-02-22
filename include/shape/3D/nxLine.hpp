#ifndef NEXUS_SHAPE_3D_LINE_HPP
#define NEXUS_SHAPE_3D_LINE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape3D {

    /**
     * @brief Represents a 3D line segment.
     */
    struct NEXUS_API Line
    {
        math::Vec3 start;   ///< The starting point of the line segment.
        math::Vec3 end;     ///< The ending point of the line segment.

        /**
         * @brief Default constructor initializing start and end to zero vectors.
         */
        Line() : start(), end() { }

        /**
         * @brief Constructs a line segment with the specified start and end points.
         * @param a The starting point of the line segment.
         * @param b The ending point of the line segment.
         */
        Line(const math::Vec3& a, const math::Vec3& b)
        : start(a), end(b)
        { }

        /**
         * @brief Checks for collision with a point.
         * @param point The point to check for collision.
         * @return Information about the collision between the line segment and the point.
         */
        RayCollision CollisionPoint(const nexus::math::Vec3& point) const;

        /**
         * @brief Checks for collision with an axis-aligned bounding box (AABB).
         * @param aabb The AABB to check for collision.
         * @return Information about the collision between the line segment and the AABB.
         */
        RayCollision CollisionAABB(const nexus::shape3D::AABB& aabb) const;

        /**
         * @brief Checks for collision with a cube.
         * @param cube The cube to check for collision.
         * @return Information about the collision between the line segment and the cube.
         */
        RayCollision CollisionCube(const nexus::shape3D::Cube& cube) const;

        /**
         * @brief Checks for collision with a plane.
         * @param plane The plane to check for collision.
         * @return Information about the collision between the line segment and the plane.
         */
        RayCollision CollisionPlane(const nexus::shape3D::Plane& plane) const;

        /**
         * @brief Checks for collision with a sphere.
         * @param sphere The sphere to check for collision.
         * @return Information about the collision between the line segment and the sphere.
         */
        RayCollision CollisionSphere(const nexus::shape3D::Sphere& sphere) const;

        /**
         * @brief Checks for collision with a capsule.
         * @param capsule The capsule to check for collision.
         * @return Information about the collision between the line segment and the capsule.
         */
        RayCollision CollisionCapsule(const nexus::shape3D::Capsule& capsule) const;

        /**
         * @brief Checks for collision with a cylinder.
         * @param cylinder The cylinder to check for collision.
         * @return Information about the collision between the line segment and the cylinder.
         */
        RayCollision CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const;

        /**
         * @brief Checks for collision with a ray.
         * @param ray The ray to check for collision.
         * @return Information about the collision between the line segment and the ray.
         */
        RayCollision CollisionRay(nexus::shape3D::Ray& ray) const;

        /**
         * @brief Checks for collision with another line segment.
         * @param line The line segment to check for collision.
         * @return Information about the collision between the line segment and the other line segment.
         */
        RayCollision CollisionLine(const nexus::shape3D::Line& line) const;

        /**
         * @brief Checks for collision with a mesh transformed by a matrix.
         * @param mesh The mesh to check for collision.
         * @param transform The transformation matrix to apply to the mesh.
         * @return Information about the collision between the line segment and the transformed mesh.
         */
        RayCollision CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const;

        /**
         * @brief Checks for collision with a triangle.
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @return Information about the collision between the line segment and the triangle.
         */
        RayCollision CollisionTriangle(const math::Vec3& v0, const math::Vec3& v1, const math::Vec3& v2) const;

        /**
         * @brief Checks for collision with a quad.
         * @param v0 The first vertex of the quad.
         * @param v1 The second vertex of the quad.
         * @param v2 The third vertex of the quad.
         * @param v3 The fourth vertex of the quad.
         * @return Information about the collision between the line segment and the quad.
         */
        RayCollision CollisionQuad(const math::Vec3& v0, const math::Vec3& v1, const math::Vec3& v2, const math::Vec3& v3) const;
    };

}}

#endif //NEXUS_SHAPE_3D_LINE_HPP
