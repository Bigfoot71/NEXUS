#ifndef NEXUS_SHAPE_2D_COLLISION_SHAPE_HPP
#define NEXUS_SHAPE_2D_COLLISION_SHAPE_HPP

#include "../../math/nxVec2.hpp"

namespace nexus { namespace shape2D {

    class AABB;
    class Line;
    class Mesh;
    class Circle;
    class Polygon;
    class Ellipse;
    class Triangle;
    class TriangleF;
    class Rectangle;
    class RectangleF;

    struct NEXUS_API CollisionShape
    {
        virtual bool CollisionAABB(const AABB& aabb)                const = 0;
        virtual bool CollisionPoint(const nexus::math::Vec2& point)    const = 0;
        virtual bool CollisionLine(const Line& line)                const = 0;
        virtual bool CollisionMesh(const Mesh& mesh)                const = 0;
        virtual bool CollisionCircle(const Circle& circle)          const = 0;
        virtual bool CollisionEllipse(const Ellipse& ellipse)       const = 0;
        virtual bool CollisionPolygon(const Polygon& poly)          const = 0;
        virtual bool CollisionTriangle(const Triangle& tri)         const = 0;
        virtual bool CollisionTriangle(const TriangleF& tri)        const = 0;
        virtual bool CollisionRectangle(const Rectangle& rect)      const = 0;
        virtual bool CollisionRectangle(const RectangleF& rect)     const = 0;
    };

}}

#endif //NEXUS_SHAPE_2D_COLLISION_SHAPE_HPP
