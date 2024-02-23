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
