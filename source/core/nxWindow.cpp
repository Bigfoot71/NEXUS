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

#include "core/nxWindow.hpp"
#include <SDL_video.h>
#include <SDL.h>

using namespace nexus;

/* WINDOW CLASS */

core::Window::Window()
: window(nullptr)
{
    Window::windowCount++;

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            throw core::NexusException("core::Window", "Initialization of the video subsystem failed.",
                "SDL", SDL_GetError());
        }
    }
}

core::Window::Window(const std::string& title, Uint32 width, Uint32 height, WindowFlag flags)
: window(nullptr)
{
    Window::windowCount++;

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            throw core::NexusException("core::Window", "Initialization of the video subsystem failed.",
                "SDL", SDL_GetError());
        }
    }

    Create(title, width, height, flags);
}

core::Window::~Window()
{
    Destroy();

    Window::windowCount--;

    if (Window::windowCount == 0 && SDL_WasInit(SDL_INIT_VIDEO))
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}

SDL_Window* core::Window::Get() const
{
    return window;
}

Uint32 core::Window::GetID() const
{
    return SDL_GetWindowID(window);
}

int core::Window::GetMonitor() const
{
    return SDL_GetWindowDisplayIndex(window);
}

core::WindowFlag core::Window::GetState() const
{
    return static_cast<WindowFlag>(SDL_GetWindowFlags(window));
}

int core::Window::GetWidth() const
{
    int w;
    SDL_GetWindowSize(window, &w, nullptr);
    return w;
}

int core::Window::GetHeight() const
{
    int h;
    SDL_GetWindowSize(window, nullptr, &h);
    return h;
}

math::IVec2 core::Window::GetSize() const
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return math::Vec2(w, h);
}

shape2D::Rectangle core::Window::GetRectSize() const
{
    return { math::IVec2(0, 0), GetSize() };
}

float core::Window::GetAspect() const
{
    const math::Vec2 s = GetSize();
    return (s.x > s.y) ? s.x / s.y : s.x / s.y;
}

core::DisplayMode core::Window::GetDisplayMode(int modeIndex)
{
    core::DisplayMode displayMode{};
    if (SDL_GetDisplayMode(this->GetMonitor(), modeIndex, &displayMode) < 0)
    {
        NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
    }
    return displayMode;
}

core::DisplayMode core::Window::GetCurrentDisplayMode() const
{
    DisplayMode displayMode{};
    if (SDL_GetWindowDisplayMode(window, &displayMode) < 0)
    {
        NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
    }
    return displayMode;
}

gfx::Surface core::Window::GetSurface()
{
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    if (surface == nullptr)
    {
        throw core::NexusException("core::Window", "Failed to get window surface.",
            "SDL", SDL_GetError());
    }

    return gfx::Surface(surface, false);
}

void core::Window::UpdateSurface()
{
    if (SDL_UpdateWindowSurface(window) < 0)
    {
        throw core::NexusException("core::Window", "Unable to update the window surface.",
            "SDL", SDL_GetError());
    }
}

bool core::Window::IsValid()
{
    return (window != nullptr);
}

bool core::Window::IsState(WindowFlag flags)
{
    WindowFlag f = WindowFlag::Minimized | WindowFlag::Maximized;
    Uint32 _flags = SDL_GetWindowFlags(window);
    return _flags & static_cast<Uint32>(flags);
}

void core::Window::ToggleFullscreen()
{
    Uint32 flags = SDL_GetWindowFlags(window);

    if (IsState(WindowFlag::Fullscreen))
    {
        if (SDL_SetWindowFullscreen(window, 0) < 0)
        {
            NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
        }
    }
    else
    {
        if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0)
        {
            NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
        }
    }
}

void core::Window::ToggleFullscreenDesktop()
{
    Uint32 flags = SDL_GetWindowFlags(window);

    if (IsState(WindowFlag::FullscreenDesktop))
    {
        if (SDL_SetWindowFullscreen(window, 0) < 0)
        {
            NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
        }
    }
    else
    {
        if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
        {
            NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
        }
    }
}

void core::Window::SetPosition(Uint32 x, Uint32 y)
{
    SDL_SetWindowPosition(window, x, y);
}

void core::Window::SetSize(Uint32 width, Uint32 height)
{
    SDL_SetWindowSize(window, width, height);
}

void core::Window::SetDisplayMode(const DisplayMode& mode)
{
    if (SDL_SetWindowDisplayMode(window, &mode) < 0)
    {
        NEXUS_LOG(Warning) << "[SDL] " << SDL_GetError();
    }
}
