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
