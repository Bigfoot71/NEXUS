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

#include "phys/3D/nxWorld.hpp"

using namespace nexus;

/* Public Implementation World */

phys3D::World::World(const nexus::math::Vec3& gravity)
{
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);

    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(
        dispatcher, overlappingPairCache,
        solver, collisionConfig);
    
    dynamicsWorld->setGravity(
        btVector3(gravity.x, gravity.y, gravity.z));
}

phys3D::World::~World()
{
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        dynamicsWorld->removeCollisionObject(obj);
    }

    // Destroy objects
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }

    // Destroy world
    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfig;
}

bool phys3D::World::RemoveObject(RigidObject* obj)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i] == obj)
        {
            objects[i] = objects[objects.size()-1];
            objects.pop_back();
            delete obj;

            return true;
        }
    }
    return false;
}

bool phys3D::World::RemoveObject(RigidObject& obj)
{
    return this->RemoveObject(&obj);
}

int phys3D::World::GetObjectCount() const
{
    return objects.size();
}

void phys3D::World::Step(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
    dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep); 
}

phys3D::RigidObject** phys3D::World::begin()
{
    return &objects[0];
}

phys3D::RigidObject** phys3D::World::end()
{
    return &objects[objects.size() - 1];
}

const phys3D::RigidObject* const* phys3D::World::begin() const
{
    return &objects[0];
}

const phys3D::RigidObject* const* phys3D::World::end() const
{
    return &objects[objects.size() - 1];
}
