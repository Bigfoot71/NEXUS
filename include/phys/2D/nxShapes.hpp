#ifndef NEXUS_PHYSICS_2D_SHAPES_HPP
#define NEXUS_PHYSICS_2D_SHAPES_HPP

#include "./nxPhysics.hpp"
#include "../../core/nxException.hpp"

#include "../../shape/2D/nxLine.hpp"
#include "../../shape/2D/nxCircle.hpp"
#include "../../shape/2D/nxPolygon.hpp"

namespace nexus { namespace phys2D {

    NEXUS_API shape2D::Circle GetCircle(const Shape* shape);

    NEXUS_API shape2D::Polygon GetChain(const Shape* shape);

    NEXUS_API shape2D::Line GetEdge(const Shape* shape);

    NEXUS_API shape2D::Polygon GetPolygon(const Shape* shape);

}}

#endif //NEXUS_PHYSICS_2D_SHAPES_HPP
