#ifndef NEXUS_GAPI_IMPL_PRIMITIVES_3D_HPP
#define NEXUS_GAPI_IMPL_PRIMITIVES_3D_HPP

#include "../../shape/3D/nxCylinder.hpp"
#include "../../shape/3D/nxCapsule.hpp"
#include "../../shape/3D/nxSphere.hpp"
#include "../../shape/3D/nxPlane.hpp"
#include "../../shape/3D/nxCube.hpp"
#include "../../shape/3D/nxLine.hpp"
#include "../../shape/3D/nxAABB.hpp"
#include "../../shape/3D/nxRay.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxMath.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxContext.hpp"
#include <vector>

namespace _gapi_primitives_3d {

    constexpr float CircleSegStepLen = nexus::math::Tau / 36;

    /**
     * @brief Draws a 3D line on the screen using the provided start and end positions, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The start position of the 3D line.
     * @param endPos The end position of the 3D line.
     * @param color The color of the 3D line.
     */
    NEXUS_API void DrawLine3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D line on the screen using the provided Line object and color.
     * 
     * @param ctx The rendering context.
     * @param line The Line object representing the 3D line.
     * @param color The color of the 3D line.
     */
    NEXUS_API inline void DrawLine3D(nexus::gapi::Context& ctx, const nexus::shape3D::Line& line, const nexus::gfx::Color& color)
    {
        DrawLine3D(ctx, line.start, line.end, color);
    }

    /**
     * @brief Draws a 3D point on the screen using the provided position and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the 3D point.
     * @param color The color of the 3D point.
     */
    NEXUS_API void DrawPoint3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D circle on the screen using the provided center, radius, rotation axis, rotation angle, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center position of the 3D circle.
     * @param radius The radius of the 3D circle.
     * @param rotationAxis The axis of rotation for the 3D circle.
     * @param rotationAngle The angle of rotation for the 3D circle.
     * @param color The color of the 3D circle.
     */
    NEXUS_API void DrawCircle3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& center, float radius, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D ellipse on the screen using the provided center, radii, rotation axis, rotation angle, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center position of the 3D ellipse.
     * @param rx The radius along the x-axis of the 3D ellipse.
     * @param ry The radius along the y-axis of the 3D ellipse.
     * @param rotationAxis The axis of rotation for the 3D ellipse.
     * @param rotationAngle The angle of rotation for the 3D ellipse.
     * @param color The color of the 3D ellipse.
     */
    NEXUS_API void DrawEllipse3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& center, float rx, float ry, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D triangle on the screen using the provided vertices and color.
     * 
     * @param ctx The rendering context.
     * @param v1 The first vertex of the triangle.
     * @param v2 The second vertex of the triangle.
     * @param v3 The third vertex of the triangle.
     * @param color The color of the triangle.
     */
    NEXUS_API void DrawTriangle3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& v1, const nexus::math::Vec3& v2, const nexus::math::Vec3& v3, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D triangle strip on the screen using the provided vertices and color.
     * 
     * @param ctx The rendering context.
     * @param points The vertices of the triangle strip.
     * @param color The color of the triangle strip.
     */
    NEXUS_API void DrawTriangleStrip3D(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec3>& points, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param width The width of the cube.
     * @param height The height of the cube.
     * @param length The length of the cube.
     * @param color The color of the cube.
     */
    NEXUS_API void DrawCube(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float width, float height, float length, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param size The size of the cube (width, height, length).
     * @param color The color of the cube.
     */
    NEXUS_API inline void DrawCube(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, const nexus::math::Vec3& size, const nexus::gfx::Color& color)
    {
        DrawCube(ctx, position, size.x, size.y, size.z, color);
    }

    /**
     * @brief Draws a 3D cube on the screen using the provided Cube object and color.
     * 
     * @param ctx The rendering context.
     * @param cube The Cube object representing the cube.
     * @param color The color of the cube.
     */
    NEXUS_API inline void DrawCube(nexus::gapi::Context& ctx, const nexus::shape3D::Cube& cube, const nexus::gfx::Color& color)
    {
        DrawCube(ctx, cube.center, cube.size, color);
    }

    /**
     * @brief Draws the wireframe of a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param width The width of the cube.
     * @param height The height of the cube.
     * @param length The length of the cube.
     * @param color The color of the wireframe.
     */
    NEXUS_API void DrawCubeWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float width, float height, float length, const nexus::gfx::Color& color);

    /**
     * @brief Draws the wireframe of a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param size The size of the cube (width, height, length).
     * @param color The color of the wireframe.
     */
    NEXUS_API inline void DrawCubeWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, const nexus::math::Vec3& size, const nexus::gfx::Color& color)
    {
        DrawCubeWires(ctx, position, size.x, size.y, size.z, color);
    }

    /**
     * @brief Draws the wireframe of a 3D cube on the screen using the provided Cube object and color.
     * 
     * @param ctx The rendering context.
     * @param cube The Cube object representing the cube.
     * @param color The color of the wireframe.
     */
    NEXUS_API inline void DrawCubeWires(nexus::gapi::Context& ctx, const nexus::shape3D::Cube& cube, const nexus::gfx::Color& color)
    {
        DrawCubeWires(ctx, cube.center, cube.size, color);
    }

    /**
     * @brief Draws the wireframe of a 3D axis-aligned bounding box (AABB) on the screen using the provided AABB object and color.
     * 
     * @param ctx The rendering context.
     * @param aabb The AABB object representing the axis-aligned bounding box.
     * @param color The color of the wireframe.
     */
    NEXUS_API inline void DrawAABB(nexus::gapi::Context& ctx, const nexus::shape3D::AABB& aabb, const nexus::gfx::Color& color)
    {
        DrawCubeWires(ctx, (aabb.min + aabb.max) * 0.5f, aabb.max - aabb.min, color);
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided center position, radius, number of rings, number of slices, and color.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param rings The number of rings used for the sphere's geometry.
     * @param slices The number of slices used for the sphere's geometry.
     * @param color The color of the sphere.
     */
    NEXUS_API void DrawSphere(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, int rings, int slices, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D sphere on the screen using the provided center position, radius, and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param color The color of the sphere.
     */
    NEXUS_API inline void DrawSphere(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, const nexus::gfx::Color& color)
    {
        DrawSphere(ctx, centerPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided Sphere object and color, with specified numbers of rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param rings The number of rings used for the sphere's geometry.
     * @param slices The number of slices used for the sphere's geometry.
     * @param color The color of the sphere.
     */
    NEXUS_API inline void DrawSphere(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, int rings, int slices, const nexus::gfx::Color& color)
    {
        DrawSphere(ctx, sphere.center, sphere.radius, rings, slices, color);
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided Sphere object and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param color The color of the sphere.
     */
    NEXUS_API inline void DrawSphere(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, const nexus::gfx::Color& color)
    {
        DrawSphere(ctx, sphere.center, sphere.radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided center position, radius, number of rings, number of slices, and color.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param rings The number of rings used for the sphere's wireframe.
     * @param slices The number of slices used for the sphere's wireframe.
     * @param color The color of the sphere's wireframe.
     */
    NEXUS_API void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, int rings, int slices, const nexus::gfx::Color& color);

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided center position, radius, and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param color The color of the sphere's wireframe.
     */
    NEXUS_API inline void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, const nexus::gfx::Color& color)
    {
        DrawSphereWires(ctx, centerPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided Sphere object and color, with specified numbers of rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param rings The number of rings used for the sphere's wireframe.
     * @param slices The number of slices used for the sphere's wireframe.
     * @param color The color of the sphere's wireframe.
     */
    NEXUS_API inline void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, int rings, int slices, const nexus::gfx::Color& color)
    {
        DrawSphereWires(ctx, sphere.center, sphere.radius, rings, slices, color);
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided Sphere object and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param color The color of the sphere's wireframe.
     */
    NEXUS_API inline void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, const nexus::gfx::Color& color)
    {
        DrawSphereWires(ctx, sphere.center, sphere.radius, 16, 16, color);
    }

    /**
     * @brief Draws a 3D cylinder on the screen using the provided position, radii, height, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cylinder's base.
     * @param radiusTop The radius of the cylinder's top.
     * @param radiusBottom The radius of the cylinder's bottom.
     * @param height The height of the cylinder.
     * @param sides The number of sides used to approximate the cylinder's geometry.
     * @param color The color of the cylinder.
     */
    NEXUS_API void DrawCylinder(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float radiusTop, float radiusBottom, float height, int sides, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D cylinder on the screen using the provided start and end positions, radii, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the cylinder's start.
     * @param endPos The position of the cylinder's end.
     * @param startRadius The radius of the cylinder at the start position.
     * @param endRadius The radius of the cylinder at the end position.
     * @param sides The number of sides used to approximate the cylinder's geometry.
     * @param color The color of the cylinder.
     */
    NEXUS_API void DrawCylinder(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float startRadius, float endRadius, int sides, const nexus::gfx::Color& color);

    /**
     * @brief Draws a 3D cylinder on the screen using the provided Cylinder object, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param cylinder The Cylinder object representing the cylinder.
     * @param sides The number of sides used to approximate the cylinder's geometry.
     * @param color The color of the cylinder.
     */
    NEXUS_API inline void DrawCylinder(nexus::gapi::Context& ctx, const nexus::shape3D::Cylinder& cylinder, int sides, const nexus::gfx::Color& color)
    {
        DrawCylinder(ctx, cylinder.center, cylinder.radius, cylinder.radius, cylinder.height, sides, color);
    }

    /**
     * @brief Draws the wireframe of a 3D cylinder on the screen using the provided parameters: position, radii, height, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cylinder's base.
     * @param radiusTop The radius of the cylinder's top.
     * @param radiusBottom The radius of the cylinder's bottom.
     * @param height The height of the cylinder.
     * @param sides The number of sides used to approximate the cylinder's wireframe.
     * @param color The color of the cylinder's wireframe.
     */
    NEXUS_API void DrawCylinderWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float radiusTop, float radiusBottom, float height, int sides, const nexus::gfx::Color& color);

    /**
     * @brief Draws the wireframe of a 3D cylinder on the screen using the provided start and end positions, radii, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the cylinder's start.
     * @param endPos The position of the cylinder's end.
     * @param startRadius The radius of the cylinder at the start position.
     * @param endRadius The radius of the cylinder at the end position.
     * @param sides The number of sides used to approximate the cylinder's wireframe.
     * @param color The color of the cylinder's wireframe.
     */
    NEXUS_API void DrawCylinderWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float startRadius, float endRadius, int sides, const nexus::gfx::Color& color);

    /**
     * @brief Draws the wireframe of a 3D cylinder on the screen using the provided Cylinder object, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param cylinder The Cylinder object representing the cylinder.
     * @param sides The number of sides used to approximate the cylinder's wireframe.
     * @param color The color of the cylinder's wireframe.
     */
    NEXUS_API inline void DrawCylinderWires(nexus::gapi::Context& ctx, const nexus::shape3D::Cylinder& cylinder, int sides, const nexus::gfx::Color& color)
    {
        DrawCylinderWires(ctx, cylinder.center, cylinder.radius, cylinder.radius, cylinder.height, sides, color);
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule.
     */
    NEXUS_API void DrawCapsule(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, int slices, int rings, const nexus::gfx::Color& color);

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param color The color of the capsule.
     */
    NEXUS_API inline void DrawCapsule(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, const nexus::gfx::Color& color)
    {
        DrawCapsule(ctx, startPos, endPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule.
     */
    NEXUS_API inline void DrawCapsule(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, int slices, int rings, const nexus::gfx::Color& color)
    {
        DrawCapsule(ctx, capsule.start, capsule.end, capsule.radius, slices, rings, color);
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param color The color of the capsule.
     */
    NEXUS_API inline void DrawCapsule(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, const nexus::gfx::Color& color)
    {
        DrawCapsule(ctx, capsule.start, capsule.end, capsule.radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule's wireframe.
     */
    NEXUS_API void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, int slices, int rings, const nexus::gfx::Color& color);

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param color The color of the capsule's wireframe.
     */
    NEXUS_API inline void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, const nexus::gfx::Color& color)
    {
        DrawCapsuleWires(ctx, startPos, endPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule's wireframe.
     */
    NEXUS_API inline void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, int slices, int rings, const nexus::gfx::Color& color)
    {
        DrawCapsuleWires(ctx, capsule.start, capsule.end, capsule.radius, slices, rings, color);
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param color The color of the capsule's wireframe.
     */
    NEXUS_API inline void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, const nexus::gfx::Color& color)
    {
        DrawCapsuleWires(ctx, capsule.start, capsule.end, capsule.radius, 16, 16, color);
    }

    /**
     * @brief Draws a plane (a flat surface) on the screen using the provided center position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the plane's center.
     * @param size The size of the plane (width and height).
     * @param color The color of the plane.
     */
    NEXUS_API void DrawPlane(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, const nexus::math::Vec2& size, const nexus::gfx::Color& color);

    /**
     * @brief Draws a plane (a flat surface) on the screen using the provided Plane object and color.
     * 
     * @param ctx The rendering context.
     * @param plane The Plane object representing the plane.
     * @param color The color of the plane.
     */
    NEXUS_API inline void DrawPlane(nexus::gapi::Context& ctx, const nexus::shape3D::Plane& plane, const nexus::gfx::Color& color)
    {
        DrawPlane(ctx, plane.center, plane.size, color);
    }

    /**
     * @brief Draws a ray (a half-infinite line) on the screen using the provided Ray object and color.
     * 
     * @param ctx The rendering context.
     * @param ray The Ray object representing the ray.
     * @param color The color of the ray.
     */
    NEXUS_API void DrawRay(nexus::gapi::Context& ctx, const nexus::shape3D::Ray& ray, const nexus::gfx::Color& color);

    /**
     * @brief Draws a grid on the screen using the provided number of slices and spacing between grid lines.
     * 
     * @param ctx The rendering context.
     * @param slices The number of slices used to create the grid.
     * @param spacing The spacing between grid lines.
     */
    NEXUS_API void DrawGrid(nexus::gapi::Context& ctx, int slices, float spacing);

}

#if 0

namespace _gapi_primitives_3d {

    constexpr float CircleSegStepLen = nexus::math::Tau / 36;

    /**
     * @brief Draws a 3D line on the screen using the provided start and end positions, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The start position of the 3D line.
     * @param endPos The end position of the 3D line.
     * @param color The color of the 3D line.
     */
    template <typename T_Texture>
    void DrawLine3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, const nexus::gfx::Color& color)
    {
        ctx.Begin(nexus::gapi::DrawMode::Lines);
            ctx.Color(color);
            ctx.Vertex(startPos.x, startPos.y, startPos.z);
            ctx.Vertex(endPos.x, endPos.y, endPos.z);
        ctx.End();
    }

    /**
     * @brief Draws a 3D line on the screen using the provided Line object and color.
     * 
     * @param ctx The rendering context.
     * @param line The Line object representing the 3D line.
     * @param color The color of the 3D line.
     */
    template <typename T_Texture>
    inline void DrawLine3D(nexus::gapi::Context& ctx, const nexus::shape3D::Line& line, const nexus::gfx::Color& color)
    {
        DrawLine3D(ctx, line.start, line.end, color);
    }

    /**
     * @brief Draws a 3D point on the screen using the provided position and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the 3D point.
     * @param color The color of the 3D point.
     */
    template <typename T_Texture>
    void DrawPoint3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, const nexus::gfx::Color& color)
    {
        ctx.PushMatrix();
            ctx.Translate(position.x, position.y, position.z);
            ctx.Begin(nexus::gapi::DrawMode::Lines);
                ctx.Color(color);
                ctx.Vertex(0.0f, 0.0f, 0.0f);
                ctx.Vertex(0.0f, 0.0f, 0.1f);
            ctx.End();
        ctx.PopMatrix();
    }

    /**
     * @brief Draws a 3D circle on the screen using the provided center, radius, rotation axis, rotation angle, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center position of the 3D circle.
     * @param radius The radius of the 3D circle.
     * @param rotationAxis The axis of rotation for the 3D circle.
     * @param rotationAngle The angle of rotation for the 3D circle.
     * @param color The color of the 3D circle.
     */
    template <typename T_Texture>
    void DrawCircle3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& center, float radius, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::gfx::Color& color)
    {
        ctx.PushMatrix();

            ctx.Translate(center.x, center.y, center.z);
            ctx.Rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

            ctx.Begin(nexus::gapi::DrawMode::Lines);
                for (float i = 0; i < nexus::math::Tau; i += CircleSegStepLen)
                {
                    ctx.Color(color);

                    ctx.Vertex(std::sin(i) * radius, std::cos(i) * radius, 0.0f);
                    ctx.Vertex(std::sin(i + CircleSegStepLen) * radius, std::cos(i + CircleSegStepLen) * radius, 0.0f);
                }
            ctx.End();

        ctx.PopMatrix();
    }

    /**
     * @brief Draws a 3D ellipse on the screen using the provided center, radii, rotation axis, rotation angle, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center position of the 3D ellipse.
     * @param rx The radius along the x-axis of the 3D ellipse.
     * @param ry The radius along the y-axis of the 3D ellipse.
     * @param rotationAxis The axis of rotation for the 3D ellipse.
     * @param rotationAngle The angle of rotation for the 3D ellipse.
     * @param color The color of the 3D ellipse.
     */
    template <typename T_Texture>
    void DrawEllipse3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& center, float rx, float ry, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::gfx::Color& color)
    {
        ctx.PushMatrix();

            ctx.Translate(center.x, center.y, center.z);
            ctx.Rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

            ctx.Begin(nexus::gapi::DrawMode::Lines);
                for (float i = 0; i < nexus::math::Tau; i += CircleSegStepLen)
                {
                    ctx.Color(color);

                    ctx.Vertex(std::sin(i) * rx, std::cos(i) * ry, 0.0f);
                    ctx.Vertex(std::sin(i + CircleSegStepLen) * rx, std::cos(i + CircleSegStepLen) * ry, 0.0f);
                }
            ctx.End();

        ctx.PopMatrix();
    }

    /**
     * @brief Draws a 3D triangle on the screen using the provided vertices and color.
     * 
     * @param ctx The rendering context.
     * @param v1 The first vertex of the triangle.
     * @param v2 The second vertex of the triangle.
     * @param v3 The third vertex of the triangle.
     * @param color The color of the triangle.
     */
    template <typename T_Texture>
    void DrawTriangle3D(nexus::gapi::Context& ctx, const nexus::math::Vec3& v1, const nexus::math::Vec3& v2, const nexus::math::Vec3& v3, const nexus::gfx::Color& color)
    {
        ctx.Begin(nexus::gapi::DrawMode::Triangles);
            ctx.Color(color);
            ctx.Vertex(v1.x, v1.y, v1.z);
            ctx.Vertex(v2.x, v2.y, v2.z);
            ctx.Vertex(v3.x, v3.y, v3.z);
        ctx.End();
    }

    /**
     * @brief Draws a 3D triangle strip on the screen using the provided vertices and color.
     * 
     * @param ctx The rendering context.
     * @param points The vertices of the triangle strip.
     * @param color The color of the triangle strip.
     */
    template <typename T_Texture>
    void DrawTriangleStrip3D(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec3>& points, const nexus::gfx::Color& color)
    {
        if (points.size() < 3) return;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);
            ctx.Color(color);

            for (int i = 2; i < points.size(); i++)
            {
                if (i % 2 == 0)
                {
                    ctx.Vertex(points[i].x, points[i].y, points[i].z);
                    ctx.Vertex(points[i - 2].x, points[i - 2].y, points[i - 2].z);
                    ctx.Vertex(points[i - 1].x, points[i - 1].y, points[i - 1].z);
                }
                else
                {
                    ctx.Vertex(points[i].x, points[i].y, points[i].z);
                    ctx.Vertex(points[i - 1].x, points[i - 1].y, points[i - 1].z);
                    ctx.Vertex(points[i - 2].x, points[i - 2].y, points[i - 2].z);
                }
            }
        ctx.End();
    }

    /**
     * @brief Draws a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param width The width of the cube.
     * @param height The height of the cube.
     * @param length The length of the cube.
     * @param color The color of the cube.
     */
    template <typename T_Texture>
    void DrawCube(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float width, float height, float length, const nexus::gfx::Color& color)
    {
        const float hw = width * 0.5f, hh = height * 0.5f, hl = length * 0.5f;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);

            ctx.Color(color);

            /* --- Front face --- */

            ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom Left
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left

            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top Right
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right

            /* --- Back face --- */

            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom Left
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left
            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right

            ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top Right
            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left

            /* --- Top face --- */

            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Bottom Left
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Bottom Right

            ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top Right
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Left
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Bottom Right

            /* --- Bottom face --- */

            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Top Left
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right
            ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom Left

            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Top Right
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Right
            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Top Left

            /* --- Right face --- */

            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right
            ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top Right
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top Left

            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom Left
            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom Right
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top Left

            /* --- Left face --- */

            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom Right
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top Right

            ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom Left
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top Left
            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom Right

        ctx.End();
    }

    /**
     * @brief Draws a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param size The size of the cube (width, height, length).
     * @param color The color of the cube.
     */
    template <typename T_Texture>
    inline void DrawCube(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, const nexus::math::Vec3& size, const nexus::gfx::Color& color)
    {
        DrawCube(ctx, position, size.x, size.y, size.z, color);
    }

    /**
     * @brief Draws a 3D cube on the screen using the provided Cube object and color.
     * 
     * @param ctx The rendering context.
     * @param cube The Cube object representing the cube.
     * @param color The color of the cube.
     */
    template <typename T_Texture>
    inline void DrawCube(nexus::gapi::Context& ctx, const nexus::shape3D::Cube& cube, const nexus::gfx::Color& color)
    {
        DrawCube(ctx, cube.center, cube.size, color);
    }

    /**
     * @brief Draws the wireframe of a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param width The width of the cube.
     * @param height The height of the cube.
     * @param length The length of the cube.
     * @param color The color of the wireframe.
     */
    template <typename T_Texture>
    void DrawCubeWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float width, float height, float length, const nexus::gfx::Color& color)
    {
        const float hw = width * 0.5f, hh = height * 0.5f, hl = length * 0.5f;

        ctx.Begin(nexus::gapi::DrawMode::Lines);

            ctx.Color(color);

            /* --- Front face --- */

            // Bottom line
            ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom left
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom right

            // Left line
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Bottom right
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top right

            // Top line
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top right
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top left

            // Right line
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top left
            ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Bottom left

            /* --- Back face --- */

            // Bottom line
            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom left
            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom right

            // Left line
            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Bottom right
            ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top right

            // Top line
            ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top right
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top left

            // Right line
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top left
            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Bottom left

            /* --- Top face --- */

            // Left line
            ctx.Vertex(position.x - hw, position.y + hh, position.z + hl);  // Top left front
            ctx.Vertex(position.x - hw, position.y + hh, position.z - hl);  // Top left back

            // Right line
            ctx.Vertex(position.x + hw, position.y + hh, position.z + hl);  // Top right front
            ctx.Vertex(position.x + hw, position.y + hh, position.z - hl);  // Top right back

            /* --- Bottom face --- */

            // Left line
            ctx.Vertex(position.x - hw, position.y - hh, position.z + hl);  // Top left front
            ctx.Vertex(position.x - hw, position.y - hh, position.z - hl);  // Top left back

            // Right line
            ctx.Vertex(position.x + hw, position.y - hh, position.z + hl);  // Top right front
            ctx.Vertex(position.x + hw, position.y - hh, position.z - hl);  // Top right back

        ctx.End();
    }

    /**
     * @brief Draws the wireframe of a 3D cube on the screen using the provided position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cube's center.
     * @param size The size of the cube (width, height, length).
     * @param color The color of the wireframe.
     */
    template <typename T_Texture>
    inline void DrawCubeWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, const nexus::math::Vec3& size, const nexus::gfx::Color& color)
    {
        DrawCubeWires(ctx, position, size.x, size.y, size.z, color);
    }

    /**
     * @brief Draws the wireframe of a 3D cube on the screen using the provided Cube object and color.
     * 
     * @param ctx The rendering context.
     * @param cube The Cube object representing the cube.
     * @param color The color of the wireframe.
     */
    template <typename T_Texture>
    inline void DrawCubeWires(nexus::gapi::Context& ctx, const nexus::shape3D::Cube& cube, const nexus::gfx::Color& color)
    {
        DrawCubeWires(ctx, cube.center, cube.size, color);
    }

    /**
     * @brief Draws the wireframe of a 3D axis-aligned bounding box (AABB) on the screen using the provided AABB object and color.
     * 
     * @param ctx The rendering context.
     * @param aabb The AABB object representing the axis-aligned bounding box.
     * @param color The color of the wireframe.
     */
    template <typename T_Texture>
    inline void DrawAABB(nexus::gapi::Context& ctx, const nexus::shape3D::AABB& aabb, const nexus::gfx::Color& color)
    {
        DrawCubeWires(ctx, (aabb.min + aabb.max) * 0.5f, aabb.max - aabb.min, color);
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided center position, radius, number of rings, number of slices, and color.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param rings The number of rings used for the sphere's geometry.
     * @param slices The number of slices used for the sphere's geometry.
     * @param color The color of the sphere.
     */
    void DrawSphere(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, int rings, int slices, const nexus::gfx::Color& color)
    {
        ctx.PushMatrix();
            // NOTE: Transformation is applied in inverse order (scale -> translate)
            ctx.Translate(centerPos.x, centerPos.y, centerPos.z);
            ctx.Scale(radius, radius, radius);

            ctx.Begin(nexus::gapi::DrawMode::Triangles);
                ctx.Color(color);

                for (int i = 0; i < (rings + 2); i++)
                {
                    for (int j = 0; j < slices; j++)
                    {
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::cos(nexus::math::Tau * j / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * (j + 1) / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * (j + 1) / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * j / slices));

                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::cos(nexus::math::Tau * j / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i)) * std::sin(nexus::math::Tau * (j + 1) / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i)) * std::cos(nexus::math::Tau * (j + 1) / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * (j + 1) / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * (j + 1) / slices));
                    }
                }
            ctx.End();
        ctx.PopMatrix();
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided center position, radius, and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param color The color of the sphere.
     */
    template <typename T_Texture>
    inline void DrawSphere(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, const nexus::gfx::Color& color)
    {
        DrawSphere(ctx, centerPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided Sphere object and color, with specified numbers of rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param rings The number of rings used for the sphere's geometry.
     * @param slices The number of slices used for the sphere's geometry.
     * @param color The color of the sphere.
     */
    template <typename T_Texture>
    inline void DrawSphere(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, int rings, int slices, const nexus::gfx::Color& color)
    {
        DrawSphere(ctx, sphere.center, sphere.radius, rings, slices, color);
    }

    /**
     * @brief Draws a 3D sphere on the screen using the provided Sphere object and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param color The color of the sphere.
     */
    template <typename T_Texture>
    inline void DrawSphere(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, const nexus::gfx::Color& color)
    {
        DrawSphere(ctx, sphere.center, sphere.radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided center position, radius, number of rings, number of slices, and color.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param rings The number of rings used for the sphere's wireframe.
     * @param slices The number of slices used for the sphere's wireframe.
     * @param color The color of the sphere's wireframe.
     */
    template <typename T_Texture>
    void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, int rings, int slices, const nexus::gfx::Color& color)
    {
        ctx.PushMatrix();
            // NOTE: Transformation is applied in inverse order (scale -> translate)
            ctx.Translate(centerPos.x, centerPos.y, centerPos.z);
            ctx.Scale(radius, radius, radius);

            ctx.Begin(nexus::gapi::DrawMode::Lines);
                ctx.Color(color);

                for (int i = 0; i < (rings + 2); i++)
                {
                    for (int j = 0; j < slices; j++)
                    {
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::cos(nexus::math::Tau * j / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * (j + 1) / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * (j + 1) / slices));

                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * (j + 1) / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * (j + 1) / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * j / slices));

                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * (i + 1)) * std::cos(nexus::math::Tau * j / slices));
                        ctx.Vertex(std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::sin(nexus::math::Tau * j / slices),
                                    std::sin((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i),
                                    std::cos((nexus::math::Pi + nexus::math::Pi / 2) + (nexus::math::Pi / (rings + 1)) * i) * std::cos(nexus::math::Tau * j / slices));
                    }
                }
            ctx.End();
        ctx.PopMatrix();
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided center position, radius, and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the sphere's center.
     * @param radius The radius of the sphere.
     * @param color The color of the sphere's wireframe.
     */
    template <typename T_Texture>
    inline void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, float radius, const nexus::gfx::Color& color)
    {
        DrawSphereWires(ctx, centerPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided Sphere object and color, with specified numbers of rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param rings The number of rings used for the sphere's wireframe.
     * @param slices The number of slices used for the sphere's wireframe.
     * @param color The color of the sphere's wireframe.
     */
    template <typename T_Texture>
    inline void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, int rings, int slices, const nexus::gfx::Color& color)
    {
        DrawSphereWires(ctx, sphere.center, sphere.radius, rings, slices, color);
    }

    /**
     * @brief Draws the wireframe of a 3D sphere on the screen using the provided Sphere object and color, with default values for rings and slices.
     * 
     * @param ctx The rendering context.
     * @param sphere The Sphere object representing the sphere.
     * @param color The color of the sphere's wireframe.
     */
    template <typename T_Texture>
    inline void DrawSphereWires(nexus::gapi::Context& ctx, const nexus::shape3D::Sphere& sphere, const nexus::gfx::Color& color)
    {
        DrawSphereWires(ctx, sphere.center, sphere.radius, 16, 16, color);
    }

    /**
     * @brief Draws a 3D cylinder on the screen using the provided position, radii, height, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cylinder's base.
     * @param radiusTop The radius of the cylinder's top.
     * @param radiusBottom The radius of the cylinder's bottom.
     * @param height The height of the cylinder.
     * @param sides The number of sides used to approximate the cylinder's geometry.
     * @param color The color of the cylinder.
     */
    template <typename T_Texture>
    void DrawCylinder(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float radiusTop, float radiusBottom, float height, int sides, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        ctx.PushMatrix();
            ctx.Translate(position.x, position.y, position.z);

            ctx.Begin(nexus::gapi::DrawMode::Triangles);
                ctx.Color(color);

                if (radiusTop > 0)
                {
                    // Draw Body
                    for (float i = 0; i < nexus::math::Tau; i += nexus::math::Tau / sides)
                    {
                        ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom); //Bottom Left
                        ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusBottom, 0, std::cos(i + nexus::math::Tau / sides) * radiusBottom); //Bottom Right
                        ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusTop, height, std::cos(i + nexus::math::Tau / sides) * radiusTop); //Top Right

                        ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop); //Top Left
                        ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom); //Bottom Left
                        ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusTop, height, std::cos(i + nexus::math::Tau / sides) * radiusTop); //Top Right
                    }

                    // Draw Cap
                    for (float i = 0; i < nexus::math::Tau; i += nexus::math::Tau / sides)
                    {
                        ctx.Vertex(0, height, 0);
                        ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop);
                        ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusTop, height, std::cos(i + nexus::math::Tau / sides) * radiusTop);
                    }
                }
                else
                {
                    // Draw Cone
                    for (float i = 0; i < nexus::math::Tau; i += nexus::math::Tau / sides)
                    {
                        ctx.Vertex(0, height, 0);
                        ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
                        ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusBottom, 0, std::cos(i + nexus::math::Tau / sides) * radiusBottom);
                    }
                }

                // Draw Base
                for (float i = 0; i < nexus::math::Tau; i += nexus::math::Tau / sides)
                {
                    ctx.Vertex(0, 0, 0);
                    ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusBottom, 0, std::cos(i + nexus::math::Tau / sides) * radiusBottom);
                    ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
                }
            ctx.End();
        ctx.PopMatrix();
    }

    /**
     * @brief Draws a 3D cylinder on the screen using the provided start and end positions, radii, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the cylinder's start.
     * @param endPos The position of the cylinder's end.
     * @param startRadius The radius of the cylinder at the start position.
     * @param endRadius The radius of the cylinder at the end position.
     * @param sides The number of sides used to approximate the cylinder's geometry.
     * @param color The color of the cylinder.
     */
    template <typename T_Texture>
    void DrawCylinder(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float startRadius, float endRadius, int sides, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        nexus::math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };
        if ((direction.x == 0) && (direction.y == 0) && (direction.z == 0)) return;

        // Construct a basis of the base and the top face:
        nexus::math::Vec3 b1 = nexus::math::Vec3::Perpendicular(direction).Normalized();
        nexus::math::Vec3 b2 = b1.Cross(direction).Normalized();

        const float baseAngle = nexus::math::Tau / sides;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);
            ctx.Color(color);

            for (int i = 0; i < sides; i++)
            {
                // Compute the four vertices
                float s1 = std::sin(baseAngle * i) * startRadius;
                float c1 = std::cos(baseAngle * i) * startRadius;
                nexus::math::Vec3 w1 = { startPos.x + s1 * b1.x + c1 * b2.x, startPos.y + s1 * b1.y + c1 * b2.y, startPos.z + s1 * b1.z + c1 * b2.z };

                float s2 = std::sin(baseAngle * (i + 1)) * startRadius;
                float c2 = std::cos(baseAngle * (i + 1)) * startRadius;
                nexus::math::Vec3 w2 = { startPos.x + s2 * b1.x + c2 * b2.x, startPos.y + s2 * b1.y + c2 * b2.y, startPos.z + s2 * b1.z + c2 * b2.z };

                float s3 = std::sin(baseAngle * i) * endRadius;
                float c3 = std::cos(baseAngle * i) * endRadius;
                nexus::math::Vec3 w3 = { endPos.x + s3 * b1.x + c3 * b2.x, endPos.y + s3 * b1.y + c3 * b2.y, endPos.z + s3 * b1.z + c3 * b2.z };

                float s4 = std::sin(baseAngle * (i + 1)) * endRadius;
                float c4 = std::cos(baseAngle * (i + 1)) * endRadius;
                nexus::math::Vec3 w4 = { endPos.x + s4 * b1.x + c4 * b2.x, endPos.y + s4 * b1.y + c4 * b2.y, endPos.z + s4 * b1.z + c4 * b2.z };

                if (startRadius > 0)
                {
                    ctx.Vertex(startPos.x, startPos.y, startPos.z);
                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w1.x, w1.y, w1.z);
                }

                ctx.Vertex(w1.x, w1.y, w1.z);
                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w3.x, w3.y, w3.z);

                ctx.Vertex(w2.x, w2.y, w2.z);
                ctx.Vertex(w4.x, w4.y, w4.z);
                ctx.Vertex(w3.x, w3.y, w3.z);

                if (endRadius > 0)
                {
                    ctx.Vertex(endPos.x, endPos.y, endPos.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);
                    ctx.Vertex(w4.x, w4.y, w4.z);
                }
            }
        ctx.End();
    }

    /**
     * @brief Draws a 3D cylinder on the screen using the provided Cylinder object, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param cylinder The Cylinder object representing the cylinder.
     * @param sides The number of sides used to approximate the cylinder's geometry.
     * @param color The color of the cylinder.
     */
    template <typename T_Texture>
    inline void DrawCylinder(nexus::gapi::Context& ctx, const nexus::shape3D::Cylinder& cylinder, int sides, const nexus::gfx::Color& color)
    {
        DrawCylinder(ctx, cylinder.center, cylinder.radius, cylinder.radius, cylinder.height, sides, color);
    }

    /**
     * @brief Draws the wireframe of a 3D cylinder on the screen using the provided parameters: position, radii, height, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the cylinder's base.
     * @param radiusTop The radius of the cylinder's top.
     * @param radiusBottom The radius of the cylinder's bottom.
     * @param height The height of the cylinder.
     * @param sides The number of sides used to approximate the cylinder's wireframe.
     * @param color The color of the cylinder's wireframe.
     */
    template <typename T_Texture>
    void DrawCylinderWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& position, float radiusTop, float radiusBottom, float height, int sides, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        ctx.PushMatrix();
            ctx.Translate(position.x, position.y, position.z);

            ctx.Begin(nexus::gapi::DrawMode::Lines);
                ctx.Color(color);

                for (float i = 0; i < nexus::math::Tau; i += nexus::math::Tau / sides)
                {
                    ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
                    ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusBottom, 0, std::cos(i + nexus::math::Tau / sides) * radiusBottom);

                    ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusBottom, 0, std::cos(i + nexus::math::Tau / sides) * radiusBottom);
                    ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusTop, height, std::cos(i + nexus::math::Tau / sides) * radiusTop);

                    ctx.Vertex(std::sin(i + nexus::math::Tau / sides) * radiusTop, height, std::cos(i + nexus::math::Tau / sides) * radiusTop);
                    ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop);

                    ctx.Vertex(std::sin(i) * radiusTop, height, std::cos(i) * radiusTop);
                    ctx.Vertex(std::sin(i) * radiusBottom, 0, std::cos(i) * radiusBottom);
                }
            ctx.End();
        ctx.PopMatrix();
    }

    /**
     * @brief Draws the wireframe of a 3D cylinder on the screen using the provided start and end positions, radii, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the cylinder's start.
     * @param endPos The position of the cylinder's end.
     * @param startRadius The radius of the cylinder at the start position.
     * @param endRadius The radius of the cylinder at the end position.
     * @param sides The number of sides used to approximate the cylinder's wireframe.
     * @param color The color of the cylinder's wireframe.
     */
    template <typename T_Texture>
    void DrawCylinderWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float startRadius, float endRadius, int sides, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        nexus::math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };
        if ((direction.x == 0) && (direction.y == 0) && (direction.z == 0))return;

        // Construct a basis of the base and the top face:
        nexus::math::Vec3 b1 = nexus::math::Vec3::Perpendicular(direction).Normalized();
        nexus::math::Vec3 b2 = b1.Cross(direction).Normalized();

        const float baseAngle = nexus::math::Tau / sides;

        ctx.Begin(nexus::gapi::DrawMode::Lines);
            ctx.Color(color);

            for (int i = 0; i < sides; i++)
            {
                // compute the four vertices
                float s1 = std::sin(baseAngle * i) * startRadius;
                float c1 = std::cos(baseAngle * i) * startRadius;
                nexus::math::Vec3 w1 = { startPos.x + s1 * b1.x + c1 * b2.x, startPos.y + s1 * b1.y + c1 * b2.y, startPos.z + s1 * b1.z + c1 * b2.z };

                float s2 = std::sin(baseAngle * (i + 1)) * startRadius;
                float c2 = std::cos(baseAngle * (i + 1)) * startRadius;
                nexus::math::Vec3 w2 = { startPos.x + s2 * b1.x + c2 * b2.x, startPos.y + s2 * b1.y + c2 * b2.y, startPos.z + s2 * b1.z + c2 * b2.z };

                float s3 = std::sin(baseAngle * i) * endRadius;
                float c3 = std::cos(baseAngle * i) * endRadius;
                nexus::math::Vec3 w3 = { endPos.x + s3 * b1.x + c3 * b2.x, endPos.y + s3 * b1.y + c3 * b2.y, endPos.z + s3 * b1.z + c3 * b2.z };

                float s4 = std::sin(baseAngle * (i + 1)) * endRadius;
                float c4 = std::cos(baseAngle * (i + 1)) * endRadius;
                nexus::math::Vec3 w4 = { endPos.x + s4 * b1.x + c4 * b2.x, endPos.y + s4 * b1.y + c4 * b2.y, endPos.z + s4 * b1.z + c4 * b2.z };

                ctx.Vertex(w1.x, w1.y, w1.z);
                ctx.Vertex(w2.x, w2.y, w2.z);

                ctx.Vertex(w1.x, w1.y, w1.z);
                ctx.Vertex(w3.x, w3.y, w3.z);

                ctx.Vertex(w3.x, w3.y, w3.z);
                ctx.Vertex(w4.x, w4.y, w4.z);
            }
        ctx.End();
    }

    /**
     * @brief Draws the wireframe of a 3D cylinder on the screen using the provided Cylinder object, number of sides, and color.
     * 
     * @param ctx The rendering context.
     * @param cylinder The Cylinder object representing the cylinder.
     * @param sides The number of sides used to approximate the cylinder's wireframe.
     * @param color The color of the cylinder's wireframe.
     */
    template <typename T_Texture>
    inline void DrawCylinderWires(nexus::gapi::Context& ctx, const nexus::shape3D::Cylinder& cylinder, int sides, const nexus::gfx::Color& color)
    {
        DrawCylinderWires(ctx, cylinder.center, cylinder.radius, cylinder.radius, cylinder.height, sides, color);
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule.
     */
    template <typename T_Texture>
    void DrawCapsule(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, int slices, int rings, const nexus::gfx::Color& color)
    {
        if (slices < 3) slices = 3;

        nexus::math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };

        // Draw a sphere if start and end points are the same
        bool sphereCase = (direction.x == 0) && (direction.y == 0) && (direction.z == 0);
        if (sphereCase) direction = (nexus::math::Vec3){0.0f, 1.0f, 0.0f};

        // Construct a basis of the base and the caps:
        nexus::math::Vec3 b0 = direction.Normalized();
        nexus::math::Vec3 b1 = nexus::math::Vec3::Perpendicular(direction).Normalized();
        nexus::math::Vec3 b2 = b1.Cross(direction).Normalized();
        nexus::math::Vec3 capCenter = endPos;

        float baseSliceAngle = nexus::math::Tau / slices;
        float baseRingAngle  = nexus::math::DegToRad(90.0f) / rings;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);

            ctx.Color(color);

            // Render both caps
            for (int c = 0; c < 2; c++)
            {
                for (int i = 0; i < rings; i++)
                {
                    for (int j = 0; j < slices; j++)
                    {

                        // We build up the rings from capCenter in the direction of the 'direction' vector we computed earlier

                        // As we iterate through the rings they must be placed higher above the center, the height we need is sin(angle(i))
                        // As we iterate through the rings they must get smaller by the cos(angle(i))

                        // Compute the four vertices
                        float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                        float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                        nexus::math::Vec3 w1 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin1 * b1.x + ringCos1 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin1 * b1.y + ringCos1 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin1 * b1.z + ringCos1 * b2.z) * radius
                        };
                        float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                        float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                        nexus::math::Vec3 w2 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin2 * b1.x + ringCos2 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin2 * b1.y + ringCos2 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin2 * b1.z + ringCos2 * b2.z) * radius
                        };

                        float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                        float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                        nexus::math::Vec3 w3 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin3 * b1.x + ringCos3 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin3 * b1.y + ringCos3 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin3 * b1.z + ringCos3 * b2.z) * radius
                        };
                        float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                        float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                        nexus::math::Vec3 w4 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin4 * b1.x + ringCos4 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin4 * b1.y + ringCos4 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin4 * b1.z + ringCos4 * b2.z) * radius
                        };

                        // Make sure cap triangle normals are facing outwards
                        if(c == 0)
                        {
                            ctx.Vertex(w1.x, w1.y, w1.z);
                            ctx.Vertex(w2.x, w2.y, w2.z);
                            ctx.Vertex(w3.x, w3.y, w3.z);

                            ctx.Vertex(w2.x, w2.y, w2.z);
                            ctx.Vertex(w4.x, w4.y, w4.z);
                            ctx.Vertex(w3.x, w3.y, w3.z);
                        }
                        else
                        {
                            ctx.Vertex(w1.x, w1.y, w1.z);
                            ctx.Vertex(w3.x, w3.y, w3.z);
                            ctx.Vertex(w2.x, w2.y, w2.z);

                            ctx.Vertex(w2.x, w2.y, w2.z);
                            ctx.Vertex(w3.x, w3.y, w3.z);
                            ctx.Vertex(w4.x, w4.y, w4.z);
                        }
                    }
                }
                capCenter = startPos;
                b0 *= -1.0f;
            }

            // Cender middle
            if (!sphereCase)
            {
                for (int j = 0; j < slices; j++)
                {
                    // Compute the four vertices
                    float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * radius;
                    float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * radius;
                    nexus::math::Vec3 w1 = {
                        startPos.x + ringSin1 * b1.x + ringCos1 * b2.x,
                        startPos.y + ringSin1 * b1.y + ringCos1 * b2.y,
                        startPos.z + ringSin1 * b1.z + ringCos1 * b2.z
                    };
                    float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * radius;
                    float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * radius;
                    nexus::math::Vec3 w2 = {
                        startPos.x + ringSin2 * b1.x + ringCos2 * b2.x,
                        startPos.y + ringSin2 * b1.y + ringCos2 * b2.y,
                        startPos.z + ringSin2 * b1.z + ringCos2 * b2.z
                    };

                    float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * radius;
                    float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * radius;
                    nexus::math::Vec3 w3 = {
                        endPos.x + ringSin3 * b1.x + ringCos3 * b2.x,
                        endPos.y + ringSin3 * b1.y + ringCos3 * b2.y,
                        endPos.z + ringSin3 * b1.z + ringCos3 * b2.z
                    };
                    float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * radius;
                    float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * radius;
                    nexus::math::Vec3 w4 = {
                        endPos.x + ringSin4 * b1.x + ringCos4 * b2.x,
                        endPos.y + ringSin4 * b1.y + ringCos4 * b2.y,
                        endPos.z + ringSin4 * b1.z + ringCos4 * b2.z
                    };

                    ctx.Vertex(w1.x, w1.y, w1.z);
                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);

                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w4.x, w4.y, w4.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);
                }
            }

        ctx.End();
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param color The color of the capsule.
     */
    template <typename T_Texture>
    inline void DrawCapsule(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, const nexus::gfx::Color& color)
    {
        DrawCapsule(ctx, startPos, endPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule.
     */
    template <typename T_Texture>
    inline void DrawCapsule(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, int slices, int rings, const nexus::gfx::Color& color)
    {
        DrawCapsule(ctx, capsule.start, capsule.end, capsule.radius, slices, rings, color);
    }

    /**
     * @brief Draws a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param color The color of the capsule.
     */
    template <typename T_Texture>
    inline void DrawCapsule(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, const nexus::gfx::Color& color)
    {
        DrawCapsule(ctx, capsule.start, capsule.end, capsule.radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule's wireframe.
     */
    template <typename T_Texture>
    void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, int slices, int rings, const nexus::gfx::Color& color)
    {
        if (slices < 3) slices = 3;

        nexus::math::Vec3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };

        // draw a sphere if start and end points are the same
        bool sphereCase = (direction.x == 0) && (direction.y == 0) && (direction.z == 0);
        if (sphereCase) direction = (nexus::math::Vec3){0.0f, 1.0f, 0.0f};

        // Construct a basis of the base and the caps:
        nexus::math::Vec3 b0 = direction.Normalized();
        nexus::math::Vec3 b1 = nexus::math::Vec3::Perpendicular(direction).Normalized();
        nexus::math::Vec3 b2 = b1.Cross(direction).Normalized();
        nexus::math::Vec3 capCenter = endPos;

        float baseSliceAngle = nexus::math::Tau / slices;
        float baseRingAngle  = nexus::math::DegToRad(90.0f) / rings;

        ctx.Begin(nexus::gapi::DrawMode::Lines);
            ctx.Color(color);

            // render both caps
            for (int c = 0; c < 2; c++)
            {
                for (int i = 0; i < rings; i++)
                {
                    for (int j = 0; j < slices; j++)
                    {

                        // we build up the rings from capCenter in the direction of the 'direction' vector we computed earlier

                        // as we iterate through the rings they must be placed higher above the center, the height we need is sin(angle(i))
                        // as we iterate through the rings they must get smaller by the cos(angle(i))

                        // compute the four vertices
                        float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                        float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 0 ));
                        nexus::math::Vec3 w1 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin1 * b1.x + ringCos1 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin1 * b1.y + ringCos1 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin1 * b1.z + ringCos1 * b2.z) * radius
                        };
                        float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                        float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 0 ));
                        nexus::math::Vec3 w2 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 0 )) * b0.x + ringSin2 * b1.x + ringCos2 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 0 )) * b0.y + ringSin2 * b1.y + ringCos2 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 0 )) * b0.z + ringSin2 * b1.z + ringCos2 * b2.z) * radius
                        };

                        float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                        float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * std::cos(baseRingAngle * ( i + 1 ));
                        nexus::math::Vec3 w3 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin3 * b1.x + ringCos3 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin3 * b1.y + ringCos3 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin3 * b1.z + ringCos3 * b2.z) * radius
                        };
                        float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                        float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * std::cos(baseRingAngle * ( i + 1 ));
                        nexus::math::Vec3 w4 = (nexus::math::Vec3){
                            capCenter.x + (std::sin(baseRingAngle * ( i + 1 )) * b0.x + ringSin4 * b1.x + ringCos4 * b2.x) * radius,
                            capCenter.y + (std::sin(baseRingAngle * ( i + 1 )) * b0.y + ringSin4 * b1.y + ringCos4 * b2.y) * radius,
                            capCenter.z + (std::sin(baseRingAngle * ( i + 1 )) * b0.z + ringSin4 * b1.z + ringCos4 * b2.z) * radius
                        };

                        ctx.Vertex(w1.x, w1.y, w1.z);
                        ctx.Vertex(w2.x, w2.y, w2.z);

                        ctx.Vertex(w2.x, w2.y, w2.z);
                        ctx.Vertex(w3.x, w3.y, w3.z);

                        ctx.Vertex(w1.x, w1.y, w1.z);
                        ctx.Vertex(w3.x, w3.y, w3.z);

                        ctx.Vertex(w2.x, w2.y, w2.z);
                        ctx.Vertex(w4.x, w4.y, w4.z);

                        ctx.Vertex(w3.x, w3.y, w3.z);
                        ctx.Vertex(w4.x, w4.y, w4.z);
                    }
                }
                capCenter = startPos;
                b0 *= -1.0f;
            }
            // render middle
            if (!sphereCase)
            {
                for (int j = 0; j < slices; j++)
                {
                    // compute the four vertices
                    float ringSin1 = std::sin(baseSliceAngle * (j + 0)) * radius;
                    float ringCos1 = std::cos(baseSliceAngle * (j + 0)) * radius;
                    nexus::math::Vec3 w1 = {
                        startPos.x + ringSin1 * b1.x + ringCos1 * b2.x,
                        startPos.y + ringSin1 * b1.y + ringCos1 * b2.y,
                        startPos.z + ringSin1 * b1.z + ringCos1 * b2.z
                    };
                    float ringSin2 = std::sin(baseSliceAngle * (j + 1)) * radius;
                    float ringCos2 = std::cos(baseSliceAngle * (j + 1)) * radius;
                    nexus::math::Vec3 w2 = {
                        startPos.x + ringSin2 * b1.x + ringCos2 * b2.x,
                        startPos.y + ringSin2 * b1.y + ringCos2 * b2.y,
                        startPos.z + ringSin2 * b1.z + ringCos2 * b2.z
                    };

                    float ringSin3 = std::sin(baseSliceAngle * (j + 0)) * radius;
                    float ringCos3 = std::cos(baseSliceAngle * (j + 0)) * radius;
                    nexus::math::Vec3 w3 = {
                        endPos.x + ringSin3 * b1.x + ringCos3 * b2.x,
                        endPos.y + ringSin3 * b1.y + ringCos3 * b2.y,
                        endPos.z + ringSin3 * b1.z + ringCos3 * b2.z
                    };
                    float ringSin4 = std::sin(baseSliceAngle * (j + 1)) * radius;
                    float ringCos4 = std::cos(baseSliceAngle * (j + 1)) * radius;
                    nexus::math::Vec3 w4 = {
                        endPos.x + ringSin4 * b1.x + ringCos4 * b2.x,
                        endPos.y + ringSin4 * b1.y + ringCos4 * b2.y,
                        endPos.z + ringSin4 * b1.z + ringCos4 * b2.z
                    };

                    ctx.Vertex(w1.x, w1.y, w1.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);

                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w4.x, w4.y, w4.z);

                    ctx.Vertex(w2.x, w2.y, w2.z);
                    ctx.Vertex(w3.x, w3.y, w3.z);
                }
            }
        ctx.End();
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided start and end positions, radius, and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param startPos The position of the capsule's start.
     * @param endPos The position of the capsule's end.
     * @param radius The radius of the capsule's cylindrical body and spherical ends.
     * @param color The color of the capsule's wireframe.
     */
    template <typename T_Texture>
    inline void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::math::Vec3& startPos, const nexus::math::Vec3& endPos, float radius, const nexus::gfx::Color& color)
    {
        DrawCapsuleWires(ctx, startPos, endPos, radius, 16, 16, color);
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object, number of slices, number of rings, and color.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param slices The number of slices used to approximate the capsule's cylindrical body.
     * @param rings The number of rings used to approximate the capsule's spherical ends.
     * @param color The color of the capsule's wireframe.
     */
    template <typename T_Texture>
    inline void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, int slices, int rings, const nexus::gfx::Color& color)
    {
        DrawCapsuleWires(ctx, capsule.start, capsule.end, capsule.radius, slices, rings, color);
    }

    /**
     * @brief Draws the wireframe of a capsule (a 3D cylinder with spherical ends) on the screen using the provided Capsule object and color, with default values for slices and rings.
     * 
     * @param ctx The rendering context.
     * @param capsule The Capsule object representing the capsule.
     * @param color The color of the capsule's wireframe.
     */
    template <typename T_Texture>
    inline void DrawCapsuleWires(nexus::gapi::Context& ctx, const nexus::shape3D::Capsule& capsule, const nexus::gfx::Color& color)
    {
        DrawCapsuleWires(ctx, capsule.start, capsule.end, capsule.radius, 16, 16, color);
    }

    /**
     * @brief Draws a plane (a flat surface) on the screen using the provided center position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param centerPos The position of the plane's center.
     * @param size The size of the plane (width and height).
     * @param color The color of the plane.
     */
    template <typename T_Texture>
    void DrawPlane(nexus::gapi::Context& ctx, const nexus::math::Vec3& centerPos, const nexus::math::Vec2& size, const nexus::gfx::Color& color)
    {
        // NOTE: Plane is always created on XZ ground
        ctx.PushMatrix();
            ctx.Translate(centerPos.x, centerPos.y, centerPos.z);
            ctx.Scale(size.x, 1.0f, size.y);

            ctx.Begin(nexus::gapi::DrawMode::Quads);
                ctx.Color(color);
                ctx.Normal(0.0f, 1.0f, 0.0f);

                ctx.Vertex(-0.5f, 0.0f, -0.5f);
                ctx.Vertex(-0.5f, 0.0f, 0.5f);
                ctx.Vertex(0.5f, 0.0f, 0.5f);
                ctx.Vertex(0.5f, 0.0f, -0.5f);
            ctx.End();
        ctx.PopMatrix();
    }

    /**
     * @brief Draws a plane (a flat surface) on the screen using the provided Plane object and color.
     * 
     * @param ctx The rendering context.
     * @param plane The Plane object representing the plane.
     * @param color The color of the plane.
     */
    template <typename T_Texture>
    inline void DrawPlane(nexus::gapi::Context& ctx, const nexus::shape3D::Plane& plane, const nexus::gfx::Color& color)
    {
        DrawPlane(ctx, plane.center, plane.size, color);
    }

    /**
     * @brief Draws a ray (a half-infinite line) on the screen using the provided Ray object and color.
     * 
     * @param ctx The rendering context.
     * @param ray The Ray object representing the ray.
     * @param color The color of the ray.
     */
    template <typename T_Texture>
    void DrawRay(nexus::gapi::Context& ctx, const nexus::shape3D::Ray& ray, const nexus::gfx::Color& color)
    {
        constexpr float scale = 10000;

        ctx.Begin(nexus::gapi::DrawMode::Lines);
            ctx.Color(color);
            ctx.Vertex(ray.position);
            ctx.Vertex(ray.position + ray.direction * scale);
        ctx.End();
    }

    /**
     * @brief Draws a grid on the screen using the provided number of slices and spacing between grid lines.
     * 
     * @param ctx The rendering context.
     * @param slices The number of slices used to create the grid.
     * @param spacing The spacing between grid lines.
     */
    template <typename T_Texture>
    void DrawGrid(nexus::gapi::Context& ctx, int slices, float spacing)
    {
        const int halfSlices = slices / 2;

        ctx.Begin(nexus::gapi::DrawMode::Lines);
            for (int i = -halfSlices; i <= halfSlices; i++)
            {
                if (i == 0)
                {
                    ctx.Color(0.5f, 0.5f, 0.5f);
                    ctx.Color(0.5f, 0.5f, 0.5f);
                    ctx.Color(0.5f, 0.5f, 0.5f);
                    ctx.Color(0.5f, 0.5f, 0.5f);
                }
                else
                {
                    ctx.Color(0.75f, 0.75f, 0.75f);
                    ctx.Color(0.75f, 0.75f, 0.75f);
                    ctx.Color(0.75f, 0.75f, 0.75f);
                    ctx.Color(0.75f, 0.75f, 0.75f);
                }

                ctx.Vertex(i * spacing, 0.0f, -halfSlices * spacing);
                ctx.Vertex(i * spacing, 0.0f, halfSlices * spacing);

                ctx.Vertex(-halfSlices * spacing, 0.0f, i * spacing);
                ctx.Vertex(halfSlices * spacing, 0.0f, i * spacing);
            }
        ctx.End();
    }

}

#endif //0

#endif //NEXUS_GAPI_IMPL_PRIMITIVES_3D_HPP
