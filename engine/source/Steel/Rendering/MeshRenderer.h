#pragma once

#include "Mesh.h"
#include "Core/RenderContext.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/Scene/Transformation.h"

class MeshRenderer : public Component
{
    COMPONENT(MeshRenderer)

public:
    explicit MeshRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void SetDefault(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(Transformation& transformation);
    void Draw(RenderContext* renderContext);

    void SetMesh(const Mesh& mesh);
    const Mesh& GetMesh();
    void SetMaterial(Material* material);
    Material* GetMaterial() const;
    void SetCustomProperties(const MaterialPropertyBlock& properties);
    const MaterialPropertyBlock& GetCustomProperties();

private:
    void RebuildInner(Transformation& transformation);

    Mesh _mesh {};
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Material* _material = nullptr;
    MaterialPropertyBlock _customProperties;

    bool isDirty = true;
    VertexBuffer vb;
    IndexBuffer ib;
    float _sortingOrder = 0;
};
