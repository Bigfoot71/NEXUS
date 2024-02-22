#ifndef NEXUS_SHAPE_2D_FUNCTIONS_HPP
#define NEXUS_SHAPE_2D_FUNCTIONS_HPP

#include "../../math/nxVec2.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief Get the spline point for a given t [0.0f .. 1.0f] using Linear interpolation.
     * @param startPos The start position of the spline.
     * @param endPos The end position of the spline.
     * @param t The parameter value in the range [0.0f, 1.0f].
     * @return The interpolated point on the spline.
     */
    NEXUS_API math::Vec2 GetSplinePointLinear(math::Vec2 startPos, math::Vec2 endPos, float t);

    /**
     * @brief Get the spline point for a given t [0.0f .. 1.0f] using B-Spline interpolation.
     * @param p1 The first control point of the spline.
     * @param p2 The second control point of the spline.
     * @param p3 The third control point of the spline.
     * @param p4 The fourth control point of the spline.
     * @param t The parameter value in the range [0.0f, 1.0f].
     * @return The interpolated point on the spline.
     */
    NEXUS_API math::Vec2 GetSplinePointBasis(math::Vec2 p1, math::Vec2 p2, math::Vec2 p3, math::Vec2 p4, float t);

    /**
     * @brief Get the spline point for a given t [0.0f .. 1.0f] using Catmull-Rom interpolation.
     * @param p1 The first control point of the spline.
     * @param p2 The second control point of the spline.
     * @param p3 The third control point of the spline.
     * @param p4 The fourth control point of the spline.
     * @param t The parameter value in the range [0.0f, 1.0f].
     * @return The interpolated point on the spline.
     */
    NEXUS_API math::Vec2 GetSplinePointCatmullRom(math::Vec2 p1, math::Vec2 p2, math::Vec2 p3, math::Vec2 p4, float t);

    /**
     * @brief Get the spline point for a given t [0.0f .. 1.0f] using Quadratic Bezier interpolation.
     * @param startPos The start position of the spline.
     * @param controlPos The control position of the spline.
     * @param endPos The end position of the spline.
     * @param t The parameter value in the range [0.0f, 1.0f].
     * @return The interpolated point on the spline.
     */
    NEXUS_API math::Vec2 GetSplinePointBezierQuad(math::Vec2 startPos, math::Vec2 controlPos, math::Vec2 endPos, float t);

    /**
     * @brief Get the spline point for a given t [0.0f .. 1.0f] using Cubic Bezier interpolation.
     * @param startPos The start position of the spline.
     * @param startControlPos The start control position of the spline.
     * @param endControlPos The end control position of the spline.
     * @param endPos The end position of the spline.
     * @param t The parameter value in the range [0.0f, 1.0f].
     * @return The interpolated point on the spline.
     */
    NEXUS_API math::Vec2 GetSplinePointBezierCubic(math::Vec2 startPos, math::Vec2 startControlPos, math::Vec2 endControlPos, math::Vec2 endPos, float t);

}}

#endif //NEXUS_SHAPE_2D_FUNCTIONS_HPP
