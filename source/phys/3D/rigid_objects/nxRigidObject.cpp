#include "phys/3D/rigid_objects/nxRigidObject.hpp"
#include "LinearMath/btTransform.h"
#include "math/nxMath.hpp"

using namespace nexus;

/* Protected Implementation RigidObject */

btRigidBody* phys3D::RigidObject::CreateBody(const math::Vec3& position, const math::Quaternion& rotation, float mass)
{
    btVector3 localInertia(0, 0, 0);

    // Objects with 0 mass are static
    if (mass != 0) colliderShape->calculateLocalInertia(mass, localInertia);

    // Set default motion state with location and rotation
    btDefaultMotionState *motionState = new btDefaultMotionState(
        btTransform(rotation, position));

    // Return rigid body
    return new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
        mass, motionState, colliderShape, localInertia));
}

phys3D::RigidObject::RigidObject(Shape shape)
: colliderShape(nullptr)
, body(nullptr)
, shape(shape)
{ }


/* Public Implementation RigidObject */

phys3D::RigidObject::~RigidObject()
{
    if (body != nullptr)
    {
        if (body->getMotionState())
        {
            delete body->getMotionState();
        }
        delete body;
        body = nullptr;
    }
    if (colliderShape != nullptr)
    {
        delete colliderShape;
        colliderShape = nullptr;
    }
}

bool phys3D::RigidObject::IsReady() const
{
    return body && body->getMotionState();
}

math::Mat4 phys3D::RigidObject::GetTransform() const
{
    btTransform btTrans;
    body->getMotionState()->getWorldTransform(btTrans);

    float mat[16];
    btTrans.getOpenGLMatrix(mat);

    math::Mat4 transform;
    std::memcpy(&transform, mat, sizeof(transform));

    return transform.Transpose();
}

math::Vec3 phys3D::RigidObject::GetPosition() const
{
    btTransform btTrans;
    body->getMotionState()->getWorldTransform(btTrans);

    return btTrans.getOrigin();
}

math::Quaternion phys3D::RigidObject::GetRotation() const
{
    btTransform btTrans;
    body->getMotionState()->getWorldTransform(btTrans);

    return btTrans.getRotation();
}

math::Vec3 phys3D::RigidObject::GetRotationAxis() const
{
    btTransform btTrans;
    body->getMotionState()->getWorldTransform(btTrans);

    return btTrans.getRotation().getAxis();
}

float phys3D::RigidObject::GetAngle() const
{
    btTransform btTrans;
    body->getMotionState()->getWorldTransform(btTrans);

    return btTrans.getRotation().getAngle() * math::Rad2Deg;
}

math::Vec3 phys3D::RigidObject::GetLinearVelocity() const
{
    return body->getLinearVelocity();
}

math::Vec3 phys3D::RigidObject::GetAngularVelocity() const
{
    return body->getAngularVelocity();
}

float phys3D::RigidObject::GetFriction() const
{
    return body->getFriction();
}

float phys3D::RigidObject::GetRestitution() const
{
    return body->getRestitution();
}

float phys3D::RigidObject::GetLinearDamping() const
{
    return body->getLinearDamping();
}

float phys3D::RigidObject::GetAngularDamping() const
{
    return body->getAngularDamping();
}

math::Vec3 phys3D::RigidObject::GetLinearFactor() const
{
    return body->getLinearFactor();
}

math::Vec3 phys3D::RigidObject::GetAngularFactor() const
{
    return body->getAngularFactor();
}

void phys3D::RigidObject::SetTransform(const math::Mat4& transform)
{
    body->getMotionState()->setWorldTransform(btTransform(
        transform.GetRotation(), transform.GetTranslation()));
}

void phys3D::RigidObject::SetPosition(const math::Vec3& position)
{
    btTransform btTrans;

    body->getMotionState()->getWorldTransform(btTrans);
    btTrans.setOrigin(position);

    body->setWorldTransform(btTrans);
    body->getMotionState()->setWorldTransform(btTrans);
}

void phys3D::RigidObject::SetRotation(const math::Quaternion& quat)
{
    btTransform btTrans;

    body->getMotionState()->getWorldTransform(btTrans);
    btTrans.setRotation(quat);

    body->setWorldTransform(btTrans);
    body->getMotionState()->setWorldTransform(btTrans);
}

void phys3D::RigidObject::SetRotation(const math::Vec3& axis, float angle)
{
    btTransform btTrans;

    body->getMotionState()->getWorldTransform(btTrans);
    btTrans.setRotation(btQuaternion(axis, angle * math::Deg2Rad));

    body->setWorldTransform(btTrans);
    body->getMotionState()->setWorldTransform(btTrans);
}

void phys3D::RigidObject::SetLinearVelocity(const math::Vec3& linearVelocity)
{
    body->setLinearVelocity(linearVelocity);
}

void phys3D::RigidObject::SetAngularVelocity(const math::Vec3& angularVelocity)
{
    body->setAngularVelocity(angularVelocity);
}

void phys3D::RigidObject::SetFriction(float friction)
{
    body->setFriction(friction);
}

void phys3D::RigidObject::SetRestitution(float restitution)
{
    body->setRestitution(restitution);
}

void phys3D::RigidObject::SetDamping(float linearDamping, float angularDamping)
{
    body->setDamping(linearDamping, angularDamping);
}

void phys3D::RigidObject::SetLinearFactor(const math::Vec3& linearFactor)
{
    body->setLinearFactor(linearFactor);
}

void phys3D::RigidObject::SetAngularFactor(const math::Vec3& angularFactor)
{
    body->setAngularFactor(angularFactor);
}

void phys3D::RigidObject::ApplyForce(const math::Vec3& force, const math::Vec3& relativePos)
{
    body->applyForce(force, relativePos);
}

void phys3D::RigidObject::ApplyImpulse(const math::Vec3& impulse, const math::Vec3& relativePos)
{
    body->applyImpulse(impulse, relativePos);
}

void phys3D::RigidObject::ApplyPushImpulse(const math::Vec3& impulse, const math::Vec3& relativePos)
{
    body->applyPushImpulse(impulse, relativePos);
}

void phys3D::RigidObject::ApplyCentralForce(const math::Vec3& force)
{
    body->applyCentralForce(force);
}

void phys3D::RigidObject::ApplyCentralImpulse(const math::Vec3& impulse)
{
    body->applyCentralImpulse(impulse);
}

void phys3D::RigidObject::ApplyCentralPushImpulse(const math::Vec3& impulse)
{
    body->applyCentralPushImpulse(impulse);
}

void phys3D::RigidObject::ApplyTorque(const math::Vec3& torque)
{
    body->applyTorque(torque);
}

void phys3D::RigidObject::ApplyTorqueImpulse(const math::Vec3& torqueImpulse)
{
    body->applyTorqueImpulse(torqueImpulse);
}

void phys3D::RigidObject::ApplyTorqueTurnImpulse(const math::Vec3& torqueImpulse)
{
    body->applyTorqueTurnImpulse(torqueImpulse);
}
