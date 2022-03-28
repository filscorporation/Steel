#include "Scene.h"
#include "Transformation.h"
#include "NameComponent.h"
#include "Hierarchy.h"
#include "Steel/Animation/Animator.h"
#include "Steel/Audio/AudioCore.h"
#include "Steel/Audio/AudioSource.h"
#include "Steel/Audio/AudioListener.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Time.h"
#include "Steel/Input/Input.h"
#include "Steel/Math/Random.h"
#include "Steel/Rendering/SpriteRenderer.h"
#include "Steel/Rendering/MeshRenderer.h"
#include "Steel/Rendering/Renderer.h"
#include "Steel/Physics/Physics.h"
#include "Steel/Scripting/ScriptingSystem.h"
#include "Steel/UI/RectTransformation.h"

Scene::Scene(const std::string& name)
{
    _name = name;
    entitiesRegistry = new EntitiesRegistry();
    TypeInfoStorage::InitializeTypesFromStorage(entitiesRegistry);

    uiLayer = new UILayer(this);
    uiLayer->LoadDefaultResources();
}

Scene::~Scene()
{
    delete uiLayer;
    delete entitiesRegistry;

    if (systemsInitialized)
    {
        AudioCore::DeleteAudioScene();
        Physics::DeletePhysicsScene();
    }
}

void Scene::Init(bool initSystems)
{
    if (initSystems)
    {
        Physics::CreatePhysicsScene(entitiesRegistry);
        AudioCore::CreateAudioScene(entitiesRegistry);
        systemsInitialized = true;
    }

    // Call on create for all loaded with scene objects
    entitiesRegistry->OnCreateAll();
}

void Scene::CreateMainCamera()
{
    _mainCameraEntity = CreateEntity();
    entitiesRegistry->GetComponent<NameComponent>(_mainCameraEntity).SetName("Main camera");
    entitiesRegistry->GetComponent<NameComponent>(_mainCameraEntity).SetTag("MainCamera");
    auto& mainCamera = entitiesRegistry->AddComponent<Camera>(_mainCameraEntity);
    entitiesRegistry->GetComponent<Transformation>(_mainCameraEntity).SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    mainCamera.SetHeight(3.0f);

    AudioCore::CreateAudioListener(_mainCameraEntity);
}

EntityID Scene::GetMainCamera()
{
    return entitiesRegistry->EntityExists(_mainCameraEntity) ? _mainCameraEntity : (_mainCameraEntity = NULL_ENTITY);
}

const std::string& Scene::GetName()
{
    return _name;
}

void Scene::SetName(const std::string& name)
{
    _name = name;
}

EntitiesRegistry* Scene::GetEntitiesRegistry()
{
    return entitiesRegistry;
}

UILayer* Scene::GetUILayer()
{
    return uiLayer;
}

EntityID Scene::CreateEntity()
{
    return CreateEntity("New entity", NULL_ENTITY);
}

EntityID Scene::CreateEntity(const std::string& name, EntityID parent)
{
    auto entity = CreateEmptyEntity();
    auto& nameComponent = entitiesRegistry->AddComponent<NameComponent>(entity);
    nameComponent.SetName(name);
    entitiesRegistry->AddComponent<Transformation>(entity);
    entitiesRegistry->AddComponent<HierarchyNode>(entity);
    LinkChildToParent(entitiesRegistry, entity, parent);

    return entity;
}

EntityID Scene::CreateEntity(AsepriteData& data)
{
    auto entity = CreateEntity(data.Name == nullptr ? "" : data.Name, NULL_ENTITY);
    auto& sr = entitiesRegistry->AddComponent<SpriteRenderer>(entity);
    if (!data.Sprites.empty())
        sr.SetImage(data.Sprites[0]);
    auto& an = entitiesRegistry->AddComponent<Animator>(entity);
    for (auto& animation : data.Animations)
    {
        an.Animations.push_back(animation);
    }

    return entity;
}

EntityID Scene::CreateEmptyEntity()
{
    EntityID entityID = entitiesRegistry->CreateNewEntity();
    auto& uuidComponent = entitiesRegistry->AddComponent<IDComponent>(entityID);
    UUID_TYPE uuid = Random::NextULong();
    uuidComponent.SetUUID(uuid);
    SetEntityByUUID(uuid, entityID);

    return entityID;
}

void Scene::DestroyEntity(EntityID entityID, float delay)
{
    if (delay <= 0.0f)
        entitiesToDelete.push_back(entityID);
    else
        delayedEntitiesToDelete.emplace_back(delay, entityID);
}

void Scene::Refresh()
{
    // Refresh hierarchy nodes
    auto nodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    for (int i = nodes.Size() - 1; i >= 0; --i)
        if (nodes[i].IsAlive())
            nodes[i].IsDirty = false;
    auto inactiveNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>(false);
    for (int i = inactiveNodes.Size() - 1; i >= 0; --i)
        if (inactiveNodes[i].IsAlive())
            inactiveNodes[i].IsDirty = false;
}

void Scene::Update()
{
    // Setup input
    Input::IgnoreEvents = Application::Context()->IgnoreEvents;

    if (!Input::IgnoreEvents)
    {
        // Poll UI events
        UIEvent uiEvent = Input::GetUIEvent();
        uiLayer->PollEvent(uiEvent);
    }

    BeforeUpdate();

    // Update scripts
    auto scripts = entitiesRegistry->GetComponentIterator<ScriptComponent>();
    int scriptsSize = scripts.Size();
    for (int i = 0; i < scriptsSize; ++i)
        if (scripts[i].IsAlive()) scripts[i].OnUpdate();

    // Update coroutines
    if (systemsInitialized)
        ScriptingSystem::UpdateCoroutines();

    while (systemsInitialized && Time::TryFixedUpdate())
    {
        for (int i = 0; i < scriptsSize; ++i)
            if (scripts[i].IsAlive()) scripts[i].OnFixedUpdate();

        // Apply transformations to physics objects and then simulate
        Physics::UpdatePhysicsTransformations();
        Physics::Simulate(Time::GetFixedDeltaTime());
        Physics::GetPhysicsTransformations();
        Physics::SendEvents();
    }

    for (int i = 0; i < scriptsSize; ++i)
        if (scripts[i].IsAlive()) scripts[i].OnLateUpdate();

    // Update inner components
    auto animators = entitiesRegistry->GetComponentIterator<Animator>();
    int animatorsSize = animators.Size();
    for (int i = 0; i < animatorsSize; ++i)
        if (animators[i].IsAlive()) animators[i].OnUpdate();

    auto audioSources = entitiesRegistry->GetComponentIterator<AudioSource>();
    int audioSourcesSize = audioSources.Size();
    for (int i = 0; i < audioSourcesSize; ++i)
        if (audioSources[i].IsAlive()) audioSources[i].OnUpdate();

    auto audioListeners = entitiesRegistry->GetComponentIterator<AudioListener>();
    int audioListenersSize = audioListeners.Size();
    for (int i = 0; i < audioListenersSize; ++i)
        if (audioListeners[i].IsAlive()) audioListeners[i].OnUpdate();

    // Update UI elements
    uiLayer->Update();

    AfterUpdate();
}

void Scene::PrepareDraw()
{
    // Clean destroyed entities
    CleanDestroyedEntities();

    // Sort hierarchy from parents to children and then apply transforms
    SortByHierarchy();
    UpdateGlobalTransformation();
    RefreshTransformation();

    BeforeRebuildUI();

    // Rebuild UI layer elements
    uiLayer->Rebuild();

    AfterPrepareDraw();

    uiLayer->Refresh();
}

void Scene::SortByHierarchy()
{
    // Sort all objects by hierarchy depth
    struct
    {
        bool operator()(const HierarchyNode& a, const HierarchyNode& b) const
        {
            return a.GetHierarchyDepth() < b.GetHierarchyDepth()
                || (a.GetHierarchyDepth() == b.GetHierarchyDepth() && a.GetNodeIndex() < b.GetNodeIndex());
        }
    } DepthComparer;
    entitiesRegistry->SortComponents<HierarchyNode>(DepthComparer);
    entitiesRegistry->ApplyOrder<HierarchyNode, Transformation>();
    entitiesRegistry->ApplyOrder<HierarchyNode, RectTransformation>();
}

void Scene::UpdateGlobalTransformation()
{
    auto hierarchyNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    auto transformationsAccessor = entitiesRegistry->GetComponentAccessor<Transformation>();
    // Components to apply changed transformation
    auto srAccessor = entitiesRegistry->GetComponentAccessor<SpriteRenderer>();
    auto mrAccessor = entitiesRegistry->GetComponentAccessor<MeshRenderer>();
    auto cameraAccessor = entitiesRegistry->GetComponentAccessor<Camera>();
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (transformationsAccessor.Has(hierarchyNode.Owner))
        {
            auto& transformation = transformationsAccessor.Get(hierarchyNode.Owner);
            transformation.UpdateTransformation(transformationsAccessor, hierarchyNode);

            bool transformationDirty = transformation.DidTransformationChange();
            if (transformationDirty && srAccessor.Has(hierarchyNode.Owner))
                srAccessor.Get(hierarchyNode.Owner).Rebuild(transformation);
            if (transformationDirty && mrAccessor.Has(hierarchyNode.Owner))
                mrAccessor.Get(hierarchyNode.Owner).Rebuild(transformation);
            if (cameraAccessor.Has(hierarchyNode.Owner))
                cameraAccessor.Get(hierarchyNode.Owner).UpdateViewProjection(transformation);
        }
    }
}

void Scene::RefreshTransformation()
{
    auto transformations = entitiesRegistry->GetComponentIterator<Transformation>();
    for (auto& transformation : transformations)
        transformation.RefreshTransformation();
}

void Scene::CleanDestroyedEntities()
{
    for (auto entity : entitiesToDelete)
    {
        if (entity != NULL_ENTITY)
            DestroyAndRemoveEntity(entity);
    }
    entitiesToDelete.clear();

    for (auto& pair : delayedEntitiesToDelete)
    {
        std::get<0>(pair) -= Time::DeltaTime();
        if (std::get<0>(pair) <= 0.0f && std::get<1>(pair) != NULL_ENTITY)
        {
            DestroyAndRemoveEntity(std::get<1>(pair));
        }
    }
    delayedEntitiesToDelete.erase(
            std::remove_if(
                    delayedEntitiesToDelete.begin(),
                    delayedEntitiesToDelete.end(),
                    [](std::tuple<float, EntityID>& pair)
                    {
                        return std::get<0>(pair) <= 0.0f;
                    }
            ),
          delayedEntitiesToDelete.end()
    );

    entitiesRegistry->ClearRemoved();
}

EntityID Scene::GetEntityByUUID(UUID_TYPE uuid)
{
    if (uuid == NULL_UUID || entitiesByUUID.find(uuid) == entitiesByUUID.end())
        return NULL_ENTITY;
    return entitiesByUUID[uuid];
}

void Scene::SetEntityByUUID(UUID_TYPE uuid, EntityID entityID)
{
    if (uuid == NULL_UUID)
        return;

    entitiesByUUID[uuid] = entityID;
}

void Scene::CleanAllEntities()
{
    entitiesRegistry->CleanAllEntities();
    entitiesByUUID.clear();
}

void Scene::DestroyAndRemoveEntity(EntityID entityID)
{
    DestroyEntityInner(entityID);
}

void Scene::DestroyEntityInner(EntityID entityID)
{
    bool clearFromUUIDs = false;
    UUID_TYPE uuid;
    if (entitiesRegistry->HasComponent<IDComponent>(entityID))
    {
        clearFromUUIDs = true;
        uuid = entitiesRegistry->GetComponent<IDComponent>(entityID).GetUUID();
    }

    entitiesRegistry->DeleteEntity(entityID);

    if (clearFromUUIDs)
        entitiesByUUID.erase(uuid);
}
