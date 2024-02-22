#include "gapi/gl/nxWindow.hpp"
#include "gapi/gl/nxContext.hpp"
#include "core/nxException.hpp"

using namespace nexus;

gl::Window::Window(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags, const Attributes& attributes) : Window()
{
    attributes.Apply();
    Create(title, width, height, flags);
}

gl::Window::Window(const std::string& title, Uint32 width, Uint32 height, Context* ctxInherit, core::WindowFlag flags)
{
    if (ctxInherit == nullptr) throw core::NexusException("Window", "The given Context is null.");
    Create(title, width, height, ctxInherit, flags);
}

gl::Window::Window(const std::string& title, Window& window, core::WindowFlag flags)
{
    Create(title, window.GetWidth(), window.GetHeight(), window, flags);
}

gl::Window::~Window()
{
    Destroy();
}

void gl::Window::Create(const std::string& title, Uint32 width, Uint32 height, Context* ctxInherit, core::WindowFlag flags)
{
    // Creation of the window capable of obtaining an OpenGL context
    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, static_cast<Uint32>(flags) | SDL_WINDOW_OPENGL);

    if (!window)
    {
        throw core::NexusException("Window", "Window creation failed.",
                                   "SDL", SDL_GetError());
    }

    // Creation of the gl::Context
    if (ctx == nullptr)
    {
        ctx = std::make_unique<Context>(*this, ctxInherit);
    }

    // Initialize default viewport
    ctx->SetViewport(0, 0, width, height);
}

void gl::Window::Create(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags)
{
    Create(title, width, height, nullptr, flags);
}

void gl::Window::Destroy()
{
    ctx = nullptr;

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void gl::Window::MakeCurrent()
{
    SDL_GL_MakeCurrent(window, *ctx);
}

void gl::Window::Clear(const gfx::Color& color) const
{
    ctx->ClearColor(color);
    ctx->ClearScreenBuffers();
}

const gl::Window& gl::Window::Begin() const
{
    ctx->LoadIdentity();                // Reset current matrix (modelview)
    //ctx->MultMatrix(screenScale);     // Apply screen scaling
    return *this;
}

const gl::Window& gl::Window::End() const
{
    ctx->DrawRenderBatchActive();       // Update and draw internal render batch
    SDL_GL_SwapWindow(window);          // Copy back buffer to front buffer (screen)
    return *this;
}