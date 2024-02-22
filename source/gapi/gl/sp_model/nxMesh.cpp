#include "gapi/gl/sp_model/nxMesh.hpp"

#include "gapi/gl/nxEnums.hpp"
#include "gfx/nxColor.hpp"
#include "math/nxMat4.hpp"
#include "math/nxVec3.hpp"
#include <unordered_map>

using namespace nexus;

namespace {
    template <typename T>
    constexpr std::size_t VectorSize(const std::vector<T>& v)
    {
        return v.size() * sizeof(typename std::remove_reference<decltype(v)>::type::value_type);
    }
}

/* Private Mesh Implementation */

_gl_impl::Mesh::~Mesh()
{
    if (vaoId > 0)
    {
        ctx.UnloadVertexArray(vaoId);
        vaoId = 0;

        for (auto& vbo : vboId)
        {
            ctx.UnloadVertexBuffer(vbo);
            vbo = 0;
        }
    }
}

void _gl_impl::Mesh::UpdateAnimation(std::vector<_gapi_impl::BoneInfo>& boneInfos)
{
    if (_gapi_impl::Mesh<gl::Context, gl::Material>::UpdateAnimation(boneInfos))
    {
        ctx.UpdateVertexBuffer(vboId[0], animVertices.data(), VectorSize(animVertices), 0); // Update vertex position

        if (!normals.empty())
        {
            ctx.UpdateVertexBuffer(vboId[2], animNormals.data(), VectorSize(animNormals), 0);  // Update vertex normals
        }
    }
}

void _gl_impl::Mesh::Upload(bool dynamic)
{
    if (vaoId != 0)
    {
        NEXUS_LOG(Warning) << "Trying to re-load an already loaded mesh\n";
        return;
    }

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

    vaoId = ctx.LoadVertexArray();
    ctx.EnableVertexArray(vaoId);

    // NOTE: Vertex attributes must be uploaded considering default locations points and available vertex data

    // Enable vertex attributes: position (shader-location = 0)
    const auto &verts = (!animVertices.empty() ? animVertices : vertices);
    vboId[0] = ctx.LoadVertexBuffer(verts.data(), VectorSize(verts), dynamic);
    ctx.SetVertexAttribute(0, 3, gl::DataType::Float, 0, 0, 0);
    ctx.EnableVertexAttribute(0);

    // WARNING: When setting default vertex attribute values, the values for each generic vertex attribute
    // is part of current state, and it is maintained even if a different program object is used

    if (!texcoords.empty())
    {
        // Enable vertex attributes: texcoords (shader-location = 1)
        vboId[1] = ctx.LoadVertexBuffer(texcoords.data(), VectorSize(texcoords), dynamic);
        ctx.SetVertexAttribute(1, 2, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(1);
    }
    else
    {
        // Default vertex attribute: texcoord
        // WARNING: Default value provided to shader if location available
        constexpr float value[2] = { 0.0f, 0.0f };
        ctx.SetVertexAttributeDefault(1, value, gl::ShaderAttributeType::Vec2, 2);
        ctx.DisableVertexAttribute(1);
    }

    if (!normals.empty())
    {
        // Enable vertex attributes: normals (shader-location = 2)
        const auto &norms = (!animNormals.empty() ? animNormals : normals);
        vboId[2] = ctx.LoadVertexBuffer(norms.data(), VectorSize(norms), dynamic);
        ctx.SetVertexAttribute(2, 3, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(2);
    }
    else
    {
        // Default vertex attribute: normal
        // WARNING: Default value provided to shader if location available
        constexpr float value[3] = { 1.0f, 1.0f, 1.0f };
        ctx.SetVertexAttributeDefault(2, value, gl::ShaderAttributeType::Vec3, 3);
        ctx.DisableVertexAttribute(2);
    }

    if (!colors.empty())
    {
        // Enable vertex attribute: color (shader-location = 3)
        vboId[3] = ctx.LoadVertexBuffer(colors.data(), VectorSize(colors), dynamic);
        ctx.SetVertexAttribute(3, 4, gl::DataType::UnsignedByte, 1, 0, 0);
        ctx.EnableVertexAttribute(3);
    }
    else
    {
        // Default vertex attribute: color
        // WARNING: Default value provided to shader if location available
        constexpr float value[4] = { 1.0f, 1.0f, 1.0f, 1.0f };    // WHITE
        ctx.SetVertexAttributeDefault(3, value, gl::ShaderAttributeType::Vec4, 4);
        ctx.DisableVertexAttribute(3);
    }

    if (!tangents.empty())
    {
        // Enable vertex attribute: tangent (shader-location = 4)
        vboId[4] = ctx.LoadVertexBuffer(tangents.data(), VectorSize(tangents), dynamic);
        ctx.SetVertexAttribute(4, 4, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(4);
    }
    else
    {
        // Default vertex attribute: tangent
        // WARNING: Default value provided to shader if location available
        constexpr float value[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        ctx.SetVertexAttributeDefault(4, value, gl::ShaderAttributeType::Vec4, 4);
        ctx.DisableVertexAttribute(4);
    }

    if (!texcoords2.empty())
    {
        // Enable vertex attribute: texcoord2 (shader-location = 5)
        vboId[5] = ctx.LoadVertexBuffer(texcoords2.data(), VectorSize(texcoords2), dynamic);
        ctx.SetVertexAttribute(5, 2, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(5);
    }
    else
    {
        // Default vertex attribute: texcoord2
        // WARNING: Default value provided to shader if location available
        constexpr float value[2] = { 0.0f, 0.0f };
        ctx.SetVertexAttributeDefault(5, value, gl::ShaderAttributeType::Vec2, 2);
        ctx.DisableVertexAttribute(5);
    }

    if (!indices.empty())
    {
        vboId[6] = ctx.LoadVertexBufferElement(indices.data(), numFaces*3*sizeof(Uint16), dynamic);
    }

    ctx.DisableVertexArray();
#endif
}

void _gl_impl::Mesh::Update(Uint8 buffer)
{
    if (buffer > VertexBufferCount)
    {
        NEXUS_LOG(Warning) << "gl::Mesh::Update(" << buffer << ") - Buffer index out of range, must be less than " << VertexBufferCount << "\n";
        return;
    }

    const void *vbuf = nullptr;
    size_t size = 0;

    switch (buffer)
    {
        case 0: {
            vbuf = vertices.data();
            size = VectorSize(vertices);
        } break;

        case 1: {
            vbuf = texcoords.data();
            size = VectorSize(texcoords);
        } break;

        case 2: {
            vbuf = normals.data();
            size = VectorSize(normals);
        } break;

        case 3: {
            vbuf = colors.data();
            size = VectorSize(colors);
        } break;

        case 4: {
            vbuf = tangents.data();
            size = VectorSize(tangents);
        } break;

        case 5: {
            vbuf = texcoords2.data();
            size = VectorSize(texcoords2);
        } break;

        case 6: {
            vbuf = indices.data();
            size = VectorSize(texcoords2);
        } break;

        default: break;
    }

    if (vbuf && size > 0)
    {
        ctx.UpdateVertexBuffer(vboId[buffer], vbuf, size, 0);
    }
}

void _gl_impl::Mesh::GenTangents(bool upload)
{
    _gapi_impl::Mesh<gl::Context, gl::Material>::GenTangents();

    if (upload)
    {
        if (vboId[gl::LocVertexTangent] != 0)
        {
            // Update existing vertex buffer
            ctx.UpdateVertexBuffer(vboId[gl::LocVertexTangent], tangents.data(), numVertices*sizeof(math::Vec4), 0);
        }
        else
        {
            // Load a new tangent attributes buffer
            vboId[gl::LocVertexTangent] = ctx.LoadVertexBuffer(tangents.data(), numVertices*sizeof(math::Vec4), false);
        }

        ctx.EnableVertexArray(vaoId);
            ctx.SetVertexAttribute(4, 4, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(4);
        ctx.DisableVertexArray();
    }
}

void _gl_impl::Mesh::Draw(const gl::Material& material, const math::Mat4& transform)
{
#if defined(GRAPHICS_API_OPENGL_11)
    #define GL_VERTEX_ARRAY         0x8074
    #define GL_NORMAL_ARRAY         0x8075
    #define GL_COLOR_ARRAY          0x8076
    #define GL_TEXTURE_COORD_ARRAY  0x8078

    ctx.EnableTexture(material->maps[Matrerial::Diffuse].texture.GetID());

    ctx.EnableStatePointer(GL_VERTEX_ARRAY, vertices.data());
    ctx.EnableStatePointer(GL_TEXTURE_COORD_ARRAY, texcoords).data();
    ctx.EnableStatePointer(GL_NORMAL_ARRAY, normals.data());
    ctx.EnableStatePointer(GL_COLOR_ARRAY, colors.data());

    ctx.PushMatrix();
        ctx.MultMatrix(transform);
        ctx.Color(material->maps[Matrerial::Diffuse].color);

        if (!indices.empty()) ctx.DrawVertexArrayElements(0, numFaces*3, indices.data());
        else ctx.DrawVertexArray(0, numVertices);
    ctx.PopMatrix();

    ctx.DisableStatePointer(GL_VERTEX_ARRAY);
    ctx.DisableStatePointer(GL_TEXTURE_COORD_ARRAY);
    ctx.DisableStatePointer(GL_NORMAL_ARRAY);
    ctx.DisableStatePointer(GL_COLOR_ARRAY);

    ctx.DisableTexture();
#endif

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    const nexus::gl::Shader &shader = material->shader;

    // Bind shader program
    ctx.EnableShader(shader->GetID());

    // Send required data to shader (matrices, values)
    //-----------------------------------------------------
    // Upload to shader material.colDiffuse
    if (shader->locs[gl::LocColorDiffuse] != -1)
    {
        const math::Vec4 ncol = material->maps[Material::MapType::Diffuse].color.Normalized();
        ctx.SetUniform(shader->locs[gl::LocColorDiffuse], reinterpret_cast<const float*>(&ncol), gl::ShaderUniformType::Vec4, 1);
    }

    // Upload to shader material.colSpecular (if location available)
    if (shader->locs[gl::LocColorSpecular] != -1)
    {
        const math::Vec4 ncol = material->maps[Material::MapType::Specular].color.Normalized();
        ctx.SetUniform(shader->locs[gl::LocColorSpecular], reinterpret_cast<const float*>(&ncol), gl::ShaderUniformType::Vec4, 1);
    }

    // Get a copy of current matrices to work with,
    // just in case stereo render is required, and we need to modify them
    // NOTE: At this point the modelview matrix just contains the view matrix (camera)
    // That's because BeginMode3D() sets it and there is no model-drawing function
    // that modifies it, all use ctx.PushMatrix() and ctx.PopMatrix()
    math::Mat4 matModel = math::Mat4::Identity();
    math::Mat4 matView(ctx.GetMatrixModelview());
    math::Mat4 matModelView = math::Mat4::Identity();
    math::Mat4 matProjection(ctx.GetMatrixProjection());

    // Upload view and projection matrices (if locations available)
    if (shader->locs[gl::LocMatrixView] != -1)
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixView], matView.m);

    if (shader->locs[gl::LocMatrixProjection] != -1)
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixProjection], matProjection.m);

    // Model transformation matrix is sent to shader uniform location: SHADER_LOC_MATRIX_MODEL
    if (shader->locs[gl::LocMatrixModel] != -1)
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixModel], transform.m);

    // Accumulate several model transformations:
    //    transform: model transformation provided (includes DrawModel() params combined with model.transform)
    //    rlGetMatrixTransform(): rlgl internal transform matrix due to push/pop matrix stack
    matModel = transform * ctx.GetMatrixTransform().m;

    // Get model-view matrix
    matModelView = matModel * matView;

    // Upload model normal matrix (if locations available)
    if (shader->locs[gl::LocMatrixNormal] != -1)
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixNormal], matModel.Invert().Transpose().m);
    //-----------------------------------------------------

    // Bind active texture maps (if available)
    for (int i = 0; i < material.MapCount; i++)
    {
        const auto &map = material->maps[i];

        if (map.texture.has_value())
        {
            // Select current shader texture slot
            ctx.ActiveTextureSlot(i);

            // Enable texture for active slot
            if (i == Material::MapType::Irradiance || i == Material::MapType::Prefilter || i == Material::MapType::Cubemap)
            {
                ctx.EnableTextureCubemap(map.texture.value()->GetID());
            }
            else
            {
                ctx.EnableTexture(map.texture.value()->GetID());
            }

            ctx.SetUniform(shader->locs[gl::LocMapDiffuse + i],
                &i, gl::ShaderUniformType::Sampler2D, 1);
        }
    }

    // Try binding vertex array objects (VAO) or use VBOs if not possible
    // WARNING: UploadMesh() enables all vertex attributes available in mesh and sets default attribute values
    // for shader expected vertex attributes that are not provided by the mesh (i.e. colors)
    // This could be a dangerous approach because different meshes with different shaders can enable/disable some attributes
    if (!ctx.EnableVertexArray(vaoId))
    {
        // Bind mesh VBO data: vertex position (shader-location = 0)
        ctx.EnableVertexBuffer(vboId[0]);
        ctx.SetVertexAttribute(shader->locs[gl::LocVertexPosition], 3, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(shader->locs[gl::LocVertexPosition]);

        // Bind mesh VBO data: vertex texcoords (shader-location = 1)
        ctx.EnableVertexBuffer(vboId[1]);
        ctx.SetVertexAttribute(shader->locs[gl::LocVertexTexCoord01], 2, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(shader->locs[gl::LocVertexTexCoord01]);

        if (shader->locs[gl::LocVertexNormal] != -1)
        {
            // Bind mesh VBO data: vertex normals (shader-location = 2)
            ctx.EnableVertexBuffer(vboId[2]);
            ctx.SetVertexAttribute(shader->locs[gl::LocVertexNormal], 3, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(shader->locs[gl::LocVertexNormal]);
        }

        // Bind mesh VBO data: vertex colors (shader-location = 3, if available)
        if (shader->locs[gl::LocVertexColor] != -1)
        {
            if (vboId[3] != 0)
            {
                ctx.EnableVertexBuffer(vboId[3]);
                ctx.SetVertexAttribute(shader->locs[gl::LocVertexColor], 4, gl::DataType::UnsignedByte, 1, 0, 0);
                ctx.EnableVertexAttribute(shader->locs[gl::LocVertexColor]);
            }
            else
            {
                // Set default value for defined vertex attribute in shader but not provided by mesh
                // WARNING: It could result in GPU undefined behaviour
                constexpr float value[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                ctx.SetVertexAttributeDefault(shader->locs[gl::LocVertexColor], value, gl::ShaderAttributeType::Vec4, 4);
                ctx.DisableVertexAttribute(shader->locs[gl::LocVertexColor]);
            }
        }

        // Bind mesh VBO data: vertex tangents (shader-location = 4, if available)
        if (shader->locs[gl::LocVertexTangent] != -1)
        {
            ctx.EnableVertexBuffer(vboId[4]);
            ctx.SetVertexAttribute(shader->locs[gl::LocVertexTangent], 4, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(shader->locs[gl::LocVertexTangent]);
        }

        // Bind mesh VBO data: vertex texcoords2 (shader-location = 5, if available)
        if (shader->locs[gl::LocVertexTexCoord02] != -1)
        {
            ctx.EnableVertexBuffer(vboId[5]);
            ctx.SetVertexAttribute(shader->locs[gl::LocVertexTexCoord02], 2, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(shader->locs[gl::LocVertexTexCoord02]);
        }

        if (!indices.empty()) ctx.EnableVertexBufferElement(vboId[6]);
    }

    // WARNING: Disable vertex attribute color input if mesh can not provide that data (despite location being enabled in shader)
    if (vboId[3] == 0) ctx.DisableVertexAttribute(shader->locs[gl::LocVertexColor]);

    int eyeCount = 1;
    if (ctx.IsStereoRenderEnabled()) eyeCount = 2;

    for (int eye = 0; eye < eyeCount; eye++)
    {
        // Calculate model-view-projection matrix (MVP)
        math::Mat4 matModelViewProjection = math::Mat4::Identity();
        if (eyeCount == 1)
        {
            matModelViewProjection = matModelView * matProjection;
        }
        else
        {
            // Setup current eye viewport (half screen width)
            ctx.Viewport(eye*ctx.GetFramebufferWidth()/2, 0, ctx.GetFramebufferWidth()/2, ctx.GetFramebufferHeight());
            matModelViewProjection = (matModelView * ctx.GetMatrixViewOffsetStereo(eye).m) * ctx.GetMatrixProjectionStereo(eye).m;
        }

        // Send combined model-view-projection matrix to shader
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixMVP], matModelViewProjection.m);

        // Draw mesh
        if (!indices.empty()) ctx.DrawVertexArrayElements(0, numFaces*3, 0);
        else ctx.DrawVertexArray(0, numVertices);
    }

    // Unbind all bound texture maps
    for (Uint8 i = 0; i < material.MapCount; i++)
    {
        const auto &map = material->maps[i];

        if (map.texture != std::nullopt)
        {
            // Select current shader texture slot
            ctx.ActiveTextureSlot(i);

            // Enable texture for active slot
            if (i == Material::MapType::Irradiance || i == Material::MapType::Prefilter || i == Material::MapType::Cubemap)
            {
                ctx.DisableTextureCubemap();
            }
            else
            {
                ctx.DisableTexture();
            }
        }
    }

    // Disable all possible vertex array objects (or VBOs)
    ctx.DisableVertexArray();
    ctx.DisableVertexBuffer();
    ctx.DisableVertexBufferElement();

    // Disable shader program
    ctx.DisableShader();

    // Restore rlgl internal modelview and projection matrices
    ctx.SetMatrixModelview(matView.m);
    ctx.SetMatrixProjection(matProjection.m);
#endif
}

void _gl_impl::Mesh::Draw(const gl::Material& material, const std::vector<math::Mat4>& transforms)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    const nexus::gl::Shader &shader = material->shader;

    // Bind shader program
    ctx.EnableShader(shader->GetID());

    // Send required data to shader (matrices, values)
    //-----------------------------------------------------
    // Upload to shader material.colDiffuse
    if (shader->locs[gl::LocColorDiffuse] != -1)
    {
        const math::Vec4 ncol = material->maps[Material::MapType::Diffuse].color.Normalized();
        ctx.SetUniform(shader->locs[gl::LocColorDiffuse], reinterpret_cast<const float*>(&ncol), gl::ShaderUniformType::Vec4, 1);
    }

    // Upload to shader material.colSpecular (if location available)
    if (shader->locs[gl::LocColorSpecular] != -1)
    {
        const math::Vec4 ncol = material->maps[Material::MapType::Specular].color.Normalized();
        ctx.SetUniform(shader->locs[gl::LocColorSpecular], reinterpret_cast<const float*>(&ncol), gl::ShaderUniformType::Vec4, 1);
    }

    // Get a copy of current matrices to work with,
    // just in case stereo render is required, and we need to modify them
    // NOTE: At this point the modelview matrix just contains the view matrix (camera)
    // That's because BeginMode3D() sets it and there is no model-drawing function
    // that modifies it, all use ctx.PushMatrix() and ctx.PopMatrix()
    math::Mat4 matModel = math::Mat4::Identity();
    math::Mat4 matView(ctx.GetMatrixModelview());
    math::Mat4 matModelView = math::Mat4::Identity();
    math::Mat4 matProjection(ctx.GetMatrixProjection());

    // Upload view and projection matrices (if locations available)
    if (shader->locs[gl::LocMatrixView] != -1)
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixView], matView.m);

    if (shader->locs[gl::LocMatrixProjection] != -1)
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixProjection], matProjection.m);

    // Enable mesh VAO to attach new buffer
    ctx.EnableVertexArray(vaoId);

    // This could alternatively use a static VBO and either glMapBuffer() or glBufferSubData().
    // It isn't clear which would be reliably faster in all cases and on all platforms,
    // anecdotally glMapBuffer() seems very slow (syncs) while glBufferSubData() seems
    // no faster, since we're transferring all the transform matrices anyway
    Uint32 instancesVboId = ctx.LoadVertexBuffer(transforms.data(), transforms.size() * sizeof(math::Mat4), false);

    // Instances transformation matrices are send to shader attribute location: SHADER_LOC_MATRIX_MODEL
    for (Uint8 i = 0; i < 4; i++)
    {
        ctx.EnableVertexAttribute(shader->locs[gl::LocMatrixModel] + i);
        ctx.SetVertexAttribute(shader->locs[gl::LocMatrixModel] + i, 4, gl::DataType::Float, 0, sizeof(math::Mat4), reinterpret_cast<const void*>(i*sizeof(math::Vec4)));
        ctx.SetVertexAttributeDivisor(shader->locs[gl::LocMatrixModel] + i, 1);
    }

    ctx.DisableVertexBuffer();
    ctx.DisableVertexArray();

    // Accumulate internal matrix transform (push/pop) and view matrix
    // NOTE: In this case, model instance transformation must be computed in the shader
    matModelView = static_cast<math::Mat4>(ctx.GetMatrixTransform()) * matView;

    // Upload model normal matrix (if locations available)
    if (shader->locs[gl::LocMatrixNormal] != -1)
    {
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixNormal], matModel.Invert().Transpose().m);
    }
    //-----------------------------------------------------

    // Bind active texture maps (if available)
    for (int i = 0; i < material->maps.size(); i++)
    {
        const auto &map = material->maps[i];

        if (map.texture.has_value())
        {
            // Select current shader texture slot
            ctx.ActiveTextureSlot(i);

            // Enable texture for active slot
            if (i == Material::MapType::Irradiance || i == Material::MapType::Prefilter || i == Material::MapType::Cubemap)
            {
                ctx.EnableTextureCubemap(map.texture.value()->GetID());
            }
            else
            {
                ctx.EnableTexture(map.texture.value()->GetID());
            }

            ctx.SetUniform(shader->locs[gl::LocMapDiffuse + i],
                &i, gl::ShaderUniformType::Sampler2D, 1);
        }
    }

    // Try binding vertex array objects (VAO)
    // or use VBOs if not possible
    if (!ctx.EnableVertexArray(vaoId))
    {
        // Bind mesh VBO data: vertex position (shader-location = 0)
        ctx.EnableVertexBuffer(vboId[0]);
        ctx.SetVertexAttribute(shader->locs[gl::LocVertexPosition], 3, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(shader->locs[gl::LocVertexPosition]);

        // Bind mesh VBO data: vertex texcoords (shader-location = 1)
        ctx.EnableVertexBuffer(vboId[1]);
        ctx.SetVertexAttribute(shader->locs[gl::LocVertexTexCoord01], 2, gl::DataType::Float, 0, 0, 0);
        ctx.EnableVertexAttribute(shader->locs[gl::LocVertexTexCoord01]);

        if (shader->locs[gl::LocVertexNormal] != -1)
        {
            // Bind mesh VBO data: vertex normals (shader-location = 2)
            ctx.EnableVertexBuffer(vboId[2]);
            ctx.SetVertexAttribute(shader->locs[gl::LocVertexNormal], 3, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(shader->locs[gl::LocVertexNormal]);
        }

        // Bind mesh VBO data: vertex colors (shader-location = 3, if available)
        if (shader->locs[gl::LocVertexColor] != -1)
        {
            if (vboId[3] != 0)
            {
                ctx.EnableVertexBuffer(vboId[3]);
                ctx.SetVertexAttribute(shader->locs[gl::LocVertexColor], 4, gl::DataType::UnsignedByte, 1, 0, 0);
                ctx.EnableVertexAttribute(shader->locs[gl::LocVertexColor]);
            }
            else
            {
                // Set default value for defined vertex attribute in shader but not provided by mesh
                // WARNING: It could result in GPU undefined behaviour
                constexpr float value[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                ctx.SetVertexAttributeDefault(shader->locs[gl::LocVertexColor], value, gl::ShaderAttributeType::Vec4, 4);
                ctx.DisableVertexAttribute(shader->locs[gl::LocVertexColor]);
            }
        }

        // Bind mesh VBO data: vertex tangents (shader-location = 4, if available)
        if (shader->locs[gl::LocVertexTangent] != -1)
        {
            ctx.EnableVertexBuffer(vboId[4]);
            ctx.SetVertexAttribute(shader->locs[gl::LocVertexTangent], 4, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(shader->locs[gl::LocVertexTangent]);
        }

        // Bind mesh VBO data: vertex texcoords2 (shader-location = 5, if available)
        if (shader->locs[gl::LocVertexTexCoord02] != -1)
        {
            ctx.EnableVertexBuffer(vboId[5]);
            ctx.SetVertexAttribute(shader->locs[gl::LocVertexTexCoord02], 2, gl::DataType::Float, 0, 0, 0);
            ctx.EnableVertexAttribute(shader->locs[gl::LocVertexTexCoord02]);
        }

        if (!indices.empty()) ctx.EnableVertexBufferElement(vboId[6]);
    }

    // WARNING: Disable vertex attribute color input if mesh can not provide that data (despite location being enabled in shader)
    if (vboId[3] == 0) ctx.DisableVertexAttribute(shader->locs[gl::LocVertexColor]);

    int eyeCount = 1;
    if (ctx.IsStereoRenderEnabled()) eyeCount = 2;

    for (int eye = 0; eye < eyeCount; eye++)
    {
        // Calculate model-view-projection matrix (MVP)
        math::Mat4 matModelViewProjection = math::Mat4::Identity();
        if (eyeCount == 1)
        {
            matModelViewProjection = matModelView * matProjection;
        }
        else
        {
            // Setup current eye viewport (half screen width)
            ctx.Viewport(eye*ctx.GetFramebufferWidth()/2, 0, ctx.GetFramebufferWidth()/2, ctx.GetFramebufferHeight());
            matModelViewProjection = (matModelView * ctx.GetMatrixViewOffsetStereo(eye).m) * ctx.GetMatrixProjectionStereo(eye).m;
        }

        // Send combined model-view-projection matrix to shader
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixMVP], matModelViewProjection.m);

        // Draw mesh
        if (!indices.empty()) ctx.DrawVertexArrayElements(0, numFaces*3, 0);
        else ctx.DrawVertexArray(0, numVertices);
    }

    // Unbind all bound texture maps
    for (int i = 0; i < material->maps.size(); i++)
    {
        if (material->maps[i].texture != std::nullopt)
        {
            // Select current shader texture slot
            ctx.ActiveTextureSlot(i);

            // Enable texture for active slot
            if (i == Material::MapType::Irradiance || i == Material::MapType::Prefilter || i == Material::MapType::Cubemap)
            {
                ctx.DisableTextureCubemap();
            }
            else
            {
                ctx.DisableTexture();
            }
        }
    }

    // Disable all possible vertex array objects (or VBOs)
    ctx.DisableVertexArray();
    ctx.DisableVertexBuffer();
    ctx.DisableVertexBufferElement();

    // Disable shader program
    ctx.DisableShader();

    // Remove instance transforms buffer
    ctx.UnloadVertexBuffer(instancesVboId);
#endif
}
