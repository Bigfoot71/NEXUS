#ifndef NEXUS_GAPI_IMPL_SP_MODEL_MATERIAL_HPP
#define NEXUS_GAPI_IMPL_SP_MODEL_MATERIAL_HPP

#include "../../../utils/nxContextual.hpp"
#include "../../../gfx/nxSurface.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxVec3.hpp"
#include "../../../math/nxVec2.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../core/nxText.hpp"
#include "../../../core/nxLog.hpp"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <filesystem>
#include <functional>
#include <optional>
#include <array>

namespace _gapi_impl {

    /**
     * @brief Template class for material representation, which is a contextual object encapsulated within a container.
     * @tparam T_Context The context type, which can be sr::Context, gl::Context, or similar.
     * @tparam T_Shader The shader type.
     * @tparam T_Texture The texture type.
     */
    template <typename T_Context, typename T_Shader, typename T_Texture>
    class Material : public nexus::utils::Contextual<T_Context>
    {
      public:
        using Shader = T_Shader;
        using Texture = T_Texture;
        static constexpr Uint8 MapCount = 11;

        /**
         * @brief Enum defining the types of material maps.
         */
        enum MapType
        {
            Albedo      = 0,                    ///< Albedo material (same as: MapType::Diffuse)
            Diffuse     = 0,                    ///< Diffuse material (same as: MapType::Albedo)
            Metalness   = 1,                    ///< Metalness material (same as: MapType::Specular)
            Specular    = 1,                    ///< Specular material (same as: MapType::Metalness)
            Normal      = 2,                    ///< Normal material
            Roughness   = 3,                    ///< Roughness material
            Occlusion   = 4,                    ///< Ambient occlusion material
            Emission    = 5,                    ///< Emission material
            Height      = 6,                    ///< Heightmap material
            Cubemap     = 7,                    ///< Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
            Irradiance  = 8,                    ///< Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
            Prefilter   = 9,                    ///< Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
            BRDF        = 10                    ///< BRDF material
        };

        /**
         * @brief Structure representing a material map.
         */
        struct Map
        {
            std::optional<T_Texture> texture = std::nullopt;        ///< Material map texture
            nexus::gfx::Color color = nexus::gfx::White;            ///< Material map color
            float value = 0;                                        ///< Material map value
        };

      public:
        std::array<Map, MapCount> maps;         ///< Material maps array (MAX_MATERIAL_MAPS)
        T_Shader shader;                        ///< Material rendering shader
        float params[4];                        ///< Material generic parameters (if required)

      protected:
        /**
         * @brief Sets the texture from Assimp material data.
         * @param ctx The context object.
         * @param material The Assimp material.
         * @param scene The Assimp scene.
         * @param textureType The type of texture.
         * @param mapType The type of material map.
         * @param loadDefaultTexture Indicates whether to load a default texture if none is linked to the base material.
         */
        void SetTextureFromAssimpMaterial(T_Context& ctx, const aiMaterial* material, const aiScene* scene, aiTextureType textureType, MapType mapType, bool loadDefaultTexture = false);

        /**
         * @brief Sets the color from Assimp material data.
         * @param material The Assimp material.
         * @param colorKey The key for the color.
         * @param type The type of color.
         * @param index The index of the color.
         * @param mapType The type of material map.
         */
        void SetColorFromAssimpMaterial(const aiMaterial* material, const char* colorKey, int type, int index, MapType mapType);

      public:
        /**
         * @brief Default constructor for Material.
         * @param ctx The context object.
         */
        Material(T_Context& ctx);

        /**
         * @brief Constructor for loading Material from Assimp material data.
         * @param ctx The context object.
         * @param material The Assimp material.
         * @param scene The Assimp scene.
         */
        Material(T_Context& ctx, const aiMaterial* material, const aiScene* scene);

        // Getter methods
        const T_Texture* GetTexture(int mapType) const { return maps[mapType].texture.has_value() ? &(*maps[mapType].texture) : nullptr; }
        T_Texture* GetTexture(int mapType) { return maps[mapType].texture.has_value() ? &(*maps[mapType].texture) : nullptr; }
        nexus::gfx::Color GetColor(int mapType) const { return maps[mapType].color; }
        float GetValue(int mapType) const { return maps[mapType].value; }

        // Setter methods
        void SetTexture(int mapType, const T_Texture& texture) { maps[mapType].texture = texture; }
        void SetColor(int mapType, const nexus::gfx::Color& color) { maps[mapType].color = color; }
        void SetValue(int mapType, float value) { maps[mapType].value = value; }
    };


    /* Protected Implementation Material */

    template <typename T_Context, typename T_Shader, typename T_Texture>
    void Material<T_Context, T_Shader, T_Texture>::SetTextureFromAssimpMaterial(T_Context& ctx, const aiMaterial* material, const aiScene* scene, aiTextureType textureType, MapType mapType, bool loadDefaultTexture)
    {
        if (aiGetMaterialTextureCount(material, textureType) > 0)
        {
            Uint8 textureIndex = 0;
            aiString path;

            if (aiGetMaterialTexture(material, textureType, textureIndex, &path, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) == aiReturn_SUCCESS)
            {
                constexpr char embedded = '*';

                // Embedded texture
                if (std::strncmp(path.data, &embedded, sizeof(embedded)) == 0)
                {
                    unsigned int index = std::atoi(nexus::core::TextSubtext(path.data, 1, path.length).c_str());
                    struct aiTexture *embeddedTexture = scene->mTextures[index];

                    nexus::gfx::Surface image(embeddedTexture->pcData, embeddedTexture->mHeight == 0
                        ? embeddedTexture->mWidth * 4 : embeddedTexture->mWidth * embeddedTexture->mHeight * 4);

                    maps[mapType].texture = T_Texture(ctx, image);
                }
                else
                {
                    try
                    {
                        maps[mapType].texture = T_Texture(ctx, path.data);
                    }
                    catch (const nexus::core::NexusException& e)
                    {
                        NEXUS_LOG(Warning) << "[Material] " << e.what() << "\n";
                        NEXUS_LOG(Warning) << "The default texture will be loaded instead\n";
                        maps[mapType].texture = T_Texture(ctx);
                    }
                }
            }
        }
        else if (loadDefaultTexture)
        {
            maps[mapType].texture = T_Texture(ctx);
        }
    }

    template <typename T_Context, typename T_Shader, typename T_Texture>
    void Material<T_Context, T_Shader, T_Texture>::SetColorFromAssimpMaterial(const aiMaterial* material, const char* colorKey, int type, int index, MapType mapType)
    {
        aiColor4D aiCol;
        if (aiGetMaterialColor(material, colorKey, type, index, &aiCol) == aiReturn_SUCCESS)
        {
            maps[mapType].color = nexus::gfx::Color(255 * aiCol.r, 255 * aiCol.g, 255 * aiCol.b, 255 * aiCol.a);
        }
    }


    /* Public Implementation Material */

    template <typename T_Context, typename T_Shader, typename T_Texture>
    Material<T_Context, T_Shader, T_Texture>::Material(T_Context& ctx)
    : nexus::utils::Contextual<T_Context>(ctx), shader(ctx)
    {
        maps[Diffuse].texture = T_Texture(ctx);
        maps[Diffuse].color = nexus::gfx::White;
        maps[Specular].color = nexus::gfx::White;
    }

    template <typename T_Context, typename T_Shader, typename T_Texture>
    Material<T_Context, T_Shader, T_Texture>::Material(T_Context& ctx, const aiMaterial* material, const aiScene* scene)
    : nexus::utils::Contextual<T_Context>(ctx), shader(ctx)
    {
        SetColorFromAssimpMaterial(material, AI_MATKEY_COLOR_DIFFUSE, Diffuse);
        SetColorFromAssimpMaterial(material, AI_MATKEY_COLOR_SPECULAR, Specular);
        SetColorFromAssimpMaterial(material, AI_MATKEY_COLOR_EMISSIVE, Emission);
        //SetColorFromAssimpMaterial(material, AI_MATKEY_COLOR_AMBIENT, Specular);
        //SetColorFromAssimpMaterial(material, AI_MATKEY_COLOR_TRANSPARENT, MapType);
        //SetColorFromAssimpMaterial(material, AI_MATKEY_COLOR_REFLECTIVE, MapType);

        SetTextureFromAssimpMaterial(ctx, material, scene, aiTextureType_DIFFUSE, Diffuse, true);
        SetTextureFromAssimpMaterial(ctx, material, scene, aiTextureType_SPECULAR, Specular);
        SetTextureFromAssimpMaterial(ctx, material, scene, aiTextureType_NORMALS, Normal);
        //SetTextureFromAssimpMaterial(ctx, material, scene, aiTextureType_AMBIENT, Specular);
        SetTextureFromAssimpMaterial(ctx, material, scene, aiTextureType_SHININESS, Roughness);
    }

}

#endif //NEXUS_GAPI_IMPL_SP_MODEL_MATERIAL_HPP
