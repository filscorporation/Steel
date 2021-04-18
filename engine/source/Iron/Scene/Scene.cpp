#include "Scene.h"
#include "Transformation.h"
#include "NameComponent.h"
#include "Hierarchy.h"
#include "../Rendering/SpriteRenderer.h"
#include "../Rendering/QuadRenderer.h"
#include "../Animation/Animator.h"

int Scene::EntitiesWasCreated = 0;

Scene::Scene()
{
    hierarchySystem = new HierarchySystem();
    transformationSystem = new TransformationSystem();
    entitiesRegistry = new EntitiesRegistry();
    uiLayer = new UILayer(this);
    entitiesRegistry->RegisterSystem<HierarchyNode>(hierarchySystem);
    entitiesRegistry->RegisterSystem<Transformation>(transformationSystem);
    entitiesRegistry->RegisterSystem<RectTransformation>(transformationSystem);
}

Scene::~Scene()
{
    delete uiLayer;
    delete entitiesRegistry;
    delete transformationSystem;
    delete hierarchySystem;
}

void Scene::CreateMainCamera()
{
    _mainCameraEntity = CreateEntity();
    auto& mainCamera = entitiesRegistry->AddComponent<Camera>(_mainCameraEntity);
    entitiesRegistry->GetComponent<Transformation>(_mainCameraEntity).SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    mainCamera.SetHeight(3.0f);
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

EntityID Scene::CreateEntity(const char* name, EntityID parent)
{
    auto entity = CreateEmptyEntity();
    auto& nameComponent = entitiesRegistry->AddComponent<NameComponent>(entity);
    nameComponent.Name = name;
    entitiesRegistry->AddComponent<Transformation>(entity);
    entitiesRegistry->AddComponent<HierarchyNode>(entity);
    LinkChildToParent(entitiesRegistry, entity, parent);

    return entity;
}

EntityID Scene::CreateEntity(AsepriteData& data)
{
    auto entity = CreateEntity(data.Name, NULL_ENTITY);
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
    EntitiesWasCreated++;

    return entitiesRegistry->CreateNewEntity();
}

void Scene::DestroyEntity(EntityID entity)
{
    entitiesToDelete.push_back(entity);
}

void Scene::DestroyAndRemoveEntity(EntityID entity)
{
    DestroyEntityInner(entity);
}

void Scene::DestroyEntityInner(EntityID entity)
{
    entitiesRegistry->DeleteEntity(entity);
}

void Scene::SortByHierarchy()
{
    // Sort all objects by hierarchy depth
    struct
    {
        bool operator()(const HierarchyNode& a, const HierarchyNode& b) const { return a.HierarchyDepth < b.HierarchyDepth; }
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
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (transformationsAccessor.Has(hierarchyNode.Owner))
        {
            auto& transformation = transformationsAccessor.Get(hierarchyNode.Owner);
            transformation.UpdateTransformation(transformationsAccessor, hierarchyNode);

            bool transformationDirty = transformation.DidTransformationChange();
            if (transformationDirty && srAccessor.Has(hierarchyNode.Owner))
                srAccessor.Get(hierarchyNode.Owner).UpdateRenderer(transformation);
        }
    }
}

void Scene::RefreshTransformation()
{
    auto transformations = entitiesRegistry->GetComponentIterator<Transformation>();
    for (auto& transformation : transformations)
        transformation.SetTransformationChanged(false);
}

void Scene::SortByDrawOrder()
{
    // Sort all objects by Z
    struct
    {
        bool operator()(QuadRenderer& a, QuadRenderer& b) const
        { return a.SortingOrder > b.SortingOrder; }
    } ZComparer;
    entitiesRegistry->SortComponents<QuadRenderer>(ZComparer);
}

void Scene::Draw()
{
    auto quadRenderers = entitiesRegistry->GetComponentIterator<QuadRenderer>();

    // Opaque pass
    for (int i = 0; i < quadRenderers.Size(); ++i)
        if (quadRenderers[i].Queue == RenderingQueue::Opaque)
            quadRenderers[i].Render();

    // Transparent pass
    for (int i = quadRenderers.Size() - 1; i >=0; --i)
        if (quadRenderers[i].Queue == RenderingQueue::Transparent)
            quadRenderers[i].Render();
}

void Scene::CleanDestroyedEntities()
{
    for (auto entity : entitiesToDelete)
    {
        if (entity != NULL_ENTITY)
            DestroyAndRemoveEntity(entity);
    }
    entitiesToDelete.clear();

    entitiesRegistry->ClearRemoved();
}

void Scene::CleanAllEntities()
{
    entitiesRegistry->CleanAllEntities();
}

Camera& Scene::GetMainCamera()
{
    return entitiesRegistry->GetComponent<Camera>(_mainCameraEntity);
}
