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

#ifndef NEXUS_EXT_2D_EXT_3D_IMPL_PARTICLES_HPP
#define NEXUS_EXT_2D_EXT_3D_IMPL_PARTICLES_HPP

#include "../../math/nxVec2.hpp"
#include "../nxSurface.hpp"
#include "../nxColor.hpp"
#include <random>
#include <array>
#include <ctime>

namespace _gfx_impl {

    /**
     * @brief Structure representing a 2D/3D particle with position, velocity, color, lifetime, and size.
     *
     * This structure defines a 2D/3D particle, encapsulating properties such as position, velocity, color,
     * lifetime, and size. It serves as the fundamental building block for the particle system.
     *
     * @tparam T_Vec Type representing the vector used for position and velocity (e.g., 2D or 3D vector).
     * @tparam T_Texture Texture type used to render particles.
     */
    template <typename T_Texture, typename T_Vec>
    struct Particle
    {
        typedef T_Texture Texture;
        typedef T_Vec Vec;

        T_Texture       &texture;       ///< The texture used for the rendering.
        T_Vec           position;       ///< The position of the particle.
        T_Vec           velocity;       ///< The velocity of the particle.
        nexus::gfx::Color  color;          ///< The color of the particle.
        float           rotation;       ///< The rotation of the particle.
        float           velRot;         ///< The rotation velocity of the particle.
        float           baseScale;      ///< The texture scale obtained from the desired size.
        float           invLifeTime;    ///< The inverse of the total lifetime of the particle.
        float           time;           ///< The remaining lifetime of the particle.
        bool            colorVar;       ///< Indicates whether to fade in transparency according to the life time.
        bool            sizeVar;        ///< Indicates whether to reduce the size according to life time.

        /**
         * @brief Constructor for the Particle structure.
         *
         * @param texture The texture used for the rendering.
         * @param position Initial position of the particle.
         * @param velocity Initial velocity of the particle.
         * @param color Color of the particle.
         * @param rotation Initial rotation of the particle.
         * @param velRot Rotation velocity of the particle.
         * @param invLifeTime Inverse lifetime of the particle.
         * @param time Remaining lifetime of the particle.
         * @param size Desired dimensions of the particle.
         * @param sizeVar Indicates whether to reduce the size according to life time.
         */
        Particle(T_Texture& texture, const T_Vec& position, const T_Vec& velocity, const nexus::gfx::Color& color,
                 float rotation, float velRot, float invLifeTime, float time, float size, bool colorVar, bool sizeVar)
        : texture(texture), position(position), velocity(velocity), color(color), rotation(rotation), velRot(velRot)
        , baseScale(size / texture->GetWidthF()), invLifeTime(invLifeTime)
        , time(time), colorVar(colorVar), sizeVar(sizeVar)
        { }

        /**
         * @brief Virtual destructor for the Particle structure.
         */
        virtual ~Particle() = default;

        /**
         * @brief Copy assignment operator for the Particle structure.
         *
         * This operator assigns the values of another Particle instance to this instance.
         *
         * @param other The Particle instance to copy from.
         * @return Reference to the modified Particle instance.
         */
        Particle& operator=(const Particle& other) noexcept
        {
            if (this != &other)
            {
                texture     = other.texture;
                position    = other.position;
                velocity    = other.velocity;
                color       = other.color;
                rotation    = other.rotation;
                velRot      = other.velRot;
                baseScale   = other.baseScale;
                invLifeTime = other.invLifeTime;
                time        = other.time;
                colorVar    = other.colorVar;
                sizeVar     = other.sizeVar;
            }

            return *this;
        }

        /**
         * @brief Obtains the rendering data for the particle drawing functions.
         *
         * This function retrieves the rendering data necessary for drawing the particle. It provides
         * the color and scale factors that should be applied during rendering, taking into account
         * variations based on particle properties such as color and size over its lifetime.
         *
         * @param color Reference to store the particle color data.
         * @param scale Reference to store the particle scale factor data.
         */
        void GetRenderData(nexus::gfx::Color& color, float& scale) const
        {
            color = this->color, scale = this->baseScale;
            if (colorVar) color.a *= time * invLifeTime;
            if (sizeVar) scale *= time * invLifeTime;
        }

        /**
         * @brief Updates the particle's position and lifetime.
         *
         * This function updates the particle's position based on its velocity and the specified time step.
         * It also decreases the remaining lifetime of the particle, accounting for any gravitational force.
         *
         * @param gravity The gravitational force affecting the particle.
         * @param dt The time step for the update.
         * @return True if the particle is still alive, false if it has expired.
         */
        bool Update(const T_Vec& gravity, float dt)
        {
            position += velocity * dt;
            velocity += gravity * dt;
            rotation += velRot * dt;
            return (time -= dt) > 0.0f;
        }
    };

    /**
     * @brief Class for managing a system of 2D/3D particles.
     *
     * This class is responsible for managing a system of 2D/3D particles, including their initialization, updates,
     * and rendering. It encapsulates various parameters such as emission position, initial velocities, gravitational
     * force, particle lifetime, and color.
     *
     * @tparam T_Particle Type representing the particle class used in the system.
     * @tparam T_Context Type representing the rendering context (e.g., SDL renderer, OpenGL context).
     */
    template <typename T_Particle, typename T_Context>
    class ParticleSystem
    {
        static_assert(std::is_base_of<Particle<typename T_Particle::Texture, typename T_Particle::Vec>, T_Particle>::value,
            "T_Particle must be derived from _gfx_impl::Particle.");

      public:
        typedef typename T_Particle::Texture Texture;
        typedef typename T_Particle::Vec Vec;
        typedef T_Context Context;

      protected:
        std::mt19937                                            gen;                        ///< Random number generator.
        std::array<std::uniform_real_distribution<float>, 3>    velDistribution;            ///< Distribution for randomizing particle velocity.
        std::uniform_real_distribution<float>                   rotationDistribution;       ///< Distribution for randomizing particle rotation.
        std::uniform_real_distribution<float>                   velRotDistribution;         ///< Distribution for randomizing particle rotation velocity.
        std::uniform_real_distribution<float>                   sizeDistribution;           ///< Distribution for randomizing particle size.
        std::vector<T_Particle>                                 particles;                  ///< Vector to hold active particles.
        Texture                                                 texture;                    ///< Texture used to render particles.
        T_Context                                               &ctx;                       ///< Context used to render particles.
        Vec                                                     position;                   ///< The emission position for new particles.
        Vec                                                     minVel, maxVel;             ///< The range of initial velocities for new particles.
        Vec                                                     gravity;                    ///< The gravitational force affecting the particles.
        float                                                   minRotation, maxRotation;   ///< The range of initial rotation for new particles.
        float                                                   minVelRot, maxVelRot;       ///< The range of initial rotation velocities for new particles.
        float                                                   minSize, maxSize;           ///< The range of sizes for new particles.
        float                                                   lifeTime;                   ///< The lifetime of each emitted particle.
        float                                                   invLifeTime;                ///< The inverse lifetime of each emitted particle.
        nexus::gfx::Color                                          color;                      ///< The color of the emitted particles.
        bool                                                    colorVariation;             ///< Indicates whether to fade in transparency according to the life time.
        bool                                                    sizeVariation;              ///< Indicates whether to reduce the size according to life time.

      public:
        /**
         * @brief Parameterized constructor for the ParticleSystem class.
         *
         * This constructor initializes a ParticleSystem with the specified maximum number of particles.
         *
         * @param maxParticles The maximum number of particles allowed in the system.
         * @param texture Pointer to the texture to use for rendering particles. (Optional, a 64x64 white circle texture will be created by default)
         */
        ParticleSystem(T_Context& ctx, Uint32 maxParticles, Texture* texture = nullptr);

        /**
         * @brief Virtual destructor for the ParticleSystem class.
         */
        virtual ~ParticleSystem() = default;

        /**
         * @brief Deleted copy constructor to prevent copying ParticleSystem instances.
         *
         * Copying instances of ParticleSystem is disallowed to avoid unintended consequences and resource conflicts.
         */
        ParticleSystem(const ParticleSystem&) = delete;

        /**
         * @brief Deleted copy assignment operator to prevent copying ParticleSystem instances.
         *
         * Copy assignment of ParticleSystem instances is prohibited to maintain system integrity.
         */
        ParticleSystem& operator=(const ParticleSystem&) = delete;

        /**
         * @brief Move constructor for the ParticleSystem class.
         *
         * This move constructor transfers ownership of resources from the provided ParticleSystem instance.
         *
         * @param other The ParticleSystem instance to move.
         */
        ParticleSystem(ParticleSystem&& other) noexcept;

        /**
         * @brief Move assignment operator for the ParticleSystem class.
         *
         * This move assignment operator transfers ownership of resources from the provided ParticleSystem instance.
         *
         * @param other The ParticleSystem instance to move.
         * @return A reference to the moved ParticleSystem instance.
         */
        ParticleSystem& operator=(ParticleSystem&& other) noexcept;

        /**
         * @brief Gets the current count of active particles in the system.
         *
         * @return The number of active particles.
         */
        Uint32 Count() const { return particles.size(); }

        /**
         * @brief Gets the maximum capacity of particles allowed in the system.
         *
         * @return The maximum number of particles allowed.
         */
        Uint32 Capacity() const { return particles.capacity(); }

        /**
         * @brief Reserves space in the particle system for a specified number of particles.
         *
         * This function reserves space in the particle system for the specified number of particles,
         * optimizing memory allocation and improving performance.
         *
         * @param size The number of particles to reserve space for.
         */
        void Reserve(Uint32 size) const { particles.reserve(size); }

        /**
         * @brief Gets the emission position for new particles.
         *
         * @return The emission position.
         */
        Vec GetPosition() const { return position; }

        /**
         * @brief Gets the lifetime value for each emitted particle.
         *
         * @return The lifetime value.
         */
        float SetLifeTime() const { return lifeTime; }

        /**
         * @brief Gets the gravitational force affecting the particles.
         *
         * @return The gravitational force.
         */
        Vec GetGravity() const { return gravity; }

        /**
         * @brief Gets the color of emitted particles.
         *
         * @return The color of emitted particles.
         */
        nexus::gfx::Color GetColor() const { return color; }

        /**
         * @brief Gets the texture used for rendering particles.
         *
         * This function retrieves a reference to the texture used for rendering particles.
         *
         * @return Reference to the texture used for rendering particles.
         */
        Texture& GetTexture() { return texture; }

        /**
         * @brief Gets the texture used for rendering particles (const version).
         *
         * This function retrieves a const reference to the texture used for rendering particles.
         *
         * @return Const-reference to the texture used for rendering particles.
         */
        const Texture& GetTexture() const { return texture; }

        /**
         * @brief Checks if color variation is active for the particles.
         *
         * This function checks whether color variation is active for the particles in the system.
         *
         * @return True if color variation is active, false otherwise.
         */
        bool HasColorVariation() const { return colorVariation; }

        /**
         * @brief Checks if size variation is active for the particles.
         *
         * This function checks whether size variation is active for the particles in the system.
         *
         * @return True if size variation is active, false otherwise.
         */
        bool HasSizeVariation() const { return sizeVariation; }

        /**
         * @brief Gets the rendering context used by this particle system.
         *
         * @return Const-reference to the rendering context used by this particle system.
         */
        const T_Context& GetContext() const { return ctx; }

        /**
         * @brief Gets the rendering context used by this particle system.
         *
         * @return Reference to the rendering context used by this particle system.
         */
        T_Context& GetContext() { return ctx; }

        /**
         * @brief Sets the emission position for new particles.
         *
         * This function sets the emission position for new particles in the system.
         *
         * @param position The emission position.
         */
        void SetPosition(const Vec& position)
        {
            this->position = position;
        }

        /**
         * @brief Sets the lifetime for each emitted particle.
         *
         * This function sets the lifetime value for each emitted particle in the system.
         *
         * @param lifeTime The lifetime value.
         */
        void SetLifeTime(float lifeTime)
        {
            this->invLifeTime = 1.0f / lifeTime;
            this->lifeTime = lifeTime;
        }

        /**
         * @brief Sets the gravitational force affecting the particles.
         *
         * This function sets the gravitational force affecting the particles in the system.
         *
         * @param gravity The gravitational force.
         */
        void SetGravity(const Vec& gravity)
        {
            this->gravity = gravity;
        }

        /**
         * @brief Sets the color of emitted particles.
         *
         * This function sets the color of emitted particles in the system.
         *
         * @param color The color of emitted particles.
         */
        void SetColor(const nexus::gfx::Color& color)
        {
            this->color = color;
        }

        /**
         * @brief Sets the texture used for rendering particles.
         *
         * This function sets the texture used for rendering particles in the system.
         *
         * @param texture The texture to set.
         */
        void SetTexture(Texture& texture)
        {
            this->texture = texture;
        }

        /**
         * @brief Sets the color variation for particles.
         *
         * This function sets whether color variation is active for the particles in the system.
         *
         * @param active True to enable color variation, false to disable it.
         */
        void SetColorVariation(bool active)
        {
            this->colorVariation = active;
        }

        /**
         * @brief Sets the size variation for particles.
         *
         * This function sets whether size variation is active for the particles in the system.
         *
         * @param active True to enable size variation, false to disable it.
         */
        void SetSizeVariation(bool active)
        {
            this->sizeVariation = active;
        }

        /**
         * @brief Sets the range of sizes for new particles.
         *
         * This function sets the range of sizes for new particles in the system.
         *
         * @param minSize The minimum size.
         * @param maxSize The maximum size.
         */
        void SetSize(float minSize, float maxSize)
        {
            if (minSize != maxSize)
            {
                sizeDistribution = std::uniform_real_distribution<float>(minSize, maxSize);
            }
            this->minSize = minSize;
            this->maxSize = maxSize;
        }

        /**
         * @brief Sets the range of initial velocities for new particles.
         *
         * This function sets the range of initial velocities for new particles in the system.
         *
         * @param minVel The minimum velocity.
         * @param maxVel The maximum velocity.
         */
        void SetVelocity(const Vec& minVel, const Vec& maxVel)
        {
            for (Uint8 i = 0; i < T_Particle::Vec::Dimensions; i++)
            {
                velDistribution[i] = std::uniform_real_distribution<float>(minVel[i], maxVel[i]);
            }
            this->minVel = minVel;
            this->maxVel = maxVel;
        }

        /**
         * @brief Sets the range of initial rotation angles for new particles.
         *
         * This function sets the range of initial rotation angles for new particles in the system.
         * Rotation angle determines the initial orientation of particles.
         *
         * @param minAngle The minimum rotation angle in degrees.
         * @param maxAngle The maximum rotation angle in degrees.
         */
        void SetRotation(float minAngle, float maxAngle)
        {
            if (minAngle != maxAngle)
            {
                rotationDistribution = std::uniform_real_distribution<float>(minAngle, maxAngle);
            }
            this->minRotation = minAngle;
            this->maxRotation = maxAngle;
        }

        /**
         * @brief Sets the range of rotational velocities for new particles.
         *
         * This function sets the range of rotational velocities for new particles in the system.
         * Rotational velocity determines how fast particles rotate around their center.
         *
         * @param minVelRot The minimum rotational velocity.
         * @param maxVelRot The maximum rotational velocity.
         */
        void SetRotationVelocity(float minVelRot, float maxVelRot)
        {
            if (minVelRot != maxVelRot)
            {
                velRotDistribution = std::uniform_real_distribution<float>(minVelRot, maxVelRot);
            }
            this->minVelRot = minVelRot;
            this->maxVelRot = maxVelRot;
        }

        /**
         * @brief Clears the particle system by resetting the number of particles to zero.
         *
         * This function clears the particle system by resetting the number of particles to zero.
         */
        void Clear()
        {
            particles.clear();
        }

        /**
         * @brief Emits a specified number of new particles.
         *
         * This function emits a specified number of new particles into the system.
         *
         * @param num The number of particles to emit.
         */
        void Emit(Uint32 num = 1);

        /**
         * @brief Updates the state of all active particles.
         *
         * This function updates the state of all active particles in the system based on the specified time step.
         *
         * @param dt The time step for the update.
         */
        void Update(float dt);
    };

    /* Implementaiton ParticleSystem */

    template <typename T_Particle, typename T_Context>
    ParticleSystem<T_Particle, T_Context>::ParticleSystem(T_Context& ctx, Uint32 maxParticles, Texture* texture)
    : gen(std::time(nullptr))
    , texture(ctx)
    , ctx(ctx)
    , position()
    , minVel{ -10.0f, -10.0f }
    , maxVel{ 10.0f, 10.0f }
    , gravity()
    , minRotation(0)
    , maxRotation(0)
    , minVelRot(0)
    , maxVelRot(0)
    , minSize(1.0f)
    , maxSize(2.0f)
    , lifeTime(1.0f)
    , invLifeTime(1.0f)
    , color(nexus::gfx::White)
    , colorVariation(false)
    , sizeVariation(false)
    {
        this->SetSize(minSize, maxSize);
        this->SetVelocity(minVel, maxVel);
        //this->SetRotation(minRotation, maxRotation);
        //this->SetRotationVelocity(minVelRot, maxVelRot);

        particles.reserve(maxParticles);

        if (texture == nullptr)
        {
            nexus::gfx::Surface circle(64, 64, nexus::gfx::Blank);
            circle.DrawCircle(32, 32, 32, nexus::gfx::White);
            this->texture = Texture(ctx, circle);
        }
        else
        {
            this->texture = *texture;
        }
    }

    template <typename T_Particle, typename T_Context>
    ParticleSystem<T_Particle, T_Context>::ParticleSystem(ParticleSystem&& other) noexcept
    : gen(std::move(other.gen))
    , velDistribution(std::move(other.velDistribution))
    , rotationDistribution(std::move(other.rotationDistribution))
    , velRotDistribution(std::move(other.velRotDistribution))
    , sizeDistribution(std::move(other.sizeDistribution))
    , particles(std::move(other.particles))
    , texture(std::move(other.texture))
    , ctx(other.ctx)
    , position(other.position)
    , minVel(other.minVel)
    , maxVel(other.maxVel)
    , gravity(other.gravity)
    , minRotation(other.minRotation)
    , maxRotation(other.maxRotation)
    , minVelRot(other.minVelRot)
    , maxVelRot(other.maxVelRot)
    , minSize(other.minSize)
    , maxSize(other.maxSize)
    , lifeTime(other.lifeTime)
    , invLifeTime(other.invLifeTime)
    , color(other.color)
    , colorVariation(other.colorVariation)
    , sizeVariation(other.sizeVariation)
    { }

    template <typename T_Particle, typename T_Context>
    ParticleSystem<T_Particle, T_Context>& ParticleSystem<T_Particle, T_Context>::operator=(ParticleSystem&& other) noexcept
    {
        if (this != &other)
        {
            gen                     = std::move(other.gen);
            velDistribution         = std::move(other.velDistribution);
            rotationDistribution    = std::move(other.rotationDistribution);
            velRotDistribution      = std::move(other.velRotDistribution);
            sizeDistribution        = std::move(other.sizeDistribution);
            particles               = std::move(other.particles);
            texture                 = std::move(other.texture);
            ctx                     = other.ctx;
            position                = other.position;
            minVel                  = other.minVel;
            maxVel                  = other.maxVel;
            gravity                 = other.gravity;
            minRotation             = other.minRotation;
            maxRotation             = other.maxRotation;
            minVelRot               = other.minVelRot;
            maxVelRot               = other.maxVelRot;
            minSize                 = other.minSize;
            maxSize                 = other.maxSize;
            lifeTime                = other.lifeTime;
            invLifeTime             = other.invLifeTime;
            color                   = other.color;
            colorVariation          = other.colorVariation;
            sizeVariation           = other.sizeVariation;
        }
        return *this;
    }

    template <typename T_Particle, typename T_Context>
    void ParticleSystem<T_Particle, T_Context>::Emit(Uint32 num)
    {
        for (Uint32 i = 0; i < num && particles.size() < particles.capacity(); i++)
        {
            Vec velocity;
            for (Uint8 i = 0; i < velocity.Dimensions; i++)
            {
                velocity[i] = velDistribution[i](gen);
            }

            float rotation;
            if (minRotation == maxRotation) rotation = minRotation;
            else rotation = rotationDistribution(gen);

            float velRot;
            if (minVelRot == maxVelRot) velRot = minVelRot;
            else velRot = velRotDistribution(gen);

            float size;
            if (minSize == maxSize) size = minSize;
            else size = sizeDistribution(gen);

            particles.emplace_back(texture, position, velocity, color,
                rotation, velRot, invLifeTime, lifeTime, size,
                colorVariation, sizeVariation);
        }
    }

    template <typename T_Particle, typename T_Context>
    void ParticleSystem<T_Particle, T_Context>::Update(float dt)
    {
        for (int i = particles.size()-1; i >= 0; i--)
        {
            if (!(particles[i].Update(gravity, dt)))
            {
                particles[i] = particles.back();
                particles.pop_back();
            }
        }
    }

}

#endif //NEXUS_EXT_2D_EXT_3D_IMPL_PARTICLES_HPP
