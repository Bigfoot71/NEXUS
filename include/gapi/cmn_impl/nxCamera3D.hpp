#ifndef NEXUS_GAPI_IMPL_CAMERA_3D_HPP
#define NEXUS_GAPI_IMPL_CAMERA_3D_HPP

#include "../../core/nxWindow.hpp"
#include "../../math/nxVec2.hpp"
#include "../../math/nxVec3.hpp"

namespace _gapi_impl {

    struct Camera3D
    {
        /**
         * @brief Enumeration of camera modes.
         */
        enum Mode
        {
            Free,                               ///< Freely movable camera.
            Orbital,                            ///< Orbiting camera mode.
            FirstPerson,                        ///< First-person view.
            ThirdPerson                         ///< Third-person view.
        };

        nexus::math::Vec3 position;                ///< Camera position.
        nexus::math::Vec3 target;                  ///< Camera target.
        nexus::math::Vec3 up;                      ///< Camera up vector.
        float aspect;                           ///< Aspect ratio of the camera view.
        float fovy;                             ///< Field of view angle.
        float near;                             ///< Near clipping plane distance.
        float far;                              ///< Far clipping plane distance.

        float movementSpeed = 0.09f;            ///< Movement speed of the camera.
        float rotationSpeed = 0.03f;            ///< Rotation speed of the camera.
        float panSpeed = 0.2f;                  ///< Pan speed of the camera.
        float orbitalSpeed = 0.5f;              ///< Orbital speed of the camera (radians per second).
        float mouseMoveSensitivity = 0.003f;    ///< Mouse movement sensitivity of the camera.
        float mouseWheelSensitivity = 1.5f;     ///< Mouse wheel sensitivity of the camera.

        bool orthographic;                      ///< Flag indicating if the camera is in orthographic mode.

        /**
         * @brief Default constructor.
         */
        Camera3D() = default;

        /**
         * @brief Constructor with parameters.
         * @param position Camera position.
         * @param target Camera target.
         * @param up Camera up vector.
         * @param fovy Field of view angle.
         * @param aspect Aspect ratio of the camera view.
         * @param near Near clipping plane distance.
         * @param far Far clipping plane distance.
         * @param orthographic Flag indicating if the camera is in orthographic mode.
         */
        Camera3D(const nexus::math::Vec3& position,
               const nexus::math::Vec3& target,
               const nexus::math::Vec3& up = { 0, 1, 0 },
               float fovy = 60.0f,
               float aspect = 1.0f,
               float near = 0.01f,
               float far = 1000.0f,
               bool orthographic = false);

        /**
         * @brief Returns the right vector of the camera.
         * @return The right vector.
         */
        nexus::math::Vec3 GetRight() const { return GetForward().Cross(GetUp()); }

        /**
         * @brief Returns the forward vector of the camera.
         * @return The forward vector.
         */
        nexus::math::Vec3 GetForward() const { return (target - position).Normalized(); }

        /**
         * @brief Returns the up vector of the camera.
         * @return The up vector.
         */
        nexus::math::Vec3 GetUp() const { return up.Normalized(); }

        /**
         * @brief Moves the camera forward.
         * @param distance The distance to move.
         * @param moveInWorldPlane Flag indicating whether to move in the world plane or camera plane.
         */
        void MoveForward(float distance, bool moveInWorldPlane = true);

        /**
         * @brief Moves the camera up.
         * @param distance The distance to move.
         */
        void MoveUp(float distance);

        /**
         * @brief Moves the camera right.
         * @param distance The distance to move.
         * @param moveInWorldPlane Flag indicating whether to move in the world plane or camera plane.
         */
        void MoveRight(float distance, bool moveInWorldPlane = true);

        /**
         * @brief Moves the camera towards its target.
         * @param delta The amount to move towards the target.
         */
        void MoveToTarget(float delta);

        /**
         * @brief Rotates the camera around the vertical axis (Yaw).
         * @param angle The angle to rotate (in radians).
         * @param rotateAroundTarget Flag indicating whether to rotate around the target point.
         */
        void Yaw(float angle, bool rotateAroundTarget = false);

        /**
         * @brief Rotates the camera around the horizontal axis (Pitch).
         * @param angle The angle to rotate (in radians).
         * @param lockView Flag indicating whether to lock the view.
         * @param rotateAroundTarget Flag indicating whether to rotate around the target point.
         * @param rotateUp Flag indicating whether to rotate the up vector.
         */
        void Pitch(float angle, bool lockView = true, bool rotateAroundTarget = false, bool rotateUp = false);

        /**
         * @brief Rotates the camera around its forward axis (Roll).
         * @param angle The angle to rotate (in radians).
         */
        void Roll(float angle);

        /**
         * @brief Returns the direction vector of the camera.
         * @return The direction vector.
         */
        nexus::math::Vec3 GetDirection() const;

        /**
         * @brief Returns the view matrix of the camera.
         * @return The view matrix.
         */
        nexus::math::Mat4 GetViewMatrix() const;

        /**
         * @brief Returns the projection matrix of the camera.
         * @return The projection matrix.
         */
        nexus::math::Mat4 GetProjectionMatrix() const;

        /**
         * @brief Translates the camera by the specified delta vector.
         * @param delta The translation delta vector.
         */
        void Translate(const nexus::math::Vec3& delta);

        /**
         * @brief Rotates the camera by the specified delta vector.
         * @param delta The rotation delta vector.
         * @param lockView Flag indicating whether to lock the view.
         */
        void Rotate(const nexus::math::Vec3& delta, bool lockView = true);

        /**
         * @brief Sets the aspect ratio of the camera.
         * @param aspect The aspect ratio to set.
         */
        void SetAspect(float aspect);

        /**
         * @brief Sets the aspect ratio of the camera based on a 2D vector.
         * @param viewport The viewport dimensions as a 2D vector.
         */
        void SetAspect(const nexus::math::Vec2& viewport);

        /**
         * @brief Sets the aspect ratio of the camera based on a window.
         * @param win The window to use for setting the aspect ratio.
         */
        void SetAspect(const nexus::core::Window& win);

        /**
         * @brief Updates the camera state.
         *
         * @warning: This function uses `input::GetMouseDelta`, which can be problematic
         *          if you also call it elsewhere in your code because the function returns
         *          the delta only since its last call. This simplified Update function is
         *          more suitable for simple programs. Alternatively, you can use other
         *          Update functions in Camera3D that do not themselves call the
         *          `input::GetMouseDelta` function.
         *
         * @param dt The time delta since the last update.
         * @param mode The camera mode (default is Free).
         */
        void Update(float dt, Mode mode = Mode::Free);

        /**
         * @brief Updates the camera state with specified movement, rotation, and zoom values.
         * @param movement The movement vector.
         * @param rotation The rotation vector.
         * @param zoom The zoom value (default is 0).
         */
        void Update(const nexus::math::Vec3& movement, const nexus::math::Vec3& rotation, float zoom = 0);

        /**
         * @brief Prints information about the camera.
         */
        void PrintInfo();
    };

}

#endif //NEXUS_GAPI_IMPL_CAMERA_3D_HPP
