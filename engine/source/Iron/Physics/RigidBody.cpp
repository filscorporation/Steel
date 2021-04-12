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
    _mass = info->Body->GetMass();
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
    _mass = info->Body->GetMass();
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
    _mass = info->Body->GetMass();
    _type = RigidBodyTypes::Kinematic;

    SetAutoFixture();
}

float RigidBody::GetMass() const
{
    return _mass;
}

void RigidBody::SetMass(float mass)
{
    _mass = mass;
    b2MassData massData;
    massData.mass = _mass;
    info->Body->SetMassData(&massData);
}

RigidBodyTypes::RigidBodyType RigidBody::GetType() const
{
    return _type;
}

void RigidBody::SetType(RigidBodyTypes::RigidBodyType type)
{
    if (_type != RigidBodyTypes::None && type != _type)
    {
        Log::LogWarning("Rigid body type cannot be changed, remove and add new component instead");
        return;
    }

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
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        info->Body->CreateFixture(&fixtureDef);
    }
    if (HasComponentS<CircleCollider>(Owner))
    {
        auto& cc = GetComponentS<CircleCollider>(Owner);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = cc.info->CircleShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        info->Body->CreateFixture(&fixtureDef);
    }
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
