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

#include "gapi/gl/nxExtensions.hpp"
#include "gapi/gl/nxContext.hpp"
#include "gapi/gl/nxTexture.hpp"
#include "gapi/gl/nxWindow.hpp"
#include "core/nxException.hpp"
#include "gapi/gl/nxEnums.hpp"
#include "core/nxText.hpp"
#include "math/nxMath.hpp"
#include "core/nxLog.hpp"

using namespace nexus;

// Implementation: Attributes

void gl::Attributes::Apply() const
{
#   if defined(GRAPHICS_API_OPENGL_21)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#   elif defined(GRAPHICS_API_OPENGL_33)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#       if defined(__APPLE__)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // OSX Requires forward compatibility
#       else
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#       endif
#   elif defined(GRAPHICS_API_OPENGL_43)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#   elif defined(GRAPHICS_API_OPENGL_ES2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#   elif defined(GRAPHICS_API_OPENGL_ES3)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#   endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffer ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, redSize);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, greenSize);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, blueSize);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alphaSize);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, bufferSize);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthSize);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilSize);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, accumRedSize);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, accumGreenSize);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, accumBlueSize);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, accumAlphaSize);
    SDL_GL_SetAttribute(SDL_GL_STEREO, stereo ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, multiSampleBuffers);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multiSampleSamples);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, acceleratedVisual ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, retainedBacking ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, contextProfileMask);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, shareWithCurrentContext ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, frameBufferSRGBCapable ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, contextReleaseBehavior);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, contextResetNotification);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, contextNoError ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_FLOATBUFFERS, floatBuffers ? 1 : 0);
}

/* Private Implementation CXX_SDL_GLContext */

_gl_impl::CXX_SDL_GLContext::CXX_SDL_GLContext(SDL_Window* window, const std::shared_ptr<SDL_GLContext>& _glCtx) : glCtx(_glCtx)
{

    if (glCtx.get() == nullptr)
    {
        glCtx = std::make_shared<SDL_GLContext>(SDL_GL_CreateContext(window));

        if (glCtx.get() == nullptr) throw nexus::core::NexusException(
            "GLContext", "Failed to create an OpenGL context");
    }
    else
    {
        SDL_GL_MakeCurrent(window, *glCtx);
    }
}

_gl_impl::CXX_SDL_GLContext::~CXX_SDL_GLContext()
{
    if (*glCtx != nullptr && glCtx.use_count() == 1)
    {
        SDL_GL_DeleteContext(*glCtx);
        glCtx = nullptr;
    }
}

/* Public Implementation Context */

gl::Context::Context(gl::Window& window, Context* other)
: _gl_impl::CXX_SDL_GLContext(window, other ? other->glCtx : nullptr), state{}
{
    // Load OpenGL extensions automatically if a loader is given
    if (!IsExtensionsLoaded())
    {
        LoadExtensions(SDL_GL_GetProcAddress);
    }

    // Enable OpenGL debug context if required
#   if defined(RLGL_ENABLE_OPENGL_DEBUG_CONTEXT) && defined(GRAPHICS_API_OPENGL_43)

        if ((glDebugMessageCallback != nullptr) && (glDebugMessageControl != nullptr))
        {
            glDebugMessageCallback(rlDebugMessageCallback, 0);
            // glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, 0, GL_TRUE);

            // Debug context options:
            //  - GL_DEBUG_OUTPUT - Faster version but not useful for breakpoints
            //  - GL_DEBUG_OUTPUT_SYNCHRONUS - Callback is in sync with errors, so a breakpoint can be placed on the callback in order to get a stacktrace for the GL error
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }

#   endif

#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

        // Init default white texture
        constexpr Uint8 pixels[4] = { 255, 255, 255, 255 };   // 1 pixel RGBA (4 bytes)
        state.defaultTextureId = LoadTexture(pixels, 1, 1, TextureFormat::RGBA8888, 1);

        if (state.defaultTextureId != 0)
        {
            NEXUS_LOG(Info) << "[gl::Context::Context] [ID " << state.defaultTextureId << "] Default texture loaded successfully\n";
        }
        else
        {
            throw core::NexusException("gl::Context::Context", "Failed to load default texture");
        }

        // Init default Shader (customized for GL 3.3 and ES2)
        // Loaded: state.defaultShaderId + state.defaultShaderLocs
        LoadShaderDefault();
        state.currentShaderId = state.defaultShaderId;
        state.currentShaderLocs = state.defaultShaderLocs;

        // Init default vertex arrays buffers
        defaultBatch = std::make_unique<RenderBatch>(*this, GL_DEFAULT_BATCH_BUFFERS, GL_DEFAULT_BATCH_BUFFER_ELEMENTS, GL_DEFAULT_BATCH_DRAWCALLS);
        currentBatch = defaultBatch.get();

        // Init counters
        state.stackCounter = 0;

        // Init stack matrices (emulating OpenGL 1.1)
        std::fill(state.stack, state.stack + GL_MAX_MATRIX_STACK_SIZE, math::Mat4::Identity());

        // Init internal matrices
        state.transform = math::Mat4::Identity();
        state.projection = math::Mat4::Identity();
        state.modelview = math::Mat4::Identity();
        state.currentMatrix = &state.modelview;

#   endif  // GRAPHICS_API_OPENGL_33 || GRAPHICS_API_OPENGL_ES2

    // Initialize OpenGL default states
    //----------------------------------------------------------
    // Init state: Depth test
    glDepthFunc(GL_LEQUAL);                                 // Type of depth testing to apply
    glDisable(GL_DEPTH_TEST);                               // Disable depth testing for 2D (only used for 3D)

    // Init state: Blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Color blending function (how colors are mixed)
    glEnable(GL_BLEND);                                     // Enable color blending (required to work with transparencies)

    // Init state: Culling
    // NOTE: All shapes/models triangles are drawn CCW
    glCullFace(GL_BACK);                                    // Cull the back face (default)
    glFrontFace(GL_CCW);                                    // Front face are defined counter clockwise (default)
    glEnable(GL_CULL_FACE);                                 // Enable backface culling

    // Init state: Cubemap seamless
#   if defined(GRAPHICS_API_OPENGL_33)
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);             // Seamless cubemaps (not supported on OpenGL ES 2.0)
#   endif

#   if defined(GRAPHICS_API_OPENGL_11)
        // Init state: Color hints (deprecated in OpenGL 3.0+)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Improve quality of color and texture coordinate interpolation
        glShadeModel(GL_SMOOTH);                                // Smooth shading between vertex (vertex colors interpolation)
#   endif

#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
        // Store screen size into global variables
        state.framebufferWidth = window.GetWidth();
        state.framebufferHeight = window.GetHeight();

        NEXUS_LOG(Info) << "[gl::Context::Context] Default OpenGL state initialized successfully\n";
        //----------------------------------------------------------
#   endif

    // Init state: Color/Depth buffers clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Set clear color (black)
    glClearDepth(1.0f);                                     // Set clear depth value (default)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear color and depth buffers (depth buffer required for 3D)
}

gl::Context::~Context()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

    UnloadShaderDefault();                          // Unload default shader
    glDeleteTextures(1, &state.defaultTextureId);   // Unload default texture

    NEXUS_LOG(Info) << "[gl::Context::~Context] [ID " << state.defaultTextureId << "] Default texture unloaded successfully\n";

#endif
}

// Choose the current matrix to be transformed
void gl::Context::MatrixMode(gl::MatrixMode mode)
{
#if defined(GRAPHICS_API_OPENGL_11)
    switch (mode)
    {
        case MatrixMode::Projection: glMatrixMode(GL_PROJECTION);   break;
        case MatrixMode::ModelView:  glMatrixMode(GL_MODELVIEW);    break;
        case MatrixMode::Texture:    glMatrixMode(GL_TEXTURE);      break;
        default:                                                    break;
    }
#else
    switch (mode)
    {
        case MatrixMode::Projection:
            state.currentMatrix = &state.projection;
            break;

        case MatrixMode::ModelView:
            state.currentMatrix = &state.modelview;
            break;

        case MatrixMode::Texture:
            // Not supported
            break;
    }

    state.currentMatrixMode = mode;
#endif
}

// Push the current matrix into state.stack
void gl::Context::PushMatrix()
{
#if defined(GRAPHICS_API_OPENGL_11)
    glPushMatrix();
#else
    if (state.stackCounter >= GL_MAX_MATRIX_STACK_SIZE)
    {
        throw core::NexusException("Context::PushMatrix", core::TextFormat("Matrix stack overflow (MAX=%i)", GL_MAX_MATRIX_STACK_SIZE));
    }

    if (state.currentMatrixMode == MatrixMode::ModelView)
    {
        state.transformRequired = true;
        state.currentMatrix = &state.transform;
    }

    state.stack[state.stackCounter++] = *state.currentMatrix;
#endif
}

// Pop lattest inserted matrix from state.stack
void gl::Context::PopMatrix()
{
#if defined(GRAPHICS_API_OPENGL_11)
    glPopMatrix();
#else
    if (state.stackCounter > 0)
    {
        *state.currentMatrix = state.stack[--state.stackCounter];
    }

    if (state.stackCounter == 0 && state.currentMatrixMode == MatrixMode::ModelView)
    {
        state.currentMatrix = &state.modelview;
        state.transformRequired = false;
    }
#endif
}

// Reset current matrix to identity matrix
void gl::Context::LoadIdentity()
{
#if defined(GRAPHICS_API_OPENGL_11)
    glLoadIdentity();
#else
    *state.currentMatrix = math::Mat4::Identity();
#endif
}

// Multiply the current matrix by a translation matrix
void gl::Context::Translate(float x, float y, float z)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glTranslatef(x, y, z);
#else
    // NOTE: We transpose matrix with multiplication order
    *state.currentMatrix = math::Mat4::Translate(x, y, z) * (*state.currentMatrix);
#endif
}

// Multiply the current matrix by a rotation matrix
// NOTE: The provided angle must be in degrees
void gl::Context::Rotate(float angle, float x, float y, float z)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glRotatef(angle, x, y, z);
#else
    // NOTE: We transpose matrix with multiplication order
    *state.currentMatrix = math::Mat4::Rotate(x, y, z, angle * math::Deg2Rad) * (*state.currentMatrix);
#endif
}

// Multiply the current matrix by a scaling matrix
void gl::Context::Scale(float x, float y, float z)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glScalef(x, y, z);
#else
    // NOTE: We transpose matrix with multiplication order
    *state.currentMatrix = math::Mat4::Scale(x, y, z) * (*state.currentMatrix);
#endif
}

// Multiply the current matrix by another matrix
void gl::Context::MultMatrix(const math::Mat4& mat)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glMultMatrixf(mat);
#else
    *state.currentMatrix = (*state.currentMatrix) * mat;
#endif
}

// Multiply the current matrix by a perspective matrix generated by parameters
void gl::Context::Frustum(double left, double right, double bottom, double top, double znear, double zfar)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glFrustum(left, right, bottom, top, znear, zfar);
#else
    *state.currentMatrix = (*state.currentMatrix) * math::Mat4::Frustum(left, right, bottom, top, znear, zfar);
#endif
}

// Multiply the current matrix by an orthographic matrix generated by parameters
void gl::Context::Ortho(double left, double right, double bottom, double top, double znear, double zfar)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glOrtho(left, right, bottom, top, znear, zfar);
#else
    *state.currentMatrix = (*state.currentMatrix) * math::Mat4::Ortho(left, right, bottom, top, znear, zfar);
#endif
}

// Set the viewport area (transformation from normalized device coordinates to window coordinates)
void gl::Context::SetViewport(int x, int y, int width, int height)
{
#if defined(__APPLE__)
    math::Vec2 scale = SDL_GetWindowDisplayScale();    // TODO: Function added in SDL3
    glViewport(x * scale.x, y * scale.y, width * scale.x, height * scale.y);
#else
    glViewport(x, y, width, height);
#endif

    SetFramebufferWidth(width);
    SetFramebufferHeight(height);

    MatrixMode(gl::MatrixMode::Projection);     // Switch to projection matrix
    LoadIdentity();                             // Reset current matrix (projection)

    // Set orthographic projection to the current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    Ortho(x, width, height, y, 0.0f, 1.0f);

    MatrixMode(gl::MatrixMode::ModelView);      // Switch back to modelview matrix
    LoadIdentity();                             // Reset current matrix (modelview)
}

// Get the current viewport area
shape2D::Rectangle gl::Context::GetViewport() const
{
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    return { vp[0], vp[1], vp[2], vp[3] };
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vertex level operations
//----------------------------------------------------------------------------------

// Initialize drawing mode (how to organize vertex)
void gl::Context::Begin(DrawMode mode)
{
#if defined(GRAPHICS_API_OPENGL_11)
    switch (mode)
    {
        case DrawMode::Lines:       glBegin(GL_LINES);          break;
        case DrawMode::Triangles:   glBegin(GL_TRIANGLES);      break;
        case DrawMode::Quads:       glBegin(GL_QUADS);          break;
        default:                                                break;
    }
#else
    currentBatch->Begin(mode);
#endif
}

// Finish vertex providing
void gl::Context::End()
{
#if defined(GRAPHICS_API_OPENGL_11)
    glEnd();
#else
    currentBatch->End();
#endif
}

// Define one vertex (position)
// NOTE: Vertex position data is the basic information required for drawing
void gl::Context::Vertex(float x, float y, float z)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glVertex3f(x, y, z);
#else
    Vertex({ x, y, z });
#endif
}

void gl::Context::Vertex(float x, float y)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glVertex2f(x, y);
#else
    Vertex({ x, y, currentBatch->GetCurrentDepth() });
#endif
}

void gl::Context::Vertex(const math::Vec3& vertex)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glVertex3f(vertex.x, vertex.y, vertex.z);
#else
    if (state.transformRequired)
    {
        currentBatch->AddVertex(vertex.Transformed(state.transform), state.texcoord, state.color);
    }
    else
    {
        currentBatch->AddVertex(vertex, state.texcoord, state.color);
    }
#endif
}

void gl::Context::Vertex(const math::Vec2& vertex)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glVertex2f(vertex.x, vertex.y);
#else
    Vertex({ vertex.x, vertex.y, currentBatch->GetCurrentDepth() });
#endif
}

void gl::Context::TexCoord(float x, float y)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glTexCoord2f(x, y);
#else
    state.texcoord.x = x;
    state.texcoord.y = y;
#endif
}

void gl::Context::TexCoord(const math::Vec2& texcoord)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glTexCoord2f(texcoord.x, texcoord.y);
#else
    state.texcoord = texcoord;
#endif
}

void gl::Context::Normal(float x, float y, float z)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glNormal3f(x, y, z);
#else
    state.normal.x = x;
    state.normal.y = y;
    state.normal.z = z;
#endif
}

void gl::Context::Normal(const math::Vec3& normal)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glNormal3f(normal.x, normal.y, normal.z);
#else
    state.normal = normal;
#endif
}

void gl::Context::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glColor4ub(r, g, b, a);
#else
    state.color.r = r;
    state.color.g = g;
    state.color.b = b;
    state.color.a = a;
#endif
}

void gl::Context::Color(float r, float g, float b, float a)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glColor4f(r, g, b, a);
#else
    state.color.r = static_cast<Uint8>(r*255);
    state.color.g = static_cast<Uint8>(g*255);
    state.color.b = static_cast<Uint8>(b*255);
    state.color.a = static_cast<Uint8>(a*255);
#endif
}

void gl::Context::Color(const gfx::Color& color)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glColor4ub(color.r, color.g, color.b, color.a);
#else
    state.color = color;
#endif
}

void gl::Context::Color(const math::Vec4& color)
{
#if defined(GRAPHICS_API_OPENGL_11)
    glColor4f(color.x, color.y, color.z, color.w);
#else
    state.color = gfx::Color(color);
#endif
}

//--------------------------------------------------------------------------------------
// Module Functions Definition - OpenGL style functions (common to 1.1, 3.3+, ES2)
//--------------------------------------------------------------------------------------

// Set current texture to use
void gl::Context::SetTexture(Uint32 id)
{
#if defined(GRAPHICS_API_OPENGL_11)
    (id == 0 ? DisableTexture() : EnableTexture(id));
#else
    currentBatch->SetTexture(id);
#endif
}

// Set current texture to use
void gl::Context::SetTexture(const Texture& texture)
{
    this->SetTexture(texture->GetID());
}

// Set current texture to use
void gl::Context::SetTexture(const Texture* texture)
{
    this->SetTexture(texture ? (*texture)->GetID() : 0u);
}

// Select and active a texture slot
void gl::Context::ActiveTextureSlot(int slot)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glActiveTexture(GL_TEXTURE0 + slot);
#endif
}

// Enable texture
void gl::Context::EnableTexture(Uint32 id)
{
#   if defined(GRAPHICS_API_OPENGL_11)
        glEnable(GL_TEXTURE_2D);
#   endif

    glBindTexture(GL_TEXTURE_2D, id);
}

// Disable texture
void gl::Context::DisableTexture()
{
#   if defined(GRAPHICS_API_OPENGL_11)
        glDisable(GL_TEXTURE_2D);
#   endif

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Enable texture cubemap
void gl::Context::EnableTextureCubemap(Uint32 id)
{
#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
#   endif
}

// Disable texture cubemap
void gl::Context::DisableTextureCubemap()
{
#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
#   endif
}

// Set texture parameters (wrap mode)
void gl::Context::TextureParameters(Uint32 id, TextureParam param, TextureWrap wrap)
{
    if (param == TextureParam::Wrap_S || param == TextureParam::Wrap_T)
    {
        glBindTexture(GL_TEXTURE_2D, id);

            if (wrap == TextureWrap::MirrorClamp)
            {
#               if !defined(GRAPHICS_API_OPENGL_11)
                    if (GetExtensions().texMirrorClamp) glTexParameteri(GL_TEXTURE_2D, static_cast<int>(param), static_cast<int>(wrap));
                    else NEXUS_LOG(Warning) << "[GL] Clamp mirror wrap mode not supported (GL_MIRROR_CLAMP_EXT)\n";
#               endif
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, static_cast<int>(param), static_cast<int>(wrap));
            }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else NEXUS_LOG(Warning) << "[gl::Context::TextureParameters] Invalid texture parameter given\n";
}

// Set texture parameters (filter mode)
void gl::Context::TextureParameters(Uint32 id, TextureParam param, TextureFilter filter)
{
    if (param == TextureParam::MagFilter || param == TextureParam::MinFilter)
    {
        glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, static_cast<int>(param), static_cast<int>(filter));
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else NEXUS_LOG(Warning) << "[gl::Context::TextureParameters] Invalid texture parameter given\n";
}

// Set texture parameters
void gl::Context::TextureParameters(Uint32 id, TextureParam param, float value)
{
    if (param == TextureParam::Anisotropy)
    {
#       if !defined(GRAPHICS_API_OPENGL_11)

            glBindTexture(GL_TEXTURE_2D, id);

                if (value <= GetExtensions().maxAnisotropyLevel)
                {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(value));
                }
                else if (GetExtensions().maxAnisotropyLevel > 0.0f)
                {
                    NEXUS_LOG(Warning) << "[GL] Maximum anisotropic filter level supported is " << static_cast<int>(GetExtensions().maxAnisotropyLevel) << "X\n";
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GetExtensions().maxAnisotropyLevel);
                }
                else
                {
                    NEXUS_LOG(Warning) << "[GL] Anisotropic filtering not supported\n";
                }

            glBindTexture(GL_TEXTURE_2D, 0);

#       endif
    }
    else if (param == TextureParam::MipmapBiasRatio)
    {
#       if defined(GRAPHICS_API_OPENGL_33)
            glBindTexture(GL_TEXTURE_2D, id);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, value);
            glBindTexture(GL_TEXTURE_2D, 0);
#       endif
    }
    else NEXUS_LOG(Warning) << "[gl::Context::TextureParameters] Invalid texture parameter given\n";
}

// Set cubemap parameters (wrap mode)
void gl::Context::CubemapParameters(Uint32 id, TextureParam param, TextureWrap wrap)
{
#if !defined(GRAPHICS_API_OPENGL_11)

    if (param == TextureParam::Wrap_S || param == TextureParam::Wrap_T)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

            if (wrap == TextureWrap::MirrorClamp)
            {
#               if !defined(GRAPHICS_API_OPENGL_11)
                    if (GetExtensions().texMirrorClamp) glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<int>(param), static_cast<int>(wrap));
                    else NEXUS_LOG(Warning) << "GL: Clamp mirror wrap mode not supported (GL_MIRROR_CLAMP_EXT)\n";
#               endif
            }
            else
            {
                glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<int>(param), static_cast<int>(wrap));
            }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    else NEXUS_LOG(Warning) << "[gl::Context::TextureParameters] Invalid texture parameter given\n";

#endif
}

// Set cubemap parameters (filter mode)
void gl::Context::CubemapParameters(Uint32 id, TextureParam param, TextureFilter filter)
{
#if !defined(GRAPHICS_API_OPENGL_11)

    if (param == TextureParam::MagFilter || param == TextureParam::MinFilter)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<int>(param), static_cast<int>(filter));
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    else NEXUS_LOG(Warning) << "[gl::Context::TextureParameters] Invalid texture parameter given\n";

#endif
}

// Set cubemap parameters
void gl::Context::CubemapParameters(Uint32 id, TextureParam param, float value)
{
#if !defined(GRAPHICS_API_OPENGL_11)

    if (param == TextureParam::Anisotropy)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

            if (value <= GetExtensions().maxAnisotropyLevel)
            {
                glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(value));
            }
            else if (GetExtensions().maxAnisotropyLevel > 0.0f)
            {
                NEXUS_LOG(Warning) << "[GL] Maximum anisotropic filter level supported is " << static_cast<int>(GetExtensions().maxAnisotropyLevel) << "X\n";
                glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, GetExtensions().maxAnisotropyLevel);
            }
            else
            {
                NEXUS_LOG(Warning) << "[GL] Anisotropic filtering not supported\n";
            }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    else if (param == TextureParam::MipmapBiasRatio)
    {
#       if defined(GRAPHICS_API_OPENGL_33)
            glBindTexture(GL_TEXTURE_CUBE_MAP, id);
                glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, value);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
#       endif
    }
    else NEXUS_LOG(Warning) << "[gl::Context::TextureParameters] Invalid texture parameter given\n";

#endif
}

// Enable shader program
void gl::Context::EnableShader(Uint32 id)
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2))
    glUseProgram(id);
#endif
}

// Disable shader program
void gl::Context::DisableShader()
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2))
    glUseProgram(0);
#endif
}

// Enable rendering to texture (fbo)
void gl::Context::EnableFramebuffer(Uint32 id)
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(GL_RENDER_TEXTURES_HINT)
    glBindFramebuffer(GL_FRAMEBUFFER, id);
#endif
}

// Disable rendering to texture
void gl::Context::DisableFramebuffer()
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(GL_RENDER_TEXTURES_HINT)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}

// Blit active framebuffer to main framebuffer
void gl::Context::BlitFramebuffer(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, int bufferMask)
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES3)) && defined(GL_RENDER_TEXTURES_HINT)
    glBlitFramebuffer(srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, bufferMask, GL_NEAREST);
#endif
}

// Blit active framebuffer to main framebuffer
void gl::Context::BlitFramebuffer(const shape2D::Rectangle& src, const shape2D::Rectangle& dst, int bufferMask)
{
    BlitFramebuffer(src.x, src.y, src.w, src.h, dst.x, dst.y, dst.w, dst.h, bufferMask);
}

// Activate multiple draw color buffers
// NOTE: One color buffer is always active by default
void gl::Context::ActiveDrawBuffers(int count)
{
#if ((defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES3)) && defined(GL_RENDER_TEXTURES_HINT))

    // NOTE: Maximum number of draw buffers supported is implementation dependant,
    // it can be queried with glGet*() but it must be at least 8
    //GLint maxDrawBuffers = 0;
    //glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

    if (count > 0)
    {
        if (count > 8)
        {
            NEXUS_LOG(Warning) << "[GL] Max color buffers limited to 8\n";
        }
        else
        {
            Uint32 buffers[8] = {
#           if defined(GRAPHICS_API_OPENGL_ES3)
                GL_COLOR_ATTACHMENT0_EXT,
                GL_COLOR_ATTACHMENT1_EXT,
                GL_COLOR_ATTACHMENT2_EXT,
                GL_COLOR_ATTACHMENT3_EXT,
                GL_COLOR_ATTACHMENT4_EXT,
                GL_COLOR_ATTACHMENT5_EXT,
                GL_COLOR_ATTACHMENT6_EXT,
                GL_COLOR_ATTACHMENT7_EXT,
#           else
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,
                GL_COLOR_ATTACHMENT5,
                GL_COLOR_ATTACHMENT6,
                GL_COLOR_ATTACHMENT7,
#           endif
            };

#           if defined(GRAPHICS_API_OPENGL_ES3)
                glDrawBuffersEXT(count, buffers);
#           else
                glDrawBuffers(count, buffers);
#           endif
        }
    }
    else
    {
        NEXUS_LOG(Warning) << "[GL] One color buffer active by default\n";
    }
#endif
}

//----------------------------------------------------------------------------------
// General render state configuration
//----------------------------------------------------------------------------------

// Enable color blending
void gl::Context::EnableColorBlend()
{
    glEnable(GL_BLEND);
}

// Disable color blending
void gl::Context::DisableColorBlend()
{
    glDisable(GL_BLEND);
}

// Enable depth test
void gl::Context::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

// Disable depth test
void gl::Context::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

// Enable depth write
void gl::Context::EnableDepthMask()
{
    glDepthMask(GL_TRUE);
}

// Disable depth write
void gl::Context::DisableDepthMask()
{
    glDepthMask(GL_FALSE);
}

// Enable backface culling
void gl::Context::EnableBackfaceCulling()
{
    glEnable(GL_CULL_FACE);
}

// Disable backface culling
void gl::Context::DisableBackfaceCulling()
{
    glDisable(GL_CULL_FACE);
}

// Set face culling mode
void gl::Context::SetCullFace(CullMode mode)
{
    switch (mode)
    {
        case CullMode::FaceBack: glCullFace(GL_BACK); break;
        case CullMode::FaceFront: glCullFace(GL_FRONT); break;
        default: break;
    }
}

// Enable scissor test
void gl::Context::EnableScissorTest()
{
    glEnable(GL_SCISSOR_TEST);
}

// Disable scissor test
void gl::Context::DisableScissorTest()
{
    glDisable(GL_SCISSOR_TEST);
}

// Scissor test
void gl::Context::Scissor(int x, int y, int width, int height)
{
    glScissor(x, y, width, height);
}

// Enable wire mode
void gl::Context::EnableWireMode()
{
#if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)
    // NOTE: glPolygonMode() not available on OpenGL ES
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
}

void gl::Context::EnablePointMode()
{
#if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)
    // NOTE: glPolygonMode() not available on OpenGL ES
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif
}

// Disable wire mode
void gl::Context::DisableWireMode()
{
#if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)
    // NOTE: glPolygonMode() not available on OpenGL ES
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

// Set the line drawing width
void gl::Context::SetLineWidth(float width)
{
    glLineWidth(width);
}

// Get the line drawing width
float gl::Context::GetLineWidth() const
{
    float width = 0;
    glGetFloatv(GL_LINE_WIDTH, &width);
    return width;
}

// Enable line aliasing
void gl::Context::EnableSmoothLines()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_11)
    glEnable(GL_LINE_SMOOTH);
#endif
}

// Disable line aliasing
void gl::Context::DisableSmoothLines()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_11)
    glDisable(GL_LINE_SMOOTH);
#endif
}

// Enable stereo rendering
void gl::Context::EnableStereoRender()
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2))
    state.stereoRender = true;
#endif
}

// Disable stereo rendering
void gl::Context::DisableStereoRender()
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2))
    state.stereoRender = false;
#endif
}

// Check if stereo render is enabled
bool gl::Context::IsStereoRenderEnabled()
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2))
    return state.stereoRender;
#else
    return false;
#endif
}

// Clear color buffer with color
void gl::Context::ClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Color values clamp to 0.0f(0) and 1.0f(255)
    glClearColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

void gl::Context::ClearColor(const gfx::Color& color)
{
    glClearColor(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
}

void gl::Context::ClearColor(const math::Vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

// Clear used screen buffers (color and depth)
void gl::Context::ClearScreenBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear used buffers: Color and Depth (Depth is used for 3D)
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);     // Stencil buffer not used...
}

// Check and log OpenGL error codes
void gl::Context::CheckErrors()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    int check = 1;
    while (check)
    {
        const GLenum err = glGetError();
        switch (err)
        {
            case GL_NO_ERROR: check = 0; break;
            case 0x0500: NEXUS_LOG(Warning) << "[GL] Error detected: GL_INVALID_ENUM";                     break;
            case 0x0501: NEXUS_LOG(Warning) << "[GL] Error detected: GL_INVALID_VALUE";                    break;
            case 0x0502: NEXUS_LOG(Warning) << "[GL] Error detected: GL_INVALID_OPERATION";                break;
            case 0x0503: NEXUS_LOG(Warning) << "[GL] Error detected: GL_STACK_OVERFLOW";                   break;
            case 0x0504: NEXUS_LOG(Warning) << "[GL] Error detected: GL_STACK_UNDERFLOW";                  break;
            case 0x0505: NEXUS_LOG(Warning) << "[GL] Error detected: GL_OUT_OF_MEMORY";                    break;
            case 0x0506: NEXUS_LOG(Warning) << "[GL] Error detected: GL_INVALID_FRAMEBUFFER_OPERATION";    break;
            default: NEXUS_LOG(Warning) << "[GL] Error detected: Unknown error code: " << err << "\n";     break;
        }
    }
#endif
}

// Set blend mode
void gl::Context::SetBlendMode(BlendMode mode)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if ((state.currentBlendMode != mode) || ((mode == BlendMode::Custom || mode == BlendMode::CustomSeparate) && state.glCustomBlendModeModified))
    {
        DrawRenderBatch(currentBatch);

        switch (mode)
        {
            case BlendMode::Alpha: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glBlendEquation(GL_FUNC_ADD); break;
            case BlendMode::Additive: glBlendFunc(GL_SRC_ALPHA, GL_ONE); glBlendEquation(GL_FUNC_ADD); break;
            case BlendMode::Multiplied: glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA); glBlendEquation(GL_FUNC_ADD); break;
            case BlendMode::AddColors: glBlendFunc(GL_ONE, GL_ONE); glBlendEquation(GL_FUNC_ADD); break;
            case BlendMode::SubtractColors: glBlendFunc(GL_ONE, GL_ONE); glBlendEquation(GL_FUNC_SUBTRACT); break;
            case BlendMode::AlphaPremultiply: glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); glBlendEquation(GL_FUNC_ADD); break;
            case BlendMode::Custom:
            {
                // NOTE: Using GL blend src/dst factors and GL equation configured with Context::SetBlendFactors()
                glBlendFunc(state.glBlendSrcFactor, state.glBlendDstFactor); glBlendEquation(state.glBlendEquation);

            } break;
            case BlendMode::CustomSeparate:
            {
                // NOTE: Using GL blend src/dst factors and GL equation configured with Context::SetBlendFactorsSeparate()
                glBlendFuncSeparate(state.glBlendSrcFactorRGB, state.glBlendDestFactorRGB, state.glBlendSrcFactorAlpha, state.glBlendDestFactorAlpha);
                glBlendEquationSeparate(state.glBlendEquationRGB, state.glBlendEquationAlpha);

            } break;
            default: break;
        }

        state.currentBlendMode = mode;
        state.glCustomBlendModeModified = false;
    }
#endif
}

// Set blending mode factor and equation
void gl::Context::SetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if ((state.glBlendSrcFactor != glSrcFactor) ||
        (state.glBlendDstFactor != glDstFactor) ||
        (state.glBlendEquation != glEquation))
    {
        state.glBlendSrcFactor = glSrcFactor;
        state.glBlendDstFactor = glDstFactor;
        state.glBlendEquation = glEquation;

        state.glCustomBlendModeModified = true;
    }
#endif
}

// Set blending mode factor and equation separately for RGB and alpha
void gl::Context::SetBlendFactorsSeparate(int glSrcRGB, int glDstRGB, int glSrcAlpha, int glDstAlpha, int glEqRGB, int glEqAlpha)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if ((state.glBlendSrcFactorRGB != glSrcRGB) ||
        (state.glBlendDestFactorRGB != glDstRGB) ||
        (state.glBlendSrcFactorAlpha != glSrcAlpha) ||
        (state.glBlendDestFactorAlpha != glDstAlpha) ||
        (state.glBlendEquationRGB != glEqRGB) ||
        (state.glBlendEquationAlpha != glEqAlpha))
    {
        state.glBlendSrcFactorRGB = glSrcRGB;
        state.glBlendDestFactorRGB = glDstRGB;
        state.glBlendSrcFactorAlpha = glSrcAlpha;
        state.glBlendDestFactorAlpha = glDstAlpha;
        state.glBlendEquationRGB = glEqRGB;
        state.glBlendEquationAlpha = glEqAlpha;

        state.glCustomBlendModeModified = true;
    }
#endif
}

//----------------------------------------------------------------------------------
// Module Functions Definition - OpenGL Debug
//----------------------------------------------------------------------------------
#if defined(RLGL_ENABLE_OPENGL_DEBUG_CONTEXT) && defined(GRAPHICS_API_OPENGL_43)
static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    // Ignore non-significant error/warning codes (NVidia drivers)
    // NOTE: Here there are the details with a sample output:
    // - #131169 - Framebuffer detailed info: The driver allocated storage for renderbuffer 2. (severity: low)
    // - #131185 - Buffer detailed info: Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_ENUM_88e4)
    //             will use VIDEO memory as the source for buffer object operations. (severity: low)
    // - #131218 - Program/shader state performance warning: Vertex shader in program 7 is being recompiled based on GL state. (severity: medium)
    // - #131204 - Texture state usage warning: The texture object (0) bound to texture image unit 0 does not have
    //             a defined base level and cannot be used for texture mapping. (severity: low)
    if ((id == 131169) || (id == 131185) || (id == 131218) || (id == 131204)) return;

    const char *msgSource = nullptr;
    switch (source)
    {
        case GL_DEBUG_SOURCE_API: msgSource = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: msgSource = "WINDOW_SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: msgSource = "SHADER_COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: msgSource = "THIRD_PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION: msgSource = "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER: msgSource = "OTHER"; break;
        default: break;
    }

    const char *msgType = nullptr;
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR: msgType = "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: msgType = "DEPRECATED_BEHAVIOR"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: msgType = "UNDEFINED_BEHAVIOR"; break;
        case GL_DEBUG_TYPE_PORTABILITY: msgType = "PORTABILITY"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: msgType = "PERFORMANCE"; break;
        case GL_DEBUG_TYPE_MARKER: msgType = "MARKER"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: msgType = "PUSH_GROUP"; break;
        case GL_DEBUG_TYPE_POP_GROUP: msgType = "POP_GROUP"; break;
        case GL_DEBUG_TYPE_OTHER: msgType = "OTHER"; break;
        default: break;
    }

    const char *msgSeverity = "DEFAULT";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_LOW: msgSeverity = "LOW"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: msgSeverity = "MEDIUM"; break;
        case GL_DEBUG_SEVERITY_HIGH: msgSeverity = "HIGH"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: msgSeverity = "NOTIFICATION"; break;
        default: break;
    }

    NEXUS_LOG(Warning) << "[GL] OpenGL debug message: " << message << "\n";
    NEXUS_LOG(Warning) << "    > Type: " << msgType << "\n";
    NEXUS_LOG(Warning) << "    > Source = " << msgSource << "\n";
    NEXUS_LOG(Warning) << "    > Severity = " << msgSeverity << "\n";
}
#endif

//----------------------------------------------------------------------------------
// Module Functions Definition - rlgl functionality
//----------------------------------------------------------------------------------

// Set current framebuffer width
void gl::Context::SetFramebufferWidth(int width)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    state.framebufferWidth = width;
#endif
}

// Set current framebuffer height
void gl::Context::SetFramebufferHeight(int height)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    state.framebufferHeight = height;
#endif
}

// Get default framebuffer width
int gl::Context::GetFramebufferWidth() const
{
    int width = 0;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    width = state.framebufferWidth;
#endif
    return width;
}

// Get default framebuffer height
int gl::Context::GetFramebufferHeight() const
{
    int height = 0;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    height = state.framebufferHeight;
#endif
    return height;
}

// Get default internal texture (white texture)
// NOTE: Default texture is a 1x1 pixel UNCOMPRESSED_R8G8B8A8
Uint32 gl::Context::GetTextureIdDefault() const
{
    Uint32 id = 0;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    id = state.defaultTextureId;
#endif
    return id;
}

// Get default shader id
Uint32 gl::Context::GetShaderIdDefault() const
{
    Uint32 id = 0;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    id = state.defaultShaderId;
#endif
    return id;
}

// Get default shader locs
const int *gl::Context::GetShaderLocsDefault() const
{
    const int *locs = nullptr;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    locs = state.defaultShaderLocs;
#endif
    return locs;
}

// Render batch management
//------------------------------------------------------------------------------------------------

// Draw render batch
// NOTE: We require a pointer to reset batch and increase current buffer (multi-buffer)
void gl::Context::DrawRenderBatch(RenderBatch* batch)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

    batch->Draw();

    // Reset active texture units for next batch
    std::fill(state.activeTextureId, state.activeTextureId + GL_DEFAULT_BATCH_MAX_TEXTURE_UNITS, 0);

#endif
}

// Set the active render batch for rlgl
void gl::Context::SetRenderBatchActive(RenderBatch* batch)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

    DrawRenderBatch(currentBatch);

    if (batch == nullptr)
    {
        currentBatch = defaultBatch.get();
        return;
    }

    if (batch->GetContext() != this)
    {
        throw core::NexusException("gl::Context::SetRenderBatchActive", "The context of the RenderBatch must be the same as the one it is passed to");    
    }

    currentBatch = batch;

#endif
}

// Update and draw internal render batch
void gl::Context::DrawRenderBatchActive()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    DrawRenderBatch(currentBatch);    // NOTE: Stereo rendering is checked inside
#endif
}

// Check internal buffer overflow for a given number of vertex
// and force a Context::RenderBatch draw call if required
bool gl::Context::CheckRenderBatchLimit(int vCount)
{
    bool overflow = false;

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    overflow = currentBatch->CheckLimit(vCount);
#endif

    return overflow;
}

// Textures data management
//-----------------------------------------------------------------------------------------
// Convert image data to OpenGL texture (returns OpenGL valid Id)
Uint32 gl::Context::LoadTexture(const void *data, int width, int height, TextureFormat format, int mipmapCount)
{
    Uint32 id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);    // Free any old binding

    // Check texture format support by OpenGL 1.1 (compressed textures not supported)
#   if defined(GRAPHICS_API_OPENGL_11)

        if (format >= PixelFormat::DXT1_RGB)
        {
            NEXUS_LOG(Warning) << "[GL] OpenGL 1.1 does not support GPU compressed texture formats\n";
            return id;
        }

#   else

        if ((!GetExtensions().texCompDXT) && ((format == TextureFormat::DXT1_RGB) || (format == TextureFormat::DXT1_RGBA) ||
            (format == TextureFormat::DXT3_RGBA) || (format == TextureFormat::DXT5_RGBA)))
        {
            NEXUS_LOG(Warning) << "[GL] DXT compressed texture format not supported\n";
            return id;
        }

#       if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

            if ((!GetExtensions().texCompETC1) && (format == TextureFormat::ETC1_RGB))
            {
                NEXUS_LOG(Warning) << "[GL] ETC1 compressed texture format not supported\n";
                return id;
            }

            if ((!GetExtensions().texCompETC2) && ((format == TextureFormat::ETC2_RGB) || (format == TextureFormat::ETC2_EAC_RGBA)))
            {
                NEXUS_LOG(Warning) << "[GL] ETC2 compressed texture format not supported\n";
                return id;
            }

            if ((!GetExtensions().texCompPVRT) && ((format == TextureFormat::PVRT_RGB) || (format == TextureFormat::PVRT_RGBA)))
            {
                NEXUS_LOG(Warning) << "[GL] PVRT compressed texture format not supported\n";
                return id;
            }

            if ((!GetExtensions().texCompASTC) && ((format == TextureFormat::ASTC_4x4_RGBA) || (format == TextureFormat::ASTC_8x8_RGBA)))
            {
                NEXUS_LOG(Warning) << "[GL] ASTC compressed texture format not supported\n";
                return id;
            }

#       endif

#   endif  // GRAPHICS_API_OPENGL_11

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &id); // Generate texture id
    glBindTexture(GL_TEXTURE_2D, id);

    int mipWidth = width;
    int mipHeight = height;
    int mipOffset = 0;          // Mipmap data offset, only used for tracelog

    // NOTE: Added pointer math separately from function to avoid UBSAN complaining
    const Uint8 *dataPtr = static_cast<const Uint8*>(data);

    // Load the different mipmap levels
    for (int i = 0; i < mipmapCount; i++)
    {
        Uint32 mipSize = GetPixelDataSize(mipWidth, mipHeight, format);

        Uint32 glInternalFormat, glFormat, glType;
        GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);

        NEXUS_LOG(Debug) << "[gl::Context::LoadTexture] Load mipmap level " << i << " (" << mipWidth << " x " << mipHeight << "), size: " << mipSize << ", offset: " << mipOffset << "\n";

        if (glInternalFormat != 0)
        {
            if (format < TextureFormat::DXT1_RGB)
            {
                glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, dataPtr);
            }
#           if !defined(GRAPHICS_API_OPENGL_11)
            else
            {
                glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, mipSize, dataPtr);
            }
#           endif

#           if defined(GRAPHICS_API_OPENGL_33)
            if (format == TextureFormat::Grayscale)
            {
                constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
            else if (format == TextureFormat::GrayAlpha)
            {
#               if defined(GRAPHICS_API_OPENGL_21)
                    constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ALPHA };
#               elif defined(GRAPHICS_API_OPENGL_33)
                    constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
#               endif

                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
#           endif
        }

        mipWidth /= 2;
        mipHeight /= 2;
        mipOffset += mipSize;                       // Increment offset position to next mipmap
        if (data != nullptr) dataPtr += mipSize;    // Increment data pointer to next mipmap

        // Security check for NPOT textures
        if (mipWidth < 1) mipWidth = 1;
        if (mipHeight < 1) mipHeight = 1;
    }

    // Texture parameters configuration
    // NOTE: glTexParameteri does NOT affect texture uploading, just the way it's used
#   if defined(GRAPHICS_API_OPENGL_ES2)

        // NOTE: OpenGL ES 2.0 with no GL_OES_texture_npot support (i.e. WebGL) has limited NPOT support, so CLAMP_TO_EDGE must be used
        if (GetExtensions().texNPOT)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // Set texture to repeat on x-axis
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       // Set texture to repeat on y-axis
        }
        else
        {
            // NOTE: If using negative texture coordinates (LoadOBJ()), it does not work!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);       // Set texture to clamp on x-axis
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);       // Set texture to clamp on y-axis
        }

#   else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // Set texture to repeat on x-axis
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       // Set texture to repeat on y-axis
#   endif

    // Magnification and minification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // Alternative: GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // Alternative: GL_LINEAR

#   if defined(GRAPHICS_API_OPENGL_33)

        if (mipmapCount > 1)
        {
            // Activate Trilinear filtering if mipmaps are available
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

#   endif

    // At this point we have the texture loaded in GPU and texture parameters configured
    // NOTE: If mipmaps were not in data, they are not generated automatically

    // Unbind current texture
    glBindTexture(GL_TEXTURE_2D, 0);

    if (id > 0) NEXUS_LOG(Info) << "[gl::Context::LoadTexture] [ID " << id << "] Texture loaded successfully (" << width << "x" << height << " | " << gl::GetTextureFormatName(format) << " | " << mipmapCount << " mipmaps)\n";
    else NEXUS_LOG(Warning) << "[gl::Context::LoadTexture] Failed to load texture\n";

    return id;
}

// Load depth texture/renderbuffer (to be attached to fbo)
// WARNING: OpenGL ES 2.0 requires GL_OES_depth_texture and WebGL requires WEBGL_depth_texture extensions
Uint32 gl::Context::LoadTextureDepth(int width, int height, bool useRenderBuffer)
{
    Uint32 id = 0;

#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

        // In case depth textures not supported, we force renderbuffer usage
        if (!GetExtensions().texDepth) useRenderBuffer = true;

        // NOTE: We let the implementation to choose the best bit-depth
        // Possible formats: GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32 and GL_DEPTH_COMPONENT32F
        Uint32 glInternalFormat = GL_DEPTH_COMPONENT;

#   if (defined(GRAPHICS_API_OPENGL_ES2) || defined(GRAPHICS_API_OPENGL_ES3))

        // WARNING: WebGL platform requires unsized internal format definition (GL_DEPTH_COMPONENT)
        // while other platforms using OpenGL ES 2.0 require/support sized internal formats depending on the GPU capabilities
        if (!GetExtensions().texDepthWebGL || useRenderBuffer)
        {
            if (GetExtensions().maxDepthBits == 32) glInternalFormat = GL_DEPTH_COMPONENT32_OES;
            else if (GetExtensions().maxDepthBits == 24) glInternalFormat = GL_DEPTH_COMPONENT24_OES;
            else glInternalFormat = GL_DEPTH_COMPONENT16;
        }

#   endif

    if (!useRenderBuffer && GetExtensions().texDepth)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

        NEXUS_LOG(Info) << "[gl::Context::LoadTextureDepth] [ID " << id << "] Depth texture loaded successfully\n";
    }
    else
    {
        // Create the renderbuffer that will serve as the depth attachment for the framebuffer
        // NOTE: A renderbuffer is simpler than a texture and could offer better performance on embedded devices
        glGenRenderbuffers(1, &id);
        glBindRenderbuffer(GL_RENDERBUFFER, id);
        glRenderbufferStorage(GL_RENDERBUFFER, glInternalFormat, width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        NEXUS_LOG(Info) << "[gl::Context::LoadTextureDepth] [ID " << id << "] Depth renderbuffer loaded successfully (" << (GetExtensions().maxDepthBits >= 24 ? GetExtensions().maxDepthBits : 16) << " bits)\n";
    }
#endif

    return id;
}

// Load texture cubemap
// NOTE: Cubemap data is expected to be 6 images in a single data array (one after the other),
// expected the following convention: +X, -X, +Y, -Y, +Z, -Z
Uint32 gl::Context::LoadTextureCubemap(const void *data, int size, TextureFormat format)
{
    Uint32 id = 0;

#   if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)

        Uint32 dataSize = GetPixelDataSize(size, size, format);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        Uint32 glInternalFormat, glFormat, glType;
        GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);

        if (glInternalFormat != 0)
        {
            // Load cubemap faces
            for (Uint32 i = 0; i < 6; i++)
            {
                if (data == nullptr)
                {
                    if (format < TextureFormat::DXT1_RGB)
                    {
                        if ((format == TextureFormat::R32) || (format == TextureFormat::RGBA32323232)
                         || (format == TextureFormat::R16) || (format == TextureFormat::RGBA16161616))
                            NEXUS_LOG(Warning) << "[gl::Context::LoadTextureCubemap] Cubemap requested format not supported\n";
                        else glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, size, size, 0, glFormat, glType, nullptr);
                    }
                    else NEXUS_LOG(Warning) << "[gl::Context::LoadTextureCubemap] Empty cubemap creation does not support compressed format\n";
                }
                else
                {
                    if (format < TextureFormat::DXT1_RGB) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, size, size, 0, glFormat, glType, static_cast<const Uint8*>(data) + i*dataSize);
                    else glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, size, size, 0, dataSize, static_cast<const Uint8*>(data) + i*dataSize);
                }

#           if defined(GRAPHICS_API_OPENGL_33)

                if (format == TextureFormat::Grayscale)
                {
                    constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                    glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }
                else if (format == TextureFormat::GrayAlpha)
                {
#                   if defined(GRAPHICS_API_OPENGL_21)
                        constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ALPHA };
#                   elif defined(GRAPHICS_API_OPENGL_33)
                        constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
#                   endif

                    glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }

#           endif
        }
    }

    // Set cubemap texture sampling parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#   if defined(GRAPHICS_API_OPENGL_33)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  // Flag not supported on OpenGL ES 2.0
#   endif

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
#endif

    if (id > 0) NEXUS_LOG(Info) << "[gl::Context::LoadTextureCubemap] [ID " << id << "] Cubemap texture loaded successfully (" << size << "x" << size << ")\n";
    else NEXUS_LOG(Warning) << "[gl::Context::LoadTextureCubemap] Failed to load cubemap texture\n";

    return id;
}

// Update already loaded texture in GPU with new data
// NOTE: We don't know safely if internal texture format is the expected one...
void gl::Context::UpdateTexture(Uint32 id, int offsetX, int offsetY, int width, int height, TextureFormat format, const void *data)
{
    glBindTexture(GL_TEXTURE_2D, id);

    Uint32 glInternalFormat, glFormat, glType;
    GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);

    if ((glInternalFormat != 0) && (format < TextureFormat::DXT1_RGB))
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, glFormat, glType, data);
    }
    else NEXUS_LOG(Warning) << "[gl::Context::UpdateTexture] [ID " << id << "] Failed to update for current texture format (" << static_cast<int>(format) << ")\n";
}

// Unload texture from GPU memory
void gl::Context::UnloadTexture(Uint32 id)
{
    glDeleteTextures(1, &id);
}

// Generate mipmap data for selected texture
// NOTE: Only supports GPU mipmap generation
void gl::Context::GenTextureMipmaps(Uint32 id, int width, int height, TextureFormat format, int& mipmaps)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindTexture(GL_TEXTURE_2D, id);

    // Check if texture is power-of-two (POT)
    bool texIsPOT = false;

    if (((width > 0) && ((width & (width - 1)) == 0)) &&
        ((height > 0) && ((height & (height - 1)) == 0))) texIsPOT = true;

    if ((texIsPOT) || (GetExtensions().texNPOT))
    {
        //glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);   // Hint for mipmaps generation algorithm: GL_FASTEST, GL_NICEST, GL_DONT_CARE
        glGenerateMipmap(GL_TEXTURE_2D);    // Generate mipmaps automatically

        mipmaps = 1 + std::floor(std::log(std::max(width, height))/std::log(2));
        NEXUS_LOG(Info) << "[gl::Context::GenTextureMipmaps] [ID " << id << "] Mipmaps generated automatically, total: " << mipmaps << "\n";
    }
    else NEXUS_LOG(Warning) << "[gl::Context::GenTextureMipmaps] [ID " << id << "] Failed to generate mipmaps\n";

    glBindTexture(GL_TEXTURE_2D, 0);
#else
    NEXUS_LOG(Warning) << "[gl::Context::GenTextureMipmaps] [ID " << id << "] GPU mipmap generation not supported\n";
#endif
}

// Read texture pixel data
std::vector<Uint8> gl::Context::ReadTexturePixels(Uint32 id, int width, int height, TextureFormat format)
{
    std::vector<Uint8> pixels;

#   if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)

        glBindTexture(GL_TEXTURE_2D, id);

        // NOTE: Using texture id, we can retrieve some texture info (but not on OpenGL ES 2.0)
        // Possible texture info: GL_TEXTURE_RED_SIZE, GL_TEXTURE_GREEN_SIZE, GL_TEXTURE_BLUE_SIZE, GL_TEXTURE_ALPHA_SIZE
        //int width, height, format;
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);

        // NOTE: Each row written to or read from by OpenGL pixel operations like glGetTexImage are aligned to a 4 byte boundary by default, which may add some padding.
        // Use glPixelStorei to modify padding with the GL_[UN]PACK_ALIGNMENT setting.
        // GL_PACK_ALIGNMENT affects operations that read from OpenGL memory (glReadPixels, glGetTexImage, etc.)
        // GL_UNPACK_ALIGNMENT affects operations that write to OpenGL memory (glTexImage, etc.)
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        Uint32 glInternalFormat, glFormat, glType;
        GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);
        Uint32 size = GetPixelDataSize(width, height, format);

        if ((glInternalFormat != 0) && (format < TextureFormat::DXT1_RGB))
        {
            pixels.resize(size);
            glGetTexImage(GL_TEXTURE_2D, 0, glFormat, glType, pixels.data());
        }
        else NEXUS_LOG(Warning) << "[gl::Context::ReadTexturePixels] [ID " << id << "] Data retrieval not suported for pixel format (" << static_cast<int>(format) << ")\n";

        glBindTexture(GL_TEXTURE_2D, 0);

#   endif

#   if defined(GRAPHICS_API_OPENGL_ES2) || defined(PLATFORM_DESKTOP)

        // glGetTexImage() is not available on OpenGL ES 2.0
        // Texture width and height are required on OpenGL ES 2.0. There is no way to get it from texture id.
        // Two possible Options:
        // 1 - Bind texture to color fbo attachment and glReadPixels()
        // 2 - Create an fbo, activate it, render quad with texture, glReadPixels()
        // We are using Option 1, just need to care for texture format on retrieval
        // NOTE: This behaviour could be conditioned by graphic driver...
        Uint32 fboId = LoadFramebuffer(width, height);

        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Attach our texture to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

        // We read data as RGBA because FBO texture is configured as RGBA, despite binding another texture format
        pixels.resize(GetPixelDataSize(width, height, TextureFormat::RGBA8888));
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Clean up temporal fbo
        UnloadFramebuffer(fboId);

#   endif

    return pixels;
}

// Read screen pixel data (color buffer)
std::vector<Uint8> gl::Context::ReadScreenPixels(int width, int height)
{
    std::vector<Uint8> screenData(width*height*4, 0);

    // NOTE 1: glReadPixels returns image flipped vertically -> (0,0) is the bottom left corner of the framebuffer
    // NOTE 2: We are getting alpha channel! Be careful, it can be transparent if not cleared properly!
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, screenData.data());

    // Flip image vertically!
    std::vector<Uint8> imgData(screenData.size());

    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < (width*4); x++)
        {
            imgData[((height - 1) - y)*width*4 + x] = screenData[(y*width*4) + x];  // Flip line

            // Set alpha component value to 255 (no trasparent image retrieval)
            // NOTE: Alpha value has already been applied to RGB in framebuffer, we don't need it!
            if (((x + 1)%4) == 0) imgData[((height - 1) - y)*width*4 + x] = 255;
        }
    }

    return imgData;
}

// Framebuffer management (fbo)
//-----------------------------------------------------------------------------------------
// Load a framebuffer to be used for rendering
// NOTE: No textures attached
Uint32 gl::Context::LoadFramebuffer(int width, int height)
{
    Uint32 fboId = 0;

#   if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(GL_RENDER_TEXTURES_HINT)
        glGenFramebuffers(1, &fboId);       // Create the framebuffer object
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Unbind any framebuffer
#   endif

    return fboId;
}

// Attach color buffer texture to an fbo (unloads previous attachment)
// NOTE: Attach type: 0-Color, 1-Depth renderbuffer, 2-Depth texture
void gl::Context::FramebufferAttach(Uint32 fboId, Uint32 texId, FramebufferAttachType attachType, FramebufferAttachTextureType texType, int mipLevel)
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(GL_RENDER_TEXTURES_HINT)
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    switch (attachType)
    {
        case FramebufferAttachType::ColorChannel0:
        case FramebufferAttachType::ColorChannel1:
        case FramebufferAttachType::ColorChannel2:
        case FramebufferAttachType::ColorChannel3:
        case FramebufferAttachType::ColorChannel4:
        case FramebufferAttachType::ColorChannel5:
        case FramebufferAttachType::ColorChannel6:
        case FramebufferAttachType::ColorChannel7:
        {
            if (texType == FramebufferAttachTextureType::Texture2D)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<int>(attachType), GL_TEXTURE_2D, texId, mipLevel);

            else if (texType == FramebufferAttachTextureType::RenderBuffer)
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<int>(attachType), GL_RENDERBUFFER, texId);

            else if (texType >= FramebufferAttachTextureType::CubemapPositiveX)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<int>(attachType), GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(texType), texId, mipLevel);

        } break;
        case FramebufferAttachType::Depth:
        {
            if (texType == FramebufferAttachTextureType::Texture2D)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, mipLevel);

            else if (texType == FramebufferAttachTextureType::RenderBuffer)
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, texId);

        } break;
        case FramebufferAttachType::Stencil:
        {
            if (texType == FramebufferAttachTextureType::Texture2D)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texId, mipLevel);

            else if (texType == FramebufferAttachTextureType::RenderBuffer)
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, texId);

        } break;

        default: break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}

// Verify render texture is complete
bool gl::Context::FramebufferComplete(Uint32 id)
{
    bool result = false;

#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(GL_RENDER_TEXTURES_HINT)

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (status)
        {
            case GL_FRAMEBUFFER_UNSUPPORTED:
                NEXUS_LOG(Warning) << "[gl::Context::FramebufferComplete] [ID " << id << "] Framebuffer is unsupported\n";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                NEXUS_LOG(Warning) << "[gl::Context::FramebufferComplete] [ID " << id << "] Framebuffer has incomplete attachment\n";
                break;

#           if defined(GRAPHICS_API_OPENGL_ES2)
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                NEXUS_LOG(Warning) << "[gl::Context::FramebufferComplete] [ID " << id << "] Framebuffer has incomplete dimensions\n";
                break;
#           endif

            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                NEXUS_LOG(Warning) << "[gl::Context::FramebufferComplete] [ID " << id << "] Framebuffer has a missing attachment\n";
                break;

            default: break;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    result = (status == GL_FRAMEBUFFER_COMPLETE);

#endif

    return result;
}

// Unload framebuffer from GPU memory
// NOTE: All attached textures/cubemaps/renderbuffers are also deleted
void gl::Context::UnloadFramebuffer(Uint32 id)
{
#if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(GL_RENDER_TEXTURES_HINT)
    // Query depth attachment to automatically delete texture/renderbuffer
    int depthType = 0, depthId = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, id);   // Bind framebuffer to query depth texture type
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &depthType);

    // TODO: Review warning retrieving object name in WebGL
    // WARNING: WebGL: INVALID_ENUM: getFramebufferAttachmentParameter: invalid parameter name
    // https://registry.khronos.org/webgl/specs/latest/1.0/
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &depthId);

    Uint32 depthIdU = (Uint32)depthId;
    if (depthType == GL_RENDERBUFFER) glDeleteRenderbuffers(1, &depthIdU);
    else if (depthType == GL_TEXTURE) glDeleteTextures(1, &depthIdU);

    // NOTE: If a texture object is deleted while its image is attached to the *currently bound* framebuffer,
    // the texture image is automatically detached from the currently bound framebuffer.

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &id);

    NEXUS_LOG(Info) << "[gl::Context::UnloadFramebuffer] [ID " << id << "] Unloaded framebuffer from VRAM (GPU)\n";
#endif
}


// Vertex data management
//-----------------------------------------------------------------------------------------
// Load a new attributes buffer
Uint32 gl::Context::LoadVertexBuffer(const void *buffer, int size, bool dynamic)
{
    Uint32 id = 0;

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
#endif

    return id;
}

// Load a new attributes element buffer
Uint32 gl::Context::LoadVertexBufferElement(const void *buffer, int size, bool dynamic)
{
    Uint32 id = 0;

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
#endif

    return id;
}

// Enable vertex buffer (VBO)
void gl::Context::EnableVertexBuffer(Uint32 id)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindBuffer(GL_ARRAY_BUFFER, id);
#endif
}

// Disable vertex buffer (VBO)
void gl::Context::DisableVertexBuffer()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

// Enable vertex buffer element (VBO element)
void gl::Context::EnableVertexBufferElement(Uint32 id)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
#endif
}

// Disable vertex buffer element (VBO element)
void gl::Context::DisableVertexBufferElement()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

// Update vertex buffer with new data
// NOTE: dataSize and offset must be provided in bytes
void gl::Context::UpdateVertexBuffer(Uint32 id, const void *data, int dataSize, int offset)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
#endif
}

// Update vertex buffer elements with new data
// NOTE: dataSize and offset must be provided in bytes
void gl::Context::UpdateVertexBufferElements(Uint32 id, const void *data, int dataSize, int offset)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, dataSize, data);
#endif
}

// Enable vertex array object (VAO)
bool gl::Context::EnableVertexArray(Uint32 vaoId)
{
    bool result = false;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if (GetExtensions().vao)
    {
        glBindVertexArray(vaoId);
        result = true;
    }
#endif
    return result;
}

// Disable vertex array object (VAO)
void gl::Context::DisableVertexArray()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if (GetExtensions().vao) glBindVertexArray(0);
#endif
}

// Enable vertex attribute index
void gl::Context::EnableVertexAttribute(Uint32 index)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glEnableVertexAttribArray(index);
#endif
}

// Disable vertex attribute index
void gl::Context::DisableVertexAttribute(Uint32 index)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glDisableVertexAttribArray(index);
#endif
}

// Draw vertex array
void gl::Context::DrawVertexArray(int offset, int count)
{
    glDrawArrays(GL_TRIANGLES, offset, count);
}

// Draw vertex array elements
void gl::Context::DrawVertexArrayElements(int offset, int count, const void *buffer)
{
    // NOTE: Added pointer math separately from function to avoid UBSAN complaining
    const Uint16 *bufferPtr = static_cast<const Uint16*>(buffer);
    if (offset > 0) bufferPtr += offset;

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, bufferPtr);
}

// Draw vertex array instanced
void gl::Context::DrawVertexArrayInstanced(int offset, int count, int instances)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glDrawArraysInstanced(GL_TRIANGLES, 0, count, instances);
#endif
}

// Draw vertex array elements instanced
void gl::Context::DrawVertexArrayElementsInstanced(int offset, int count, const void *buffer, int instances)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    // NOTE: Added pointer math separately from function to avoid UBSAN complaining
    const Uint16 *bufferPtr = static_cast<const Uint16*>(buffer);
    if (offset > 0) bufferPtr += offset;

    glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, bufferPtr, instances);
#endif
}

#if defined(GRAPHICS_API_OPENGL_11)
// Enable vertex state pointer
void gl::Context::EnableStatePointer(int vertexAttribType, void *buffer)
{
    if (buffer != nullptr) glEnableClientState(vertexAttribType);
    switch (vertexAttribType)
    {
        case GL_VERTEX_ARRAY: glVertexPointer(3, GL_FLOAT, 0, buffer); break;
        case GL_TEXTURE_COORD_ARRAY: glTexCoordPointer(2, GL_FLOAT, 0, buffer); break;
        case GL_NORMAL_ARRAY: if (buffer != nullptr) glNormalPointer(GL_FLOAT, 0, buffer); break;
        case GL_COLOR_ARRAY: if (buffer != nullptr) glColorPointer(4, GL_UNSIGNED_BYTE, 0, buffer); break;
        //case GL_INDEX_ARRAY: if (buffer != nullptr) glIndexPointer(GL_SHORT, 0, buffer); break; // Indexed colors
        default: break;
    }
}

// Disable vertex state pointer
void gl::Context::DisableStatePointer(int vertexAttribType)
{
    glDisableClientState(vertexAttribType);
}
#endif

// Load vertex array object (VAO)
Uint32 gl::Context::LoadVertexArray()
{
    Uint32 vaoId = 0;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if (GetExtensions().vao)
    {
        glGenVertexArrays(1, &vaoId);
    }
#endif
    return vaoId;
}

// Set vertex attribute
void gl::Context::SetVertexAttribute(Uint32 index, int compSize, DataType type, bool normalized, int stride, const void *pointer)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glVertexAttribPointer(index, compSize, static_cast<int>(type), normalized, stride, pointer);
#endif
}

// Set vertex attribute divisor
void gl::Context::SetVertexAttributeDivisor(Uint32 index, int divisor)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glVertexAttribDivisor(index, divisor);
#endif
}

// Unload vertex array object (VAO)
void gl::Context::UnloadVertexArray(Uint32 vaoId)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if (GetExtensions().vao)
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vaoId);
        NEXUS_LOG(Info) << "[VAO] [ID " << vaoId << "] Unloaded vertex array data from VRAM (GPU)\n";
    }
#endif
}

// Unload vertex buffer (VBO)
void gl::Context::UnloadVertexBuffer(Uint32 vboId)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glDeleteBuffers(1, &vboId);
    //nx_LOG(Info) << "VBO: Unloaded vertex data from VRAM (GPU)\n";
#endif
}

// Shaders management
//-----------------------------------------------------------------------------------------------
// Load shader from code strings
// NOTE: If shader string is nullptr, using default vertex/fragment shaders
Uint32 gl::Context::LoadShaderCode(const char *vsCode, const char *fsCode)
{
    Uint32 id = 0;

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    Uint32 vertexShaderId = 0;
    Uint32 fragmentShaderId = 0;

    // Compile vertex shader (if provided)
    if (vsCode != nullptr) vertexShaderId = CompileShader(vsCode, GL_VERTEX_SHADER);
    // In case no vertex shader was provided or compilation failed, we use default vertex shader
    if (vertexShaderId == 0) vertexShaderId = state.defaultVShaderId;

    // Compile fragment shader (if provided)
    if (fsCode != nullptr) fragmentShaderId = CompileShader(fsCode, GL_FRAGMENT_SHADER);
    // In case no fragment shader was provided or compilation failed, we use default fragment shader
    if (fragmentShaderId == 0) fragmentShaderId = state.defaultFShaderId;

    // In case vertex and fragment shader are the default ones, no need to recompile, we can just assign the default shader program id
    if ((vertexShaderId == state.defaultVShaderId) && (fragmentShaderId == state.defaultFShaderId)) id = state.defaultShaderId;
    else
    {
        // One of or both shader are new, we need to compile a new shader program
        id = LoadShaderProgram(vertexShaderId, fragmentShaderId);

        // We can detach and delete vertex/fragment shaders (if not default ones)
        // NOTE: We detach shader before deletion to make sure memory is freed
        if (vertexShaderId != state.defaultVShaderId)
        {
            // WARNING: Shader program linkage could fail and returned id is 0
            if (id > 0) glDetachShader(id, vertexShaderId);
            glDeleteShader(vertexShaderId);
        }
        if (fragmentShaderId != state.defaultFShaderId)
        {
            // WARNING: Shader program linkage could fail and returned id is 0
            if (id > 0) glDetachShader(id, fragmentShaderId);
            glDeleteShader(fragmentShaderId);
        }

        // In case shader program loading failed, we assign default shader
        if (id == 0)
        {
            // In case shader loading fails, we return the default shader
            NEXUS_LOG(Warning) << "[gl::Context::LoadShaderCode] Failed to load custom shader code, using default shader\n";
            id = state.defaultShaderId;
        }
        /*
        else
        {
            // Get available shader uniforms
            // NOTE: This information is useful for debug...
            int uniformCount = -1;
            glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount);

            for (int i = 0; i < uniformCount; i++)
            {
                int namelen = -1;
                int num = -1;
                char name[256] = { 0 };     // Assume no variable names longer than 256
                GLenum type = GL_ZERO;

                // Get the name of the uniforms
                glGetActiveUniform(id, i, sizeof(name) - 1, &namelen, &num, &type, name);

                name[namelen] = 0;
                NEXUS_LOG(Debug) << "[gl::Context::LoadShaderCode] [ID " << id << "] Active uniform (" << name << ") set at location: " << glGetUniformLocation(id, name) << "\n";
            }
        }
        */
    }
#endif

    return id;
}

// Compile custom shader and return shader id
Uint32 gl::Context::CompileShader(const char *shaderCode, int type)
{
    Uint32 shader = 0;

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, nullptr);

    GLint success = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: NEXUS_LOG(Error) << "[gl::Context::CompileShader] [ID " << shader << "] Failed to compile vertex shader code\n"; break;
            case GL_FRAGMENT_SHADER: NEXUS_LOG(Error) << "[gl::Context::CompileShader] [ID " << shader << "] Failed to compile fragment shader code\n"; break;
            //case GL_GEOMETRY_SHADER:
        #if defined(GRAPHICS_API_OPENGL_43)
            case GL_COMPUTE_SHADER: NEXUS_LOG(Warning) << "[gl::Context::CompileShader] [ID " << shader << "] Failed to compile compute shader code\n"; break;
        #endif
            default: break;
        }

        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            std::string log(maxLength, ' ');
            glGetShaderInfoLog(shader, maxLength, &length, log.data());
            NEXUS_LOG(Error) << "[gl::Context::CompileShader] [ID " << shader << "] Compile error: " << log.c_str() << "\n";
        }
    }
    else
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: NEXUS_LOG(Info) << "[gl::Context::CompileShader] [ID " << shader << "] Vertex shader compiled successfully\n"; break;
            case GL_FRAGMENT_SHADER: NEXUS_LOG(Info) << "[gl::Context::CompileShader] [ID " << shader << "] Fragment shader compiled successfully\n"; break;
            //case GL_GEOMETRY_SHADER:
#           if defined(GRAPHICS_API_OPENGL_43)
            case GL_COMPUTE_SHADER: NEXUS_LOG(Info) << "[gl::Context::CompileShader] [ID " << shader << "] Compute shader compiled successfully\n"; break;
#           endif
            default: break;
        }
    }
#endif

    return shader;
}

// Load custom shader strings and return program id
Uint32 gl::Context::LoadShaderProgram(Uint32 vShaderId, Uint32 fShaderId)
{
    Uint32 program = 0;

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    GLint success = 0;
    program = glCreateProgram();

    glAttachShader(program, vShaderId);
    glAttachShader(program, fShaderId);

    // NOTE: Default attribute shader locations must be Bound before linking
    glBindAttribLocation(program, 0, GL_DEFAULT_SHADER_ATTRIB_NAME_POSITION);
    glBindAttribLocation(program, 1, GL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
    glBindAttribLocation(program, 2, GL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL);
    glBindAttribLocation(program, 3, GL_DEFAULT_SHADER_ATTRIB_NAME_COLOR);
    glBindAttribLocation(program, 4, GL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT);
    glBindAttribLocation(program, 5, GL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2);

    // NOTE: If some attrib name is no found on the shader, it locations becomes -1

    glLinkProgram(program);

    // NOTE: All uniform variables are intitialised to 0 when a program links

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        NEXUS_LOG(Error) << "[gl::Context::LoadShaderProgram] [ID " << program << "] Failed to link shader program\n";

        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            std::string log(maxLength, ' ');
            glGetProgramInfoLog(program, maxLength, &length, log.data());
            NEXUS_LOG(Error) << "[gl::Context::LoadShaderProgram] [ID " << program << "] Link error: " << log.c_str() << "\n";
        }

        glDeleteProgram(program);

        program = 0;
    }
    else
    {
        // Get the size of compiled shader program (not available on OpenGL ES 2.0)
        // NOTE: If GL_LINK_STATUS is GL_FALSE, program binary length is zero.
        //GLint binarySize = 0;
        //glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binarySize);

        NEXUS_LOG(Info) << "[gl::Context::LoadShaderProgram] [ID " << program << "] Program shader loaded successfully\n";
    }
#endif
    return program;
}

// Unload shader program
void gl::Context::UnloadShaderProgram(Uint32 id)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glDeleteProgram(id);

    NEXUS_LOG(Info) << "[gl::Context::UnloadShaderProgram] [ID " << id << "] Unloaded shader program data from VRAM (GPU)\n";
#endif
}

// Get shader location uniform
int gl::Context::GetLocationUniform(Uint32 shaderId, const std::string& uniformName) const
{
    int location = -1;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    location = glGetUniformLocation(shaderId, uniformName.c_str());

    if (location == -1) NEXUS_LOG(Warning) << "[gl::Context::GetLocationUniform] [ID " << shaderId << "] Failed to find shader uniform: " << uniformName << "\n";
    else NEXUS_LOG(Info) << "[gl::Context::GetLocationUniform] [ID " << shaderId << "] Shader uniform (" << uniformName << ") set at location: " << location << "\n";
#endif
    return location;
}

// Get shader location attribute
int gl::Context::GetLocationAttrib(Uint32 shaderId, const std::string& attribName) const
{
    int location = -1;
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    location = glGetAttribLocation(shaderId, attribName.c_str());

    if (location == -1) NEXUS_LOG(Warning) << "[gl::Context::GetLocationAttrib] [ID " << shaderId << "] Failed to find shader attribute: " << attribName << "\n";
    else NEXUS_LOG(Info) << "[gl::Context::GetLocationAttrib] [ID " << shaderId << "] Shader attribute (" << attribName << ") set at location: " << location << "\n";
#endif
    return location;
}

// Set shader value uniform
void gl::Context::SetUniform(int locIndex, const void *value, ShaderUniformType uniformType, int count)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    switch (uniformType)
    {
        case ShaderUniformType::Float:      glUniform1fv(locIndex, count, static_cast<const float*>(value));   break;
        case ShaderUniformType::Vec2:       glUniform2fv(locIndex, count, static_cast<const float*>(value));   break;
        case ShaderUniformType::Vec3:       glUniform3fv(locIndex, count, static_cast<const float*>(value));   break;
        case ShaderUniformType::Vec4:       glUniform4fv(locIndex, count, static_cast<const float*>(value));   break;
        case ShaderUniformType::Int:        glUniform1iv(locIndex, count, static_cast<const int*>(value));     break;
        case ShaderUniformType::IVec2:      glUniform2iv(locIndex, count, static_cast<const int*>(value));     break;
        case ShaderUniformType::IVec3:      glUniform3iv(locIndex, count, static_cast<const int*>(value));     break;
        case ShaderUniformType::IVec4:      glUniform4iv(locIndex, count, static_cast<const int*>(value));     break;
        case ShaderUniformType::Sampler2D:  glUniform1iv(locIndex, count, static_cast<const int*>(value));     break;
        default: NEXUS_LOG(Warning) << "[gl::Context::SetUniform] Failed to set uniform value, data type not recognized\n";
    }
#endif
}

// Set shader value attribute
void gl::Context::SetVertexAttributeDefault(int locIndex, const void *value, ShaderAttributeType attribType, int count)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    switch (attribType)
    {
        case ShaderAttributeType::Float: if (count == 1) glVertexAttrib1fv(locIndex, static_cast<const float*>(value)); break;
        case ShaderAttributeType::Vec2: if (count == 2) glVertexAttrib2fv(locIndex, static_cast<const float*>(value)); break;
        case ShaderAttributeType::Vec3: if (count == 3) glVertexAttrib3fv(locIndex, static_cast<const float*>(value)); break;
        case ShaderAttributeType::Vec4: if (count == 4) glVertexAttrib4fv(locIndex, static_cast<const float*>(value)); break;
        default: NEXUS_LOG(Warning) << "[gl::Context::SetVertexAttributeDefault] Failed to set attrib default value, data type not recognized\n";
    }
#endif
}

// Set shader value uniform matrix
void gl::Context::SetUniformMatrix(int locIndex, const math::Mat4& mat)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    glUniformMatrix4fv(locIndex, 1, false, mat);
#endif
}

// Set shader value uniform sampler
void gl::Context::SetUniformSampler(int locIndex, Uint32 textureId)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    // Check if texture is already active
    for (int i = 0; i < GL_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++)
        if (state.activeTextureId[i] == textureId) return;

    // Register a new active texture for the internal batch system
    // NOTE: Default texture is always activated as GL_TEXTURE0
    for (int i = 0; i < GL_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++)
    {
        if (state.activeTextureId[i] == 0)
        {
            glUniform1i(locIndex, 1 + i);              // Activate new texture unit
            state.activeTextureId[i] = textureId; // Save texture id for binding on drawing
            break;
        }
    }
#endif
}

// Set shader currently active (id and locations)
void gl::Context::SetShader(Uint32 id, const int *locs)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    if (state.currentShaderId != id)
    {
        DrawRenderBatch(currentBatch);
        state.currentShaderId = id;
        state.currentShaderLocs = locs;
    }
#endif
}

// Load compute shader program
Uint32 gl::Context::LoadComputeShaderProgram(Uint32 shaderId)
{
    Uint32 program = 0;

#if defined(GRAPHICS_API_OPENGL_43)
    GLint success = 0;
    program = glCreateProgram();
    glAttachShader(program, shaderId);
    glLinkProgram(program);

    // NOTE: All uniform variables are intitialised to 0 when a program links

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        NEXUS_LOG(Warning) << "[gl::Context::LoadComputeShaderProgram] [ID " << program << "] Failed to link compute shader program\n";

        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            std::string log(maxLength, ' ');
            glGetProgramInfoLog(program, maxLength, &length, log.data());
            NEXUS_LOG(Warning) << "[gl::Context::LoadComputeShaderProgram] [ID " << program << "] Link error: " << log.c_str() << "\n";
        }

        glDeleteProgram(program);

        program = 0;
    }
    else
    {
        // Get the size of compiled shader program (not available on OpenGL ES 2.0)
        // NOTE: If GL_LINK_STATUS is GL_FALSE, program binary length is zero.
        //GLint binarySize = 0;
        //glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binarySize);

        NEXUS_LOG(Info) << "[gl::Context::LoadComputeShaderProgram] [ID " << program << "] Compute shader program loaded successfully\n";
    }
#endif

    return program;
}

// Dispatch compute shader (equivalent to *draw* for graphics pilepine)
void gl::Context::ComputeShaderDispatch(Uint32 groupX, Uint32 groupY, Uint32 groupZ)
{
#if defined(GRAPHICS_API_OPENGL_43)
    glDispatchCompute(groupX, groupY, groupZ);
#endif
}

// Load shader storage buffer object (SSBO)
Uint32 gl::Context::LoadShaderBuffer(Uint32 size, const void *data, BufferUsage usageHint)
{
    Uint32 ssbo = 0;

#if defined(GRAPHICS_API_OPENGL_43)
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, static_cast<int>(usageHint));
    if (data == nullptr) glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);    // Clear buffer data to 0
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#endif

    return ssbo;
}

// Unload shader storage buffer object (SSBO)
void gl::Context::UnloadShaderBuffer(Uint32 ssboId)
{
#if defined(GRAPHICS_API_OPENGL_43)
    glDeleteBuffers(1, &ssboId);
#endif
}

// Update SSBO buffer data
void gl::Context::UpdateShaderBuffer(Uint32 id, const void *data, Uint32 dataSize, Uint32 offset)
{
#if defined(GRAPHICS_API_OPENGL_43)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, dataSize, data);
#endif
}

// Get SSBO buffer size
Uint32 gl::Context::GetShaderBufferSize(Uint32 id) const
{
    long long size = 0;

#if defined(GRAPHICS_API_OPENGL_43)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glGetInteger64v(GL_SHADER_STORAGE_BUFFER_SIZE, &size);
#endif

    return (size > 0)? (Uint32)size : 0;
}

// Read SSBO buffer data (GPU->CPU)
void gl::Context::ReadShaderBuffer(Uint32 id, void *dest, Uint32 count, Uint32 offset)
{
#if defined(GRAPHICS_API_OPENGL_43)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, count, dest);
#endif
}

// Bind SSBO buffer
void gl::Context::BindShaderBuffer(Uint32 id, Uint32 index)
{
#if defined(GRAPHICS_API_OPENGL_43)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
#endif
}

// Copy SSBO buffer data
void gl::Context::CopyShaderBuffer(Uint32 destId, Uint32 srcId, Uint32 destOffset, Uint32 srcOffset, Uint32 count)
{
#if defined(GRAPHICS_API_OPENGL_43)
    glBindBuffer(GL_COPY_READ_BUFFER, srcId);
    glBindBuffer(GL_COPY_WRITE_BUFFER, destId);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffset, destOffset, count);
#endif
}

// Bind image texture
void gl::Context::BindImageTexture(Uint32 id, Uint32 index, int format, bool readonly)
{
#if defined(GRAPHICS_API_OPENGL_43)
    Uint32 glInternalFormat = 0, glFormat = 0, glType = 0;

    GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);
    glBindImageTexture(index, id, 0, 0, 0, readonly? GL_READ_ONLY : GL_READ_WRITE, glInternalFormat);
#endif
}


// Matrix state management
//-----------------------------------------------------------------------------------------
// Get internal modelview matrix
math::Mat4 gl::Context::GetMatrixModelview() const
{
    math::Mat4 matrix;

#if defined(GRAPHICS_API_OPENGL_11)
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
#else
    matrix = state.modelview;
#endif

    return matrix;
}

// Get internal projection matrix
math::Mat4 gl::Context::GetMatrixProjection() const
{
    math::Mat4 matrix;

#if defined(GRAPHICS_API_OPENGL_11)
    glGetFloatv(GL_PROJECTION_MATRIX, matrix);
#else
    matrix = state.projection;
#endif

    return matrix;
}

// Get internal accumulated transform matrix
math::Mat4 gl::Context::GetMatrixTransform() const
{
    math::Mat4 mat = math::Mat4::Identity();

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    // TODO: Consider possible transform matrices in the state.stack
    // Is this the right order? or should we start with the first stored matrix instead of the last one?
    //math::Mat4 matStackTransform = math::Mat4::Identity();
    //for (int i = state.stackCounter; i > 0; i--) matStackTransform = state.stack[i] * matStackTransform;
    mat = state.transform;
#endif

    return mat;
}

// Get internal projection matrix for stereo render (selected eye)
math::Mat4 gl::Context::GetMatrixProjectionStereo(int eye) const
{
    math::Mat4 mat = math::Mat4::Identity();
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    mat = state.projectionStereo[eye];
#endif
    return mat;
}

// Get internal view offset matrix for stereo render (selected eye)
math::Mat4 gl::Context::GetMatrixViewOffsetStereo(int eye) const
{
    math::Mat4 mat = math::Mat4::Identity();
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    mat = state.viewOffsetStereo[eye];
#endif
    return mat;
}

// Set a custom modelview matrix (replaces internal modelview matrix)
void gl::Context::SetMatrixModelview(const math::Mat4& view)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    state.modelview = view;
#endif
}

// Set a custom projection matrix (replaces internal projection matrix)
void gl::Context::SetMatrixProjection(const math::Mat4& projection)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    state.projection = projection;
#endif
}

// Set eyes projection matrices for stereo rendering
void gl::Context::SetMatrixProjectionStereo(const math::Mat4& right, const math::Mat4& left)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    state.projectionStereo[0] = right;
    state.projectionStereo[1] = left;
#endif
}

// Set eyes view offsets matrices for stereo rendering
void gl::Context::SetMatrixViewOffsetStereo(const math::Mat4& right, const math::Mat4& left)
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    state.viewOffsetStereo[0] = right;
    state.viewOffsetStereo[1] = left;
#endif
}

// Load and draw a quad in NDC
void gl::Context::LoadDrawQuad()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    Uint32 quadVAO = 0;
    Uint32 quadVBO = 0;

    constexpr float vertices[] = {
         // Positions         Texcoords
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    };

    // Gen VAO to contain VBO
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    // Gen and fill vertex buffer (VBO)
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // Bind vertex attributes (position, texcoords)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); // Positions
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), reinterpret_cast<const void*>(3*sizeof(float))); // Texcoords

    // Draw quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    // Delete buffers (VBO and VAO)
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
#endif
}

// Load and draw a cube in NDC
void gl::Context::LoadDrawCube()
{
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    Uint32 cubeVAO = 0;
    Uint32 cubeVBO = 0;

    constexpr float vertices[] = {
         // Positions          Normals               Texcoords
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f
    };

    // Gen VAO to contain VBO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Gen and fill vertex buffer (VBO)
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind vertex attributes (position, normals, texcoords)
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0); // Positions
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<const void*>(3*sizeof(float))); // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<const void*>(6*sizeof(float))); // Texcoords
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Draw cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Delete VBO and VAO
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
#endif
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
// Load default shader (just vertex positioning and texture coloring)
// NOTE: This shader program is used for internal buffers
// NOTE: Loaded: state.defaultShaderId, state.defaultShaderLocs
void gl::Context::LoadShaderDefault()
{
    state.defaultShaderLocs = new int[GL_MAX_SHADER_LOCATIONS];

    // NOTE: All locations must be reseted to -1 (no location)
    std::fill(state.defaultShaderLocs, state.defaultShaderLocs + GL_MAX_SHADER_LOCATIONS, -1);

    // Vertex shader directly defined, no external file required
    constexpr char defaultVShaderCode[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
        "precision mediump float;"
#   endif
#   if defined(GLSL_ES_100) || defined(GLSL_120)
        "attribute vec3 vertexPosition;"
        "attribute vec2 vertexTexCoord;"
        "attribute vec4 vertexColor;"
        "varying vec2 fragTexCoord;"
        "varying vec4 fragColor;"
#   else
        "in vec3 vertexPosition;"
        "in vec2 vertexTexCoord;"
        "in vec4 vertexColor;"
        "out vec2 fragTexCoord;"
        "out vec4 fragColor;"
#   endif
        "uniform mat4 mvp;"
        "void main()"
        "{"
            "fragTexCoord = vertexTexCoord;"
            "fragColor = vertexColor;"
            "gl_Position = mvp*vec4(vertexPosition, 1.0);"
        "}";

    // Fragment shader directly defined, no external file required
    constexpr char defaultFShaderCode[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
        "precision mediump float;"
#   endif
#   if defined(GLSL_ES_100) || defined(GLSL_120)
        "varying vec2 fragTexCoord;"
        "varying vec4 fragColor;"
        "uniform sampler2D texture0;"
        "uniform vec4 colDiffuse;"
        "void main()"
        "{"
            "vec4 texelColor = texture2D(texture0, fragTexCoord);"
            "gl_FragColor = texelColor*colDiffuse*fragColor;"
        "}";
#   else
        "in vec2 fragTexCoord;"
        "in vec4 fragColor;"
        "out vec4 finalColor;"
        "uniform sampler2D texture0;"
        "uniform vec4 colDiffuse;"
        "void main()"
        "{"
            "vec4 texelColor = texture(texture0, fragTexCoord);"
            "finalColor = texelColor*colDiffuse*fragColor;"
        "}";
#   endif

    // NOTE: Compiled vertex/fragment shaders are not deleted,
    // they are kept for re-use as default shaders in case some shader loading fails
    state.defaultVShaderId = CompileShader(defaultVShaderCode, GL_VERTEX_SHADER);     // Compile default vertex shader
    state.defaultFShaderId = CompileShader(defaultFShaderCode, GL_FRAGMENT_SHADER);   // Compile default fragment shader

    state.defaultShaderId = LoadShaderProgram(state.defaultVShaderId, state.defaultFShaderId);

    if (state.defaultShaderId > 0)
    {
        NEXUS_LOG(Info) << "[gl::Context::LoadShaderDefault] [ID " << state.defaultShaderId << "] Default shader loaded successfully\n";

        // Set default shader locations: attributes locations
        state.defaultShaderLocs[LocVertexPosition] = glGetAttribLocation(state.defaultShaderId, "vertexPosition");
        state.defaultShaderLocs[LocVertexTexCoord01] = glGetAttribLocation(state.defaultShaderId, "vertexTexCoord");
        state.defaultShaderLocs[LocVertexColor] = glGetAttribLocation(state.defaultShaderId, "vertexColor");

        // Set default shader locations: uniform locations
        state.defaultShaderLocs[LocMatrixMVP]  = glGetUniformLocation(state.defaultShaderId, "mvp");
        state.defaultShaderLocs[LocColorDiffuse] = glGetUniformLocation(state.defaultShaderId, "colDiffuse");
        state.defaultShaderLocs[LocMapDiffuse] = glGetUniformLocation(state.defaultShaderId, "texture0");
    }
    else NEXUS_LOG(Error) << "[gl::Context::LoadShaderDefault] [ID " << state.defaultShaderId << "] Failed to load default shader\n";
}

// Unload default shader
// NOTE: Unloads: state.defaultShaderId, state.defaultShaderLocs
void gl::Context::UnloadShaderDefault()
{
    glUseProgram(0);

    glDetachShader(state.defaultShaderId, state.defaultVShaderId);
    glDetachShader(state.defaultShaderId, state.defaultFShaderId);
    glDeleteShader(state.defaultVShaderId);
    glDeleteShader(state.defaultFShaderId);

    glDeleteProgram(state.defaultShaderId);

    delete[] state.defaultShaderLocs;

    NEXUS_LOG(Info) << "[gl::Context::UnloadShaderDefault] [ID " << state.defaultShaderId << "] Default shader unloaded successfully\n";
}

#endif  // GRAPHICS_API_OPENGL_33 || GRAPHICS_API_OPENGL_ES2
