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

#ifndef NEXUS_EXT_2D_EXT_3D_IMPL_SPRITE_HPP
#define NEXUS_EXT_2D_EXT_3D_IMPL_SPRITE_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../nxSurface.hpp"
#include "../nxColor.hpp"
#include <unordered_map>
#include <memory>

namespace _gfx_impl {

    /**
     * @brief Class for managing 2D sprite animations.
     * 
     * This template class provides functionality for handling 2D sprite animations
     * with the ability to render using various types of textures, such as SDL texture,
     * OpenGL texture, etc.
     * 
     * @tparam T_Texture The type representing the texture used by the sprite.
     */
    template <typename T_Texture>
    class Sprite
    {
      public:
        /**
         * @brief Struct representing an animation within the sprite.
         * 
         * This structure holds information about an animation, including its speed,
         * the number of frames, the starting and ending frame indices, and whether
         * the animation should loop.
         */
        struct Animation
        {
            float speed;    ///< The animation speed.
            Uint16 count;   ///< The number of frames in the animation.
            Uint16 start;   ///< The starting frame index.
            Uint16 end;     ///< The ending frame index.
            bool loop;      ///< Whether the animation should loop.

            /**
             * @brief Constructor for the Animation struct.
             * 
             * @param s The animation speed.
             * @param c The number of frames in the animation.
             * @param st The starting frame index.
             * @param e The ending frame index.
             * @param l Whether the animation should loop.
             */
            Animation(float s, Uint16 c, Uint16 st, Uint16 e, bool l)
                : speed(s), count(c), start(st), end(e), loop(l)
            { }
        };

        /**
         * @brief Struct representing an instance of a sprite animation.
         * 
         * This structure encapsulates the state of an instance of a sprite animation,
         * including the current frame rectangle, associated animation, current animation
         * time, and the index of the current frame.
         */
        struct Instance
        {
            nexus::shape2D::Rectangle frameRec;    ///< The current frame rectangle.
            Animation* animation;               ///< Pointer to the associated animation.
            float animTime;                     ///< The current animation time.
            Uint16 currentFrame;                ///< The current frame index.

            /**
             * @brief Constructor for the Instance struct.
             * 
             * @param fr The current frame rectangle.
             * @param a Pointer to the associated animation.
             * @param at The current animation time.
             * @param cf The current frame index.
             */
            Instance(const nexus::shape2D::Rectangle&& fr, Animation* a, float at, Uint16 cf)
                : frameRec(fr), animation(a), animTime(at), currentFrame(cf)
            { }
        };

        using MapAnimations = std::unordered_map<std::string, std::unique_ptr<Animation>>; ///< Type alias for the map of animations.
        using MapInstances = std::unordered_map<std::string, std::unique_ptr<Instance>>;   ///< Type alias for the map of animation instances.

      protected:
        MapAnimations animations;           ///< Map of animation names to Animation objects.
        MapInstances instances;             ///< Map of animation instance names to Instance objects.

        T_Texture texture;                  ///< Pointer to the texture used by the sprite.

        nexus::math::IVec2 frameSize;          ///< The size of each frame in the sprite sheet.
        nexus::math::IVec2 frameCenter;        ///< The center of each frame in the sprite sheet.
        nexus::shape2D::Rectangle texSource;   ///< The source rectangle for the sprite sheet.

        Uint8 cols, rows;                   ///< The number of columns and rows in the sprite sheet.
        Uint16 frameNum;                    ///< The total number of frames in the sprite sheet.

      public:
        /**
         * @brief Constructor for the Sprite class with image path, columns, rows, and animation speed.
         * 
         * This constructor initializes a Sprite instance using an image file, specifying
         * the number of columns and rows in the sprite sheet, and setting the animation speed.
         * 
         * @tparam T_Context The type representing the rendering context (e.g., SDL, OpenGL).
         * @param ctx The rendering context.
         * @param imPath The path to the image file.
         * @param rows The number of rows in the sprite sheet.
         * @param cols The number of columns in the sprite sheet.
         * @param speed The animation speed.
         */
        template <typename T_Context>
        Sprite(T_Context& ctx, const std::string& imPath, int rows, int cols, float speed = 0.1f);

        /**
         * @brief Constructor for the Sprite class with image path, columns, rows, texture source, and animation speed.
         * 
         * This constructor initializes a Sprite instance using an image file, specifying
         * the number of columns and rows in the sprite sheet, a custom texture source rectangle,
         * and setting the animation speed.
         * 
         * @tparam T_Context The type representing the rendering context (e.g., SDL, OpenGL).
         * @param ctx The rendering context.
         * @param imPath The path to the image file.
         * @param rows The number of rows in the sprite sheet.
         * @param cols The number of columns in the sprite sheet.
         * @param texSource The source rectangle for the sprite sheet.
         * @param speed The animation speed.
         */
        template <typename T_Context>
        Sprite(T_Context& ctx, const std::string& imPath, int rows, int cols, const nexus::shape2D::Rectangle& texSource, float speed = 0.1f);

        /**
         * @brief Constructor for the Sprite class with texture, columns, rows, texture source, and animation speed.
         * 
         * This constructor initializes a Sprite instance using a pre-existing texture,
         * specifying the number of columns and rows in the sprite sheet, a custom texture
         * source rectangle, and setting the animation speed.
         * 
         * @param texture Reference to the texture used by the sprite.
         * @param rows The number of rows in the sprite sheet.
         * @param cols The number of columns in the sprite sheet.
         * @param texSource The source rectangle for the sprite sheet.
         * @param speed The animation speed.
         */
        Sprite(const T_Texture& texture, int rows, int cols, const nexus::shape2D::Rectangle&& texSource, float speed = 0.1f);

        /**
         * @brief Virtual destructor for the Sprite class.
         */
        virtual ~Sprite() = default;

        /**
         * @brief Deleted copy constructor to disable copying.
         * 
         * Copying of Sprite instances is disabled by deleting the copy constructor.
         */
        Sprite(const Sprite&) = delete;

        /**
         * @brief Deleted copy assignment operator to disable copying.
         * 
         * Copying of Sprite instances is disabled by deleting the copy assignment operator.
         */
        Sprite& operator=(const Sprite&) = delete;

        /**
         * @brief Move constructor for the Sprite class.
         * 
         * This move constructor transfers ownership of resources from another
         * instance of the Sprite class to the current instance using move semantics.
         * 
         * @param other Another instance of the Sprite class.
         */
        Sprite(Sprite&& other) noexcept;

        /**
         * @brief Move assignment operator for the Sprite class.
         * 
         * This move assignment operator transfers ownership of resources from another
         * instance of the Sprite class to the current instance using move semantics.
         * 
         * @param other Another instance of the Sprite class.
         * @return Reference to the current instance.
         */
        Sprite& operator=(Sprite&& other) noexcept;

        // ANIMATION MANAGEMENT //

        /**
         * @brief Create a new animation and store it in the animations map.
         * 
         * This function creates a new animation and associates it with a key in the animations map.
         * The animation is defined by specifying the starting and ending frame indices, animation speed,
         * and whether it should loop.
         * 
         * @param keyAnimation The key to identify the animation.
         * @param startFrame The starting frame index of the animation.
         * @param endFrame The ending frame index of the animation.
         * @param speed The animation speed.
         * @param loop Whether the animation should loop.
         * @return A pointer to the created animation.
         */
        Animation* NewAnimation(const std::string& keyAnimation, Uint16 startFrame, Uint16 endFrame, float speed, bool loop);

        /**
         * @brief Set the active animation for a given instance.
         * 
         * This function sets the active animation for a specified instance using the animation key.
         * If no keyInstance is provided, the default key "main" is used.
         * 
         * @param keyAnimation The key identifying the animation.
         * @param keyInstance The key identifying the instance (default is "main").
         */
        void SetAnimation(const std::string& keyAnimation, const std::string& keyInstance = "main");

        /**
         * @brief Get a pointer to the animation with the specified key.
         * 
         * This function retrieves a pointer to the animation associated with the provided key.
         * 
         * @param keyAnimation The key identifying the animation.
         * @return A pointer to the animation.
         */
        Animation* GetAnimation(const std::string& keyAnimation);

        /**
         * @brief Get a const pointer to the animation with the specified key.
         * 
         * This function retrieves a const pointer to the animation associated with the provided key.
         * 
         * @param keyAnimation The key identifying the animation.
         * @return A const pointer to the animation.
         */
        const Animation* GetAnimation(const std::string& keyAnimation) const;

        /**
         * @brief Get the source rectangle for a specific frame of an animation.
         * 
         * This function retrieves the source rectangle for a specific frame of the provided animation.
         * 
         * @param frameIndex The index of the frame.
         * @param animation A pointer to the animation.
         * @return The source rectangle for the frame.
         */
        nexus::shape2D::Rectangle GetAnimationFrameRec(Uint16 frameIndex, const Animation * const animation) const;

        /**
         * @brief Get the source rectangle for a specific frame of the active animation.
         * 
         * This function retrieves the source rectangle for a specific frame of the active animation.
         * If no keyAnimation is provided, the default key "main" is used to identify the animation.
         * 
         * @param frameIndex The index of the frame.
         * @param keyAnimation The key identifying the animation (default is "main").
         * @return The source rectangle for the frame.
         */
        nexus::shape2D::Rectangle GetAnimationFrameRec(Uint16 frameIndex, const std::string& keyAnimation = "main") const;

        /**
         * @brief Set the speed of the active animation.
         * 
         * This function sets the speed of the active animation identified by the provided key.
         * If no keyAnimation is provided, the default key "main" is used.
         * 
         * @param time The animation speed.
         * @param keyAnimation The key identifying the animation (default is "main").
         */
        void SetAnimationSpeed(float time, const std::string& keyAnimation = "main");

        /**
         * @brief Set whether the active animation should loop.
         * 
         * This function sets whether the active animation identified by the provided key should loop.
         * If no keyAnimation is provided, the default key "main" is used.
         * 
         * @param loop Whether the animation should loop.
         * @param keyAnimation The key identifying the animation (default is "main").
         */
        void SetAnimationLoop(bool loop, const std::string& keyAnimation = "main");

        /**
         * @brief Get an iterator pointing to the beginning of the animations map.
         * 
         * This function returns a const iterator pointing to the beginning of the animations map.
         * It allows iterating through the animations map in a read-only manner.
         * 
         * @return Const iterator pointing to the beginning of the animations map.
         */
        typename MapAnimations::const_iterator GetBeginAnimations() const;

        /**
         * @brief Get an iterator pointing to the end of the animations map.
         * 
         * This function returns a const iterator pointing to the end of the animations map.
         * It allows iterating through the animations map in a read-only manner.
         * 
         * @return Const iterator pointing to the end of the animations map.
         */
        typename MapAnimations::const_iterator GetEndAnimations() const;

        // INSTANCE MANAGEMENT //

        /**
         * @brief Create a new instance and store it in the instances map.
         * 
         * This function creates a new instance and associates it with the provided animation.
         * The instance is identified by the given keyInstance, and the animation is specified
         * by the provided animation pointer. The created instance is then stored in the instances map.
         * 
         * @param keyInstance The key to identify the instance.
         * @param animation A pointer to the animation to associate with the instance.
         * @return A pointer to the created instance.
         */
        Instance* NewInstance(const std::string& keyInstance, Animation* animation);

        /**
         * @brief Create a new instance and store it in the instances map using the specified animation key.
         * 
         * This function creates a new instance and associates it with the animation identified by the provided keyAnimation.
         * The instance is identified by the given keyInstance, and the animation is retrieved using the keyAnimation.
         * The created instance is then stored in the instances map.
         * 
         * @param keyInstance The key to identify the instance.
         * @param keyAnimation The key identifying the animation to associate with the instance (default is "main").
         * @return A pointer to the created instance.
         */
        Instance* NewInstance(const std::string& keyInstance, const std::string& keyAnimation = "main");

        /**
         * @brief Remove an instance from the instances map.
         * 
         * This function removes the instance identified by the provided keyInstance from the instances map.
         * 
         * @param keyInstance The key identifying the instance to remove.
         */
        void RemoveInstance(const std::string& keyInstance);

        /**
         * @brief Remove all instances from the instances map.
         * 
         * This function clears all instances from the instances map, leaving it empty.
         */
        void ClearInstances();

        /**
         * @brief Get a pointer to the instance with the specified key.
         * 
         * This function retrieves a pointer to the instance identified by the provided keyInstance.
         * 
         * @param keyInstance The key identifying the instance.
         * @return A pointer to the instance.
         */
        Instance* GetInstance(const std::string& keyInstance);

        /**
         * @brief Get a const pointer to the instance with the specified key.
         * 
         * This function retrieves a const pointer to the instance identified by the provided keyInstance.
         * 
         * @param keyInstance The key identifying the instance.
         * @return A const pointer to the instance.
         */
        const Instance* GetInstance(const std::string& keyInstance) const;

        /**
         * @brief Set the current frame of the active animation for a specific instance.
         * 
         * This function sets the current frame of the active animation for the instance identified by the 
         * provided keyInstance to the specified frame index.
         * 
         * @param position The frame index to set.
         * @param keyInstance The key identifying the instance (default is "main").
         */
        void GotoFrame(Uint16 position, const std::string& keyInstance = "main");

        /**
         * @brief Check if the current frame of the active animation for a specific instance is equal to the specified frame index.
         * 
         * This function checks whether the current frame of the active animation for the instance identified by the provided 
         * keyInstance is equal to the specified frame index.
         * 
         * @param position The frame index to compare.
         * @param keyInstance The key identifying the instance (default is "main").
         * @return True if the current frame is equal to the specified frame index, otherwise false.
         */
        bool IsCurrentFrameEqual(Uint16 position, const std::string& keyInstance = "main") const;

        /**
         * @brief Check if the current frame of the active animation for a specific instance is before the specified frame index.
         * 
         * This function checks whether the current frame of the active animation for the instance identified by the provided 
         * keyInstance is before the specified frame index.
         * 
         * @param position The frame index to compare.
         * @param keyInstance The key identifying the instance (default is "main").
         * @return True if the current frame is before the specified frame index, otherwise false.
         */
        bool IsCurrentFrameBefore(Uint16 position, const std::string& keyInstance = "main") const;

        /**
         * @brief Check if the current frame of the active animation for a specific instance is after the specified frame index.
         * 
         * This function checks whether the current frame of the active animation for the instance identified by the provided 
         * keyInstance is after the specified frame index.
         * 
         * @param position The frame index to compare.
         * @param keyInstance The key identifying the instance (default is "main").
         * @return True if the current frame is after the specified frame index, otherwise false.
         */
        bool IsCurrentFrameAfter(Uint16 position, const std::string& keyInstance = "main") const;

        /**
         * @brief Check if the active animation for a specific instance has finished.
         * 
         * This function checks whether the active animation for the instance identified by the provided keyInstance has finished.
         * 
         * @param keyInstance The key identifying the instance (default is "main").
         * @return True if the animation has finished, otherwise false.
         */
        bool IsAnimFinished(const std::string& keyInstance = "main") const;

        /**
         * @brief Get an iterator pointing to the beginning of the instances map.
         * @return Const iterator pointing to the beginning of the instances map.
         */
        typename MapInstances::const_iterator GetBeginInstances() const;

        /**
         * @brief Get an iterator pointing to the end of the instances map.
         * @return Const iterator pointing to the end of the instances map.
         */
        typename MapInstances::const_iterator GetEndInstances() const;

        // INSTANCE - UPDATE //

        /**
         * @brief Update the animation state of a specific instance.
         * 
         * This function updates the animation state of the provided instance based on the elapsed time since the last frame.
         * 
         * @param dt The time elapsed since the last frame.
         * @param instance A pointer to the instance to update.
         */
        void Update(float dt, Instance* instance);

        /**
         * @brief Update the animation state of a specific instance using the specified key.
         * 
         * This function updates the animation state of the instance identified by the provided keyInstance based on the 
         * elapsed time since the last frame.
         * 
         * @param dt The time elapsed since the last frame.
         * @param keyInstance The key identifying the instance (default is "main").
         */
        void Update(float dt, const std::string& keyInstance = "main");

        /**
         * @brief Update the animation state of all instances.
         * 
         * This function updates the animation state of all instances based on the elapsed time since the last frame.
         * 
         * @param dt The time elapsed since the last frame.
         */
        void UpdateAll(float dt);

        // MISC METHODS //

        /**
         * @brief Get the grid size of the sprite (number of rows and columns).
         * 
         * This function returns a Vector2 representing the number of rows and columns in the sprite.
         * 
         * @return A Vector2 representing the number of rows and columns.
         */
        nexus::math::IVec2 GetGrid() const
        {
            return { rows, cols };
        }

        /**
         * @brief Get the size of each frame in the sprite.
         * 
         * This function returns a Vector2 representing the size of each frame in the sprite.
         * 
         * @return A Vector2 representing the size of each frame.
         */
        nexus::math::IVec2 GetFrameSize() const
        {
            return frameSize;
        }

        /**
         * @brief Get the center of each frame in the sprite.
         * 
         * This function returns a Vector2 representing the center of each frame in the sprite.
         * 
         * @return A Vector2 representing the center of each frame.
         */
        nexus::math::IVec2 GetFrameCenter() const
        {
            return frameCenter;
        }
    };


    /* Implementation Sprite */

    template <typename T_Texture>
    template <typename T_Context>
    Sprite<T_Texture>::Sprite(T_Context& ctx, const std::string& imPath, int rows, int cols, float speed)
    : texture(ctx, imPath)
    {
        // Calculate frame size and center based on texture dimensions and grid size
        frameSize = nexus::math::IVec2(texture->GetWidth() / cols, texture->GetHeight() / rows);
        frameCenter = frameSize * 0.5f;

        // Set the entire texture as the source rectangle
        texSource = texture->GetRectSize();

        // Set the number of columns, rows, and total frames
        this->cols = cols, this->rows = rows;
        frameNum = cols * rows;

        // Create a default animation ("main") spanning all frames with specified speed
        NewAnimation("main", 0, frameNum - 1, speed, true);

        // Create a default instance using the "main" animation
        NewInstance("main");
    }

    template <typename T_Texture>
    template <typename T_Context>
    Sprite<T_Texture>::Sprite(T_Context& ctx, const std::string& imPath, int rows, int cols, const nexus::shape2D::Rectangle& texSource, float speed)
    : texture(ctx, imPath), texSource(texSource)
    {
        // Calculate frame size and center based on custom texture source rectangle and grid size
        frameSize = nexus::math::IVec2(texSource.w / cols, texSource.h / rows);
        frameCenter = frameSize * 0.5f;

        // Set the number of columns, rows, and total frames
        this->cols = cols, this->rows = rows;
        frameNum = cols * rows;

        // Create a default animation ("main") spanning all frames with specified speed
        NewAnimation("main", 0, frameNum - 1, speed, true);

        // Create a default instance using the "main" animation
        NewInstance("main");
    }

    template <typename T_Texture>
    Sprite<T_Texture>::Sprite(const T_Texture& texture, int rows, int cols, const nexus::shape2D::Rectangle&& texSource, float speed)
    : texture(texture), texSource(texSource)
    {
        // Calculate frame size and center based on custom texture source rectangle and grid size
        frameSize = nexus::math::IVec2(texSource.w / cols, texSource.h / rows);
        frameCenter = frameSize * 0.5f;

        // Set the number of columns, rows, and total frames
        this->cols = cols, this->rows = rows;
        frameNum = cols * rows;

        // Create a default animation ("main") spanning all frames with specified speed
        NewAnimation("main", 0, frameNum - 1, speed, true);

        // Create a default instance using the "main" animation
        NewInstance("main");
    }

    template <typename T_Texture>
    Sprite<T_Texture>::Sprite(Sprite&& other) noexcept
    : animations(std::move(other.animations))
    , instances(std::move(other.instances))
    , texture(std::move(other.texture))
    , frameSize(other.frameSize)
    , frameCenter(other.frameCenter)
    , texSource(other.texSource)
    , cols(other.cols)
    , rows(other.rows)
    , frameNum(other.frameNum)
    { }

    template <typename T_Texture>
    Sprite<T_Texture>& Sprite<T_Texture>::operator=(Sprite&& other) noexcept
    {
        if (this != &other)
        {
            animations = std::move(other.animations);
            instances = std::move(other.instances);
            texture = std::move(other.texture);
            frameSize = other.frameSize;
            frameCenter = other.frameCenter;
            texSource = other.texSource;
            cols = other.cols;
            rows = other.rows;
            frameNum = other.frameNum;
        }
        return *this;
    }


    /* Animation Management */

    template <typename T_Texture>
    typename Sprite<T_Texture>::Animation* Sprite<T_Texture>::NewAnimation(const std::string& keyAnimation, Uint16 startFrame, Uint16 endFrame, float speed, bool loop)
    {
        return (animations[keyAnimation] = std::make_unique<Animation>(
            speed, static_cast<Uint16>((endFrame - startFrame) + 1),
            startFrame, endFrame, loop)).get();
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::SetAnimation(const std::string& keyAnimation, const std::string& keyInstance)
    {
    #ifndef NDEBUG
        if (animations.find(keyAnimation) != animations.end())
        {
            Instance* instance = GetInstance(keyInstance);
            instance->animation = GetAnimation(keyAnimation);
            instance->currentFrame = instance->animTime = 0;
        }
        else
        {
            NEXUS_LOG(Error) << "Animation key [" << keyAnimation.c_str() << "] not found\n";
        }
    #else
        Instance* instance = GetInstance(keyInstance);
        instance->animation = GetAnimation(keyAnimation);
        instance->currentFrame = instance->animTime = 0;
    #endif
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::Animation* Sprite<T_Texture>::GetAnimation(const std::string& keyAnimation)
    {
    #ifndef NDEBUG
        const auto itAnimation = animations.find(keyAnimation);

        if (itAnimation == animations.end())
        {
            NEXUS_LOG(Error) << "Animation key [" << keyAnimation.c_str() << "] not found\n";
            return animations.find("main")->second.get();
        }

        return itAnimation->second.get();
    #else
        return animations[keyAnimation].get();
    #endif
    }

    template <typename T_Texture>
    const typename Sprite<T_Texture>::Animation* Sprite<T_Texture>::GetAnimation(const std::string& keyAnimation) const
    {
    #ifndef NDEBUG
        const auto itAnimation = animations.find(keyAnimation);

        if (itAnimation == animations.end())
        {
            NEXUS_LOG(Error) << "Animation key [" << keyAnimation.c_str() << "] not found\n";
            return animations.find("main")->second.get();
        }

        return itAnimation->second.get();
    #else
        return animations.find(keyAnimation)->second.get();
    #endif
    }

    template <typename T_Texture>
    nexus::shape2D::Rectangle Sprite<T_Texture>::GetAnimationFrameRec(Uint16 frameIndex, const Sprite::Animation * const animation) const
    {
        frameIndex %= animation->count;

        return {
            texSource.x + ((animation->start + frameIndex) % cols * frameSize.x),
            texSource.y + ((animation->start + frameIndex) / cols * frameSize.y),
            frameSize.x, frameSize.y
        };
    }

    template <typename T_Texture>
    nexus::shape2D::Rectangle Sprite<T_Texture>::GetAnimationFrameRec(Uint16 frameIndex, const std::string& keyAnimation) const
    {
        return GetAnimationFrameRec(frameIndex, GetAnimation(keyAnimation));
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::SetAnimationSpeed(float time, const std::string& keyAnimation)
    {
        GetAnimation(keyAnimation)->speed = time;
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::SetAnimationLoop(bool loop, const std::string& keyAnimation)
    {
        GetAnimation(keyAnimation)->loop = loop;
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::MapAnimations::const_iterator Sprite<T_Texture>::GetBeginAnimations() const
    {
        return animations.cbegin();
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::MapAnimations::const_iterator Sprite<T_Texture>::GetEndAnimations() const
    {
        return animations.cend();
    }


    /* Instance Management */

    template <typename T_Texture>
    typename Sprite<T_Texture>::Instance* Sprite<T_Texture>::NewInstance(const std::string& keyInstance, Animation* animation)
    {
        return (instances[keyInstance] = std::make_unique<Instance>(nexus::shape2D::Rectangle{
            texSource.x, texSource.y, frameSize.x, frameSize.y
            }, animation, 0.0f, 0)).get();
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::Instance* Sprite<T_Texture>::NewInstance(const std::string& keyInstance, const std::string& keyAnimation)
    {
        return NewInstance(keyInstance, GetAnimation(keyAnimation));
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::RemoveInstance(const std::string& keyInstance)
    {
    #ifndef NDEBUG
        if (keyInstance == "main")
        {
            NEXUS_LOG(Warning) << "Attempt to delete instance [\"main\"]. Attempt cancelled.\n";
            return;
        }

        auto itInstance = instances.find(keyInstance);

        if (itInstance != instances.end())
        {
            itInstance = instances.erase(itInstance);
            NEXUS_LOG(Warning) << "Instance [\"" << keyInstance << "\"] deleted successfully.\n";
        }
        else
        {
            NEXUS_LOG(Warning) << "Attempt to delete instance [\"" << keyInstance << "\"] which does not exist. Attempt cancelled.";
        }
    #else
        auto itInstance = instances.find(keyInstance);
        itInstance = instances.erase(itInstance);
    #endif
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::ClearInstances()
    {
        auto itInstance = instances.begin();

        while (itInstance != instances.end())
        {
            if (itInstance->first != "main")
            {
                itInstance = instances.erase(itInstance);
            }
            else
            {
                ++itInstance;
            }
        }
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::Instance* Sprite<T_Texture>::GetInstance(const std::string& keyInstance)
    {
    #ifndef NDEBUG
        const auto itInstance = instances.find(keyInstance);

        if (itInstance == instances.end())
        {
            NEXUS_LOG(Error) << "Instance key [" << keyInstance << "] not found.\n";
            return instances.find("main")->second.get();
        }

        return itInstance->second.get();
    #else
        return instances[keyInstance].get();
    #endif
    }

    template <typename T_Texture>
    const typename Sprite<T_Texture>::Instance* Sprite<T_Texture>::GetInstance(const std::string& keyInstance) const
    {
    #ifndef NDEBUG
        const auto itInstance = instances.find(keyInstance);

        if (itInstance == instances.end())
        {
            NEXUS_LOG(Error) << "Instance key [" << keyInstance << "] not found.\n";
            return instances.find("main")->second.get();
        }

        return itInstance->second.get();
    #else
        return instances.find(keyInstance)->second.get();
    #endif
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::GotoFrame(Uint16 position, const std::string& keyInstance)
    {
        Instance* instance = GetInstance(keyInstance);
        Animation* animation = instance->animation;

        instance->animTime = 0;
        instance->currentFrame = position % animation->count;
        instance->frameRec = GetAnimationFrameRec(instance->currentFrame, animation);
    }

    template <typename T_Texture>
    bool Sprite<T_Texture>::IsCurrentFrameEqual(Uint16 position, const std::string& keyInstance) const
    {
        return GetInstance(keyInstance)->currentFrame == position;
    }

    template <typename T_Texture>
    bool Sprite<T_Texture>::IsCurrentFrameBefore(Uint16 position, const std::string& keyInstance) const
    {
        return GetInstance(keyInstance)->currentFrame < position;
    }

    template <typename T_Texture>
    bool Sprite<T_Texture>::IsCurrentFrameAfter(Uint16 position, const std::string& keyInstance) const
    {
        return GetInstance(keyInstance)->currentFrame > position;
    }

    template <typename T_Texture>
    bool Sprite<T_Texture>::IsAnimFinished(const std::string& keyInstance) const
    {
        const Instance* instance = GetInstance(keyInstance);
        const Animation* animation = instance->animation;
        const bool isLastFrame = (instance->currentFrame == animation->count - 1);
        const bool isTimeExceeded = (instance->animTime >= animation->speed);
        return (isLastFrame && isTimeExceeded);
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::MapInstances::const_iterator Sprite<T_Texture>::GetBeginInstances() const
    {
        return instances.cbegin();
    }

    template <typename T_Texture>
    typename Sprite<T_Texture>::MapInstances::const_iterator Sprite<T_Texture>::GetEndInstances() const
    {
        return instances.cend();
    }


    /* Implementation Instance - Update */

    template <typename T_Texture>
    void Sprite<T_Texture>::Update(float dt, Instance* instance)
    {
        Animation* animation = instance->animation;

        const Sint8 direction = (dt > 0) ? 1 : -1; // Animation direction (1 for forward, -1 for backward)

        if (animation->loop
         || (direction > 0 && instance->currentFrame < animation->count - 1)
         || (direction < 0 && instance->currentFrame > 0)                 )
        {
            if ((instance->animTime += dt) * direction >= animation->speed) // Animation (until the first or last frame depending on the direction)
            {
                instance->currentFrame = (instance->currentFrame + direction + animation->count) % animation->count;
                instance->animTime = 0;
            }
        }
        else if (instance->animTime < animation->speed) // First or last (depending on the direction) frame reached in non-loop, increment of the timer until its end (for the use of `IsAnimFinished`)
        {
            instance->animTime += dt;
        }

        instance->frameRec.x = texSource.x + ((animation->start + instance->currentFrame) % cols * frameSize.x);
        instance->frameRec.y = texSource.y + ((animation->start + instance->currentFrame) / cols * frameSize.y);
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::Update(float dt, const std::string& keyInstance)
    {
        Update(dt, GetInstance(keyInstance));
    }

    template <typename T_Texture>
    void Sprite<T_Texture>::UpdateAll(float dt)
    {
        for (auto & itInstance : instances)
        {
            Update(dt, itInstance.first);
        }
    }

}

#endif //NEXUS_EXT_2D_EXT_3D_IMPL_SPRITE_HPP
