#include "Entity.h"
#include "../Core/Application.h"
#include "../Core/Time.h"
#include "../Math/Random.h"

Entity::Entity()
{
    this->ID = Random::NextUint();
    this->Transform = AddComponent<Transformation>();
}

Entity::~Entity()
{
    for (auto component : components)
    {
        delete component.second;
    }
}

void Entity::Destroy()
{
    if (isDestroyed)
        return;

    isDestroyed = true;
    destroyDelayed = false;
    Application::Instance->GetCurrentScene()->DestroyEntity(this);
}

void Entity::Destroy(float time)
{
    if (isDestroyed)
        return;

    destroyDelayed = true;
    destroyTimer = time;
}

bool Entity::IsDestroyed()
{
    return isDestroyed;
}

void Entity::OnUpdate()
{

}

void Entity::OnFixedUpdate()
{

}

void Entity::OnLateUpdate()
{
    if (destroyDelayed)
    {
        destroyTimer -= Time::DeltaTime();
        if (destroyTimer <= 0)
        {
            Destroy();
        }
    }
}
