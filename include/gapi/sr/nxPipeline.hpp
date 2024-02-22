#ifndef NEXUS_SR_PIPELINE_HPP
#define NEXUS_SR_PIPELINE_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec4.hpp"
#include "../../math/nxVec3.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxFramebuffer.hpp"
#include "./nxShader.hpp"
#include "./nxEnums.hpp"
#include <array>

namespace _sr_impl {

    struct Vertex
    {
        nexus::math::Vec4 position;
        nexus::math::Vec3 normal;
        nexus::math::Vec2 texcoord;
        nexus::gfx::Color color;
    };

}

namespace nexus { namespace sr {

    class NEXUS_API Pipeline
    {
      private:
        std::array<_sr_impl::Vertex, 4> vertices{};
        Uint8 vertexCounter{};
        DrawMode mode;

      private:
        /**
         * @brief Converts normalized homogeneous coordinates to screen coordinates.
         *
         * This function takes homogeneous coordinates that are normalized and converts them into screen coordinates.
         * The transformation includes adjustments for viewport dimensions.
         *
         * @note: The function takes the viewport from which 1 has been subtracted from the width and height.
         *
         * @param homogeneous The vertex with normalized homogeneous coordinates to be converted.
         * @param viewport The 2D rectangle representing the viewport.
         */
        static void HomogeneousToScreen(math::Vec4& homogeneous, const shape2D::Rectangle& viewport);

        /**
         * @brief Interpolates between two vertices based on a specified parameter.
         *
         * This function performs linear interpolation between two vertices, producing a new vertex
         * with interpolated position, normal, texture coordinates, and color.
         *
         * @param start The starting vertex.
         * @param end The ending vertex.
         * @param t The interpolation parameter (should be in the range [0, 1]).
         * @return The interpolated vertex.
         */
        static _sr_impl::Vertex VertexInterpolation(const _sr_impl::Vertex& start, const _sr_impl::Vertex& end, float t);

        /**
         * @brief Clips a 2D line against a rectangular viewport.
         *
         * This function clips a 2D line segment defined by two vertices against a rectangular viewport.
         * The function modifies the input vertices to ensure that the line segment lies within the viewport.
         *
         * @param v0 The first vertex of the line segment.
         * @param v1 The second vertex of the line segment.
         * @param viewport The 2D rectangle representing the viewport. (NOTE: Width and height should have 1 subtracted.)
         * @return True if the line segment is visible after clipping, false otherwise.
         */
        static bool ClipLine2D(_sr_impl::Vertex& v0, _sr_impl::Vertex& v1, const shape2D::Rectangle& viewport);

        /**
         * @brief Clips a 3D line segment against the near and far planes using homogeneous coordinates.
         *
         * This function clips a 3D line segment defined by two vertices against the near and far planes
         * using homogeneous coordinates. The function modifies the input vertices to ensure that the line
         * segment lies within the view frustum.
         *
         * @param v0 The first vertex of the line segment.
         * @param v1 The second vertex of the line segment.
         * @return True if the line segment is visible after clipping, false otherwise.
         */
        static bool ClipLine3D(_sr_impl::Vertex& v0, _sr_impl::Vertex& v1);

        /**
         * @brief Clips a polygon against the near and far planes using homogeneous coordinates.
         *
         * This function clips a polygon against the near and far planes using homogeneous coordinates.
         * It checks each vertex's depth value (w coordinate) against a small epsilon (ClipEpsilon).
         * Vertices with depth less than ClipEpsilon are considered to be outside the near plane,
         * and vertices with depth greater than ClipEpsilon are considered to be inside the near plane.
         * The function interpolates new vertices for edges crossing the near and far planes and adds them to the clipped polygon.
         *
         * @param polygon The input and output polygon, which may be clipped against the near and far planes.
         *                The function modifies this polygon in place.
         *
         * @return True if the resulting clipped polygon is not empty, false otherwise.
         */
        static bool ClipPolygonW(std::array<_sr_impl::Vertex, 12>& polygon, Uint8& vertexCounter);

        /**
         * @brief Clips a polygon against the view frustum in the XYZ (3D) space.
         *
         * This function clips a polygon against the view frustum in the XYZ (3D) space.
         * It checks each vertex's coordinates against its homogeneous coordinate (w).
         * Vertices with x, y, or z coordinates outside the normalized range [-w, w] are considered outside the frustum.
         * The function interpolates new vertices for edges crossing the frustum planes and adds them to the clipped polygon.
         *
         * @param polygon The input and output polygon, which may be clipped against the view frustum.
         *                The function modifies this polygon in place.
         *
         * @return True if the resulting clipped polygon is not empty, false otherwise.
         */
        static bool ClipPolygonXYZ(std::array<_sr_impl::Vertex, 12>& polygon, Uint8& vertexCounter);

        /**
         * @brief Projects and clips vertices to screen space.
         *
         * This function projects vertices onto the screen and clips them against the viewport.
         * The viewport dimensions are adjusted to account for the framebuffer dimensions.
         *
         * @param line The array of vertices representing the line segment.
         * @param vertexCounter The counter for vertices.
         * @param mvp The model-view-projection matrix.
         * @param viewport The viewport dimensions adjusted for the framebuffer.
         * @param shader The shader to be used.
         */
        static void ProjectAndClipLine(std::array<_sr_impl::Vertex, 2>& line, Uint8& vertexCounter, const math::Mat4& mvp, const shape2D::Rectangle& viewport, Shader* shader);

        /**
         * @brief Projects and clips vertices of a polygon to screen space.
         *
         * This function projects vertices of a polygon onto the screen and clips them against the viewport.
         * The viewport dimensions are adjusted to account for the framebuffer dimensions.
         *
         * @param polygon The array of vertices representing the polygon.
         * @param vertexCounter The counter for vertices.
         * @param mvp The model-view-projection matrix.
         * @param viewport The viewport dimensions adjusted for the framebuffer.
         * @param shader The shader to be used.
         * @param is2D Flag indicating whether the rendering is in 2D.
         */
        static void ProjectAndClipTriangle(std::array<_sr_impl::Vertex, 12>& polygon, Uint8& vertexCounter, const math::Mat4& mvp, const shape2D::Rectangle& viewport, nexus::sr::Shader* shader, bool& is2D);

      private:
        /**
         * @brief Rasterizes a line on the screen with vertices already transformed into screen coordinates.
         * @param framebuffer The framebuffer we should render to.
         * @param v0 The first vertex of the line.
         * @param v1 The second vertex of the line.
         * @param depthTest Flag indicating whether depth testing should be applied.
         */
        static void RasterizeLine(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, bool depthTest);

        /**
         * @brief Rasterizes a triangle on the screen with vertices already transformed into screen coordinates.
         *
         * @note: Specially designed for orthographic rendering simulating 2D,
         *       in this case the triangles will not have been clipped.
         *
         * @param framebuffer The framebuffer we should render to.
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param shader The shader to be used for rendering the triangle.
         * @param depthTest Flag indicating whether depth testing should be applied.
         * @param vieport Viewport in (1,1) will have been subtracted from the dimensions. (necessary for the calculation of the boundings boxes, given that the triangles will not have been clipped)
         */
        static void RasterizeTriangleColor2D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, bool depthTest, const shape2D::Rectangle& viewport);

        /**
         * @brief Rasterizes a triangle on the screen with vertices already transformed into screen coordinates.
         *
         * @note: Specially designed for orthographic rendering simulating 2D,
         *       in this case the triangles will not have been clipped.
         *
         * @param framebuffer The framebuffer we should render to.
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param shader The shader to be used for rendering the triangle.
         * @param image The image to be used for rendering the triangle.
         * @param depthTest Flag indicating whether depth testing should be applied.
         * @param vieport Viewport in (1,1) will have been subtracted from the dimensions. (necessary for the calculation of the boundings boxes, given that the triangles will not have been clipped)
         */
        static void RasterizeTriangleImage2D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, const gfx::Surface* image, bool depthTest, const shape2D::Rectangle& viewport);

        /**
         * @brief Rasterizes a triangle on the screen with vertices already transformed into screen coordinates.
         *
         * @note: Specially designed for 3D rendering, in this case the triangles will have been clipped.
         *
         * @param framebuffer The framebuffer we should render to.
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param shader The shader to be used for rendering the triangle.
         * @param depthTest Flag indicating whether depth testing should be applied.
         */
        static void RasterizeTriangleColor3D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, bool depthTest);

        /**
         * @brief Rasterizes a triangle on the screen with vertices already transformed into screen coordinates.
         *
         * @note: Specially designed for 3D rendering, in this case the triangles will have been clipped.
         *
         * @param framebuffer The framebuffer we should render to.
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param shader The shader to be used for rendering the triangle.
         * @param image The image to be used for rendering the triangle.
         * @param depthTest Flag indicating whether depth testing should be applied.
         */
        static void RasterizeTriangleImage3D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, const gfx::Surface* image, bool depthTest);

      public:
        /**
         * @brief Resets the vertex counter.
         *
         * This function resets the vertex counter to zero.
         */
        void Reset();

        /**
         * @brief Adds a vertex to the buffer.
         *
         * This function adds a vertex to the buffer based on the specified draw mode.
         *
         * @param mode The draw mode.
         * @param position The position of the vertex.
         * @param normal The normal vector of the vertex.
         * @param texcoord The texture coordinates of the vertex.
         * @param color The color of the vertex.
         * @return True if the vertex buffer is full according to the draw mode, false otherwise.
         */
        bool AddVertex(DrawMode mode, const math::Vec3& position, const math::Vec3& normal, const math::Vec2& texcoord, const gfx::Color& color);

        /**
         * @brief Processes and renders the vertices.
         *
         * This function processes and renders the vertices to the framebuffer using the specified shader and image.
         *
         * @param framebuffer The framebuffer to render to.
         * @param mvp The model-view-projection matrix.
         * @param viewport The viewport dimensions adjusted for the framebuffer.
         * @param shader The shader to be used.
         * @param image The image to be used for rendering.
         * @param depthTest Flag indicating whether depth testing should be applied.
         */
        void ProcessAndRender(Framebuffer& framebuffer, const math::Mat4& mvp, const shape2D::Rectangle& viewport, Shader* shader, const gfx::Surface* image, bool depthTest);
    };

}}

#endif //NEXUS_SR_PIPELINE_HPP