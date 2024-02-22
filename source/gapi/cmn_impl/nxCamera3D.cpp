#include "gapi/cmn_impl/nxCamera3D.hpp"
#include "input/nxGamepad.hpp"
#include "input/nxKeyboard.hpp"
#include "input/nxMouse.hpp"
#include "math/nxMath.hpp"
#include "core/nxLog.hpp"

using namespace nexus;

_gapi_impl::Camera3D::Camera3D(const math::Vec3& position, const math::Vec3& target, const math::Vec3& up, float fovy, float aspect, float near, float far, bool orthographic)
: position(position)
, target(target)
, up(up)
, aspect(aspect)
, fovy(fovy)
, near(near)
, far(far)
, orthographic(orthographic)
{ }

void _gapi_impl::Camera3D::MoveForward(float distance, bool moveInWorldPlane)
{
    math::Vec3 forward = GetForward();

    if (moveInWorldPlane)
    {
        // Project vector onto world plane
        forward.y = 0;
        forward.Normalize();
    }

    // Scale by distance
    forward *= distance;

    // Move position and target
    position += forward;
    target += forward;
}

void _gapi_impl::Camera3D::MoveUp(float distance)
{
    math::Vec3 up = GetUp();

    // Scale by distance
    up *= distance;

    // Move position and target
    position += up;
    target += up;
}

void _gapi_impl::Camera3D::MoveRight(float distance, bool moveInWorldPlane)
{
    math::Vec3 right = GetRight();

    if (moveInWorldPlane)
    {
        // Project vector onto world plane
        right.y = 0;
        right.Normalize();
    }

    // Scale by distance
    right *= distance;

    // Move position and target
    position += right;
    target += right;
}

void _gapi_impl::Camera3D::MoveToTarget(float delta)
{
    float distance = position.Distance(target);

    // Apply delta
    distance += delta;

    // Distance must be greater than 0
    if (distance < 0) distance = 0.001f;

    // Set new distance by moving the position along the forward vector
    position = target + GetForward() * -distance;
}

void _gapi_impl::Camera3D::Yaw(float angle, bool rotateAroundTarget)
{
    // Rotation axis
    math::Vec3 up = GetUp();

    // View vector
    math::Vec3 targetPosition = target - position;

    // Rotate view vector around up axis
    targetPosition.Rotate(up, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        position = target - targetPosition;
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        target = position + targetPosition;
    }
}

void _gapi_impl::Camera3D::Pitch(float angle, bool lockView, bool rotateAroundTarget, bool rotateUp)
{
    // View vector
    math::Vec3 targetPosition = target - position;

    if (lockView)
    {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = up.Angle(targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = (-up).Angle(targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    // Rotation axis
    math::Vec3 right = GetRight();

    // Rotate view vector around right axis
    targetPosition.Rotate(right, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        position = target - targetPosition;
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        target = position + targetPosition;
    }

    if (rotateUp)
    {
        // Rotate up direction around right axis
        up.Rotate(right, angle);
    }
}

void _gapi_impl::Camera3D::Roll(float angle)
{
    up.Rotate(GetForward(), angle);
}

math::Vec3 _gapi_impl::Camera3D::GetDirection() const
{
    return (target - position).Normalized();
}

math::Mat4 _gapi_impl::Camera3D::GetViewMatrix() const
{
    return math::Mat4::LookAt(position, target, up);
}

math::Mat4 _gapi_impl::Camera3D::GetProjectionMatrix() const
{
    if (orthographic)
    {
        double top = fovy / 2.0;
        double right = top * aspect;
        return math::Mat4::Ortho(-right, right, -top, top, near, far);
    }
    return math::Mat4::Perspective(fovy*math::Deg2Rad, aspect, near, far);
}

void _gapi_impl::Camera3D::Translate(const math::Vec3& delta)
{
    MoveForward(delta.z);
    MoveRight(delta.x);
    MoveUp(delta.y);
}

// NOTE: angle must be provided in degrees
void _gapi_impl::Camera3D::Rotate(const math::Vec3& delta, bool lockView)
{
    Pitch(-delta.y * math::Deg2Rad, lockView);
    Yaw(-delta.x * math::Deg2Rad);
    Roll(delta.z * math::Deg2Rad);
}

void _gapi_impl::Camera3D::SetAspect(float aspect)
{
    this->aspect = aspect;
}

void _gapi_impl::Camera3D::SetAspect(const math::Vec2& viewport)
{
    aspect = (viewport.x > viewport.y)
        ? viewport.x / viewport.y
        : viewport.y / viewport.x;
}

void _gapi_impl::Camera3D::SetAspect(const core::Window& win)
{
    aspect = win.GetAspect();
}

void _gapi_impl::Camera3D::Update(float dt, Mode mode)
{
    const math::Vec2 mouseDelta = input::GetMouseDelta();

    bool moveInWorldPlane = (mode == Mode::FirstPerson || mode == Mode::ThirdPerson);
    bool rotateAroundTarget = (mode == Mode::ThirdPerson || mode == Mode::Orbital);
    bool lockView = (mode == Mode::Free || mode == Mode::FirstPerson || mode == Mode::ThirdPerson || mode == Mode::Orbital);
    bool rotateUp = false;

    if (mode == Mode::Orbital)
    {
        // Orbital can just orbit
        math::Mat4 rotation = math::Mat4::Rotate(up, orbitalSpeed*dt);
        math::Vec3 view = (position - target).Transformed(rotation);
        position = target + view;
    }
    else
    {
        // Camera rotation
        if (input::IsKeyPressed(input::Scancode::Down))     Pitch(-rotationSpeed, lockView, rotateAroundTarget, rotateUp);
        if (input::IsKeyPressed(input::Scancode::Up))       Pitch(rotationSpeed, lockView, rotateAroundTarget, rotateUp);
        if (input::IsKeyPressed(input::Scancode::Right))    Yaw(-rotationSpeed, rotateAroundTarget);
        if (input::IsKeyPressed(input::Scancode::Left))     Yaw(rotationSpeed, rotateAroundTarget);
        if (input::IsKeyPressed(input::Scancode::Q))        Roll(-rotationSpeed);
        if (input::IsKeyPressed(input::Scancode::E))        Roll(rotationSpeed);

        // Camera movement
        // Camera pan (for CAMERA_FREE)
        if (mode == Mode::Free && input::IsMouseButtonPressed(input::MouseButton::Middle))
        {
            if (mouseDelta.x > 0.0f) MoveRight(panSpeed, moveInWorldPlane);
            if (mouseDelta.x < 0.0f) MoveRight(-panSpeed, moveInWorldPlane);
            if (mouseDelta.y > 0.0f) MoveUp(-panSpeed);
            if (mouseDelta.y < 0.0f) MoveUp(panSpeed);
        }
        else
        {
            // Mouse support
            Yaw(-mouseDelta.x * mouseMoveSensitivity, rotateAroundTarget);
            Pitch(-mouseDelta.y * mouseMoveSensitivity, lockView, rotateAroundTarget, rotateUp);
        }

        // Keyboard support
        if (input::IsKeyPressed(input::Scancode::W)) MoveForward(movementSpeed, moveInWorldPlane);
        if (input::IsKeyPressed(input::Scancode::A)) MoveRight(-movementSpeed, moveInWorldPlane);
        if (input::IsKeyPressed(input::Scancode::S)) MoveForward(-movementSpeed, moveInWorldPlane);
        if (input::IsKeyPressed(input::Scancode::D)) MoveRight(movementSpeed, moveInWorldPlane);

        // Gamepad movement
        if (input::IsGamepadConnected(0))
        {
            Yaw(-(input::GetGamepadAxisMovement(0, input::GamepadAxis::RightX) * 2) * mouseMoveSensitivity, rotateAroundTarget);
            Pitch(-(input::GetGamepadAxisMovement(0, input::GamepadAxis::RightY) * 2) * mouseMoveSensitivity, lockView, rotateAroundTarget, rotateUp);
            MoveForward(-input::GetGamepadAxisMovement(0, input::GamepadAxis::LeftY) * movementSpeed, moveInWorldPlane);
            MoveRight(input::GetGamepadAxisMovement(0, input::GamepadAxis::LeftX) * movementSpeed, moveInWorldPlane);
        }

        if (mode == Mode::Free)
        {
            if (input::IsKeyPressed(input::Scancode::Space)) MoveUp(movementSpeed);
            if (input::IsKeyPressed(input::Scancode::LShift)) MoveUp(-movementSpeed);
        }
    }
}

void _gapi_impl::Camera3D::Update(const math::Vec3& movement, const math::Vec3& rotation, float zoom)
{
    Rotate(rotation);
    Translate(movement);
    if (zoom) MoveToTarget(zoom);
}

void _gapi_impl::Camera3D::PrintInfo()
{
    NEXUS_LOG(Debug) << "*~~~~~~~~~~~~~~ CAMERA ~~~~~~~~~~~~~~*" << "\n";
    NEXUS_LOG(Debug) << "Position: X=" << position.x << " Y=" << position.y << " Z=" << position.z << "\n";
    NEXUS_LOG(Debug) << "Target: X=" << target.x << " Y=" << target.y << " Z=" << target.z << "\n";
    NEXUS_LOG(Debug) << "Up: X=" << up.x << " Y=" << up.y << " Z=" << up.z << "\n";
    NEXUS_LOG(Debug) << "Aspect: " << aspect << "\n";
    NEXUS_LOG(Debug) << "FOV: " << fovy << "\n";
    NEXUS_LOG(Debug) << "Near: " << near << "\n";
    NEXUS_LOG(Debug) << "Far: " << far << "\n";
    NEXUS_LOG(Debug) << "orthographic: " << (orthographic ? "True" : "False") << "\n";
    NEXUS_LOG(Debug) <<  "*~~~~~~~~~~~~~~ END ~~~~~~~~~~~~~~~~~*" << "\n";
}
