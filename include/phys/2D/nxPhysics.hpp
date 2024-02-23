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