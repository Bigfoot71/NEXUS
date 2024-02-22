#ifndef NEXUS_CMN_EXT_GFX_GAPI_IMPL_NINE_PATCH_HPP
#define NEXUS_CMN_EXT_GFX_GAPI_IMPL_NINE_PATCH_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../utils/nxContextual.hpp"
#include "../../shape/2D/nxAABB.hpp"
#include "../../gfx/nxColor.hpp"

namespace _ext_gfx_gapi_impl {

    /**
     * @brief Structure for defining Nine Patch information for texture rendering.
     */
    struct NEXUS_API NinePatchInfo
    {
        enum Layout : Uint8
        {
            NinePatch = 0,                  ///< 3x3 tiles layout
            ThreePatchVertical,             ///< 1x3 tiles layout
            ThreePatchHorizontal            ///< 3x1 tiles layout
        };

        nexus::shape2D::Rectangle source;      ///< Texture source rectangle
        nexus::shape2D::AABB bounds;           ///< Min = Left-Top border offset | Max = Right-Bottom border offset
        Layout layout;                      ///< Layout of the n-patch: 3x3, 1x3, or 3x1

        /**
         * @brief Constructor for NinePatchInfo.
         * @param source The source rectangle of the texture.
         * @param bounds The border offsets (Min = Left-Top, Max = Right-Bottom).
         * @param layout The layout type of the Nine Patch (default is 3x3).
         */
        NinePatchInfo(const nexus::shape2D::Rectangle& source, const nexus::shape2D::AABB& bounds = {}, Layout layout = Layout::NinePatch)
        : source(source), bounds(bounds), layout(layout)
        { }

        /**
         * @brief Constructor for NinePatchInfo using individual border offsets.
         * @param source The source rectangle of the texture.
         * @param left Left border offset.
         * @param top Top border offset.
         * @param right Right border offset.
         * @param bottom Bottom border offset.
         * @param layout The layout type of the Nine Patch (default is 3x3).
         */
        NinePatchInfo(const nexus::shape2D::Rectangle& source, int left, int top, int right, int bottom, Layout layout = Layout::NinePatch)
        : source(source), bounds(left, top, right, bottom), layout(layout)
        { }

        /**
         * @brief Draw the Nine Patch texture on a destination rectangle.
         * @tparam T_Texture Type of the texture container.
         * @param texture The texture container.
         * @param dest Destination rectangle to draw the Nine Patch texture.
         * @param origin Origin point for rotation and scaling (default is top-left).
         * @param rotation Rotation angle in degrees (default is 0.0).
         * @param tint Tint color for the Nine Patch texture (default is White).
         */
        template <typename T_Texture>
        void Draw(const T_Texture& texture, const nexus::shape2D::RectangleF& dest, const nexus::math::Vec2& origin = { 0, 0 }, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White)
        {
            static_assert(nexus::utils::IsContainer<T_Texture>::value, "<T_Texture> must be derived from a contextual container.");
            texture->DrawNinePatch(*this, dest, origin, rotation, tint);
        }
    };

}

#endif //NEXUS_CMN_EXT_GFX_GAPI_IMPL_NINE_PATCH_HPP
