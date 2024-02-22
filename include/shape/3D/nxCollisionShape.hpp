#ifndef NEXUS_SHAPE_3D_COLLISION_SHAPE_HPP
#define NEXUS_SHAPE_3D_COLLISION_SHAPE_HPP

#include "../../math/nxVec3.hpp"

// TODO: Add Cone shape

namespace nexus { namespace shape3D {

    // Shapes forward declarations

    class Ray;
    class Line;
    class Mesh;
    class AABB;
    class Cube;
    class Plane;
    class Sphere;
    class Capsule;
    class Cylinder;

    // Collisions class result definitions

    struct NEXUS_API RayCollision
    {
        math::Vec3 point;       // Point of the nearest hit
        math::Vec3 normal;      // Surface normal of hit
        float distance;         // Distance to the nearest hit
        bool hit;               // Did the ray hit something?
    };

    struct NEXUS_API CollisionShape
    {
        virtual bool CollisionPoint(const math::Vec3& point)                        const = 0;
        virtual bool CollisionAABB(const AABB& aabb)                                const = 0;
        virtual bool CollisionCube(const Cube& cube)                                const = 0;
        virtual bool CollisionPlane(const Plane& plane)                             const = 0;
        virtual bool CollisionSphere(const Sphere& sphere)                          const = 0;
        virtual bool CollisionCapsule(const Capsule& circle)                        const = 0;
        virtual bool CollisionCylinder(const Cylinder& cylinder)                    const = 0;

        virtual RayCollision CollisionRay(Ray& ray)                                 const = 0;
        virtual RayCollision CollisionLine(const Line& line)                        const = 0;

        virtual bool CollisionMesh(const Mesh& mesh, const math::Mat4& transform)   const = 0;
    };

}}

#endif //NEXUS_SHAPE_3D_COLLISION_SHAPE_HPP
