#include "Object.h"
#include "../Core/Application.h"
#include "../Core/Time.h"

Object::Object()
{
    this->Transform = AddComponent<Transformation>();
}

Object::~Object()
{
    for (auto component : Components())
    {
        delete(component);
    }
    components.clear();
}

void Object::Destroy()
{
    if (isDestroyed)
        return;

    isDestroyed = true;
    destroyDelayed = false;
    Application::Instance->GetCurrentScene()->DestroyObject(this);
}

void Object::Destroy(float time)
{
    if (isDestroyed)
        return;

    destroyDelayed = true;
    destroyTimer = time;
}

bool Object::IsDestroyed()
{
    return isDestroyed;
}

void Object::OnUpdate()
{

}

void Object::OnLateUpdate()
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
