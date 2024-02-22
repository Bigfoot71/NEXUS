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
