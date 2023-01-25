#include "Steel/Core/Log.h"
#include "Steel/Core/Application.h"
#include "Steel/Input/Input.h"
#include "Steel/Rendering/Camera.h"
#include "Steel/Scene/Transformation.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Physics.h"
#include "PhysicsCore.h"

EntityID Physics::lastMouseOverCollider = NULL_ENTITY;

void Physics::CreatePhysicsScene(EntitiesRegistry* entitiesRegistry)
{
    PhysicsCore::CreateWorld();

    Log::LogDebug("Physics scene created");
}

void Physics::DeletePhysicsScene()
{
    PhysicsCore::DeleteWorld();
}

void Physics::UpdatePhysicsTransformations()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto rigidBodies = registry->GetComponentIterator<RigidBody>();
    int rigidBodiesSize = rigidBodies.Size();
    for (int i = 0; i < rigidBodiesSize; ++i)
    {
        if (rigidBodies[i].IsAlive())
            rigidBodies[i].UpdatePhysicsTransformation();
    }

    // TODO: use Collider type after components relations will be implemented
    auto boxColliders = registry->GetComponentIterator<BoxCollider>();
    int boxCollidersSize = boxColliders.Size();
    for (int i = 0; i < boxCollidersSize; ++i)
    {
        if (boxColliders[i].IsAlive())
            boxColliders[i].SetSizeAutomatically();
    }

    auto circleColliders = registry->GetComponentIterator<CircleCollider>();
    int circleCollidersSize = circleColliders.Size();
    for (int i = 0; i < circleCollidersSize; ++i)
    {
        if (circleColliders[i].IsAlive())
            circleColliders[i].SetSizeAutomatically();
    }
}

void Physics::Simulate(float deltaTime)
{
    PhysicsCore::Simulate(deltaTime);
}

void Physics::GetPhysicsTransformations()
{
    auto rigidBodies = Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->GetComponentIterator<RigidBody>();
    int rigidBodiesSize = rigidBodies.Size();
    for (int i = 0; i < rigidBodiesSize; ++i)
        if (rigidBodies[i].IsAlive())
            rigidBodies[i].GetPhysicsTransformation();
}

void Physics::SendEvents()
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto rigidBodies = entitiesRegistry->GetComponentIterator<RigidBody>();
    auto scriptsAccessor = entitiesRegistry->GetComponentAccessor<ScriptComponent>();
    int rigidBodiesSize = rigidBodies.Size();
    for (int i = 0; i < rigidBodiesSize; ++i)
    {
        if (rigidBodies[i].IsAlive() && scriptsAccessor.Has(rigidBodies[i].Owner))
        {
            for (auto contact : rigidBodies[i].ActiveContacts)
            {
                scriptsAccessor.Get(rigidBodies[i].Owner).OnCollisionStay(contact.second);
            }
        }
    }

    EntityID cameraID = Application::Instance->GetCurrentScene()->GetMainCamera();
    if (cameraID == NULL_ENTITY)
    {
        lastMouseOverCollider = NULL_ENTITY;
        return;
    }

    auto& camera = entitiesRegistry->GetComponent<Camera>(cameraID);
    glm::vec2 worldMP = camera.ScreenToWorldPoint(Input::GetMousePosition());
    auto hits = PointCast(worldMP);
    if (hits.empty())
    {
        // Nothing mouse is over
        if (lastMouseOverCollider != NULL_ENTITY)
        {
            // We had stored last object mouse was over, send mouse exit event
            if (entitiesRegistry->HasComponent<ScriptComponent>(lastMouseOverCollider))
                entitiesRegistry->GetComponent<ScriptComponent>(lastMouseOverCollider).OnMouseExit();

            lastMouseOverCollider = NULL_ENTITY;
        }

        return;
    }

    // Pick the nearest by z object
    float z, maxZ = entitiesRegistry->GetComponent<Transformation>(hits[0]).GetPosition().z;
    int closestEntityID = 0;
    for (size_t i = 0; i < hits.size(); i++)
    {
        z = entitiesRegistry->GetComponent<Transformation>(hits[i]).GetPosition().z;
        if (z > maxZ)
        {
            maxZ = z;
            closestEntityID = i;
        }
    }

    // Mouse is over some object
    if (lastMouseOverCollider != NULL_ENTITY)
    {
        // We had stored last object mouse was over
        if (lastMouseOverCollider != hits[closestEntityID])
        {
            // Last object differs from new one, send exit to last, and enter to new
            if (entitiesRegistry->HasComponent<ScriptComponent>(lastMouseOverCollider))
                entitiesRegistry->GetComponent<ScriptComponent>(lastMouseOverCollider).OnMouseExit();

            lastMouseOverCollider = hits[closestEntityID];

            if (entitiesRegistry->HasComponent<ScriptComponent>(lastMouseOverCollider))
                entitiesRegistry->GetComponent<ScriptComponent>(lastMouseOverCollider).OnMouseEnter();
        }
        else
        {
            // We are still over the same object as last frame, send him over event
            if (entitiesRegistry->HasComponent<ScriptComponent>(lastMouseOverCollider))
                entitiesRegistry->GetComponent<ScriptComponent>(lastMouseOverCollider).OnMouseOver();
        }
    }
    else
    {
        // Nothing was stored, send enter to new one
        lastMouseOverCollider = hits[closestEntityID];

        if (entitiesRegistry->HasComponent<ScriptComponent>(lastMouseOverCollider))
            entitiesRegistry->GetComponent<ScriptComponent>(lastMouseOverCollider).OnMouseEnter();
    }

    if (entitiesRegistry->HasComponent<ScriptComponent>(lastMouseOverCollider))
    {
        auto& sr = entitiesRegistry->GetComponent<ScriptComponent>(lastMouseOverCollider);

        if (Input::IsAnyMouseButtonJustPressed())
            sr.OnMouseJustPressed();
        if (Input::IsAnyMouseButtonPressed())
            sr.OnMousePressed();
        if (Input::IsAnyMouseButtonJustReleased())
            sr.OnMouseJustReleased();
    }
}

std::vector<EntityID> Physics::PointCast(glm::vec2 center)
{
    return PhysicsCore::PointCast(center);
}

std::vector<EntityID> Physics::AABBCast(glm::vec2 center, glm::vec2 size)
{
    return PhysicsCore::AABBCast(center, size);
}

std::vector<RayCastHit> Physics::LineCast(glm::vec2 pointA, glm::vec2 pointB)
{
    return PhysicsCore::LineCast(pointA, pointB);
}
