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

#ifndef NEXUS_GFX_SURFACE_HPP
#define NEXUS_GFX_SURFACE_HPP

#include "../shape/2D/nxRectangle.hpp"
#include "../shape/2D/nxMesh.hpp"

#include "../core/nxFileSystem.hpp"
#include "../core/nxFileFormat.hpp"
#include "../core/nxException.hpp"
#include "../math/nxMath.hpp"
#include "../math/nxVec2.hpp"
#include "./nxBlendMode.hpp"
#include "./nxPixel.hpp"
#include "./nxColor.hpp"

#include <SDL_blendmode.h>
#include <SDL_surface.h>
#include <SDL_stdinc.h>
#include <SDL_image.h>
#include <SDL_rwops.h>
#include <SDL_error.h>

#include <stdexcept>
#include <utility>
#include <cstring>

namespace nexus { namespace gfx {

    using Image = class Surface;

    /**
     * @brief The Surface class represents an SDL surface.
     */
    class NEXUS_API Surface
    {
      protected:
        SDL_Surface* surface = nullptr;     ///< The SDL surface pointer.
        bool autoLifetimeManagement = true; ///< Automatic lifetime management (if true, the SDL_Surface will be deallocated in the destructor)

      public:
        /**
         * @brief Generate a new Surface with the specified dimensions and fill it with the specified color.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
         * filling it with the provided color.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param color The color to fill the Surface with.
         * @return A new Surface with the specified dimensions and color.
         */
        static Surface New(int width, int height, const Color& color);

        /**
         * @brief Generate a new Surface with a linear gradient between two colors.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
          * filled with a linear gradient between the provided start and end colors.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param direction The direction of the linear gradient (0: horizontal, 1: vertical).
         * @param start The starting color of the gradient.
         * @param end The ending color of the gradient.
         * @return A new Surface with a linear gradient.
         */
        static Surface NewGradientLinear(int width, int height, float direction, const Color& start, const Color& end);

        /**
         * @brief Generate a new Surface with a radial gradient between two colors.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
         * filled with a radial gradient between the provided inner and outer colors.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param density The density of the radial gradient.
         * @param inner The color at the center of the gradient.
         * @param outer The color at the outer edge of the gradient.
         * @return A new Surface with a radial gradient.
         */
        static Surface NewGradientRadial(int width, int height, float density, const Color& inner, const Color& outer);

        /**
         * @brief Generate a new Surface with a square gradient between two colors.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
         * filled with a square gradient between the provided inner and outer colors.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param density The density of the square gradient.
         * @param inner The color at the center of the gradient.
         * @param outer The color at the outer edge of the gradient.
         * @return A new Surface with a square gradient.
         */
        static Surface NewGradientSquare(int width, int height, float density, const Color& inner, const Color& outer);

        /**
         * @brief Generate a new Surface with a checked pattern of two colors.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
         * filled with a checked pattern of the provided two colors.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param checksX The number of checks along the X-axis.
         * @param checksY The number of checks along the Y-axis.
         * @param col1 The color of the first check.
         * @param col2 The color of the second check.
         * @return A new Surface with a checked pattern.
         */
        static Surface NewChecked(int width, int height, int checksX, int checksY, const Color& col1, const Color& col2);

        /**
         * @brief Generate a new Surface with white noise.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
         * filled with white noise. The factor parameter controls the intensity of the noise.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param factor The factor controlling the intensity of the white noise.
         * @return A new Surface with white noise.
         */
        static Surface NewWhiteNoise(int width, int height, float factor);

        /**
         * @brief Generate a new Surface with a cellular pattern.
         *
         * This function creates a new RGBA32-format Surface with the specified width and height,
         * filled with a cellular pattern. The tileSize parameter controls the size of the cells.
         *
         * @param width The width of the new Surface.
         * @param height The height of the new Surface.
         * @param tileSize The size of the cells in the cellular pattern.
         * @return A new Surface with a cellular pattern.
         */
        static Surface NewCellular(int width, int height, int tileSize);

      public:
        /**
         * @brief Default constructor for the Surface class.
         * 
         * This constructor initializes the surface pointer to nullptr, allowing
         * you to create the surface later using the Create() method.
         */
        Surface() = default;

        /**
         * @brief Constructor to create a Surface object from an existing surface.
         *
         * @param sdlSurface The existing SDL surface to encapsulate.
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         */
        Surface(SDL_Surface* sdlSurface, bool autoLifetimeManagement = true)
        : surface(sdlSurface), autoLifetimeManagement(autoLifetimeManagement)
        { }

        /**
         * @brief Constructor to create a Surface object with the specified width, height, pixel format, and optional fill color.
         *
         * This constructor creates a new SDL surface with the specified width, height, and pixel format.
         * If the surface creation fails, it throws an exception with an error message indicating the SDL error.
         * Optionally, you can provide a fill color using the gfx::Color parameter to initialize the surface with a specific color.
         *
         * @param width The width of the surface.
         * @param height The height of the surface.
         * @param color Optional fill color for initializing the surface (default is gfx::Blank).
         * @param format Optional pixel format of the surface (default is PixelFormat::RGBA32).
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         */
        Surface(int width, int height, const Color& color = Blank, PixelFormat format = PixelFormat::RGBA32, bool autoLifetimeManagement = true)
        : autoLifetimeManagement(autoLifetimeManagement)
        {
            Create(width, height, format);
            Fill(color);
        }

        /**
         * @brief Constructor to create a Surface object by loading an image from a file.
         *
         * This constructor loads an image file and creates an SDL surface from it.
         * If the image loading or surface creation fails, it throws a throw
         * with an error message indicating the SDL or SDL_image error.
         *
         * @param filePath The path to the image file to load.
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         */
        Surface(const std::string& filePath, bool autoLifetimeManagement = true)
        : autoLifetimeManagement(autoLifetimeManagement)
        {
            Load(filePath);
        }

        /**
         * @brief Constructs a Surface from image data stored in a vector of Uint8.
         *
         * This constructor loads an image from the provided vector of Uint8 data
         * and creates a Surface object. It attempts to automatically determine the image format.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data The vector containing the image data.
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Surface(const std::vector<Uint8>& data, bool autoLifetimeManagement = true)
        : autoLifetimeManagement(autoLifetimeManagement)
        {
            LoadFromMemory(data);
        }

        /**
         * @brief Constructs a Surface from image data stored in a vector of Uint8 with a specified format.
         *
         * This constructor loads an image from the provided vector of Uint8 data
         * and creates a Surface object. It allows specifying the image format to use.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data The vector containing the image data.
         * @param format The format of the image data.
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Surface(const std::vector<Uint8>& data, core::ImageFormat format, bool autoLifetimeManagement = true)
        : autoLifetimeManagement(autoLifetimeManagement)
        {
            LoadFromMemory(data, format);
        }

        /**
         * @brief Constructs a Surface from image data stored in a block of memory.
         *
         * This constructor loads an image from the provided block of memory specified
         * by a const void pointer and its size. It attempts to automatically determine the image format.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data A pointer to the start of the memory block containing the image data.
         * @param size The size of the memory block.
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Surface(const void* data, size_t size, bool autoLifetimeManagement = true)
        : autoLifetimeManagement(autoLifetimeManagement)
        {
            LoadFromMemory(data, size);
        }

        /**
         * @brief Constructs a Surface from image data stored in a block of memory with a specified format.
         *
         * This constructor loads an image from the provided block of memory specified
         * by a const void pointer and its size. It also allows specifying the image format to use.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data A pointer to the start of the memory block containing the image data.
         * @param size The size of the memory block.
         * @param format The format of the image data.
         * @param autoLifetimeManagement Automatic surface deallocation in the destructor.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Surface(const void* data, size_t size, core::ImageFormat format, bool autoLifetimeManagement = true)
        : autoLifetimeManagement(autoLifetimeManagement)
        {
            LoadFromMemory(data, size, format);
        }

        /**
         * @brief Destructor to clean up the Surface object.
         */
        ~Surface()
        {
            if (autoLifetimeManagement)
            {
                Destroy();
            }
        }

        /**
         * @brief Copy constructor that creates a new Surface by cloning another Surface.
         *
         * This constructor creates a new Surface with the same dimensions, pixel format, and pixel data
         * as the specified Surface by using the Clone method.
         *
         * @param other The Surface to copy from.
         */
        Surface(const Surface& other)
        {
            surface = other.Clone();
        }

        /**
         * @brief Copy assignment operator that assigns the content of another Surface to this Surface.
         *
         * This operator checks for self-assignment and then clones the content of the specified Surface
         * using the Clone method to avoid shared ownership of pixel data.
         *
         * @param other The Surface to copy from.
         * @return A reference to the modified Surface.
         */
        Surface& operator=(const Surface& other)
        {
            if (this != &other)
            {
                surface = other.Clone();
            }
            return *this;
        }

        /**
         * @brief Move constructor to transfer ownership of the Surface's data.
         * 
         * @param other The Surface to move from.
         */
        Surface(Surface&& other)
        : surface(std::exchange(other.surface, nullptr))
        , autoLifetimeManagement(std::exchange(other.autoLifetimeManagement, false))
        { }

        /**
         * @brief Move assignment operator to transfer ownership of the Surface's data.
         * 
         * @param other The Surface to move from.
         * @return A reference to the modified Surface.
         */
        Surface& operator=(Surface&& other)
        {
            if (this != &other)
            {
                surface = std::exchange(other.surface, nullptr);
                autoLifetimeManagement = std::exchange(other.autoLifetimeManagement, false);
            }
            return *this;
        }

        /**
        * @brief Implicit conversion operator to retrieve a non-const pointer to the SDL_Surface.
        *
        * This operator allows implicit conversion to a non-const pointer to the SDL_Surface.
        * It can be used to access the underlying SDL_Surface data, but use it with caution to prevent unintended modifications.
        *
        * @return A non-const pointer to the SDL_Surface.
        */
        operator SDL_Surface*() const { return surface; }

        /**
         * @brief Create an SDL surface.
         *
         * This function creates an SDL surface with the specified width, height,
         * and pixel format. If the surface creation fails, it throws a throw
         * with an error message indicating the SDL error.
         *
         * @param width The width of the surface.
         * @param height The height of the surface.
         * @param format The pixel format of the surface.
         */
        void Create(int width, int height, PixelFormat format = PixelFormat::RGBA8888);

        /**
         * @brief Load an image from a file and create an SDL surface.
         *
         * This function loads an image file and creates an SDL surface from it.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param filePath The path to the image file to load.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        void Load(const std::string& filePath);

        /**
         * @brief Load an image from memory and create an SDL surface.
         *
         * This function attempts to automatically determine the image format.
         * It loads an image from a vector of Uint8 data and creates an SDL surface from it.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data The vector containing the image data.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        void LoadFromMemory(const std::vector<Uint8>& data);

        /**
         * @brief Load an image from memory and create an SDL surface with a specified format.
         *
         * This function loads an image from a vector of Uint8 data and creates an SDL surface from it.
         * It also allows specifying the image format to use. If the image loading or surface creation fails,
         * it throws an exception with an error message indicating the SDL error.
         *
         * @param data The vector containing the image data.
         * @param format The format of the image data.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        void LoadFromMemory(const std::vector<Uint8>& data, core::ImageFormat format);

        /**
         * @brief Load an image from memory and create an SDL surface.
         *
         * This function loads an image from a block of memory specified by a const void pointer
         * and its size. It attempts to automatically determine the image format.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data A pointer to the start of the memory block containing the image data.
         * @param size The size of the memory block.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        void LoadFromMemory(const void* data, size_t size);

        /**
         * @brief Load an image from memory and create an SDL surface with a specified format.
         *
         * This function loads an image from a block of memory specified by a const void pointer
         * and its size. It also allows specifying the image format to use.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param data A pointer to the start of the memory block containing the image data.
         * @param size The size of the memory block.
         * @param format The format of the image data.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        void LoadFromMemory(const void* data, size_t size, core::ImageFormat format);

        /**
         * @brief Destroy the SDL surface.
         *
         * This function destroys the SDL surface if it exists and sets the surface
         * pointer to nullptr.
         */
        void Destroy()
        {
            if (surface)
            {
                SDL_FreeSurface(surface);
                surface = nullptr;
            }
        }

        /**
         * @brief Get the SDL_Surface pointer.
         *
         * @return A pointer to the SDL surface.
         */
        SDL_Surface* Get() const
        {
            return surface;
        }

        /**
         * @brief Get the width of the surface.
         *
         * @return The width of the surface.
         */
        int GetWidth() const
        {
            return surface ? surface->w : 0;
        }

        /**
         * @brief Get the height of the surface.
         *
         * @return The height of the surface.
         */
        int GetHeight() const
        {
            return surface ? surface->h : 0;
        }

        /**
         * @brief Get the width of the surface as a floating-point value.
         *
         * @return The width of the surface as a floating-point value.
         */
        float GetWidthF() const
        {
            return static_cast<float>(surface ? surface->w : 0);
        }

        /**
         * @brief Get the height of the surface as a floating-point value.
         *
         * @return The height of the surface as a floating-point value.
         */
        float GetHeightF() const
        {
            return static_cast<float>(surface ? surface->h : 0);
        }

        /**
         * @brief Get the size of the surface.
         *
         * This function returns a 2D vector representing the size of the surface,
         * with the x-coordinate being the width and the y-coordinate being the height.
         *
         * @return A 2D vector representing the size of the surface.
         */
        math::IVec2 GetSize() const
        {
            return surface ? math::IVec2(surface->w, surface->h) : math::IVec2(0, 0);
        }

        /**
         * @brief Gets the size of the surface as a Rectangle with (0, 0) as the origin.
         *
         * @return The size of the texture as a shape2D::Rectangle.
         */
        shape2D::Rectangle GetRectSize() const
        {
            return { 0, 0, surface->w, surface->h };
        }

        /**
         * @brief Get the pixel format of the surface as a Uint32 value.
         *
         * This function retrieves the pixel format of the surface as a Uint32 value.
         * If the surface is valid, it returns the format; otherwise, it returns 0.
         *
         * @return The pixel format of the surface as a Uint32 value, or 0 if the surface is invalid.
         */
        Uint32 GetPixelFormatUint32() const
        {
            return surface ? surface->format->format : 0;
        }

        /**
         * @brief Get the pixel format of the surface.
         *
         * This function retrieves the pixel format of the surface and returns it as
         * a PixelFormat enumeration value. If the surface is valid, it returns the
         * corresponding enumeration value; otherwise, it returns PixelFormat::UNKNOWN.
         *
         * @return The pixel format of the surface as a PixelFormat enumeration value, or PixelFormat::UNKNOWN if the surface is invalid.
         */
        PixelFormat GetPixelFormat() const
        {
            return static_cast<PixelFormat>(surface ? surface->format->format : 0);
        }

        /**
         * @brief Get the SDL pixel format structure of the surface.
         *
         * This function retrieves the SDL pixel format structure associated with the surface.
         * If the surface is valid, it returns a pointer to the format structure; otherwise, it returns nullptr.
         *
         * @return A pointer to the SDL pixel format structure of the surface, or nullptr if the surface is invalid.
         */
        const SDL_PixelFormat* GetFormat() const
        {
            return surface ? surface->format : nullptr;
        }

        /**
         * @brief Get the color of the pixel at the specified memory address without performing bounds checks.
         *
         * @param pixelAddress The memory address of the pixel.
         * @return The color of the pixel at the specified address.
         */
        Color GetPixelUnsafe(const void* pixelAddress) const;

        /**
         * @brief Get the color of the pixel at the specified byte offset without performing bounds checks.
         *
         * @param byteOffset The byte offset of the pixel within the surface data.
         * @return The color of the pixel at the specified byte offset.
         */
        Color GetPixelUnsafe(int byteOffset) const;

        /**
         * @brief Get the color of the pixel at the specified coordinates without performing bounds checks.
         *
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @return The color of the pixel at the specified coordinates.
         */
        Color GetPixelUnsafe(int x, int y) const;

        /**
         * @brief Get the color of the pixel at the specified position without performing bounds checks.
         *
         * @param position The 2D position (x, y) of the pixel.
         * @return The color of the pixel at the specified position.
         */
        Color GetPixelUnsafe(const math::IVec2& position) const;

        /**
         * @brief Get the color of the pixel at the specified normalized texture coordinate without performing bounds checks.
         *
         * @param uv The normalized texture coordinate (Vec2) representing the position on the surface.
         * @return The color of the pixel at the specified normalized texture coordinate.
         */
        Color GetFragUnsafe(const math::Vec2& uv) const;

        /**
         * @brief Get the color of the pixel at the specified memory address, performing bounds checks.
         *
         * @param pixelAddress The memory address of the pixel.
         * @return The color of the pixel at the specified address, or Blank if the address is out of bounds.
         */
        Color GetPixel(const void* pixelAddress) const;

        /**
         * @brief Get the color of the pixel at the specified byte offset, performing bounds checks.
         *
         * @param byteOffset The byte offset of the pixel within the surface data.
         * @return The color of the pixel at the specified byte offset, or Blank if the offset is out of bounds.
         */
        Color GetPixel(int byteOffset) const;

        /**
         * @brief Get the color of the pixel at the specified coordinates, performing bounds checks.
         *
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @return The color of the pixel at the specified coordinates, or Blank if the coordinates are out of bounds.
         */
        Color GetPixel(int x, int y) const;

        /**
         * @brief Get the color of the pixel at the specified position, performing bounds checks.
         *
         * @param position The 2D position (x, y) of the pixel.
         * @return The color of the pixel at the specified position, or Blank if the position is out of bounds.
         */
        Color GetPixel(const math::IVec2& position) const;

        /**
         * @brief Get the color of the pixel at the specified normalized UV coordinates, performing bounds checks.
         *
         * @param uv The 2D vector representing the normalized UV coordinates (in the range [0, 1]).
         * @return The color of the pixel at the specified normalized UV coordinates, or Blank if the coordinates are out of bounds.
         */
        Color GetFrag(const math::Vec2& uv) const;

        /**
         * @brief Get a pointer to the pixel data.
         *
         * This function returns a pointer to the raw pixel data of the surface. Use this
         * pointer to directly access or manipulate the pixel data.
         *
         * @return A pointer to the pixel data of the surface.
         */
        void* GetPixels() const
        {
            return surface ? surface->pixels : nullptr;
        }

        /**
         * @brief Get the pitch (or stride) of the pixel data.
         *
         * The pitch is the number of bytes in a row of pixel data, including padding.
         * It represents the distance, in bytes, between the starting points of consecutive lines in the surface.
         * This information is useful for correctly iterating through the pixel data.
         *
         * @return The pitch of the pixel data of the surface.
         */
        int GetPitch() const
        {
            return surface ? surface->pitch : 0;
        }

        /**
         * @brief Get the number of bytes used to represent a pixel on the surface.
         *
         * Each pixel on the surface is represented by a certain number of bytes, determined by the surface's pixel format.
         * This function returns the BytesPerPixel value, which indicates the size of a pixel in bytes.
         *
         * @return The number of bytes used to represent a pixel on the surface.
         */
        int GetBytesPerPixel() const
        {
            return surface ? surface->format->BytesPerPixel : 0;
        }

        /**
         * @brief Check if the surface requires locking before making modifications.
         *
         * This function returns true if the surface must be locked before making modifications to its pixel data.
         * It is typically used to determine whether a surface manipulation function requires a lock operation.
         *
         * @return True if the surface must be locked before modifications, false otherwise.
         */
        virtual bool MustLock() const
        {
            return SDL_MUSTLOCK(surface);
        }

        /**
         * @brief Check if the surface is currently locked.
         *
         * This function returns true if the surface is locked, meaning that its pixel data is currently being manipulated,
         * and false otherwise.
         *
         * @return True if the surface is locked, false otherwise.
         */
        virtual bool IsLocked() const
        {
            return surface ? surface->locked : false;
        }

        /**
         * @brief Check if the surface is valid.
         *
         * This function returns true if the surface is not nullptr, indicating that it is a valid surface object,
         * and false otherwise.
         *
         * @return True if the surface is valid, false otherwise.
         */
        virtual bool IsValid() const
        {
            return surface != nullptr;
        }

        /**
         * @brief Check if the lifetime of the SDL_Surface is managed automatically.
         *
         * This function returns true if the SDL_Surface contained within the class will be automatically deallocated
         * in the destructor, and false if it is the responsibility of the user to manage the lifetime of the original SDL_Surface.
         *
         * @return True if the lifetime of the SDL_Surface is automatically managed, false otherwise.
         */
        bool IsLifetimeAutomaticallyManaged() const
        {
            return autoLifetimeManagement;
        }

        /**
         * @brief Set whether the lifetime of the SDL_Surface should be managed automatically.
         *
         * This function allows the user to specify whether the SDL_Surface contained within the class
         * should be automatically deallocated in the destructor or if it is the responsibility of the user
         * to manage the lifetime of the original SDL_Surface.
         *
         * @param autoManagement True to enable automatic lifetime management, false otherwise.
         */
        void SetAutoLifetimeManagement(bool autoManagement)
        {
            autoLifetimeManagement = autoManagement;
        }

        /**
         * @brief Get the blend mode of the surface.
         *
         * This function retrieves the blend mode currently set for the surface and returns it.
         * If an error occurs during the retrieval, a NexusException is thrown with relevant details.
         *
         * @return The blend mode of the surface.
         * @throws core::NexusException if an error occurs during retrieval.
         */
        BlendMode GetBlendMode() const;

        /**
         * @brief Set the blend mode of the surface.
         *
         * This function sets the blend mode for the surface based on the provided BlendMode enum.
         * If an error occurs during the set operation, a NexusException is thrown with relevant details.
         *
         * @param blendMode The blend mode to set for the surface.
         * @throws core::NexusException if an error occurs during the set operation.
         */
        void SetBlendMode(BlendMode blendMode) const;

        /**
         * @brief Copy the contents of another Surface to this Surface.
         *
         * This function copies the attributes and pixel data from the source Surface to this Surface.
         *
         * @param source The source Surface to copy from.
         */
        void CopyFrom(const Surface& source);

        /**
         * @brief Create a clone of this Surface.
         *
         * This function creates a new Surface with the same dimensions and pixel format as the current Surface
         * and copies the pixel data from the current Surface to the new one.
         *
         * @param format The pixel format for the cloned Surface. If not specified (default is PixelFormat::Unknown),
         *              the cloned Surface will have the same format as the original one.
         *
         * @return A cloned Surface.
         */
        Surface Clone(PixelFormat format = PixelFormat::Unknown) const;

        /**
         * @brief Creates a new Surface by cropping a specified area from the current Surface.
         *
         * This function creates a new Surface with dimensions defined by the specified cropping area
         * and copies the pixel data from the cropped area of the current Surface to the new one.
         *
         * @param areaToKeep The rectangular area to keep in the new Surface.
         * @return A new Surface containing the cropped area.
         */
        Surface Crop(shape2D::Rectangle areaToKeep) const;

        /**
         * @brief Resize the Surface to a new width and height.
         *
         * This function resizes the Surface to the specified width and height, preserving the existing pixel data.
         *
         * @param newWidth The new width of the Surface.
         * @param newHeight The new height of the Surface.
         *
         * @throws core::NexusException if the Surface is not created yet, and therefore cannot be resized.
         */
        void Resize(int newWidth, int newHeight);

        /**
         * @brief Resize the Surface's canvas to a new width and height with optional offset and background color.
         *
         * This function resizes the Surface to the specified width and height, preserving the existing pixel data.
         * Additionally, it allows specifying an offset and background color to fill the extended canvas area.
         *
         * @param newWidth The new width of the Surface.
         * @param newHeight The new height of the Surface.
         * @param offsetX The horizontal offset for the existing pixel data.
         * @param offsetY The vertical offset for the existing pixel data.
         * @param background The color to fill the extended canvas area. Default is gfx::Blank.
         *
         * @throws core::NexusException if the Surface is not created yet, and therefore cannot be resized.
         */
        void ResizeCanvas(int newWidth, int newHeight, int offsetX, int offsetY, const Color& background = Blank);

        /**
         * @brief Resize the Surface's canvas to the next power of two dimensions.
         *
         * This function resizes the Surface's canvas to the next power of two width and height,
         * preserving the existing pixel data. Optionally, a background color can be specified
         * to fill the extended canvas area.
         *
         * @param background The color to fill the extended canvas area. Default is gfx::Blank.
         *
         * @throws core::NexusException if the Surface is not created yet, and therefore cannot be resized.
         */
        void ToPOT(const Color& background = Blank);

        /**
         * @brief Save the Surface as an image file.
         *
         * This function saves the current Surface as an image file (e.g., PNG) with the specified file path.
         *
         * @param filePath The path to the image file where the Surface will be saved.
         */
        void SaveImage(const std::string& filePath) const;

        /**
         * @brief Locks the Surface to allow direct pixel manipulation.
         *
         * Locking the surface prevents unwanted modifications during pixel manipulation operations.
         * Use Unlock() to release the lock after performing pixel operations.
         *
         * @throws core::NexusException If unable to lock the surface.
         */
        virtual void Lock()
        {
            if (SDL_LockSurface(surface) < 0)
            {
                throw core::NexusException("gfx::Surface", "Unable to lock surface."
                    "SDL", SDL_GetError());
            }
        }

        /**
         * @brief Unlocks the Surface after direct pixel manipulation.
         *
         * Call this function after completing pixel manipulation operations using Lock().
         * Unlocking the surface allows it to be used for rendering or other purposes.
         */
        virtual void Unlock()
        {
            SDL_UnlockSurface(surface);
        }

        /**
         * @brief Sets the pixel at the specified memory address on the surface without performing bounds checks.
         *
         * @param pixelAddress The memory address of the pixel.
         * @param color The color to set the pixel to (in RGBA format).
         */
        void SetPixelUnsafe(void* pixelAddress, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified byte offset on the surface without performing bounds checks.
         *
         * @param byteOffset The byte offset of the pixel within the surface data.
         * @param color The color to set the pixel to (in RGBA format).
         */
        void SetPixelUnsafe(int byteOffset, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified coordinates on the surface without performing bounds checks.
         *
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @param color The color to set the pixel to (in RGBA format).
         */
        void SetPixelUnsafe(int x, int y, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified position on the surface without performing bounds checks.
         *
         * @param position The 2D position (x, y) of the pixel.
         * @param color The color to set the pixel to (in RGBA format).
         */
        void SetPixelUnsafe(const math::IVec2& position, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified normalized texture coordinate on the surface without performing bounds checks.
         *
         * @param uv The normalized texture coordinate (Vec2) representing the position on the surface.
         * @param color The color to set the pixel to (in RGBA format).
         */
        void SetFragUnsafe(const math::Vec2 uv, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified memory address on the surface without performing bounds checks.
         *
         * @param pixelAddress The memory address of the pixel.
         * @param color The color to set the pixel to (in RGBA format).
         * @return True if the pixel was successfully set, false if it was not possible (outside surface bounds or null surface).
         */
        bool SetPixel(void* pixelAddress, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified byte offset on the surface without performing bounds checks.
         *
         * @param byteOffset The byte offset of the pixel within the surface data.
         * @param color The color to set the pixel to (in RGBA format).
         * @return True if the pixel was successfully set, false if it was not possible (outside surface bounds or null surface).
         */
        bool SetPixel(int byteOffset, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified coordinates on the surface, performing bounds checks if the surface is non-null.
         *
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @param color The color to set the pixel to (in RGBA format).
         * @return True if the pixel was successfully set, false if it was not possible (outside surface bounds or null surface).
         */
        bool SetPixel(int x, int y, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified position on the surface, performing bounds checks if the surface is non-null.
         *
         * @param position The 2D position (x, y) of the pixel.
         * @param color The color to set the pixel to (in RGBA format).
         * @return True if the pixel was successfully set, false if it was not possible (outside surface bounds or null surface).
         */
        bool SetPixel(const math::IVec2& position, const Color& color) const;

        /**
         * @brief Sets the pixel at the specified normalized UV coordinates on the surface, performing bounds checks if the surface is non-null.
         *
         * @param uv The 2D vector representing the normalized UV coordinates (in the range [0, 1]).
         * @param color The color to set the pixel to (in RGBA format).
         * @return True if the pixel was successfully set, false if it was not possible (outside surface bounds or null surface).
         */
        bool SetFrag(const math::Vec2& uv, const Color& color) const;

        /**
         * @brief Rotate the Surface by the specified angle in degrees.
         *
         * This function rotates the Surface by the given angle in degrees. The result is stored in the same Surface.
         *
         * @param angle The angle of rotation in degrees.
         */
        void Rotate(float angle);

        /**
         * @brief Rotate the Surface 90 degrees counterclockwise.
         *
         * This function rotates the Surface 90 degrees counterclockwise. The result is stored in the same Surface.
         */
        void RotateCCW();

        /**
         * @brief Flip the Surface horizontally.
         *
         * This function flips the Surface horizontally. The result is stored in the same Surface.
         */
        void FlipHorizontal();

        /**
         * @brief Flip the Surface vertically.
         *
         * This function flips the Surface vertically. The result is stored in the same Surface.
         */
        void FlipVertical();

        /**
         * @brief Fill the entire surface with the specified color.
         *
         * @param color The color to fill the surface with.
         */
        void Fill(const Color& color) const;

        /**
         * @brief Draw a linear gradient on the surface within the specified rectangle.
         *
         * @param dst The rectangle defining the area to fill with the gradient.
         * @param direction The direction of the gradient (in radians).
         * @param start The starting color of the gradient.
         * @param end The ending color of the gradient.
         */
        void DrawGradientLinear(shape2D::Rectangle dst, float direction, const Color& start, const Color& end) const;

        /**
         * @brief Draw a radial gradient on the surface within the specified rectangle.
         *
         * @param dst The rectangle defining the area to fill with the gradient.
         * @param density The density of the gradient.
         * @param inner The color at the center of the gradient.
         * @param outer The color at the outer edge of the gradient.
         */
        void DrawGradientRadial(shape2D::Rectangle dst, float density, const Color& inner, const Color& outer) const;

        /**
         * @brief Draw a square gradient on the surface within the specified rectangle.
         *
         * @param dst The rectangle defining the area to fill with the gradient.
         * @param density The density of the gradient.
         * @param inner The color at the center of the gradient.
         * @param outer The color at the outer edge of the gradient.
         */
        void DrawGradientSquare(shape2D::Rectangle dst, float density, const Color& inner, const Color& outer) const;

        /**
         * @brief Draw a checkered pattern on the surface within the specified rectangle.
         *
         * @param dst The rectangle defining the area to fill with the pattern.
         * @param checksX The number of horizontal checks.
         * @param checksY The number of vertical checks.
         * @param col1 The color of the first check.
         * @param col2 The color of the second check.
         */
        void DrawChecked(shape2D::Rectangle dst, int checksX, int checksY, const Color& col1, const Color& col2) const;

        /**
         * @brief Draw white noise on the surface within the specified rectangle.
         *
         * @param dst The rectangle defining the area to fill with the noise.
         * @param factor The factor determining the density of the noise.
         */
        void DrawWhiteNoise(shape2D::Rectangle dst, float factor) const;

        /**
         * @brief Draw a cellular pattern on the surface within the specified rectangle.
         *
         * @param dst The rectangle defining the area to fill with the pattern.
         * @param tileSize The size of the individual cells in the pattern.
         */
        void DrawCellular(shape2D::Rectangle dst, int tileSize) const;

        /**
         * @brief Draw a line on the surface between two points with the specified color.
         *
         * @param x1 The x-coordinate of the starting point.
         * @param y1 The y-coordinate of the starting point.
         * @param x2 The x-coordinate of the ending point.
         * @param y2 The y-coordinate of the ending point.
         * @param color The color of the line (default is White).
         */
        void DrawLine(int x1, int y1, int x2, int y2, const Color& color = White) const;

        /**
         * @brief Draw a line on the surface between two points with the specified color.
         *
         * @param start The starting point of the line.
         * @param end The ending point of the line.
         * @param color The color of the line (default is White).
         */
        void DrawLine(const math::Vec2& start, const math::Vec2& end, const Color& color = White) const;

        /**
         * @brief Draw a line on the surface with the specified color.
         *
         * @param line The 2D line to draw.
         * @param color The color of the line (default is White).
         */
        void DrawLine(const shape2D::Line& line, const Color& color = White) const;

        /**
         * @brief Draw a filled rectangle on the surface between two points with the specified color.
         *
         * @param x The x-coordinate of the top-left corner.
         * @param y The y-coordinate of the top-left corner.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         * @param color The color of the rectangle (default is White).
         */
        void DrawRectangle(int x, int y, int w, int h, const Color& color = White) const;

        /**
         * @brief Draw a filled rectangle on the surface with the specified color.
         *
         * @param rect The rectangle to draw.
         * @param color The color of the rectangle (default is White).
         */
        void DrawRectangle(const shape2D::Rectangle& rect, const Color& color = White) const;

        /**
         * @brief Draw a filled rectangle on the surface with the specified color.
         *        This version takes a RectangleF for floating-point precision.
         *
         * @param rect The rectangle to draw.
         * @param color The color of the rectangle (default is White).
         */
        void DrawRectangle(const shape2D::RectangleF& rect, const Color& color = White) const;

        /**
         * @brief Draw the lines of a rectangle on the surface with the specified color.
         *
         * @param x The x-coordinate of the top-left corner.
         * @param y The y-coordinate of the top-left corner.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         * @param thick The thickness of the lines (default is 1).
         * @param color The color of the lines (default is White).
         */
        void DrawRectangleLines(int x, int y, int w, int h, int thick = 1, const Color& color = White) const;

        /**
         * @brief Draw the lines of a rectangle on the surface with the specified color.
         *
         * @param rect The rectangle to draw the lines of.
         * @param thick The thickness of the lines (default is 1).
         * @param color The color of the lines (default is White).
         */
        void DrawRectangleLines(const shape2D::Rectangle& rect, int thick = 1, const Color& color = White) const;

        /**
         * @brief Draw the lines of a rectangle on the surface with the specified color.
         *        This version takes a RectangleF for floating-point precision.
         *
         * @param rect The rectangle to draw the lines of.
         * @param thick The thickness of the lines (default is 1).
         * @param color The color of the lines (default is White).
         */
        void DrawRectangleLines(const shape2D::RectangleF& rect, int thick = 1, const Color& color = White) const;

        /**
         * @brief Draw the lines of an Axis-Aligned Bounding Box (AABB) on the surface with the specified color.
         *        An AABB represents a rectangular box that is aligned with the axes of the coordinate system.
         *
         * @param aabb The AABB to draw the lines of.
         * @param color The color of the lines (default is White).
         */
        void DrawAABB(const shape2D::AABB& aabb, const Color& color = White) const;

        /**
         * @brief Draw a filled circle on the surface with the specified color.
         *
         * @param cx The x-coordinate of the center of the circle.
         * @param cy The y-coordinate of the center of the circle.
         * @param radius The radius of the circle.
         * @param color The color of the circle (default is White).
         */
        void DrawCircle(int cx, int cy, int radius, const Color& color = White) const;

        /**
         * @brief Draw a filled circle on the surface with the specified color.
         *
         * @param circle The 2D circle to draw.
         * @param color The color of the circle (default is White).
         */
        void DrawCircle(const shape2D::Circle& circle, const Color& color = White) const;

        /**
         * @brief Draw the lines of a circle on the surface with the specified color.
         *
         * @param cx The x-coordinate of the center of the circle.
         * @param cy The y-coordinate of the center of the circle.
         * @param radius The radius of the circle.
         * @param color The color of the lines (default is White).
         */
        void DrawCircleLines(int cx, int cy, int radius, const Color& color = White) const;

        /**
         * @brief Draw the lines of a circle on the surface with the specified color.
         *
         * @param circle The 2D circle to draw the lines of.
         * @param color The color of the lines (default is White).
         */
        void DrawCircleLines(const shape2D::Circle& circle, const Color& color = White) const;

        /**
         * @brief Draws a filled polygon on the surface.
         * 
         * @note: This function can also handle concave polygons but may be slow.
         *
         * @param poly The 2D polygon to be drawn.
         * @param color The fill color of the polygon (default is White).
         */
        void DrawPolygon(const shape2D::Polygon& poly, const Color& color = White) const;

        /**
         * @brief Draw the lines of a polygon on the surface with the specified color.
         *
         * @param poly The 2D polygon to draw the lines of.
         * @param color The color of the lines (default is White).
         */
        void DrawPolygonLines(const shape2D::Polygon& poly, const Color& color = White) const;

        /**
         * @brief Draws a filled triangle on the surface.
         *
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param color The fill color of the triangle (default is White).
         */
        void DrawTriangle(const math::Vec2& v0, const math::Vec2& v1, const math::Vec2& v2, const Color& color = White) const;

        /**
         * @brief Draws a filled triangle on the surface.
         *
         * @param tri The triangle to be drawn.
         * @param color The fill color of the triangle (default is White).
         */
        void DrawTriangle(const shape2D::Triangle& tri, const Color& color = White) const;

        /**
         * @brief Draws the lines of a triangle on the surface.
         *
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param color The color of the lines (default is White).
         */
        void DrawTriangleLines(const math::Vec2& v0, const math::Vec2& v1, const math::Vec2& v2, const Color& color = White) const;

        /**
         * @brief Draws the lines of a triangle on the surface.
         *
         * @param tri The triangle to draw the lines of.
         * @param color The color of the lines (default is White).
         */
        void DrawTriangleLines(const shape2D::Triangle& tri, const Color& color = White) const;

        /**
         * @brief Draws a filled triangle on the surface using vertices with position and color.
         *
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         */
        void DrawTriangleColors(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2) const;

        /**
         * @brief Draws a filled triangle on the surface using vertices with position, color, and texture coordinates.
         *
         * @param v0 The first vertex of the triangle.
         * @param v1 The second vertex of the triangle.
         * @param v2 The third vertex of the triangle.
         * @param surface Reference of another surface to render in the triangle.
         */
        void DrawTriangleImage(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const Surface& image) const;

        /**
         * @brief Draws a filled quadrilateral on the surface using vertices with position, and color.
         *
         * @param v0 The first vertex of the quadrilateral.
         * @param v1 The second vertex of the quadrilateral.
         * @param v2 The third vertex of the quadrilateral.
         * @param v3 The fourth vertex of the quadrilateral.
         * @param surface (Optional) Pointer to the surface on which to draw the quadrilateral (default is nullptr).
         */
        void DrawQuadColors(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const shape2D::Vertex& v3) const;

        /**
         * @brief Draws a filled quadrilateral on the surface using vertices with position, color, and texture coordinates.
         *
         * @param v0 The first vertex of the quadrilateral.
         * @param v1 The second vertex of the quadrilateral.
         * @param v2 The third vertex of the quadrilateral.
         * @param v3 The fourth vertex of the quadrilateral.
         * @param surface Reference of another surface to render in the quad.
         */
        void DrawQuadImage(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const shape2D::Vertex& v3, const Surface& image) const;

        /**
         * @brief Draws a mesh composed of triangles on the surface using vertices with position, color, and texture coordinates.
         *
         * @param mesh The mesh containing the vertices and triangles to draw.
         * @param surface (Optional) Pointer to the surface on which to draw the mesh (default is nullptr).
         */
        void DrawMesh(const shape2D::Mesh& mesh, const Surface* image = nullptr) const;

        /**
         * @brief Draws an image from another surface onto this surface at the specified position.
         * @param other The surface containing the image to draw.
         * @param x The x-coordinate of the top-left corner of the destination rectangle.
         * @param y The y-coordinate of the top-left corner of the destination rectangle.
         * @param ox The x-coordinate of the origin of the image within the source surface (default is 0).
         * @param oy The y-coordinate of the origin of the image within the source surface (default is 0).
         * @throw core::NexusException if an error occurs while drawing the image.
         */
        virtual void DrawImage(const Surface& other, int x, int y, int ox = 0, int oy = 0);

        /**
         * @brief Draws an image from another surface onto this surface at the specified position.
         * @param other The surface containing the image to draw.
         * @param position The position of the top-left corner of the destination rectangle.
         * @param origin The origin of the image within the source surface (default is {0, 0}).
         * @throw core::NexusException if an error occurs while drawing the image.
         */
        virtual void DrawImage(const Surface& other, const math::IVec2& position, const math::IVec2& origin = { 0, 0 });

        /**
         * @brief Draws a scaled image from another surface onto this surface at the specified position.
         * @param other The surface containing the image to draw.
         * @param x The x-coordinate of the top-left corner of the destination rectangle.
         * @param y The y-coordinate of the top-left corner of the destination rectangle.
         * @param sx The horizontal scale factor.
         * @param sy The vertical scale factor.
         * @param ox The x-coordinate of the origin of the image within the source surface (default is 0).
         * @param oy The y-coordinate of the origin of the image within the source surface (default is 0).
         * @throw core::NexusException if an error occurs while drawing the scaled image.
        */
        virtual void DrawImageScaled(const Surface& other, float x, float y, float sx, float sy, float ox = 0, float oy = 0);

        /**
         * @brief Draws a scaled image from another surface onto this surface at the specified position.
         * @param other The surface containing the image to draw.
         * @param position The position of the top-left corner of the destination rectangle.
         * @param scale The scale factors in the x and y directions.
         * @param origin The origin of the image within the source surface (default is {0, 0}).
         * @throw core::NexusException if an error occurs while drawing the scaled image.
         */
        virtual void DrawImageScaled(const Surface& other, const math::Vec2& position, const math::Vec2& scale, const math::Vec2& origin = { 0, 0 });

        /**
         * @brief Draws a image from another surface onto this surface using specified source and destination rectangles (scaled if needed).
         * @param other The surface containing the image to draw.
         * @param rectSrc The source rectangle specifying the portion of the source surface to draw.
         * @param rectDst The destination rectangle specifying where to draw the image on this surface.
         * @throw core::NexusException if an error occurs while drawing the scaled image.
         */
        virtual void DrawImage(const Surface& other, const shape2D::Rectangle& rectSrc, shape2D::Rectangle rectDst);
    };

}}

#endif //NEXUS_GFX_SURFACE_HPP
