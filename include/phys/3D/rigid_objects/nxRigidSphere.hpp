#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_SPHERE_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_SPHERE_HPP

#include "./nxRigidObject.hpp"

namespace nexus { namespace phys3D {

    /**
     * @brief Struct representing a sphere-shaped 3D physics object.
     */
    class NEXUS_API RigidSphere : public RigidObject
    {
      public:
        /**
         * @brief Constructor for the RigidSphere class.
         * @param position Initial position of the sphere.
         * @param rotation Initial rotation of the sphere.
         * @param radius Radius of the sphere.
         * @param mass Mass of the sphere (0.0f for static spheres).
         */
        RigidSphere(const nexus::math::Vec3& position,
                    const nexus::math::Quaternion& rotation,
                    float radius, float mass = Static);

        /**
         * @brief Constructor for the RigidSphere class using an AABB bounding box.
         * @param bb AABB bounding box defining the sphere's dimensions.
         * @param rotation Initial rotation of the sphere.
         * @param mass Mass of the sphere (0.0f for static spheres).
         */
        RigidSphere(const nexus::shape3D::AABB bb,
                    const nexus::math::Quaternion& rotation = { 0, 0, 0, 1 },
                    float mass = Static);

        /**
         * @brief Constructor for the RigidSphere class using a sphere shape.
         * @param sphere Sphere shape defining the sphere's dimensions.
         * @param rotation Initial rotation of the sphere.
         * @param mass Mass of the sphere (0.0f for static spheres).
         */
        RigidSphere(const nexus::shape3D::Sphere& sphere,
                    const nexus::math::Quaternion& rotation = { 0, 0, 0, 1 },
                    float mass = Static);
    };

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_SPHERE_HPP
