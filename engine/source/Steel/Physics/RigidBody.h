#pragma once

#include "Steel/EntityComponentSystem/Component.h"
#include "Collision.h"

#include <unordered_map>
#include <glm/vec2.hpp>

namespace RigidBodyTypes
{
    enum RigidBodyType
    {
        None,
        Dynamic,
        Static,
        Kinematic,
    };
}

class RigidBody : public Component
{
    DEFINE_COMPONENT(RigidBody)

    struct RigidBodyInfo;

public:
    explicit RigidBody(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    RigidBodyTypes::RigidBodyType GetType() const;
    void SetType(RigidBodyTypes::RigidBodyType type);

    float GetMass() const;
    void SetMass(float mass);
    void SetMassInner();
    glm::vec2 GetVelocity() const;
    void SetVelocity(glm::vec2 velocity);
    float GetAngularVelocity() const;
    void SetAngularVelocity(float velocity);
    float GetGravityScale() const;
    void SetGravityScale(float gravityScale);
    float GetFriction() const;
    void SetFriction(float friction);
    float GetRestitution() const;
    void SetRestitution(float restitution);
    float GetLinearDamping() const;
    void SetLinearDamping(float damping);
    float GetAngularDamping() const;
    void SetAngularDamping(float damping);
    bool GetIsFixedRotation() const;
    void SetIsFixedRotation(bool isFixed);
    bool GetUseContinuousCollisionDetection() const;
    void SetUseContinuousCollisionDetection(bool useCCD);

    void ApplyForce(const glm::vec2& force);
    void ApplyForce(const glm::vec2& force, const glm::vec2& point);
    void ApplyTorque(float torque);
    void ApplyLinearImpulse(const glm::vec2& impulse);
    void ApplyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point);
    void ApplyAngularImpulse(float impulse);

    void GetPhysicsTransformation();
    void UpdatePhysicsTransformation();
    void OnColliderUpdated();

    std::unordered_map<EntityID, Collision> ActiveContacts;

private:
    void ApplyType();
    void ApplyPhysicsProperties();

    void SetDynamic();
    void SetStatic();
    void SetKinematic();

    void SetAutoFixture();
    bool AssertInitialized();

    RigidBodyInfo* info = nullptr;
    bool inGetTransformation = false;
    float _mass = 1.0f;
    float _gravityScale = 1.0f;
    float _friction = 0.2f;
    float _restitution = 0.0f;
    float _linearDamping = 0.0f;
    float _angularDamping = 0.01f;
    bool _isFixedRotation = false;
    bool _useCCD = false;
    RigidBodyTypes::RigidBodyType _type = RigidBodyTypes::None;
};
