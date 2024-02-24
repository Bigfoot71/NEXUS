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

#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_MODEL_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_MODEL_HPP

#include "./nxRigidObject.hpp"
#include "../../../shape/3D/nxAABB.hpp"
#include "../../../core/nxException.hpp"

namespace nexus { namespace phys3D {

    /**
     * @brief Class representing a 3D physics model object.
     */
    class NEXUS_API RigidModel : public RigidObject
    {
    private:
        /**
         * @brief Generates a Bullet collision shape from a mesh.
         * @param mesh The mesh from which to generate the shape.
         * @return Pointer to the generated collision shape.
         */
        template <typename T_Mesh>
        btCollisionShape* GenShapeMesh(const T_Mesh& mesh);

        /**
         * @brief Generates a Bullet collision shape from a convex hull.
         * @param mesh The mesh from which to generate the shape.
         * @return Pointer to the generated collision shape.
         */
        template <typename T_Mesh>
        btCollisionShape* GenShapeHull(const T_Mesh& mesh);

        /**
         * @brief Generates a Bullet collision shape for a cube.
         * @param mesh The mesh from which to generate the shape.
         * @return Pointer to the generated collision shape.
         */
        template <typename T_Mesh>
        btCollisionShape* GenShapeCube(const T_Mesh& mesh);

        /**
         * @brief Generates a Bullet collision shape for a sphere.
         * @param mesh The mesh from which to generate the shape.
         * @return Pointer to the generated collision shape.
         */
        template <typename T_Mesh>
        btCollisionShape* GenShapeSphere(const T_Mesh& mesh);

        /**
         * @brief Generates a Bullet collision shape for a capsule.
         * @param mesh The mesh from which to generate the shape.
         * @return Pointer to the generated collision shape.
         */
        template <typename T_Mesh>
        btCollisionShape* GenShapeCapsule(const T_Mesh& mesh);

        /**
         * @brief Creates a Bullet collision shape from the model.
         * @return Pointer to the created collision shape.
         */
        template <typename T_Model>
        btCollisionShape* CreateCollisionShapeFromModel(const T_Model& model);

    public:
        /**
         * @brief Constructor for the RigidModel class.
         * @tparam T_Model Type of the model.
         * @param model Model defining the object's shape.
         * @param position Initial position of the object.
         * @param rotation Initial rotation of the object.
         * @param shape Shape of the collision (Mesh by default).
         * @param mass Mass of the object (0.0f for static shapes).
         */
        template <typename T_Model>
        RigidModel(const T_Model& model,
                   const math::Vec3& position,
                   const math::Quaternion& rotation,
                   Shape shape = Shape::Mesh,
                   float mass = Static);
    };


    /* Private Implementation Model */

    template <typename T_Mesh>
    btCollisionShape* RigidModel::GenShapeMesh(const T_Mesh& mesh)
    {
        btTriangleMesh* triMesh = new btTriangleMesh(
            mesh->numVertices >= std::numeric_limits<Uint16>::max(),
            false);

        triMesh->preallocateVertices(mesh->numVertices);

        const math::Vec3 *positions = mesh->positions.data();

        for (int i = 0; i < mesh->numVertices; i += 3)
        {
            const math::Vec3& vertexA = positions[i];
            const math::Vec3& vertexB = positions[i+1];
            const math::Vec3& vertexC = positions[i+2];

            triMesh->addTriangle(
                btVector3(vertexA.x, vertexA.y, vertexA.z),
                btVector3(vertexB.x, vertexB.y, vertexB.z),
                btVector3(vertexC.x, vertexC.y, vertexC.z));
        }

        return new btBvhTriangleMeshShape(triMesh, true);
    }

    template <typename T_Mesh>
    btCollisionShape* RigidModel::GenShapeHull(const T_Mesh& mesh)
    {
        return new btConvexHullShape(reinterpret_cast<const float*>(mesh->positions.data()), mesh->positions.size(), sizeof(math::Vec3));
    }

    template <typename T_Mesh>
    btCollisionShape* RigidModel::GenShapeCube(const T_Mesh& mesh)
    {
        const shape3D::AABB bb = mesh->GetAABB();
        const math::Vec3 size = (math::Vec3(bb.max) - bb.min);

        return new btBoxShape(btVector3(
            btScalar(size.x * 0.5f),
            btScalar(size.y * 0.5f),
            btScalar(size.z * 0.5f)));
    }

    template <typename T_Mesh>
    btCollisionShape* RigidModel::GenShapeSphere(const T_Mesh& mesh)
    {
        const shape3D::AABB bb = mesh->GetAABB();
        const math::Vec3 size = (math::Vec3(bb.max) - bb.min);

        const float diameter = (size.x > size.y)
            ? ( (size.x > size.z) ? size.x : size.z )
            : ( (size.y > size.z) ? size.y : size.z );

        return new btSphereShape(diameter * 0.5f);
    }

    template <typename T_Mesh>
    btCollisionShape* RigidModel::GenShapeCapsule(const T_Mesh& mesh)
    {
        const shape3D::AABB bb = mesh->GetAABB();
        const math::Vec3 size = (math::Vec3(bb.max) - bb.min);
        const float radius = ( (size.x > size.z) ? size.x : size.z ) * 0.5f;
        return new btCapsuleShape(radius, size.z + 2 * radius);
    }

    template <typename T_Model>
    btCollisionShape* RigidModel::CreateCollisionShapeFromModel(const T_Model& model)
    {
        btCollisionShape *(RigidModel::*generator)(const typename T_Model::Type::Mesh& mesh) = nullptr;

        switch (this->shape)
        {
            case Shape::Mesh:       generator = &RigidModel::GenShapeMesh;      break;
            case Shape::Hull:       generator = &RigidModel::GenShapeHull;      break;
            case Shape::Cube:       generator = &RigidModel::GenShapeCube;      break;
            case Shape::Sphere:     generator = &RigidModel::GenShapeSphere;    break;
            case Shape::Capsule:    generator = &RigidModel::GenShapeCapsule;   break;

            default:
            {
                throw core::NexusException("phys3D::Model",
                    "Shape type given when creating the Model physical object not supported.");
            }
        }

        btCollisionShape *cShape = nullptr;

        if (model->meshes.size() == 1)
        {
            cShape = (this->*generator)(model->meshes[0]);
        }
        else
        {
            btTransform transform; transform.setIdentity();
            btCompoundShape* compoundShape = new btCompoundShape();

            for (int i = 0; i < model->meshes.size(); i++)
            {
                compoundShape->addChildShape(transform,
                    (this->*generator)(model->meshes[i]));
            }

            cShape = compoundShape;
        }

        return cShape;
    }


    /* Public Implementation Model */

    template <typename T_Model>
    RigidModel::RigidModel(const T_Model& model,
                           const math::Vec3& position,
                           const math::Quaternion& rotation,
                           Shape shape,
                           float mass)
    : RigidObject(shape)
    {
        assert(shape);
        if (shape == Shape::Mesh && mass != Static) mass = Static;
        this->colliderShape = this->CreateCollisionShapeFromModel(model);
        this->body = this->CreateBody(position, rotation, mass);
    }

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_MODEL_HPP