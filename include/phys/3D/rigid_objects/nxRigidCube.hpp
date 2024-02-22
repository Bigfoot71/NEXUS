#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_CUBE_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_CUBE_HPP

#include "./nxRigidObject.hpp"
#include "../../../shape/3D/nxAABB.hpp"

namespace nexus { namespace phys3D {

    /**
     * @brief Struct representing a cube-shaped 3D physics object.
     */
    class NEXUS_API RigidCube : public RigidObject
    {
      public:
        /**
         * @brief Constructor for the RigidCube class.
         * @param position Initial position of the cube.
         * @param rotation Initial rotation of the cube.
         * @param size Size of the cube.
         * @param mass Mass of the cube (0.0f for static cubes).
         */
        RigidCube(const math::Vec3& position,
                  const math::Quaternion& rotation,
                  const math::Vec3& size,
                  float mass = Static);

        /**
         * @brief Constructor for the RigidCube class using an AABB bounding box.
         * @param bb AABB bounding box defining the cube's dimensions.
         * @param rotation Initial rotation of the cube.
         * @param mass Mass of the cube (0.0f for static cubes).
         */
        RigidCube(const shape3D::AABB bb,
                  const math::Quaternion& rotation = { 0, 0, 0, 1 },
                  float mass = Static);

        /**
         * @brief Constructor for the RigidCube class using a cube shape.
         * @param cube Cube shape defining the cube's dimensions.
         * @param rotation Initial rotation of the cube.
         * @param mass Mass of the cube (0.0f for static cubes).
         */
        RigidCube(const shape3D::Cube& cube,
                  const math::Quaternion& rotation = { 0, 0, 0, 1 },
                  float mass = Static);
    };

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_CUBE_HPP
