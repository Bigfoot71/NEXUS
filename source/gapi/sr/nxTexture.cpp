#include "gapi/sr/nxTexture.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

void _sr_impl::Texture::Draw(shape2D::RectangleF src, const shape2D::RectangleF& dst, const math::Vec2& origin, float rotation, const gfx::Color& tint)  const
{
    bool flipX = false;

    if (src.w < 0) flipX = true, src.w *= -1;
    if (src.h < 0) src.y -= src.h;

    math::Vec2 topLeft, topRight;
    math::Vec2 bottomLeft, bottomRight;

    // Only calculate rotation if needed
    if (rotation == 0.0f)
    {
        float x = dst.x - origin.x;
        float y = dst.y - origin.y;
        topLeft = { x, y };
        topRight = { x + dst.w, y };
        bottomLeft = { x, y + dst.h };
        bottomRight = { x + dst.w, y + dst.h };
    }
    else
    {
        rotation *= math::Deg2Rad;

        float s = std::sin(rotation);
        float c = std::cos(rotation);
        float x = dst.x;
        float y = dst.y;
        float dx = -origin.x;
        float dy = -origin.y;

        topLeft.x = x + dx * c - dy * s;
        topLeft.y = y + dx * s + dy * c;

        topRight.x = x + (dx + dst.w) * c - dy * s;
        topRight.y = y + (dx + dst.w) * s + dy * c;

        bottomLeft.x = x + dx * c - (dy + dst.h) * s;
        bottomLeft.y = y + dx * s + (dy + dst.h) * c;

        bottomRight.x = x + (dx + dst.w) * c - (dy + dst.h) * s;
        bottomRight.y = y + (dx + dst.w) * s + (dy + dst.h) * c;
    }

    ctx.SetTexture(this);
    ctx.Begin(sr::DrawMode::Quads);

        ctx.Color(tint.r, tint.g, tint.b, tint.a);
        ctx.Normal(0.0f, 0.0f, 1.0f);   // Normal vector pointing towards viewer

        // Top-left corner for texture and quad
        if (flipX) ctx.TexCoord((src.x + src.w)/surface->w, src.y/surface->h);
        else ctx.TexCoord(src.x/surface->w, src.y/surface->h);
        ctx.Vertex(topLeft.x, topLeft.y);

        // Bottom-left corner for texture and quad
        if (flipX) ctx.TexCoord((src.x + src.w)/surface->w, (src.y + src.h)/surface->h);
        else ctx.TexCoord(src.x/surface->w, (src.y + src.h)/surface->h);
        ctx.Vertex(bottomLeft.x, bottomLeft.y);

        // Bottom-right corner for texture and quad
        if (flipX) ctx.TexCoord(src.x/surface->w, (src.y + src.h)/surface->h);
        else ctx.TexCoord((src.x + src.w)/surface->w, (src.y + src.h)/surface->h);
        ctx.Vertex(bottomRight.x, bottomRight.y);

        // Top-right corner for texture and quad
        if (flipX) ctx.TexCoord(src.x/surface->w, src.y/surface->h);
        else ctx.TexCoord((src.x + src.w)/surface->w, src.y/surface->h);
        ctx.Vertex(topRight.x, topRight.y);

    ctx.End();
    ctx.SetTexture(0u);
}

void _sr_impl::Texture::Draw(float x, float y, const gfx::Color& tint) const
{
    Draw({ 0, 0, static_cast<float>(surface->w), static_cast<float>(surface->h) },
            { x, y, static_cast<float>(surface->w), static_cast<float>(surface->h) },
            { 0, 0 }, 0.0f, tint);
}

void _sr_impl::Texture::Draw(const math::Vec2& position, const gfx::Color& tint) const
{
    Draw({ 0, 0, static_cast<float>(surface->w), static_cast<float>(surface->h) },
            { position.x, position.y, static_cast<float>(surface->w), static_cast<float>(surface->h) },
            { 0, 0 }, 0.0f, tint);
}

void _sr_impl::Texture::Draw(const math::Vec2& position, float rotation, float scale, const gfx::Color& tint) const
{
    Draw({ 0.0f, 0.0f, static_cast<float>(surface->w), static_cast<float>(surface->h) },
            { position.x, position.y, surface->w*scale, surface->w*scale },
            { surface->w/2.0f * scale, surface->h/2.0f * scale },
            rotation, tint);
}

void _sr_impl::Texture::Draw(const math::Vec2& position, float rotation, const math::Vec2& origin, const math::Vec2& scale, const gfx::Color& tint) const
{
    Draw({ 0.0f, 0.0f, static_cast<float>(surface->w), static_cast<float>(surface->h) },
            { position.x, position.y, surface->w*scale.x, surface->h*scale.y },
            origin, rotation, tint);
}

void _sr_impl::Texture::Draw(const shape2D::RectangleF& src, const math::Vec2& position, const gfx::Color& tint) const
{
    Draw({ 0, 0, static_cast<float>(surface->w), static_cast<float>(surface->h) }, { position.x, position.y, static_cast<float>(surface->w), static_cast<float>(surface->h) }, { 0, 0 }, 0, tint);
}

void _sr_impl::Texture::DrawNinePatch(sr::NinePatchInfo ninePatchInfo, const shape2D::RectangleF& dest, const math::Vec2& origin, float rotation, const gfx::Color& tint)
{
    float patchWidth = std::max(dest.w, 0.0f);
    float patchHeight = std::max(dest.h, 0.0f);

    if (ninePatchInfo.source.w < 0) ninePatchInfo.source.x -= ninePatchInfo.source.w;
    if (ninePatchInfo.source.h < 0) ninePatchInfo.source.y -= ninePatchInfo.source.h;

    switch (ninePatchInfo.layout)
    {
        case sr::NinePatchInfo::Layout::ThreePatchHorizontal:
            patchHeight = ninePatchInfo.source.h;
            break;

        case sr::NinePatchInfo::Layout::ThreePatchVertical:
            patchWidth = ninePatchInfo.source.w;
            break;

        default:
            break;
    }

    bool drawCenter = true;
    bool drawMiddle = true;
    shape2D::AABB bounds = ninePatchInfo.bounds;

    // Adjust the lateral (left and right) border widths in case patchWidth < texture.width
    if (ninePatchInfo.layout != sr::NinePatchInfo::Layout::ThreePatchVertical && patchWidth <= bounds.min.x + bounds.max.x)
    {
        drawCenter = false;
        bounds.min.x = bounds.min.x / (bounds.min.x + bounds.max.x) * patchWidth;
        bounds.max.x = patchWidth - bounds.min.x;
    }

    // Adjust the lateral (top and bottom) border heights in case patchHeight < texture.height
    if (ninePatchInfo.layout != sr::NinePatchInfo::Layout::ThreePatchHorizontal && patchHeight <= bounds.min.y + bounds.max.y)
    {
        drawMiddle = false;
        bounds.min.y = (bounds.min.y/(bounds.min.y + bounds.max.y))*patchHeight;
        bounds.max.y = patchHeight - bounds.min.y;
    }

    math::Vec2 vertA, vertB, vertC, vertD;
    vertA.x = 0.0f;                             // outer left
    vertA.y = 0.0f;                             // outer top
    vertB.x = bounds.min.x;                     // inner left
    vertB.y = bounds.min.y;                     // inner top
    vertC.x = patchWidth  - bounds.max.x;       // inner right
    vertC.y = patchHeight - bounds.max.y;       // inner bottom
    vertD.x = patchWidth;                       // outer right
    vertD.y = patchHeight;                      // outer bottom

    math::Vec2 coordA, coordB, coordC, coordD;
    coordA.x = static_cast<float>(ninePatchInfo.source.x) / surface->w;
    coordA.y = static_cast<float>(ninePatchInfo.source.y) / surface->h;
    coordB.x = static_cast<float>(ninePatchInfo.source.x + bounds.min.x) / surface->w;
    coordB.y = static_cast<float>(ninePatchInfo.source.y + bounds.min.y) / surface->h;
    coordC.x = static_cast<float>(ninePatchInfo.source.x + ninePatchInfo.source.w  - bounds.max.x) / surface->w;
    coordC.y = static_cast<float>(ninePatchInfo.source.y + ninePatchInfo.source.h - bounds.max.y) / surface->h;
    coordD.x = static_cast<float>(ninePatchInfo.source.x + ninePatchInfo.source.w) / surface->w;
    coordD.y = static_cast<float>(ninePatchInfo.source.y + ninePatchInfo.source.h) / surface->h;

    ctx.SetTexture(this);
    ctx.PushMatrix();

        ctx.Translate(dest.x, dest.y, 0.0f);
        ctx.Rotate(rotation, 0.0f, 0.0f, 1.0f);
        ctx.Translate(-origin.x, -origin.y, 0.0f);

        ctx.Begin(sr::DrawMode::Quads);

            ctx.Color(tint);

            switch (ninePatchInfo.layout)
            {
                case sr::NinePatchInfo::Layout::NinePatch:
                {
                    // TOP-LEFT QUAD
                    ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordA.y); ctx.Vertex(vertA.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // TOP-CENTER QUAD
                        ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-left corner for texture and quad
                    }

                    // TOP-RIGHT QUAD
                    ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordA.y); ctx.Vertex(vertD.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawMiddle)
                    {
                        // MIDDLE-LEFT QUAD
                        ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Top-left corner for texture and quad

                        if (drawCenter)
                        {
                            // MIDDLE-CENTER QUAD
                            ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Bottom-left corner for texture and quad
                            ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Bottom-right corner for texture and quad
                            ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Top-right corner for texture and quad
                            ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Top-left corner for texture and quad
                        }

                        // MIDDLE-RIGHT QUAD
                        ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Top-left corner for texture and quad
                    }

                    // BOTTOM-LEFT QUAD
                    ctx.TexCoord(coordA.x, coordD.y); ctx.Vertex(vertA.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // BOTTOM-CENTER QUAD
                        ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Top-left corner for texture and quad
                    }

                    // BOTTOM-RIGHT QUAD
                    ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordD.y); ctx.Vertex(vertD.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Top-left corner for texture and quad

                }
                break;

                case sr::NinePatchInfo::Layout::ThreePatchVertical:
                {
                    // TOP QUAD
                    ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordA.y); ctx.Vertex(vertD.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordA.y); ctx.Vertex(vertA.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // MIDDLE QUAD
                        ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Top-left corner for texture and quad
                    }

                    // BOTTOM QUAD
                    ctx.TexCoord(coordA.x, coordD.y); ctx.Vertex(vertA.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordD.y); ctx.Vertex(vertD.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Top-left corner for texture and quad
                }
                break;

                case sr::NinePatchInfo::Layout::ThreePatchHorizontal:
                {
                    // LEFT QUAD
                    ctx.TexCoord(coordA.x, coordD.y); ctx.Vertex(vertA.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordA.y); ctx.Vertex(vertA.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // CENTER QUAD
                        ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-left corner for texture and quad
                    }

                    // RIGHT QUAD
                    ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordD.y); ctx.Vertex(vertD.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordA.y); ctx.Vertex(vertD.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-left corner for texture and quad
                }
                break;
            }

        ctx.End();

    ctx.PopMatrix();
    ctx.SetTexture(0u);
}

void _sr_impl::Texture::DrawBillboard(const sr::Camera3D& camera, shape2D::Rectangle source, const math::Vec3& position, const math::Vec3& up, const math::Vec2& size, const math::Vec2& origin, float rotation, const gfx::Color& tint) const
{
    bool flipX = source.w < 0;
    bool flipY = source.h < 0;

    source.w = std::abs(source.w);
    source.h = std::abs(source.h);

    // NOTE: Billboard size will maintain source rectangle aspect ratio, size will represent billboard width
    math::Vec2 sizeRatio(size.x * static_cast<float>(source.w) / source.h, size.y);

    math::Mat4 matView = math::Mat4::LookAt(camera.position, camera.target, camera.up);

    math::Vec3 right(matView.m[0], matView.m[4], matView.m[8]);
    //math::Vec3 up = { matView.m[1], matView.m[5], matView.m[9] };

    math::Vec3 rightScaled = right * (sizeRatio.x / 2);
    math::Vec3 upScaled = up * (sizeRatio.y / 2);

    math::Vec3 p1 = rightScaled + upScaled;
    math::Vec3 p2 = rightScaled - upScaled;

    math::Vec3 topLeft = -p2;
    math::Vec3 topRight = p1;
    math::Vec3 bottomRight = p2;
    math::Vec3 bottomLeft = -p1;

    if (rotation != 0.0f)
    {
        rotation *= math::Deg2Rad;
        float s = std::sin(rotation);
        float c = std::cos(rotation);

        // NOTE: (-1, 1) is the range where origin.x, origin.y is inside the texture
        float rotateAboutX = sizeRatio.x * origin.x / 2;
        float rotateAboutY = sizeRatio.y * origin.y / 2;

        float xtvalue, ytvalue;
        float rotatedX, rotatedY;

        xtvalue = right.Dot(topLeft) - rotateAboutX;                // Project points to x and y coordinates on the billboard plane
        ytvalue = up.Dot(topLeft) - rotateAboutY;
        rotatedX = xtvalue * c - ytvalue * s + rotateAboutX;        // Rotate about the point origin
        rotatedY = xtvalue * s + ytvalue * c + rotateAboutY;
        topLeft = (up * rotatedY) + (right * rotatedX);             // Translate back to cartesian coordinates

        xtvalue = right.Dot(topRight) - rotateAboutX;
        ytvalue = up.Dot(topRight) - rotateAboutY;
        rotatedX = xtvalue * c - ytvalue * s + rotateAboutX;
        rotatedY = xtvalue * s + ytvalue * c + rotateAboutY;
        topRight = (up * rotatedY) + (right * rotatedX);

        xtvalue = right.Dot(bottomRight) - rotateAboutX;
        ytvalue = up.Dot(bottomRight) - rotateAboutY;
        rotatedX = xtvalue * c - ytvalue * s + rotateAboutX;
        rotatedY = xtvalue * s + ytvalue * c + rotateAboutY;
        bottomRight = (up * rotatedY) + (right * rotatedX);

        xtvalue = right.Dot( bottomLeft) - rotateAboutX;
        ytvalue = up.Dot(bottomLeft) - rotateAboutY;
        rotatedX = xtvalue * c - ytvalue * s + rotateAboutX;
        rotatedY = xtvalue * s + ytvalue * c + rotateAboutY;
        bottomLeft = (up * rotatedY) + (right * rotatedX);
    }

    // Translate points to the draw center (position)
    topLeft = topLeft + position;
    topRight = topRight + position;
    bottomRight = bottomRight + position;
    bottomLeft = bottomLeft + position;

    ctx.SetTexture(this);
    ctx.Begin(sr::DrawMode::Quads);

        ctx.Color(tint.r, tint.g, tint.b, tint.a);

        // Bottom-left corner for texture and quad
        float u = flipX ? static_cast<float>(source.x + source.w) / surface->w : static_cast<float>(source.x) / surface->w;
        float v = flipY ? static_cast<float>(source.y + source.h) / surface->h : static_cast<float>(source.y) / surface->h;
        ctx.TexCoord(u,v);
        ctx.Vertex(topLeft.x, topLeft.y, topLeft.z);

        // Top-left corner for texture and quad
        u = flipX ? static_cast<float>(source.x + source.w) / surface->w : static_cast<float>(source.x) / surface->w;
        v = !flipY ? static_cast<float>(source.y + source.h) / surface->h : static_cast<float>(source.y) / surface->h;
        ctx.TexCoord(u,v);
        ctx.Vertex(bottomLeft.x, bottomLeft.y, bottomLeft.z);

        // Top-right corner for texture and quad
        u = !flipX ? static_cast<float>(source.x + source.w) / surface->w : static_cast<float>(source.x) / surface->w;
        v = !flipY ? static_cast<float>(source.y + source.h) / surface->h : static_cast<float>(source.y) / surface->h;
        ctx.TexCoord(u, v);
        ctx.Vertex(bottomRight.x, bottomRight.y, bottomRight.z);

        // Bottom-right corner for texture and quad
        u = !flipX ? static_cast<float>(source.x + source.w) / surface->w : static_cast<float>(source.x) / surface->w;
        v = flipY ? static_cast<float>(source.y + source.h) / surface->h : static_cast<float>(source.y) / surface->h;
        ctx.TexCoord(u, v);
        ctx.Vertex(topRight.x, topRight.y, topRight.z);

    ctx.End();
    ctx.SetTexture(0u);
}

void _sr_impl::Texture::DrawBillboard(const sr::Camera3D& camera, const math::Vec3& position, float size, float rotation, const gfx::Color& tint) const
{
    DrawBillboard(camera, { 0, 0, surface->w, surface->h }, position, { size, size }, rotation, tint);
}

void _sr_impl::Texture::DrawBillboard(const sr::Camera3D& camera, const shape2D::Rectangle& source, const math::Vec3& position, const math::Vec2& size, float rotation, const gfx::Color& tint) const
{
    DrawBillboard(camera, source, position, { 0.0f, 1.0f, 0.0f }, size, {}, rotation, tint);
}
