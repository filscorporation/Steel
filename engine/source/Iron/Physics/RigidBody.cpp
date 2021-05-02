#include <box2d/box2d.h>

#include "RigidBody.h"
#include "PhysicsCore.h"
#include "BoxCollider.h"
#include "PhysicsInfo.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"

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
        b2FixtureDef fixtureDef;
        fixtureDef.shape = bc.info->BoxShape;
        fixtureDef.density = 1;
        fixtureDef.friction = _friction;
        fixtureDef.restitution = _restitution;
        info->Body->CreateFixture(&fixtureDef);
    }
    if (HasComponentS<CircleCollider>(Owner))
    {
        auto& cc = GetComponentS<CircleCollider>(Owner);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = cc.info->CircleShape;
        fixtureDef.density = 1;
        fixtureDef.friction = _friction;
        fixtureDef.restitution = _restitution;
        info->Body->CreateFixture(&fixtureDef);
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
    if (_type != RigidBodyTypes::None)
        SetMassInner();
}

void RigidBody::SetMassInner()
{
    b2MassData massData;
    massData.mass = _mass;
    info->Body->SetMassData(&massData);
    info->Body->ResetMassData();
    info->Body->SetAwake(true);
}

glm::vec2 RigidBody::GetVelocity() const
{
    auto& v = info->Body->GetLinearVelocity();
    return glm::vec2(v.x, v.y);
}

void RigidBody::SetVelocity(glm::vec2 velocity)
{
    auto v = b2Vec2(velocity.x, velocity.y);
    info->Body->SetLinearVelocity(v);
    info->Body->SetAwake(true);
}

float RigidBody::GetAngularVelocity() const
{
    return info->Body->GetAngularVelocity();
}

void RigidBody::SetAngularVelocity(float velocity)
{
    info->Body->SetAngularVelocity(velocity);
    info->Body->SetAwake(true);
}

float RigidBody::GetGravityScale() const
{
    return info->Body->GetGravityScale();
}

void RigidBody::SetGravityScale(float gravityScale)
{
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

    if (_type == RigidBodyTypes::None)
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

    if (_type == RigidBodyTypes::None)
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
    return info->Body->GetLinearDamping();
}

void RigidBody::SetLinearDamping(float damping)
{
    info->Body->SetLinearDamping(damping);
    info->Body->SetAwake(true);
}

float RigidBody::GetAngularDamping() const
{
    return info->Body->GetAngularDamping();
}

void RigidBody::SetAngularDamping(float damping)
{
    info->Body->SetAngularDamping(damping);
    info->Body->SetAwake(true);
}

float RigidBody::GetIsFixedRotation() const
{
    return _isFixedRotation;
}

void RigidBody::SetIsFixedRotation(bool isFixed)
{
    _isFixedRotation = isFixed;
    info->Body->SetFixedRotation(_isFixedRotation);
    info->Body->SetAwake(true);
}

float RigidBody::GetUseContinuousCollisionDetection() const
{
    return info->Body->IsBullet();
}

void RigidBody::SetUseContinuousCollisionDetection(bool useCCD)
{
    info->Body->SetBullet(useCCD);
    info->Body->SetAwake(true);
}

void RigidBody::ApplyForce(const glm::vec2& force)
{
    info->Body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void RigidBody::ApplyForce(const glm::vec2& force, const glm::vec2& point)
{
    info->Body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

void RigidBody::ApplyTorque(float torque)
{
    info->Body->ApplyTorque(torque, true);
}

void RigidBody::ApplyLinearImpulse(const glm::vec2& impulse)
{
    info->Body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
}

void RigidBody::ApplyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point)
{
    info->Body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
}

void RigidBody::ApplyAngularImpulse(float impulse)
{
    info->Body->ApplyAngularImpulse(impulse, true);
}

void RigidBody::GetPhysicsTransformation()
{
    if (_type == RigidBodyTypes::Static)
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
    if (inGetTransformation)
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
