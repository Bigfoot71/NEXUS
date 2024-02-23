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

#ifndef NEXUS_GFX_COLOR_HPP
#define NEXUS_GFX_COLOR_HPP

#include "../math/nxVec4.hpp"
#include "../math/nxVec3.hpp"
#include <SDL_pixels.h>

namespace nexus { namespace gfx {

    /**
     * @struct Color
     * @brief A structure representing a color with various constructors and color manipulation methods.
     *
     * The Color struct encapsulates color information, providing constructors to create colors
     * from various representations and methods to manipulate and convert color values.
     */
    struct NEXUS_API Color : ::SDL_Color
    {
        /**
         * @brief Convert HSV (Hue, Saturation, Value) components to RGB and create a Color.
         *
         * This static function converts HSV color space to RGB color space based on the provided
         * Hue, Saturation, and Value components. The resulting RGB values are used to create a Color.
         *
         * @param hue The hue component of the color in degrees (0 to 360).
         * @param saturation The saturation component of the color as a float in the range [0.0, 1.0].
         * @param value The value component of the color as a float in the range [0.0, 1.0].
         * @param alpha The alpha component of the color as a float in the range [0.0, 1.0].
         * @return The resulting Color after conversion from HSV components.
         */
        static constexpr Color NewFromHSV(float hue, float saturation = 1.0f, float value = 1.0f, float alpha = 1.0f);

        /**
         * @brief Convert HSV (Hue, Saturation, Value) components to RGB and create a Color.
         *
         * This static function converts HSV color space to RGB color space based on the provided
         * Vec3 containing Hue, Saturation, and Value components. The resulting RGB values are used
         * to create a Color.
         *
         * @param hsv The Vec3 containing Hue, Saturation, and Value components.
         * @param alpha The alpha component of the color as a float in the range [0.0, 1.0].
         * @return The resulting Color after conversion from HSV components.
         */
        static constexpr Color NewFromHSV(const math::Vec3& hsv, float alpha = 1.0f);

        /**
         * @brief Create a new color by performing alpha blending of two colors with an optional tint.
         *
         * This static function performs alpha blending of the source color (src) onto the destination color (dst)
         * with an optional tint color. The result is a new color obtained by blending the colors based on their
         * alpha values and applying the tint color if provided.
         *
         * @param dst The destination color.
         * @param src The source color.
         * @param tint The optional tint color (default is fully opaque white).
         * @return The resulting color after alpha blending with optional tint.
         */
        static constexpr Color NewAlphaBlend(const Color& dst, Color src, const Color& tint = Color(255, 255, 255, 255));

        /**
         * @brief Default constructor for Color, initializes the color as transparent black.
         */
        constexpr Color() : ::SDL_Color{ 0, 0, 0, 0 }
        { }

        /**
         * @brief Constructor for Color with RGB and optional alpha values.
         *
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         * @param a The optional alpha (transparency) component of the color (default is 255, fully opaque).
         */
        constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        : ::SDL_Color{ r, g, b, a }
        { }

        /**
         * @brief Constructor for Color taking an SDL_Color object.
         *
         * @param color The SDL_Color object representing the color.
         */
        constexpr Color(::SDL_Color color)
        : ::SDL_Color(color)
        { }

        /**
         * @brief Constructor for Color from a 32-bit color value.
         *
         * @param color The 32-bit color value.
         */
        constexpr Color(Uint32 color)
        : ::SDL_Color {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            static_cast<Uint8>((color >> 24) & 0xFF),
            static_cast<Uint8>((color >> 16) & 0xFF),
            static_cast<Uint8>((color >> 8)  & 0xFF),
            static_cast<Uint8>(color         & 0xFF)
        #else // Assume little endian (ABGR format)
            static_cast<Uint8>(color         & 0xFF),
            static_cast<Uint8>((color >> 8)  & 0xFF),
            static_cast<Uint8>((color >> 16) & 0xFF),
            static_cast<Uint8>((color >> 24) & 0xFF)
        #endif
        } { }

        /**
         * @brief Constructor for Color from a 32-bit color value and an SDL pixel format.
         *
         * @param color The 32-bit color value in ARGB format.
         * @param format The SDL pixel format to use for interpreting the color value.
         */
        Color(Uint32 color, const SDL_PixelFormat* format)
        {
            SDL_GetRGBA(color, format, &r, &g, &b, &a);
        }

        /**
         * @brief Constructor for Color from a Vec4 color in the range [0.0, 1.0].
         *
         * @param color A Vec4 representing the color components as floats in the range [0.0, 1.0].
         */
        constexpr Color(math::Vec4 color)
        : ::SDL_Color {
            static_cast<Uint8>(255 * std::clamp(color.x, 0.0f, 1.0f)),
            static_cast<Uint8>(255 * std::clamp(color.y, 0.0f, 1.0f)),
            static_cast<Uint8>(255 * std::clamp(color.z, 0.0f, 1.0f)),
            static_cast<Uint8>(255 * std::clamp(color.w, 0.0f, 1.0f)),
        } { }

        /**
         * @brief Constructor for Color from HSV (Hue, Saturation, Value) components.
         *
         * @param hsv The Vec3 containing Hue, Saturation, and Value components.
         * @param alpha The alpha component of the color as a float in the range [0.0, 1.0].
         */
        Color(const math::Vec3& hsv, float alpha = 1.0f)
        {
            FromHSV(hsv.x, hsv.y, hsv.z, alpha);
        }

        /**
         * @brief Conversion operator to Uint32.
         *
         * This conversion operator allows implicit conversion of the Color object to a 32-bit ARGB color value.
         * It uses the ToUint32() method to obtain the equivalent Uint32 representation of the Color.
         *
         * @return The Color represented as a 32-bit ARGB color value.
         */
        constexpr operator Uint32() const { return this->ToUint32(); }

        /**
         * @brief Conversion operator to math::Vec4.
         *
         * This conversion operator allows implicit conversion of the Color object to a math::Vec4 representing the normalized color.
         * It uses the Normalized() method to obtain the equivalent normalized Vec4 representation of the Color.
         *
         * @return The Color represented as a normalized math::Vec4.
         */
        constexpr operator math::Vec4() const { return this->Normalized(); }

        /**
         * @brief Add two Color objects element-wise.
         *
         * This operator performs element-wise addition of the RGBA components of two Color objects.
         * It returns a new Color with the sum of corresponding components.
         *
         * @param other The Color to add.
         * @return The resulting Color after element-wise addition.
         */
        constexpr Color operator+(const Color& other) const
        {
            return {
                static_cast<Uint8>(std::min(r + other.r, 255)),
                static_cast<Uint8>(std::min(g + other.g, 255)),
                static_cast<Uint8>(std::min(b + other.b, 255)),
                static_cast<Uint8>(std::min(a + other.a, 255))
            };
        }

        /**
         * @brief Add a Color object to the current Color object element-wise.
         *
         * This operator performs element-wise addition of the RGBA components of a Color object to the current Color object.
         * It modifies the current Color object by adding the corresponding components of the provided Color object.
         *
         * @param other The Color to add.
         * @return A reference to the modified Color object after element-wise addition.
         */
        constexpr const Color& operator+=(const Color& other)
        {
            r = std::min(r + other.r, 255);
            g = std::min(g + other.g, 255);
            b = std::min(b + other.b, 255);
            a = std::min(a + other.a, 255);
            return *this;
        }

        /**
         * @brief Subtract one Color object from another element-wise.
         *
         * This operator performs element-wise subtraction of the RGBA components of two Color objects.
         * It returns a new Color with the difference of corresponding components.
         *
         * @param other The Color to subtract.
         * @return The resulting Color after element-wise subtraction.
         */
        constexpr Color operator-(const Color& other) const
        {
            return {
                static_cast<Uint8>(std::max(r - other.r, 0)),
                static_cast<Uint8>(std::max(g - other.g, 0)),
                static_cast<Uint8>(std::max(b - other.b, 0)),
                static_cast<Uint8>(std::max(a - other.a, 0))
            };
        }

        /**
         * @brief Subtract one Color object from another element-wise.
         *
         * This operator performs element-wise subtraction of the RGBA components of two Color objects.
         * It modifies the current Color object by subtracting the corresponding components of the provided Color object.
         *
         * @param other The Color to subtract.
         * @return A reference to the modified Color object after element-wise subtraction.
         */
        constexpr const Color& operator-=(const Color& other)
        {
            r = std::max(r - other.r, 0);
            g = std::max(g - other.g, 0);
            b = std::max(b - other.b, 0);
            a = std::max(a - other.a, 0);
            return *this;
        }

        /**
         * @brief Multiply two Color objects element-wise.
         *
         * This operator performs element-wise multiplication of the RGBA components of two Color objects.
         *
         * @param other The Color to multiply with.
         * @return The resulting Color after element-wise multiplication.
         */
        constexpr Color operator*(const Color& other) const
        {
            return {
                static_cast<Uint8>(static_cast<Uint16>(r * other.r) / 255),
                static_cast<Uint8>(static_cast<Uint16>(g * other.g) / 255),
                static_cast<Uint8>(static_cast<Uint16>(b * other.b) / 255),
                static_cast<Uint8>(static_cast<Uint16>(a * other.a) / 255)
            };
        }

        /**
         * @brief Multiply the current Color object by another Color object element-wise.
         *
         * This operator performs element-wise multiplication of the RGBA components of the current Color object by another Color object.
         * It modifies the current Color object by multiplying its corresponding components with the components of the provided Color object.
         *
         * @param other The Color to multiply with.
         * @return A reference to the modified Color object after element-wise multiplication.
         */
        constexpr const Color& operator*=(const Color& other)
        {
            r = static_cast<Uint16>(r * other.r) / 255;
            g = static_cast<Uint16>(g * other.g) / 255;
            b = static_cast<Uint16>(b * other.b) / 255;
            a = static_cast<Uint16>(a * other.a) / 255;
            return *this;
        }

        /**
         * @brief Multiplication operator for scaling the Color by a value.
         *
         * This operator multiplies each channel (red, green, blue, alpha) of the Color by the specified value.
         * The result is a new Color object with the scaled channel values.
         *
         * @param value The value to scale the Color channels by.
         * @return A new Color object with scaled channel values.
         */
        template <typename T>
        constexpr Color operator*(T value) const
        {
            return {
                static_cast<Uint8>(std::min(r * value, static_cast<T>(255))),
                static_cast<Uint8>(std::min(b * value, static_cast<T>(255))),
                static_cast<Uint8>(std::min(g * value, static_cast<T>(255))),
                static_cast<Uint8>(std::min(a * value, static_cast<T>(255)))
            };
        }

        /**
         * @brief Multiply each channel of the current Color object by a specified value.
         *
         * This operator multiplies each channel (red, green, blue, alpha) of the current Color object by the specified value.
         * It modifies the current Color object by scaling its channel values.
         *
         * @tparam T The type of value to scale the Color channels by.
         * @param value The value to scale the Color channels by.
         * @return A reference to the modified Color object with scaled channel values.
         */
        template <typename T>
        constexpr const Color& operator*=(T value)
        {
            r = std::min(r * value, static_cast<T>(255));
            g = std::min(g * value, static_cast<T>(255));
            b = std::min(b * value, static_cast<T>(255));
            a = std::min(a * value, static_cast<T>(255));
            return *this;
        }

        /**
         * @brief Division operator for scaling down the Color by a value.
         *
         * This operator divides each channel (red, green, blue, alpha) of the Color by the specified value.
         * The result is a new Color object with the scaled-down channel values.
         *
         * @param value The value to divide the Color channels by.
         * @return A new Color object with scaled-down channel values.
         */
        template <typename T>
        constexpr Color operator/(T value) const
        {
            return {
                static_cast<Uint8>(std::min(r / value, static_cast<T>(255))),
                static_cast<Uint8>(std::min(b / value, static_cast<T>(255))),
                static_cast<Uint8>(std::min(g / value, static_cast<T>(255))),
                static_cast<Uint8>(std::min(a / value, static_cast<T>(255)))
            };
        }

        /**
         * @brief Divide each channel of the current Color object by a specified value.
         *
         * This operator divides each channel (red, green, blue, alpha) of the current Color object by the specified value.
         * It modifies the current Color object by scaling down its channel values.
         *
         * @tparam T The type of value to divide the Color channels by.
         * @param value The value to divide the Color channels by.
         * @return A reference to the modified Color object with scaled-down channel values.
         */
        template <typename T>
        constexpr const Color& operator/=(T value)
        {
            r = std::min(r / value, static_cast<T>(255));
            g = std::min(g / value, static_cast<T>(255));
            b = std::min(b / value, static_cast<T>(255));
            a = std::min(a / value, static_cast<T>(255));
            return *this;
        }

        /**
         * @brief Check if two Color objects are equal.
         *
         * This operator checks equality by comparing the RGBA components of two Color objects.
         *
         * @param other The Color to compare with.
         * @return True if the colors are equal, false otherwise.
         */
        constexpr bool operator==(const Color& other) const
        {
            return !((r != other.r) || (g != other.g) || (b != other.b) || (a != other.a));
        }

        /**
         * @brief Check if two Color objects are not equal.
         *
         * This operator checks inequality by comparing the RGBA components of two Color objects.
         *
         * @param other The Color to compare with.
         * @return True if the colors are not equal, false otherwise.
         */
        constexpr bool operator!=(const Color& other) const
        {
            return (r != other.r) || (g != other.g) || (b != other.b) || (a != other.a);
        }

        /**
         * @brief Convert the Color to a 32-bit ARGB color value.
         *
         * @return The Color represented as a 32-bit ARGB color value.
         */
        constexpr Uint32 ToUint32() const
        {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            return (static_cast<Uint32>(b) << 24) | (static_cast<Uint32>(g) << 16) | (static_cast<Uint32>(r) << 8) | a;
        #else // Assume little endian (ARGB format)
            return (static_cast<Uint32>(a) << 24) | (static_cast<Uint32>(r) << 16) | (static_cast<Uint32>(g) << 8) | b;
        #endif
        }

        /**
         * @brief Convert the Color to a 32-bit color value using an SDL pixel format.
         *
         * @param format The SDL pixel format to use for encoding the Color.
         * @return The Color represented as a 32-bit color value using the provided format.
         */
        Uint32 ToUint32(const SDL_PixelFormat* format) const
        {
            return SDL_MapRGBA(format, r, g, b, a);
        }

        /**
         * @brief Creates a new Color with the same red, green, and blue components but a different alpha component.
         *
         * @param alpha The alpha value to set for the new Color (range: 0.0 to 1.0).
         * @return A new Color with the specified alpha value.
         */
        constexpr Color Alpha(float alpha) const
        {
            return { r, g, b, static_cast<Uint8>(255 * alpha) };
        }

        /**
         * @brief Get the Color as a Vec4 with normalized (0.0 to 1.0) color components.
         *
         * @return The Color represented as a Vec4 with normalized color components.
         */
        constexpr math::Vec4 Normalized() const
        {
            constexpr float inv = 1.0f / 255.0f;
            return math::Vec4(r * inv, g * inv, b * inv, a * inv);
        }

        /**
         * @brief Convert the Color from HSV (Hue, Saturation, Value) components.
         *
         * This function converts the Color from HSV color space to RGB color space based on the provided
         * Hue, Saturation, and Value components. The resulting RGB values are stored in the Color's
         * red, green, and blue components.
         *
         * @param hue The hue component of the color in degrees (0 to 360).
         * @param saturation The saturation component of the color as a float in the range [0.0, 1.0].
         * @param value The value component of the color as a float in the range [0.0, 1.0].
         * @param alpha The alpha component of the color as a float in the range [0.0, 1.0].
         */
        constexpr void FromHSV(float hue, float saturation, float value, float alpha = 1.0f);

        /**
         * @brief Convert the Color from HSV (Hue, Saturation, Value) components.
         *
         * This function converts the Color from HSV color space to RGB color space based on the provided
         * Vec3 containing Hue, Saturation, and Value components. The resulting RGB values are stored in
         * the Color's red, green, and blue components.
         *
         * @param hsv The Vec3 containing Hue, Saturation, and Value components.
         * @param alpha The alpha component of the color as a float in the range [0.0, 1.0].
         */
        constexpr void FromHSV(const math::Vec3& hsv, float alpha = 1.0f);

        /**
         * @brief Convert the Color to HSV (Hue, Saturation, Value) components.
         *
         * @return The Color represented as HSV components (Hue in degrees, Saturation in the range [0.0, 1.0], Value in the range [0.0, 1.0]).
         */
        constexpr math::Vec3 ToHSV() const;

        /**
         * @brief Create a new color by performing alpha blending of two colors with an this color as tint.
         *
         * This function performs alpha blending of the source color (src) onto the destination color (dst)
         * with this color as tint. The result is a new color obtained by blending the colors based on their
         * alpha values and applying this tint color.
         *
         * @param dst The destination color.
         * @param src The source color.
         * @return The resulting color after alpha blending with this tint.
         */
        constexpr Color AlphaBlend(const Color& dst, const Color& src) const;
    };


    /* Implementation */

    constexpr Color Color::NewFromHSV(float hue, float saturation, float value, float alpha)
    {
        Color tint;
        tint.FromHSV(hue, saturation, value);
        tint.a = 255 * alpha;
        return tint;
    }

    constexpr Color Color::NewFromHSV(const math::Vec3& hsv, float alpha)
    {
        return NewFromHSV(hsv.x, hsv.y, hsv.z, alpha);
    }

    constexpr Color Color::NewAlphaBlend(const Color& dst, Color src, const Color& tint)
    {
        src.r = static_cast<Uint8>((src.r * tint.r + 1) >> 8);
        src.g = static_cast<Uint8>((src.g * tint.g + 1) >> 8);
        src.b = static_cast<Uint8>((src.b * tint.b + 1) >> 8);
        src.a = static_cast<Uint8>((src.a * tint.a + 1) >> 8);

        if (src.a == 0) return dst;
        if (src.a == 255) return src;

        gfx::Color out;
        Uint32 alpha = static_cast<Uint32>(src.a) + 1;
        out.a = static_cast<Uint8>((alpha * 256 + dst.a * (256 - alpha)) >> 8);

        if (out.a > 0)
        {
            out.r = static_cast<Uint8>(((src.r * alpha * 256 + dst.r * dst.a * (256 - alpha)) / out.a) >> 8);
            out.g = static_cast<Uint8>(((src.g * alpha * 256 + dst.g * dst.a * (256 - alpha)) / out.a) >> 8);
            out.b = static_cast<Uint8>(((src.b * alpha * 256 + dst.b * dst.a * (256 - alpha)) / out.a) >> 8);
        }

        return out;
    }

    constexpr void Color::FromHSV(float hue, float saturation, float value, float alpha)
    {
        // Red channel
        float k = std::fmod((5.0f + hue/60.0f), 6);
        float t = 4.0f - k;
        k = (t < k)? t : k;
        k = (k < 1)? k : 1;
        k = (k > 0)? k : 0;
        r = static_cast<unsigned char>(
            (value - value*saturation*k)*255);

        // Green channel
        k = std::fmod((3.0f + hue/60.0f), 6);
        t = 4.0f - k;
        k = (t < k)? t : k;
        k = (k < 1)? k : 1;
        k = (k > 0)? k : 0;
        g = static_cast<unsigned char>(
            (value - value*saturation*k)*255);

        // Blue channel
        k = std::fmod((1.0f + hue/60.0f), 6);
        t = 4.0f - k;
        k = (t < k)? t : k;
        k = (k < 1)? k : 1;
        k = (k > 0)? k : 0;
        b = static_cast<unsigned char>(
            (value - value*saturation*k)*255);

        // Alpha channel
        a = 255 * alpha;
    }

    constexpr void Color::FromHSV(const math::Vec3& hsv, float alpha)
    {
        FromHSV(hsv.x, hsv.y, hsv.z, alpha);
    }

    constexpr math::Vec3 Color::ToHSV() const
    {
        math::Vec3 hsv;
        math::Vec4 rgb = Normalized();

        float min = rgb.x < rgb.y ? rgb.x : rgb.y;
        min = min < rgb.z ? min : rgb.z;

        float max = rgb.x > rgb.y ? rgb.x : rgb.y;
        max = max > rgb.z ? max : rgb.z;

        hsv.z = max;                // Value
        float delta = max - min;

        if (delta < 0.00001f)
        {
            hsv.y = 0.0f;
            hsv.x = 0.0f;
            return hsv;
        }

        if (max > 0.0f)
        {
            hsv.y = (delta/max);    // Saturation
        }
        else
        {
            hsv.y = 0.0f;
            hsv.x = NAN;
            return hsv;
        }

        if (rgb.x >= max)
        {
            hsv.x = (rgb.y - rgb.z)/delta;    // Between yellow & magenta
        }
        else
        {
            if (rgb.y >= max) hsv.x = 2.0f + (rgb.z - rgb.x)/delta;     // Between cyan & yellow
            else hsv.x = 4.0f + (rgb.x - rgb.y)/delta;                  // Between magenta & cyan
        }

        hsv.x *= 60.0f; // Convert to degrees
        if (hsv.x < 0.0f) hsv.x += 360.0f;

        return hsv;
    }

    constexpr Color Color::AlphaBlend(const Color& dst, const Color& src) const
    {
        return NewAlphaBlend(dst, src, *this);
    }


    /* Global colors definition */

    constexpr Color LightGray   (211, 211, 211, 255);
    constexpr Color Silver      (192, 192, 192, 255);
    constexpr Color Gray        (127, 127, 127, 255);
    constexpr Color DarkGray    (80, 80, 80, 255);
    constexpr Color Yellow      (255, 255, 0, 255);
    constexpr Color Gold        (255, 215, 0, 255);
    constexpr Color Orange      (255, 165, 0, 255);
    constexpr Color Pink        (255, 105, 180, 255);
    constexpr Color Red         (255, 0, 0, 255);
    constexpr Color Maroon      (128, 0, 0, 255);
    constexpr Color Green       (0, 255, 0, 255);
    constexpr Color DarkGreen   (0, 100, 0, 255);
    constexpr Color SkyBlue     (135, 206, 235, 255);
    constexpr Color Blue        (0, 0, 255, 255);
    constexpr Color DarkBlue    (0, 0, 100, 255);
    constexpr Color Purple      (127, 0, 127, 255);
    constexpr Color Violet      (238, 130, 238, 255);
    constexpr Color DarkPurple  (79, 0, 79, 255);
    constexpr Color Beige       (255, 198, 153, 255);
    constexpr Color Brown       (139, 69, 19, 255);
    constexpr Color DarkBrown   (92, 64, 51, 255);

    constexpr Color White       (255, 255, 255, 255);
    constexpr Color Black       (0, 0, 0, 255);
    constexpr Color Blank       (0, 0, 0, 0);

    constexpr Color Magenta     (255, 0, 255, 255);
    constexpr Color Cyan        (0, 255, 255, 255);
}}

#endif //NEXUS_GFX_COLOR_HPP
