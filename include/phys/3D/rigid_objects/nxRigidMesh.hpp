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

#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_MESH_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_MESH_HPP

#include "./nxRigidObject.hpp"
#include "../../../shape/3D/nxAABB.hpp"

namespace nexus { namespace phys3D {

    /**
     * @brief Struct representing a mesh-shaped 3D physics object.
     */
    struct NEXUS_API RigidMesh : RigidObject
    {
        /**
         * @brief Constructor for the RigidMesh class.
         * @tparam T_Mesh Type of the mesh.
         * @param mesh Mesh defining the object's shape.
         * @param position Initial position of the object.
         * @param rotation Initial rotation of the object.
         * @param radius Radius of the object (used for static mesh shapes).
         * @param mass Mass of the object (0.0f for static mesh shapes).
         */
        template <typename T_Mesh>
        RigidMesh(const T_Mesh& mesh, const math::Vec3& position, const math::Quaternion& rotation, float radius, float mass = Static)
        : RigidObject(mass == Static ? Shape::Mesh : Shape::Hull)
        {
            if (mass == Static)
            {
                btTriangleMesh* triMesh = new btTriangleMesh(
                    mesh->numVertices >= std::numeric_limits<Uint16>::max(),
                    false);

                triMesh->preallocateVertices(mesh->numVertices);

                const math::Vec3 *vertices = mesh->vertices.data();

                for (int i = 0; i < mesh->numVertices; i += 3)
                {
                    const math::Vec3& vertexA = vertices[i];
                    const math::Vec3& vertexB = vertices[i+1];
                    const math::Vec3& vertexC = vertices[i+2];

                    triMesh->addTriangle(
                        btVector3(vertexA.x, vertexA.y, vertexA.z),
                        btVector3(vertexB.x, vertexB.y, vertexB.z),
                        btVector3(vertexC.x, vertexC.y, vertexC.z));
                }

                this->colliderShape = new btBvhTriangleMeshShape(
                    triMesh, true);
            }
            else // Convex hull
            {
                this->colliderShape = new btConvexHullShape(
                    reinterpret_cast<const float*>(mesh->vertices.data()),
                    mesh->vertices.size(), sizeof(math::Vec3));
            }

            this->body = this->CreateBody(position, rotation, mass);
        }
    };

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_MESH_HPP
