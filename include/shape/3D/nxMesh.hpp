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

#ifndef NEXUS_SHAPE_3D_MESH_HPP
#define NEXUS_SHAPE_3D_MESH_HPP

#include "./nxCollisionShape.hpp"

#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxMath.hpp"
#include "../../math/nxVec4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "../../core/nxLog.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>
#include <array>

namespace nexus { namespace shape3D {

    /**
     * @brief Represents a 3D mesh.
     */
    struct NEXUS_API Mesh
    {
        static constexpr Uint8 VertexBufferCount = 7;

        std::vector<math::Vec3> positions;           ///< Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
        std::vector<math::Vec2> texcoords;          ///< Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
        std::vector<math::Vec3> normals;            ///< Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
        std::vector<gfx::Color> colors;             ///< Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
        std::vector<math::Vec4> tangents;           ///< Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
        std::vector<math::Vec2> texcoords2;         ///< Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
        std::vector<Uint16> indices;                ///< Vertex indices (in case vertex data comes indexed)

        int numVertices;                            ///< Number of vertices stored in arrays
        int numFaces;                               ///< Number of triangles stored (indexed or not)

        /**
         * @brief Generates a polygon mesh.
         * @param sides The number of sides of the polygon.
         * @param radius The radius of the polygon.
         * @return The generated polygon mesh.
         */
        static Mesh Polygon(int sides, float radius);

        /**
         * @brief Generates a plane mesh.
         * @param width The width of the plane.
         * @param length The length of the plane.
         * @param resX The resolution along the X-axis.
         * @param resZ The resolution along the Z-axis.
         * @return The generated plane mesh.
         */
        static Mesh Plane(float width, float length, int resX, int resZ);

        /**
         * @brief Generates a cube mesh.
         * @param width The width of the cube.
         * @param height The height of the cube.
         * @param length The length of the cube.
         * @return The generated cube mesh.
         */
        static Mesh Cube(float width, float height, float length);

        /**
         * @brief Generates a sphere mesh.
         * @param radius The radius of the sphere.
         * @param rings The number of rings in the sphere.
         * @param slices The number of slices in the sphere.
         * @return The generated sphere mesh.
         */
        static Mesh Sphere(float radius, int rings, int slices);

        /**
         * @brief Generates a hemisphere mesh.
         * @param radius The radius of the hemisphere.
         * @param rings The number of rings in the hemisphere.
         * @param slices The number of slices in the hemisphere.
         * @return The generated hemisphere mesh.
         */
        static Mesh HemiSphere(float radius, int rings, int slices);

        /**
         * @brief Generates a cylinder mesh.
         * @param radius The radius of the cylinder.
         * @param height The height of the cylinder.
         * @param slices The number of slices in the cylinder.
         * @return The generated cylinder mesh.
         */
        static Mesh Cylinder(float radius, float height, int slices);

        /**
         * @brief Generates a cone mesh.
         * @param radius The radius of the cone.
         * @param height The height of the cone.
         * @param slices The number of slices in the cone.
         * @return The generated cone mesh.
         */
        static Mesh Cone(float radius, float height, int slices);

        /**
         * @brief Generates a torus mesh.
         * @param radius The radius of the torus.
         * @param size The size of the torus.
         * @param radSeg The number of radial segments in the torus.
         * @param sides The number of sides in the torus.
         * @return The generated torus mesh.
         */
        static Mesh Torus(float radius, float size, int radSeg, int sides);

        /**
         * @brief Generates a trefoil knot mesh.
         * @param radius The radius of the trefoil knot.
         * @param size The size of the trefoil knot.
         * @param radSeg The number of radial segments in the trefoil knot.
         * @param sides The number of sides in the trefoil knot.
         * @return The generated trefoil knot mesh.
         */
        static Mesh TrefoilKnot(float radius, float size, int radSeg, int sides);

        /**
         * @brief Generates a mesh from a heightmap.
         * @param heightmap The heightmap surface.
         * @param size The size of the mesh.
         * @return The generated mesh.
         */
        static Mesh Heightmap(const gfx::Surface& heightmap, const math::Vec3& size);

        /**
         * @brief Generates a mesh from a cubicmap.
         * @param cubicmap The cubicmap surface.
         * @param cubeSize The size of the cube.
         * @return The generated mesh.
         */
        static Mesh Cubicmap(const gfx::Surface& cubicmap, const math::Vec3& cubeSize);

        /**
         * @brief Default constructor that initializes an empty mesh which the user can generate manually.
         */
        Mesh() = default;

        /**
         * @brief Copy constructor for the mesh.
         * @param other The mesh to be copied from.
         */
        Mesh(const Mesh&);

        /**
         * @brief Move constructor for the mesh.
         * @param other The mesh to be moved from.
         */
        Mesh(Mesh&&) noexcept;

        /**
         * @brief Copy assignment operator for the mesh.
         * @param other The mesh to be copied from.
         * @return Reference to the assigned mesh.
         */
        Mesh& operator=(const Mesh&);

        /**
         * @brief Move assignment operator for the mesh.
         * @param other The mesh to be moved from.
         * @return Reference to the moved mesh.
         */
        Mesh& operator=(Mesh&&) noexcept;

        /**
         * @brief Default virtual destructor.
         */
        virtual ~Mesh() = default;

        /**
         * @brief Computes the axis-aligned bounding box (AABB) of the mesh.
         * @return The AABB of the mesh.
         */
        shape3D::AABB GetAABB() const;

        /**
         * @brief Generates the tangents of the mesh.
         * Calling this function will overwrite any existing tangents and regenerate them.
         */
        void GenTangents();

        /**
         * @brief Checks for collision with a point.
         * @param point The point to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the point, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec3& point, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with another mesh.
         * @param mesh The mesh to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the other mesh, false otherwise.
         */
        bool CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with an axis-aligned bounding box (AABB).
         * @param aabb The AABB to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the AABB, false otherwise.
         */
        bool CollisionAABB(const nexus::shape3D::AABB& aabb, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a cube.
         * @param cube The cube to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the cube, false otherwise.
         */
        bool CollisionCube(const nexus::shape3D::Cube& cube, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a plane.
         * @param plane The plane to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the plane, false otherwise.
         */
        bool CollisionPlane(const nexus::shape3D::Plane& plane, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a sphere.
         * @param sphere The sphere to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the sphere, false otherwise.
         */
        bool CollisionSphere(const nexus::shape3D::Sphere& sphere, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a capsule.
         * @param capsule The capsule to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the capsule, false otherwise.
         */
        bool CollisionCapsule(const nexus::shape3D::Capsule& capsule, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a cylinder.
         * @param cylinder The cylinder to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return True if the mesh collides with the cylinder, false otherwise.
         */
        bool CollisionCylinder(const nexus::shape3D::Cylinder& cylinder, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a ray.
         * @param ray The ray to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return Information about the collision between the mesh and the ray.
         */
        RayCollision CollisionRay(nexus::shape3D::Ray& ray, const math::Mat4& meshTransform = math::Mat4::Identity()) const;

        /**
         * @brief Checks for collision with a line.
         * @param line The line to check for collision.
         * @param meshTransform The transformation matrix of the mesh.
         * @return Information about the collision between the mesh and the line.
         */
        RayCollision CollisionLine(const nexus::shape3D::Line& line, const math::Mat4& meshTransform = math::Mat4::Identity()) const;
    };

}}

#endif //NEXUS_SHAPE_3D_MESH_HPP
