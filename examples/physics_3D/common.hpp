#ifndef PHYSICS_3D_COMMON_HPP
#define PHYSICS_3D_COMMON_HPP

#include <nexus.hpp>

struct Object
{
    nexus::phys3D::RigidObject &physicalObject;
    nexus::gl::Model model;
    nexus::gfx::Color color;

    // Custom
    Object(nexus::gl::Model&& model,
           nexus::phys3D::World *world,
           const nexus::math::Vec3& position,
           const nexus::math::Quaternion& rotation,
           nexus::phys3D::Shape shape,
           float mass,
           nexus::gfx::Color color)
    : physicalObject(world->AddObject<nexus::phys3D::RigidModel>(model, position, rotation, shape, mass))
    , model(std::move(model))
    , color(color)
    { }

    // Cube
    Object(nexus::gl::Context& ctx,
           nexus::phys3D::World *world,
           const nexus::math::Vec3& position,
           const nexus::math::Quaternion& rotation,
           const nexus::math::Vec3& size,
           float mass,
           nexus::gfx::Color color)
    : physicalObject(world->AddObject<nexus::phys3D::RigidCube>(position, rotation, size, mass))
    , model(ctx, nexus::gl::Mesh::Cube(ctx, size.x, size.y, size.z))
    , color(color)
    { }

    // Sphere
    Object(nexus::gl::Context& ctx,
           nexus::phys3D::World *world,
           const nexus::math::Vec3& position,
           const nexus::math::Quaternion& rotation,
           float radius,
           float mass,
           nexus::gfx::Color color)
    : physicalObject(world->AddObject<nexus::phys3D::RigidSphere>(position, rotation, radius, mass))
    , model(ctx, nexus::gl::Mesh::Sphere(ctx, radius, 24, 24))
    , color(color)
    { }

    void Draw()
    {
        physicalObject.DrawModel(model, color);
    }
};

#endif //PHYSICS_3D_COMMON_HPP
