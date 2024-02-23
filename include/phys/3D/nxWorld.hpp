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

#ifndef RAYFLEX_PHYS_3D_WORLD_HPP
#define RAYFLEX_PHYS_3D_WORLD_HPP

#include "./rigid_objects/nxRigidObject.hpp"
#include "../../math/nxVec3.hpp"

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btScalar.h>
#include <iterator>

namespace nexus { namespace phys3D {

    /**
     * @brief Class representing a 3D physics world using Bullet3.
     */
    class NEXUS_API World
    {
      private:
        btDefaultCollisionConfiguration* collisionConfig;   ///< Configuration for Bullet collision detection stack allocator
        btCollisionDispatcher* dispatcher;                  ///< Collision dispatcher, handles collision events

        btBroadphaseInterface* overlappingPairCache;        ///< Broadphase interface, detects overlapping objects
        btSequentialImpulseConstraintSolver* solver;        ///< Constraint solver

        btDiscreteDynamicsWorld* dynamicsWorld;             ///< The world where physics takes place
        btAlignedObjectArray<RigidObject*> objects;         ///< Vector to store physics objects

      public:
        /**
         * @brief Constructor for the physics world.
         * @param gravity Gravity vector for the world.
         */
        World(const nexus::math::Vec3& gravity = { 0, -9.81, 0 });

        /**
         * @brief Destructor for the physics world.
         */
        ~World();

        /**
         * @brief Adds a physics object to the world.
         * @tparam T_Object Type of the object to be added.
         * @tparam Args Variadic template for constructor arguments.
         * @param args Constructor arguments for the object.
         * @return Pointer to the added object.
         */
        template <typename T_Object, typename... Args>
        T_Object& AddObject(Args... args);

        /**
         * @brief Removes a physics object from the world.
         * @param obj Pointer to the object to be removed.
         * @return True if the object was successfully removed, false otherwise.
         */
        bool RemoveObject(RigidObject* obj);

        /**
         * @brief Removes a physics object from the world.
         * @param obj Reference of the object to remove.
         * @return True if the object was successfully removed, false otherwise.
         */
        bool RemoveObject(RigidObject& obj);

        /**
         * @brief Retrieves the count of objects in the world.
         * @return Number of objects in the world.
         */
        int GetObjectCount() const;

        /**
         * @brief Advances the simulation by a specified time step.
         * @param timeStep Time step for the simulation.
         * @param maxSubSteps Maximum sub-steps.
         * @param fixedTimeStep Fixed time step.
         */
        void Step(btScalar timeStep, int maxSubSteps = 1, btScalar fixedTimeStep = static_cast<btScalar>(0.016666667));

        /**
         * @brief Provides an iterator to the beginning of the world objects.
         * @return Iterator pointing to the first object.
         */
        RigidObject** begin();

        /**
         * @brief Provides an iterator to the end of the world objects.
         * @return Iterator pointing to the last object.
         */
        RigidObject** end();

        /**
         * @brief Provides an const iterator to the beginning of the world objects.
         * @return Cont iterator pointing to the first object.
         */
        const RigidObject* const* begin() const;

        /**
         * @brief Provides an const iterator to the end of the world objects.
         * @return Const iterator pointing to the last object.
         */
        const RigidObject* const* end() const;
    };


    /* Public Implementation World */

    template <typename T_Object, typename... Args>
    T_Object& World::AddObject(Args... args)
    {
        T_Object *object = new T_Object(args...);
        dynamicsWorld->addRigidBody(object->body);
        objects.push_back(object);
        return *object;
    }

}}

#endif //RAYFLEX_PHYS_3D_WORLD_HPP
