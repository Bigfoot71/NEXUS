#ifndef NEXUS_PHYSICS_2D_HPP
#define NEXUS_PHYSICS_2D_HPP

#include "../../gfx/nxColor.hpp"
#include <box2d/box2d.h>

namespace nexus { namespace phys2D {

    /**
        NOTE: The nexus::math types can be used with Box2D, they are the necessary constructors and operators.
        TODO: Think about a better implementation of Box2D in the library.
     */

    using World = b2World;
    using Body = b2Body;
    using Shape = b2Shape;
    using Fixture = b2Fixture;
    using Joint = b2Joint;
    using DistanceJoint = b2DistanceJoint;
    using FrictionJoint = b2FrictionJoint;
    using GearJoint = b2GearJoint;
    using MotorJoint = b2MotorJoint;
    using MouseJoint = b2MouseJoint;
    using PrismaticJoint = b2PrismaticJoint;
    using PulleyJoint = b2PulleyJoint;
    using RevoluteJoint = b2RevoluteJoint;
    using WeldJoint = b2WeldJoint;
    using WheelJoint = b2WheelJoint;

    using BodyDef = b2BodyDef;
    using FixtureDef = b2FixtureDef;
    using JointDef = b2JointDef;
    using DistanceJointDef = b2DistanceJointDef;
    using FrictionJointDef = b2FrictionJointDef;
    using GearJointDef = b2GearJointDef;
    using MotorJointDef = b2MotorJointDef;
    using MouseJointDef = b2MouseJointDef;
    using PrismaticJointDef = b2PrismaticJointDef;
    using PulleyJointDef = b2PulleyJointDef;
    using RevoluteJointDef = b2RevoluteJointDef;
    using WeldJointDef = b2WeldJointDef;
    using WheelJointDef = b2WheelJointDef;

    using Transform = b2Transform;

    using PolygonShape = b2PolygonShape;
    using CircleShape = b2CircleShape;
    using EdgeShape = b2EdgeShape;
    using ChainShape = b2ChainShape;

    using Contact = b2Contact;
    using Filter = b2Filter;

}}

#endif //NEXUS_PHYSICS_2D_HPP