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

#include "gapi/cmn_impl/nxCamera2D.hpp"
#include "input/nxMouse.hpp"
#include "math/nxMath.hpp"
#include "math/nxVec3.hpp"

using namespace nexus;

_gapi_impl::Camera2D::Camera2D(const math::Vec2& offset, const math::Vec2& target, float rotation, float zoom)
: offset(offset), target(target), rotation(rotation), zoom(zoom)
{ }

void _gapi_impl::Camera2D::Translate(const math::Vec2& delta, bool zoomIndependant)
{
    this->target += zoomIndependant ? delta / this->zoom : delta;
}

void _gapi_impl::Camera2D::Zoom(float inc, float min, float max, bool incZoomIndependant)
{
    this->zoom = std::clamp(zoom + (incZoomIndependant ? inc * this->zoom : inc), min, max);
}

void _gapi_impl::Camera2D::ZoomToWorldPosition(const nexus::math::Vec2& worldPosition, float inc, float min, float max, bool incZoomIndependant)
{
    const math::Vec2 offset = worldPosition - this->target;
    const float previousZoom = this->zoom;

    this->Zoom(inc, min, max);
    this->target = worldPosition - offset * (previousZoom / this->zoom); // wPos - newOffset
}

void _gapi_impl::Camera2D::ZoomToScreenPosition(const nexus::math::Vec2& screenPosition, float inc, float min, float max, bool incZoomIndependant)
{
    this->ZoomToScreenPosition(this->ToWorld(screenPosition), inc, min, max);
}

void _gapi_impl::Camera2D::ZoomToMouse(float inc, float min, float max, bool incZoomIndependant)
{
    this->ZoomToWorldPosition(this->ToWorld(input::GetMousePosition()), inc, min, max);
}

math::Vec2 _gapi_impl::Camera2D::ToWorld(const math::Vec2& point) const
{
    math::Vec3 transform = math::Vec3(point.x, point.y, 0)
        .Transformed(this->GetCameraMatrix2D().Invert());

    return { transform.x, transform.y };
}

math::Vec2 _gapi_impl::Camera2D::ToScreen(const math::Vec2& point) const
{
    math::Vec3 transform = math::Vec3(point.x, point.y, 0)
        .Transformed(GetCameraMatrix2D());

    return { transform.x, transform.y };
}

math::Mat4 _gapi_impl::Camera2D::GetCameraMatrix2D() const
{
    const math::Mat4 matOrigin      = math::Mat4::Translate(-target.x, -target.y, 0.0f);
    const math::Mat4 matRotation    = math::Mat4::Rotate(math::Vec3{ 0.0f, 0.0f, 1.0f }, rotation);
    const math::Mat4 matScale       = math::Mat4::Scale(zoom, zoom, 1.0f);
    const math::Mat4 matTranslation = math::Mat4::Translate(offset.x, offset.y, 0.0f);

    return (matOrigin * (matScale * matRotation)) * matTranslation;
}
