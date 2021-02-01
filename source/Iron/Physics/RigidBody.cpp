#include <box2d/box2d.h>
#include "PhysicsCore.h"
#include "RigidBody.h"
#include "../Scene/Object.h"
#include "BoxCollider.h"
#include "PhysicsInfo.h"

RigidBody::RigidBody()
{
    info = new RigidBodyInfo();
}

RigidBody::~RigidBody()
{
    delete info;
}

void RigidBody::SetDynamic()
{
    b2BodyDef groundBodyDef;
    glm::vec3 position = ParentObject->Transform->GetPosition();
    groundBodyDef.type = b2_dynamicBody;
    groundBodyDef.position.Set(position.x, position.y);

    info->Body = PhysicsCore::GetWorld()->CreateBody(&groundBodyDef);

    SetAutoFixture();
}

void RigidBody::SetStatic()
{
    b2BodyDef groundBodyDef;
    glm::vec3 position = ParentObject->Transform->GetPosition();
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(position.x, position.y);

    info->Body = PhysicsCore::GetWorld()->CreateBody(&groundBodyDef);

    SetAutoFixture();
}

void RigidBody::SetKinematic()
{
    // TODO: not implemented
}

void RigidBody::SetAutoFixture()
{
    auto bc = ParentObject->GetComponent<BoxCollider>();
    if (bc != nullptr)
    {
        b2FixtureDef fixtureDef;
        fixtureDef.shape = bc->info->GroundBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        info->Body->CreateFixture(&fixtureDef);
    }
}

void RigidBody::SetPhysicsTransformation()
{

}

void RigidBody::GetPhysicsTransformation()
{
    // TODO: chick body type
    float z = ParentObject->Transform->GetPosition().z;
    auto position = info->Body->GetPosition();
    float rotation = info->Body->GetAngle();
    ParentObject->Transform->SetPosition(glm::vec3(position.x, position.y, z));
    ParentObject->Transform->SetRotation(glm::vec3(0, 0, rotation));
}
