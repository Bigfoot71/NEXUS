#ifndef NEXUS_SHAPE_2D_RECTANGLE_HPP
#define NEXUS_SHAPE_2D_RECTANGLE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief 2D Rectangle collision shape with integer coordinates.
     */
    struct NEXUS_API Rectangle : SDL_Rect, CollisionShape
    {
        /**
         * @brief Default constructor.
         */
        constexpr Rectangle() : SDL_Rect{} { }

        /**
         * @brief Constructor initializing the rectangle with specified coordinates and dimensions.
         * @param x The x-coordinate of the top-left corner.
         * @param y The y-coordinate of the top-left corner.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         */
        constexpr Rectangle(int x, int y, int w, int h)
        : SDL_Rect{ x, y, w, h }
        { }

        /**
         * @brief Constructor initializing the rectangle from an SDL_FRect.
         * Rounds the float coordinates to the nearest integers.
         * @param other The SDL_FRect to convert to a rectangle.
         */
        constexpr Rectangle(const SDL_FRect& other) : SDL_Rect{
            static_cast<int>(other.x + 0.5f),
            static_cast<int>(other.y + 0.5f),
            static_cast<int>(other.w + 0.5f),
            static_cast<int>(other.h + 0.5f)
        } { }

        /**
         * @brief Constructor initializing the rectangle with specified position and size.
         * @param position The position of the top-left corner.
         * @param size The size (dimensions) of the rectangle.
         */
        constexpr Rectangle(const math::IVec2& position, const math::IVec2& size)
        : SDL_Rect{ position.x, position.y, size.x, size.y }
        { }

        /**
         * @brief Equality comparison operator for rectangles.
         * @param other Another rectangle to compare against.
         * @return True if the two rectangles are equal, false otherwise.
         */
        constexpr bool operator==(const Rectangle& other) const
        {
            return x == other.x && y == other.y
                && w == other.w && h == other.h;
        }

        /**
         * @brief Inequality comparison operator for rectangles.
         * @param other Another rectangle to compare against.
         * @return True if the two rectangles are not equal, false otherwise.
         */
        constexpr bool operator!=(const Rectangle& other) const
        {
            return *this != other;
        }

        /**
         * @brief Set the position of the top-left corner of the rectangle.
         * @param x The new x-coordinate of the top-left corner.
         * @param y The new y-coordinate of the top-left corner.
         */
        void SetPosition(int x, int y)
        {
            this->x = x, this->y = y;
        }

        /**
         * @brief Set the position of the top-left corner of the rectangle.
         * @param position The new position of the top-left corner.
         */
        void SetPosition(const math::IVec2& position)
        {
            SetPosition(position.x, position.y);
        }

        /**
         * @brief Set the size (dimensions) of the rectangle.
         * @param w The new width of the rectangle.
         * @param h The new height of the rectangle.
         */
        void SetSize(int w, int h)
        {
            this->w = w, this->h = h;
        }

        /**
         * @brief Set the size (dimensions) of the rectangle.
         * @param size The new size (dimensions) of the rectangle.
         */
        void SetSize(const math::IVec2& size)
        {
            SetSize(size.x, size.y);
        }

        /**
         * @brief Get the position of the top-left corner of the rectangle.
         * @return The position of the top-left corner of the rectangle.
         */
        math::IVec2 GetPosition() const
        {
            return { x, y };
        }

        /**
         * @brief Get the size (dimensions) of the rectangle.
         * @return The size (dimensions) of the rectangle.
         */
        math::IVec2 GetSize() const
        {
            return { w, h };
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the rectangle.
         * @return The AABB representing the bounding box of the rectangle.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the rectangle and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the rectangle and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the rectangle and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the rectangle and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the rectangle and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the rectangle and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the rectangle and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the rectangle and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the rectangle and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the rectangle and another rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the rectangle and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

    /**
     * @brief 2D Rectangle collision shape with floating point coordinates.
     */
    struct NEXUS_API RectangleF : SDL_FRect, CollisionShape
    {
        /**
         * @brief Default constructor.
         */
        constexpr RectangleF() : SDL_FRect{} { }

        /**
         * @brief Constructor initializing the rectangle with specified coordinates and dimensions.
         * @param x The x-coordinate of the top-left corner.
         * @param y The y-coordinate of the top-left corner.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         */
        constexpr RectangleF(float x, float y, float w, float h)
        : SDL_FRect{ x, y, w, h }
        { }

        /**
         * @brief Constructor initializing the rectangle from an SDL_Rect.
         * @param other The SDL_Rect to convert to a rectangle.
         */
        constexpr RectangleF(const SDL_Rect& other) : SDL_FRect{
            static_cast<float>(other.x),
            static_cast<float>(other.y),
            static_cast<float>(other.w),
            static_cast<float>(other.h)
        } { }

        /**
         * @brief Constructor initializing the rectangle with specified position and size.
         * @param position The position of the top-left corner.
         * @param size The size (dimensions) of the rectangle.
         */
        constexpr RectangleF(const math::Vec2& position, const math::Vec2& size)
        : SDL_FRect{ position.x, position.y, size.x, size.y }
        { }

        /**
         * @brief Equality comparison operator for rectangles.
         * @param other Another rectangle to compare against.
         * @return True if the two rectangles are equal, false otherwise.
         */
        constexpr bool operator==(const Rectangle& other) const
        {
            return x == other.x && y == other.y
                && w == other.w && h == other.h;
        }

        /**
         * @brief Inequality comparison operator for rectangles.
         * @param other Another rectangle to compare against.
         * @return True if the two rectangles are not equal, false otherwise.
         */
        constexpr bool operator!=(const Rectangle& other) const
        {
            return *this != other;
        }

        /**
         * @brief Set the position of the top-left corner of the rectangle.
         * @param x The new x-coordinate of the top-left corner.
         * @param y The new y-coordinate of the top-left corner.
         */
        void SetPosition(float x, float y)
        {
            this->x = x, this->y = y;
        }

        /**
         * @brief Set the position of the top-left corner of the rectangle.
         * @param position The new position of the top-left corner.
         */
        void SetPosition(const math::Vec2& position)
        {
            SetPosition(position.x, position.y);
        }

        /**
         * @brief Set the size (dimensions) of the rectangle.
         * @param w The new width of the rectangle.
         * @param h The new height of the rectangle.
         */
        void SetSize(float w, float h)
        {
            this->w = w, this->h = h;
        }

        /**
         * @brief Set the size (dimensions) of the rectangle.
         * @param size The new size (dimensions) of the rectangle.
         */
        void SetSize(const math::Vec2& size)
        {
            SetSize(size.x, size.y);
        }

        /**
         * @brief Get the position of the top-left corner of the rectangle.
         * @return The position of the top-left corner of the rectangle.
         */
        math::Vec2 GetPosition() const
        {
            return { x, y };
        }

        /**
         * @brief Get the size (dimensions) of the rectangle.
         * @return The size (dimensions) of the rectangle.
         */
        math::Vec2 GetSize() const
        {
            return { w, h };
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the rectangle.
         * @return The AABB representing the bounding box of the rectangle.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the rectangle and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the rectangle and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the rectangle and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the rectangle and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the rectangle and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the rectangle and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the rectangle and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the rectangle and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the rectangle and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the rectangle and another rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the rectangle and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif // NEXUS_SHAPE_2D_RECTANGLE_HPP
