#ifndef NEXUS_EXT_3D_GL_LIGHTS_HPP
#define NEXUS_EXT_3D_GL_LIGHTS_HPP

#include "../sp_model/nxModel.hpp"
#include "../nxCamera3D.hpp"
#include "../nxTexture.hpp"
#include "../nxShader.hpp"
#include "../nxEnums.hpp"

#include <functional>

namespace nexus { namespace gl {

    /**
     * @brief Structure representing a shadow map for shadow rendering.
     */
    class NEXUS_API ShadowMap : public nexus::gl::Contextual
    {
      private:
        static inline std::unique_ptr<Shader> debugShader = nullptr;    ///< Shader used for debug drawing
        static inline int instanceCounter = 0;                          ///< Counter for the number of instances of ShadowMap
        static inline int locNear, locFar;                              ///< Location of the 'near' and 'far' uniforms in the shader

      private:
        Texture             depth;              ///< Depth texture used for storing shadow map depth
        shape2D::Rectangle  prevViewport;       ///< Previous viewport used for restoration after rendering to shadow map
        Uint32              fboId;              ///< Framebuffer object ID for shadow map
        bool                active;             ///< Flag indicating if the shadow map is active

      public:
        /**
         * @brief Constructor for creating a ShadowMap object.
         * @param ctx The rendering context.
         * @param w The width of the shadow map.
         * @param h The height of the shadow map.
         */
        ShadowMap(nexus::gl::Context& ctx, int w, int h);

        ~ShadowMap();

        /**
         * @brief Get the width of the shadow map.
         * @return The width of the shadow map.
         */
        int GetWidth() const { return depth->GetWidth(); }

        /**
         * @brief Get the height of the shadow map.
         * @return The height of the shadow map.
         */
        int GetHeight() const { return depth->GetHeight(); }

        /**
         * @brief Get the width of the shadow map as a floating point value.
         * @return The width of the shadow map as a floating point value.
         */
        float GetWidthF() const { return depth->GetWidthF(); }

        /**
         * @brief Get the height of the shadow map as a floating point value.
         * @return The height of the shadow map as a floating point value.
         */
        float GetHeightF() const { return depth->GetHeightF(); }

        /**
         * @brief Get the size of the shadow map.
         * @return The size of the shadow map.
         */
        nexus::math::IVec2 GetSize() const { return depth->GetSize(); }

        /**
         * @brief Get the depth texture of the shadow map.
         * @return The depth texture of the shadow map.
         */
        const Texture& GetDepthTexture() const { return depth; }

        /**
         * @brief Get the rendering context used by the shadow map.
         * @return The rendering context used by the shadow map.
         */
        Context& GetContext() { return depth.GetContext(); }

        /**
         * @brief Get the rendering context used by the shadow map (const version).
         * @return The rendering context used by the shadow map (const version).
         */
        const Context& GetContext() const { return depth.GetContext(); }

        /**
         * @brief Begin the shadow map rendering mode.
         * @param bounds The viewport bounds for rendering to the shadow map.
         */
        void Begin(const nexus::shape2D::Rectangle& bounds);

        /**
         * @brief End the shadow map rendering mode.
         */
        void End();

        /**
         * @brief Clear the shadow map.
         * This function activates the framebuffer if necessary,
         * clears it to white (maximum distance), and deactivates it.
         */
        void Clear();

        /**
         * @brief Debug draw the shadow map.
         * @param dst Destination rectangle for drawing the shadow map.
         * @param near Near clipping plane distance for debugging.
         * @param far Far clipping plane distance for debugging.
         */
        void Draw(const shape2D::Rectangle& dst, float near = 0.1f, float far = 1000.0f);
    };

    /**
     * @brief Class representing a light source in a 3D scene with shadow casting capabilities.
     */
    class NEXUS_API Light3D
    {
      private:
        /**
         * @brief Enumerations for shader locations indices related to light properties.
         */
        enum LocsModelShader {
            LocLightMatrix,       ///< Light matrix location.
            LocLightPosition,     ///< Light position location.
            LocLightDirection,    ///< Light direction location.
            LocLightColor,        ///< Light color location.
            LocLightCutoff,       ///< Light cutoff location.
            LocLightRadius,       ///< Light radius location.
            LocLightBounds,       ///< ShadowMap bounds dedicated to the light.
            LocLightShadow,       ///< Indicates whether the light produces shadows.
            LocLightSpot,         ///< Indicates whether the light is a spotlight.
            LocLightSpotSoftness, ///< Spot softness.
            LocLightEnabled       ///< Indicates whether the light is active.
        };

      private:
        friend class Lights3D;                            ///< Declare the Lights class as a friend to allow access to private members.

      private:
        Camera3D            caster;                     ///< Camera used for shadow casting.
        int                 locsLightModelShader[11];   ///< Array of shader locations for light properties.
        shape2D::Rectangle  boundsMap;                  ///< Rectangle defining the area allocated to this light in the depth buffer.
        Shader              *modelShader;               ///< Reference to the model shader used for rendering.
        ShadowMap           *shadowMap;                 ///< Pointer to the shadow map associated with the light.
        gfx::Color          color;                      ///< Color of the light.
        float               spotSoftness;               ///< The smooth transition for spot light. (To be adjusted according to the FOV of the caster)
        float               radius;                     ///< Radius of the light source's influence.
        bool                spotlight;                  ///< Indicates whether the light is a spotlight or not.
        bool                enabled;                    ///< Indicating whether the light is enabled or not.

      private:
        /**
         * @brief Construct a new Light object with the specified parameters.
         * @param modelShader Reference to the model shader.
         * @param shadowMap Pointer to the associated shadow map.
         * @param lightNum The index of the light (used for shader locations).
         * @param caster The casting camera used for shadows.
         * @param radius Radius of the light source's influence.
         * @param color Color of the light.
         * @param spotSoftness The softness of the spot light transition (optional, default is 0.65f).
         * @param spotlight Whether the light is a spotlight (optional, default is false).
         * @param enabled Whether the light is enabled (optional, default is true).
         */
        Light3D(Shader* modelShader, ShadowMap* shadowMap, Uint16 lightNum,
                const Camera3D& caster, const gfx::Color& color = gfx::White,
                float spotSoftness = 0.65f, float radius = 512,
                bool spotlight = false, bool enabled = true);

        /**
         * @brief Set the bounds of the shadow map for this light.
         * @param bounds The bounds of the shadow map.
         */
        void SetShadowMapBounds(const shape2D::Rectangle& bounds);

        /**
         * @brief Begin rendering to the depth buffer of the light for shadow calculations.
         */
        void BeginShadowCast();

        /**
         * @brief End rendering to the depth buffer of the light for shadow calculations.
         */
        void EndShadowCast();

      public:
        /**
         * @brief Get the position of the light.
         * @return The position of the light.
         */
        math::Vec3 GetPosition() const { return caster.position; }

        /**
         * @brief Get the target position of the light.
         * @return The target position of the light.
         */
        math::Vec3 GetTarget() const { return caster.target; }

        /**
         * @brief Get the vertical field of view (FOVY) of the light.
         * @return The FOVY of the light.
         */
        float GetFovY() const { return caster.fovy; }

        /**
         * @brief Get the radius of the light's influence.
         * @return The radius of the light.
         */
        float GetRadius() const { return radius; }

        /**
         * @brief Get the color of the light.
         * @return The color of the light.
         */
        gfx::Color GetColor() const { return color; }

        /**
         * @brief Get the caster camera of the light.
         * @return The caster camera of the light.
         */
        const Camera3D& GetCaster() const { return caster; }

        /**
         * @brief Get the softness of the spotlight transition.
         * @return The softness of the spotlight transition.
         */
        float GetSpolightSoftness() const { return spotSoftness; }

        /**
         * @brief Check if the light is a spotlight.
         * @return True if the light is a spotlight, false otherwise.
         */
        bool IsSpotlight() const { return spotlight; }

        /**
         * @brief Check if the light is active.
         * @return True if the light is active, false otherwise.
         */
        bool IsActive() const { return enabled; }

        /**
         * @brief Update the matrix of the light based on its position, target, and FOVY.
         */
        void UpdateMatrix();

        /**
         * @brief Set the position of the light.
         * @param position The new position of the light.
         * @param updateMatrix If true, update the light matrix after setting the position.
         */
        void SetPosition(const math::Vec3& position, bool updateMatrix = true);

        /**
         * @brief Set the target position of the light.
         * @param target The new target position of the light.
         * @param updateMatrix If true, update the light matrix after setting the target.
         */
        void SetTarget(const math::Vec3& target, bool updateMatrix = true);

        /**
         * @brief Set the vertical field of view (FOVY) of the light caster.
         * @param fovy The new FOVY of the light caster.
         * @param updateMatrix If true, update the light matrix after setting the FOVY.
         */
        void SetFovY(float fovy, bool updateMatrix = true);

        /**
         * @brief Set the radius of the light's influence.
         * @param radius The new radius of the light.
         */
        void SetRadius(float radius);

        /**
         * @brief Set the color of the light.
         * @param color The new color of the light.
         */
        void SetColor(const gfx::Color& color);

        /**
         * @brief Set the casting camera for the light.
         * @param caster The new casting camera for shadows.
         */
        void SetCaster(const Camera3D& caster);

        /**
         * @brief Set the softness of the spotlight transition.
         * @param spotSoftness The new softness value for the spotlight transition.
         */
        void SetSpotlightSoftness(float spotSoftness);

        /**
         * @brief Enable or disable the spotlight functionality for the light.
         * @param enabled True to enable spotlight, false to disable it.
         */
        void SetSpotlight(bool enabled);

        /**
         * @brief Set the active state of the light.
         * @param active True to enable the light, false to disable it.
         */
        void SetActive(bool active);
    };

    /**
     * @brief Class representing a manager for 3D lights in a scene.
     */
    class NEXUS_API Lights3D
    {
      private:
        enum LocsModelShader
        {
            LocShadowMap,           ///< Shadow map location.
            LocAmbientColor,        ///< Ambient color location.
            LocShadowMapTexelSize,  ///< Texel shadow map size location.
            LocUseSpecularMap,
            LocUseNormalMap,
            LocUseHeightMap
        };

      private:
        Shader modelShader;                     ///< Shader for light models.
        std::unique_ptr<Shader> shadowShader;   ///< Shader for shadow mapping.
        std::unique_ptr<ShadowMap> shadowMap;   ///< Shadow map used for shadows (optionnal).
        Uint32 bufferSize;                      ///< Size of the buffer used for shadow maps.
        Uint16 mapSize;                         ///< Size of the shadow map texture.

        std::vector<Light3D> sources;           ///< Vector containing pointers to the light sources.
        Uint16 maxLights;                       ///< Maximum number of lights supported.
        Uint16 shadowNum;                       ///< Number of shadow-casting lights.

        int locsLightModelShader[6];            ///< Shader locations for light model properties.

        gfx::Color ambient;                     ///< Ambient color of the scene lighting.

      private:
        /**
         * @brief Initialization function called by constructors.
         */
        void Load(Context& ctx);

      public:
        /**
         * @brief Constructor for the Lights class.
         * @param ctx The OpenGL context.
         * @param ambient Ambient color of the scene lighting.
         * @param maxLights Maximum number of lights supported.
         * @param bufferSize Size of the buffer used for shadow maps.
         */
        Lights3D(Context& ctx, gfx::Color ambient = { 25, 25, 25, 0 }, Uint16 maxLights = 1, Uint16 bufferSize = 1024);

        /**
         * @brief Constructor for the Lights class.
         * @param ctx The OpenGL context.
         * @param ambient Ambient intensity of the scene lighting.
         * @param maxLights Maximum number of lights supported.
         * @param bufferSize Size of the buffer used for shadow maps.
         */
        Lights3D(Context& ctx, float ambient = 0.1f, Uint16 maxLights = 1, Uint16 bufferSize = 1024);

        /**
         * @brief Add a light that does not cast shadows.
         * @param caster Camera representing the light source.
         * @param radius Radius of the light influence.
         * @param color Color of the light (optional, default is White).
         * @param spotSoftness The softness of the spot light transition (optional, default is 0.65f).
         * @param spotlight Whether the light is a spotlight (optional, default is false).
         * @param enabled Whether the light is enabled (optional, default is true).
         * @return Pointer to the added light.
         */
        Light3D* AddLight(const Camera3D& caster, const gfx::Color& color = gfx::White, float spotSoftness = 0.65f, float radius = 512, bool spotlight = false, bool enabled = true);

        /**
         * @brief Add a light that casts shadows.
         * @param caster Camera representing the light source.
         * @param radius Radius of the light influence.
         * @param color Color of the light (optional, default is White).
         * @param spotSoftness The softness of the spot light transition (optional, default is 0.65f).
         * @param spotlight Whether the light is a spotlight (optional, default is false).
         * @param enabled Whether the light is enabled (optional, default is true).
         * @return Pointer to the added light.
         */
        Light3D* AddShadowLight(const Camera3D& caster, const gfx::Color& color = gfx::White, float spotSoftness = 0.65f, float radius = 512, bool spotlight = false, bool enabled = true);

        /**
         * @brief Update the lights according to the user's camera.
         *
         * The light system Update updates the point of view according
         * to the given camera and clears the shadow map.
         *
         * @param camera Camera representing the user's viewpoint.
         */
        void Update(const Camera3D& camera);

        /**
         * @brief Render models from each light's perspective to write to the depth buffer.
         * @param model Model to render.
         * @param position Model position.
         * @param rotationAxis Model rotation axis.
         * @param rotationAngle Model rotation angle.
         * @param scale Model scale.
         */
        void ShadowCastModel(Model& model, const math::Vec3& position = {}, const math::Vec3& rotationAxis = {}, float rotationAngle = 0, const math::Vec3& scale = { 1, 1, 1 });

        /**
         * @brief Final rendering of models with the light model shader.
         * @param model Model to render.
         * @param position Model position.
         * @param rotationAxis Model rotation axis.
         * @param rotationAngle Model rotation angle.
         * @param scale Model scale.
         * @param tint Model tint color.
         */
        void DrawModel(Model& model, const math::Vec3& position = {}, const math::Vec3& rotationAxis = {}, float rotationAngle = 0, const math::Vec3& scale = { 1, 1, 1 }, const gfx::Color& tint = gfx::White);

        /**
         * @brief Draw the shadow map texture for debugging purposes.
         *
         * This function renders the shadow map texture onto the specified destination rectangle
         * with the option to specify the near and far clipping plane distances for debugging.
         *
         * @param dst Destination rectangle for drawing the shadow map.
         * @param near Near clipping plane distance for debugging (default is 0.1f).
         * @param far Far clipping plane distance for debugging (default is 1000.0f).
         */
        void DrawShadowMap(const shape2D::Rectangle& dst, float near = 0.1f, float far = 1000.0f);
    };

}}

#endif //NEXUS_EXT_3D_GL_LIGHTS_HPP
