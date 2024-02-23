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

#ifndef NEXUS_PHYSICS_3D_RIGID_OBJECT_HPP
#define NEXUS_PHYSICS_3D_RIGID_OBJECT_HPP

#include "../../../gapi/cmn_impl/sp_model/nxModel.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxVec4.hpp"

#include <LinearMath/btTransform.h>
#include <btBulletDynamicsCommon.h>
#include <type_traits>
#include <optional>
#include <cstring>

namespace nexus { namespace phys3D {

    /**
     * @brief Enum class representing the shape of a 3D physics object.
     */
    enum class Shape
    {
        None,     ///< No specific shape
        Mesh,     ///< Mesh shape (can only be static, use Capsule or Hull for a dynamic body with a mesh)
        Hull,     ///< Hull shape
        Cube,     ///< Cube shape
        Sphere,   ///< Sphere shape
        Capsule   ///< Capsule shape
    };

    /**
     * @brief Class representing a 3D physics rigid object, comprising a body and a collider shape.
     */
    struct NEXUS_API RigidObject
    {
      public:
        static constexpr float Static = 0.0f;       ///< Constant representing a static object.

      public:
        btCollisionShape        *colliderShape;     ///< Bullet's collision shape for the object.
        btRigidBody             *body;              ///< Bullet's rigid body for the physics simulation.
        const Shape             shape;              ///< Shape of the object.

      protected:
        /**
         * @brief Creates a Bullet rigid body for the object.
         * @param position Initial position of the object.
         * @param rotation Initial rotation of the object.
         * @param mass Mass of the object (0.0f for static objects).
         * @return Pointer to the created rigid body.
         */
        btRigidBody* CreateBody(const math::Vec3& position,
                                const math::Quaternion& rotation,
                                float mass);

        /**
         * @brief Constructor used internally by other object types, only initializes the shape.
         * @param shape Shape of the object.
         */
        RigidObject(Shape shape = Shape::None);

      public:
        /**
         * @brief Destructor for the Object class.
         */
        ~RigidObject();

        /**
         * @brief Checks if the object is ready.
         * @return True if the object has a valid body and motion state.
         */
        bool IsReady() const;

        /**
         * @brief Gets the transform matrix of the object.
         * @return The transformation matrix.
         */
        math::Mat4 GetTransform() const;

        /**
         * @brief Gets the position vector of the object.
         * @return The position vector.
         */
        math::Vec3 GetPosition() const;

        /**
         * @brief Gets the quaternion of the object's rotation.
         * @return The rotation quaternion.
         */
        math::Quaternion GetRotation() const;

        /**
         * @brief Gets the rotation axis vector of the object.
         * @return The rotation axis vector.
         */
        math::Vec3 GetRotationAxis() const;

        /**
         * @brief Gets the rotation angle of the object.
         * @return The rotation angle.
         */
        float GetAngle() const;

        /**
         * @brief Gets the linear velocity of the object.
         * @return The linear velocity vector.
         */
        math::Vec3 GetLinearVelocity() const;

        /**
         * @brief Gets the angular velocity of the object.
         * @return The angular velocity vector.
         */
        math::Vec3 GetAngularVelocity() const;

        /**
         * @brief Gets the friction coefficient of the object.
         * @return The friction coefficient.
         */
        float GetFriction() const;

        /**
         * @brief Gets the restitution coefficient of the object.
         * @return The restitution coefficient.
         */
        float GetRestitution() const;

        /**
         * @brief Gets the linear damping coefficient of the object.
         * @return The linear damping coefficient.
         */
        float GetLinearDamping() const;

        /**
         * @brief Gets the angular damping coefficient of the object.
         * @return The angular damping coefficient.
         */
        float GetAngularDamping() const;

        /**
         * @brief Gets the linear factor of the object.
         * @return The linear factor vector.
         */
        math::Vec3 GetLinearFactor() const;

        /**
         * @brief Gets the angular factor of the object.
         * @return The angular factor vector.
         */
        math::Vec3 GetAngularFactor() const;

        /**
         * @brief Sets the transformation of the object.
         * @param transform The transformation matrix.
         */
        void SetTransform(const math::Mat4& transform);

        /**
         * @brief Set the position of the object.
         * @param position The new position vector.
         */
        void SetPosition(const math::Vec3& position);

        /**
         * @brief Set the rotation of the object using a quaternion.
         * @param quat The new rotation quaternion.
         */
        void SetRotation(const math::Quaternion& quat);

        /**
         * @brief Set the rotation of the object using an axis and an angle in degrees.
         * @param axis The rotation axis vector.
         * @param angle The rotation angle in degrees.
         */
        void SetRotation(const math::Vec3& axis, float angle);

        /**
         * @brief Set the linear velocity of the object.
         * @param linearVelocity The new velocity vector.
         * 
         * This method sets the linear velocity of the object to the specified value.
         */
        void SetLinearVelocity(const math::Vec3& linearVelocity);

        /**
         * @brief Set the angular velocity of the object.
         * @param angularVelocity The new angular velocity vector.
         * 
         * This method sets the angular velocity of the object to the specified value.
         */
        void SetAngularVelocity(const math::Vec3& angularVelocity);

        /**
         * @brief Set the friction coefficient of the object.
         * @param friction The new friction coefficient.
         * 
         * This method sets the friction coefficient of the object to the specified value.
         * Friction coefficient determines the amount of resistance the object encounters when moving against another object.
         */
        void SetFriction(float friction);

        /**
         * @brief Set the restitution coefficient of the object.
         * @param restitution The new restitution coefficient.
         * 
         * This method sets the restitution coefficient of the object to the specified value.
         * Restitution coefficient determines how much kinetic energy is retained after a collision.
         */
        void SetRestitution(float restitution);

        /**
         * @brief Set the linear and angular damping coefficients of the object.
         * @param linearDamping The new linear damping coefficient.
         * @param angularDamping The new angular damping coefficient.
         * 
         * This method sets both the linear and angular damping coefficients of the object to the specified values.
         * Damping coefficients determine how quickly the object's linear and angular velocities decrease over time.
         */
        void SetDamping(float linearDamping, float angularDamping);

        /**
         * @brief Set the linear factor of the object.
         * @param linearFactor The new linear factor vector.
         * 
         * This method sets the linear factor of the object, which determines how much each component of linear velocity is affected by external forces.
         */
        void SetLinearFactor(const math::Vec3& linearFactor);

        /**
         * @brief Set the angular factor of the object.
         * @param angularFactor The new angular factor vector.
         * 
         * This method sets the angular factor of the object, which determines how much each component of angular velocity is affected by external torques.
         */
        void SetAngularFactor(const math::Vec3& angularFactor);

        /**
         * @brief Apply a force to the object at a specific position.
         * @param force The force vector to apply.
         * @param relativePos The position vector relative to the object's center of mass.
         * 
         * This method applies a force to the object at a specific position relative to its center of mass.
         */
        void ApplyForce(const math::Vec3& force, const math::Vec3& relativePos);

        /**
         * @brief Apply an impulse to the object at a specific position.
         * @param impulse The impulse vector to apply.
         * @param relativePos The position vector relative to the object's center of mass.
         * 
         * This method applies an impulse to the object at a specific position relative to its center of mass.
         * An impulse is a large, instantaneous change in momentum applied to the object.
         */
        void ApplyImpulse(const math::Vec3& impulse, const math::Vec3& relativePos);

        /**
         * @brief Apply a push impulse to the object at a specific position.
         * @param impulse The impulse vector to apply.
         * @param relativePos The position vector relative to the object's center of mass.
         * 
         * This method applies a push impulse to the object at a specific position relative to its center of mass.
         * A push impulse is a large, instantaneous force applied to an object to quickly change its velocity.
         */
        void ApplyPushImpulse(const math::Vec3& impulse, const math::Vec3& relativePos);

        /**
         * @brief Apply a central force to the object.
         * @param force The central force vector to apply.
         * 
         * This method applies a central force to the object, which is a force applied at its center of mass in a specific direction.
         */
        void ApplyCentralForce(const math::Vec3& force);

        /**
         * @brief Apply a central impulse to the object.
         * @param impulse The central impulse vector to apply.
         * 
         * This method applies a central impulse to the object, which is an instantaneous change in momentum applied at its center of mass in a specific direction.
         */
        void ApplyCentralImpulse(const math::Vec3& impulse);

        /**
         * @brief Apply a central push impulse to the object.
         * @param impulse The impulse vector to apply.
         * 
         * This method applies a central push impulse to the object, which is a large, instantaneous force applied at its center of mass to quickly change its velocity.
         */
        void ApplyCentralPushImpulse(const math::Vec3& impulse);

        /**
         * @brief Apply a torque to the object.
         * @param torque The torque vector to apply.
         * 
         * This method applies a torque to the object, causing it to rotate around its center of mass.
         */
        void ApplyTorque(const math::Vec3& torque);

        /**
         * @brief Apply an impulse of torque to the object.
         * @param torqueImpulse The torque impulse vector to apply.
         * 
         * This method applies an impulse of torque to the object, causing an instantaneous change in its rotational motion.
         */
        void ApplyTorqueImpulse(const math::Vec3& torqueImpulse);

        /**
         * @brief Apply a torque-turn impulse to the object.
         * @param torqueImpulse The torque impulse vector to apply.
         * 
         * This method applies a torque-turn impulse to the object, which is a momentary force applied perpendicular to the object's rotational axis to change its angular velocity.
         */
        void ApplyTorqueTurnImpulse(const math::Vec3& torqueImpulse);

        /**
         * @brief Draw a model according to the configuration of the physical body.
         * @param color Color used for rendering the model.
         */
        template <typename T_Model>
        void DrawModel(T_Model& model, const gfx::Color& color = gfx::White) const;

        /**
         * @brief Draw model wires according to the configuration of the physical body.
         * @param color Color used for rendering the model.
         */
        template <typename T_Model>
        void DrawModelWires(T_Model& model, const gfx::Color& color = gfx::White) const;
    };


    /* Public Implementation Object */

    template <typename T_Model>
    void RigidObject::DrawModel(T_Model& model, const gfx::Color& color) const
    {
        static_assert(utils::IsContainer<T_Model>::value,
            "[RigidObject] <T_Model> must be derived from a Container.");

        static_assert(_gapi_impl::IsModel<typename T_Model::Type>::value,
            "[RigidObject] <T_Model> must be a contextual container containing a Model.");

        if (!IsReady()) return;

        // Get the transform matrix of the body
        btTransform btTrans;
        body->getMotionState()->getWorldTransform(btTrans);

        // Get rotation from transform matrix
        btQuaternion quat = btTrans.getRotation();

        // Render model with position and rotation
        model->Draw(btTrans.getOrigin(), quat.getAxis(), quat.getAngle() * math::Rad2Deg, { 1, 1, 1 }, color);
    }

    template <typename T_Model>
    void RigidObject::DrawModelWires(T_Model& model, const gfx::Color& color) const
    {
        static_assert(utils::IsContainer<T_Model>::value,
            "[RigidObject] <T_Model> must be derived from a Container.");

        static_assert(_gapi_impl::IsModel<typename T_Model::Type>::value,
            "[RigidObject] <T_Model> must be a contextual container containing a Model.");

        model->GetContext().EnableWireMode();

            this->DrawModel(model, color);

        model->GetContext().DisableWireMode();
    }

}}

#endif //NEXUS_PHYSICS_3D_RIGID_OBJECT_HPP