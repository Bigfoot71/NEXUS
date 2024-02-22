#include "gapi/sr/nxWindow.hpp"
#include "gapi/sr/nxContext.hpp"
#include <memory>

using namespace nexus;

sr::Window::Window(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags) : Window()
{
    Create(title, width, height, flags);
}

sr::Window::~Window()
{
    Destroy();
}

void sr::Window::Create(const std::string& title, Uint32 width, Uint32 height, core::WindowFlag flags)
{
    core::Window::Create(title, width, height, flags);

    if (framebuffer == nullptr)
    {
        framebuffer = std::make_unique<Framebuffer>(this->GetSurface());
    }

    if (ctx == nullptr)
    {
        ctx = std::make_unique<Context>(*framebuffer);
        ctx->SetViewport(0, 0, width, height);
    }
}

void sr::Window::Destroy()
{
    ctx = nullptr;

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void sr::Window::Clear(const gfx::Color& color)
{
    bool lockedHere = false;

    if (framebuffer->MustLock() && !framebuffer->IsLocked())
    {
        framebuffer->Lock();
        lockedHere = true;
    }

    framebuffer->Clear(color);

    if (lockedHere)
    {
        framebuffer->Unlock();
    }
}

sr::Window& sr::Window::Begin()
{
    framebuffer->SetSurface(this->GetSurface());
    framebuffer->Begin();
    ctx->LoadIdentity();
    return *this;
}

sr::Window& sr::Window::End()
{
    framebuffer->End();
    UpdateSurface();
    return *this;
}
