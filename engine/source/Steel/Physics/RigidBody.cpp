#include "RigidBody.h"
#include "PhysicsCore.h"
#include "BoxCollider.h"
#include "PhysicsInfo.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scene/Transformation.h"

#include <box2d/box2d.h>

void RigidBody::RegisterType()
{
    REGISTER_COMPONENT(RigidBody);
    REGISTER_ENUM_ATTRIBUTE(RigidBody, "type", GetType, SetType, RigidBodyTypes::RigidBodyType, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "mass", GetMass, SetMass, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "gravityScale", GetGravityScale, SetGravityScale, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "friction", GetFriction, SetFriction, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "restitution", GetRestitution, SetRestitution, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "linearDamping", GetLinearDamping, SetLinearDamping, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "angularDamping", GetAngularDamping, SetAngularDamping, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "isFixedRotation", GetIsFixedRotation, SetIsFixedRotation, bool, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(RigidBody, "useCCD", GetUseContinuousCollisionDetection, SetUseContinuousCollisionDetection, bool, AttributeFlags::Public);
}

void RigidBody::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (PhysicsCore::Initialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        info = new RigidBody::RigidBodyInfo();

        ApplyPhysicsProperties();
    }
}

void RigidBody::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (info == nullptr)
        return;

    if (info->Body != nullptr)
        PhysicsCore::GetWorld()->DestroyBody(info->Body);
    delete info;
    info = nullptr;
}

void RigidBody::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (PhysicsCore::Initialized())
    {
        info = new RigidBody::RigidBodyInfo();

        ApplyPhysicsProperties();
    }
}

void RigidBody::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    if (info == nullptr || !PhysicsCore::Initialized())
        return;

    if (info->Body != nullptr)
        PhysicsCore::GetWorld()->DestroyBody(info->Body);
    delete info;
    info = nullptr;
}

void RigidBody::ApplyPhysicsProperties()
{
    // Apply type
    auto typeBackup = _type;
    _type = RigidBodyTypes::None;
    SetType(typeBackup);

    // Apply all properties
    SetGravityScale(_gravityScale);
    SetFriction(_friction);
    SetRestitution(_restitution);
    SetLinearDamping(_linearDamping);
    SetAngularDamping(_angularDamping);
    SetIsFixedRotation(_isFixedRotation);
    SetUseContinuousCollisionDetection(_useCCD);
}

void RigidBody::SetDynamic()
{
    b2BodyDef groundBodyDef;
    glm::vec3 position = GetComponentS<Transformation>(Owner).GetPosition();
    groundBodyDef.type = b2_dynamicBody;
    groundBodyDef.position.Set(position.x, position.y);
    groundBodyDef.userData = b2BodyUserData();
    groundBodyDef.userData.pointer = (uintptr_t)Owner;

    info->Body = PhysicsCore::GetWorld()->CreateBody(&groundBodyDef);
    SetMassInner();
    _type = RigidBodyTypes::Dynamic;

    SetAutoFixture();
}

void RigidBody::SetStatic()
{
    b2BodyDef groundBodyDef;
    glm::vec3 position = GetComponentS<Transformation>(Owner).GetPosition();
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(position.x, position.y);
    groundBodyDef.userData = b2BodyUserData();
    groundBodyDef.userData.pointer = (uintptr_t)Owner;

    info->Body = PhysicsCore::GetWorld()->CreateBody(&groundBodyDef);
    SetMassInner();
    _type = RigidBodyTypes::Static;

    SetAutoFixture();
}

void RigidBody::SetKinematic()
{
    b2BodyDef groundBodyDef;
    glm::vec3 position = GetComponentS<Transformation>(Owner).GetPosition();
    groundBodyDef.type = b2_kinematicBody;
    groundBodyDef.position.Set(position.x, position.y);
    groundBodyDef.userData = b2BodyUserData();
    groundBodyDef.userData.pointer = (uintptr_t)Owner;

    info->Body = PhysicsCore::GetWorld()->CreateBody(&groundBodyDef);
    SetMassInner();
    _type = RigidBodyTypes::Kinematic;

    SetAutoFixture();
}

RigidBodyTypes::RigidBodyType RigidBody::GetType() const
{
    return _type;
}

void RigidBody::SetType(RigidBodyTypes::RigidBodyType type)
{
    if (!PhysicsCore::Initialized() || info == nullptr)
    {
        _type = type;
        return;
    }

    if (type != _type && AssertInitialized())
        return;

    _type = type;
    switch (type)
    {
        case RigidBodyTypes::Dynamic:
            SetDynamic();
            break;
        case RigidBodyTypes::Static:
            SetStatic();
            break;
        case RigidBodyTypes::Kinematic:
            SetKinematic();
            break;
        case RigidBodyTypes::None:
            break;
    }
}

void RigidBody::SetAutoFixture()
{
    if (HasComponentS<BoxCollider>(Owner))
    {
        auto& bc = GetComponentS<BoxCollider>(Owner);
        bc.PrepareColliderInfo();
        b2FixtureDef fixtureDef;
        fixtureDef.shape = bc.info->BoxShape;
        fixtureDef.density = 1;
        fixtureDef.friction = _friction;
        fixtureDef.restitution = _restitution;
        info->Fixture = info->Body->CreateFixture(&fixtureDef);
    }
    if (HasComponentS<CircleCollider>(Owner))
    {
        auto& cc = GetComponentS<CircleCollider>(Owner);
        cc.PrepareColliderInfo();
        b2FixtureDef fixtureDef;
        fixtureDef.shape = cc.info->CircleShape;
        fixtureDef.density = 1;
        fixtureDef.friction = _friction;
        fixtureDef.restitution = _restitution;
        info->Fixture = info->Body->CreateFixture(&fixtureDef);
    }

    SetMassInner();
}

float RigidBody::GetMass() const
{
    return _mass;
}

void RigidBody::SetMass(float mass)
{
    _mass = mass;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    SetMassInner();
}

void RigidBody::SetMassInner()
{
    b2MassData massData;
    if (info->Fixture == nullptr)
    {
        massData.I = 0.0f;
        massData.center = { 0.0, 0.0f };
    }
    else
    {
        info->Fixture->GetMassData(&massData);
    }
    massData.mass = _mass;
    info->Body->SetMassData(&massData);
    info->Body->ResetMassData();
    info->Body->SetAwake(true);
}

glm::vec2 RigidBody::GetVelocity() const
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return glm::vec2(0.0f);

    auto& v = info->Body->GetLinearVelocity();
    return glm::vec2(v.x, v.y);
}

void RigidBody::SetVelocity(glm::vec2 velocity)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    auto v = b2Vec2(velocity.x, velocity.y);
    info->Body->SetLinearVelocity(v);
    info->Body->SetAwake(true);
}

float RigidBody::GetAngularVelocity() const
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return 0.0f;

    return info->Body->GetAngularVelocity();
}

void RigidBody::SetAngularVelocity(float velocity)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->SetAngularVelocity(velocity);
    info->Body->SetAwake(true);
}

float RigidBody::GetGravityScale() const
{
    return _gravityScale;
}

void RigidBody::SetGravityScale(float gravityScale)
{
    _gravityScale = gravityScale;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->SetGravityScale(gravityScale);
    info->Body->SetAwake(true);
}

float RigidBody::GetFriction() const
{
    return _friction;
}

void RigidBody::SetFriction(float friction)
{
    _friction = friction;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    b2Fixture* fixture = info->Body->GetFixtureList();
    while (fixture != nullptr)
    {
        fixture->SetFriction(friction);
        fixture = fixture->GetNext();
    }
}

float RigidBody::GetRestitution() const
{
    return _restitution;
}

void RigidBody::SetRestitution(float restitution)
{
    _restitution = restitution;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    b2Fixture* fixture = info->Body->GetFixtureList();
    while (fixture != nullptr)
    {
        fixture->SetRestitution(restitution);
        fixture = fixture->GetNext();
    }
}

float RigidBody::GetLinearDamping() const
{
    return _linearDamping;
}

void RigidBody::SetLinearDamping(float damping)
{
    _linearDamping = damping;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->SetLinearDamping(damping);
    info->Body->SetAwake(true);
}

float RigidBody::GetAngularDamping() const
{
    return _angularDamping;
}

void RigidBody::SetAngularDamping(float damping)
{
    _angularDamping = damping;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->SetAngularDamping(damping);
    info->Body->SetAwake(true);
}

bool RigidBody::GetIsFixedRotation() const
{
    return _isFixedRotation;
}

void RigidBody::SetIsFixedRotation(bool isFixed)
{
    _isFixedRotation = isFixed;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->SetFixedRotation(_isFixedRotation);
    info->Body->SetAwake(true);
}

bool RigidBody::GetUseContinuousCollisionDetection() const
{
    return _useCCD;
}

void RigidBody::SetUseContinuousCollisionDetection(bool useCCD)
{
    _useCCD = useCCD;

    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->SetBullet(useCCD);
    info->Body->SetAwake(true);
}

void RigidBody::ApplyForce(const glm::vec2& force)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void RigidBody::ApplyForce(const glm::vec2& force, const glm::vec2& point)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

void RigidBody::ApplyTorque(float torque)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->ApplyTorque(torque, true);
}

void RigidBody::ApplyLinearImpulse(const glm::vec2& impulse)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
}

void RigidBody::ApplyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
}

void RigidBody::ApplyAngularImpulse(float impulse)
{
    if (!PhysicsCore::Initialized() || _type == RigidBodyTypes::None || info == nullptr)
        return;

    info->Body->ApplyAngularImpulse(impulse, true);
}

void RigidBody::GetPhysicsTransformation()
{
    if (_type == RigidBodyTypes::Static || _type == RigidBodyTypes::None || info == nullptr)
        return;

    inGetTransformation = true;
    float z = GetComponentS<Transformation>(Owner).GetPosition().z;
    auto position = info->Body->GetPosition();
    float rotation = info->Body->GetAngle();
    GetComponentS<Transformation>(Owner).SetPosition(glm::vec3(position.x, position.y, z));
    GetComponentS<Transformation>(Owner).SetRotation(glm::vec3(0, 0, rotation));
    inGetTransformation = false;
}

void RigidBody::UpdatePhysicsTransformation()
{
    if (inGetTransformation || info == nullptr)
        return;

    // TODO: maybe synchronise with global positions calculation at the end of frame
    auto position = GetComponentS<Transformation>(Owner).GetPosition();
    b2Vec2 b2Position;

    b2Position.x = position.x;
    b2Position.y = position.y;
    info->Body->SetTransform(b2Position, GetComponentS<Transformation>(Owner).GetRotation().z);
}

bool RigidBody::AssertInitialized()
{
    if (_type != RigidBodyTypes::None)
    {
        Log::LogWarning("Changing rigid body properties after it's type is initialized is not supported yet");
        // TODO: support
        return true;
    }

    return false;
}
