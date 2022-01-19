#include "SceneView.h"
#include "HierarchyView.h"
#include "UIEditorTab.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"

void SceneView::RegisterType()
{
    REGISTER_TYPE(SceneView);
}

void SceneView::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    isDirty = true;
}

void SceneView::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (sceneViewImageEntity == NULL_ENTITY || !entitiesRegistry->HasComponent<UIImage>(sceneViewImageEntity))
        return;
    auto& image = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
    auto oldImage = image.GetImage();
    image.SetImage(nullptr);
    delete oldImage;
}

void SceneView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    auto scene = Application::Instance->GetCurrentScene();
    auto layer = scene->GetUILayer();

    EntityID frameEntity = layer->CreateUIImage(layer->UIResources.StraightFrameSprite, "Frame", Owner);
    auto& frameRT = entitiesRegistry->GetComponent<RectTransformation>(frameEntity);
    frameRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    frameRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    {
        auto sprite = new Sprite(editor->OpenedSceneFramebuffer->GetColorAttachment());
        sceneViewImageEntity = layer->CreateUIImage(nullptr, "Scene view image", frameEntity);
        entitiesRegistry->GetComponent<UIEventHandler>(sceneViewImageEntity).Type = EventHandlerTypes::Transparent;
        auto& viewImage = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
        viewImage.FlipImage = true;
        viewImage.SetImage(sprite);
        auto& viewImageRT = entitiesRegistry->GetComponent<RectTransformation>(sceneViewImageEntity);
        viewImageRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        viewImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        viewImageRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
        viewImageRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
    }
}

void SceneView::Update(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    auto& viewRT = entitiesRegistry->GetComponent<RectTransformation>(Owner);

    EntityID parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(Owner).GetParentNode();
    ignoreAppInput = !entitiesRegistry->GetComponent<UIEditorTab>(parentEntity).GetIsFocused();
    bool isFocused = !ignoreAppInput;

    // Controls
    if (isFocused && (Input::IsKeyPressed(KeyCodes::LeftAlt) || Input::IsKeyPressed(KeyCodes::RightAlt))
        && Input::IsMouseButtonPressed(MouseCodes::ButtonMiddle))
    {
        UpdateCameraDrag(entitiesRegistry);
    }
    if (isFocused && (Input::IsKeyPressed(KeyCodes::LeftAlt) || Input::IsKeyPressed(KeyCodes::RightAlt)))
    {
        UpdateCameraZoom(entitiesRegistry);
    }
    if (isFocused && Input::IsKeyJustPressed(KeyCodes::F))
    {
        FocusOnSelectedEntities(entitiesRegistry);
    }

    if (isDirty || viewRT.DidSizeChange() || editor->GetAppContext()->ScreenParams.IsDirty)
    {
        UpdateView(entitiesRegistry);
    }
}

void SceneView::FocusCameraOnEntity(EntitiesRegistry* entitiesRegistry, const std::vector<EntityID>& selectedEntities)
{
    auto editor = (EditorApplication*)Application::Instance;
    if (selectedEntities.empty() || !entitiesRegistry->EntityExists(editor->EditorViewCameraEntity))
        return;

    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();
    auto sceneRegistry = appScene->GetEntitiesRegistry();
    auto& cameraTransform = entitiesRegistry->GetComponent<Transformation>(editor->EditorViewCameraEntity);

    glm::vec3 focusPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    float transformsCount = 0;

    editor->SwitchContext(editor->AppContext);
    for (uint32_t selectedEntity : selectedEntities)
    {
        if (sceneRegistry->HasComponent<Transformation>(selectedEntity))
        {
            transformsCount++;
            focusPosition += sceneRegistry->GetComponent<Transformation>(selectedEntity).GetPosition();
        }
    }
    editor->SwitchContext(editor->EditorContext);

    if (transformsCount > 0)
    {
        focusPosition = glm::vec3(focusPosition.x / transformsCount, focusPosition.y / transformsCount, focusPosition.z / transformsCount);
        cameraTransform.SetPosition(focusPosition + glm::vec3(0.0f, 0.0f, 3.0f));
    }
}

void SceneView::UpdateView(EntitiesRegistry* entitiesRegistry)
{
    isDirty = false;

    auto editor = (EditorApplication*)Application::Instance;
    auto& image = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(sceneViewImageEntity);
    glm::vec2 rectSize = rt.GetRealSizeCached();

    auto width = (uint32_t)rectSize.x;
    auto height = (uint32_t)rectSize.y;

    if (entitiesRegistry->EntityExists(editor->EditorViewCameraEntity))
    {
        auto& camera = entitiesRegistry->GetComponent<Camera>(editor->EditorViewCameraEntity);
        camera.UpdateSize(width, height);
        camera.UpdateViewProjection(entitiesRegistry->GetComponent<Transformation>(editor->EditorViewCameraEntity));
    }

    editor->OpenedSceneFramebuffer->Resize(width, height);

    auto sprite = new Sprite(editor->OpenedSceneFramebuffer->GetColorAttachment());
    image.FlipImage = true;

    auto oldImage = image.GetImage();
    image.SetImage(sprite);
    delete oldImage;
}

void SceneView::UpdateCameraDrag(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    if (!entitiesRegistry->EntityExists(editor->EditorViewCameraEntity))
        return;

    auto& image = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(sceneViewImageEntity);
    glm::vec2 rectSize = rt.GetRealSizeCached();
    glm::vec2 mouseDelta = Input::GetMouseDelta();
    glm::vec2 drag = glm::vec2(mouseDelta.x / rectSize.x, mouseDelta.y / rectSize.y);

    auto& camera = entitiesRegistry->GetComponent<Camera>(editor->EditorViewCameraEntity);
    drag = glm::vec2(drag.x * camera.GetWidth(), drag.y * camera.GetHeight());

    auto& cameraTransform = entitiesRegistry->GetComponent<Transformation>(editor->EditorViewCameraEntity);
    cameraTransform.SetPosition(cameraTransform.GetPosition() - glm::vec3(drag, 0.0f));
}

void SceneView::UpdateCameraZoom(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    if (!entitiesRegistry->EntityExists(editor->EditorViewCameraEntity))
        return;

    auto& camera = entitiesRegistry->GetComponent<Camera>(editor->EditorViewCameraEntity);
    glm::vec2 scrollDelta = Input::GetMouseScrollDelta();
    float scrollStep = scrollDelta.y > 9 ? 9 : scrollDelta.y < -9 ? -9 : scrollDelta.y;
    float newSize = camera.GetHeight() - scrollStep / 10.0f * camera.GetHeight();

    // Calculate target position to zoom to
    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(sceneViewImageEntity);
    auto& cameraTransform = entitiesRegistry->GetComponent<Transformation>(editor->EditorViewCameraEntity);
    glm::vec2 localMousePosition = Input::GetMousePosition() - rt.GetRealPositionCached() + rt.GetRealSizeCached() * 0.5f;
    glm::vec2 mouseWorldPosition = {
            camera.GetWidth() * (localMousePosition.x / rt.GetRealSizeCached().x - 0.5f) + cameraTransform.GetPosition().x,
            camera.GetHeight() * (localMousePosition.y / rt.GetRealSizeCached().y - 0.5f) + cameraTransform.GetPosition().y
    };

    glm::vec2 newPosition = Math::Lerp(cameraTransform.GetPosition(), mouseWorldPosition, camera.GetHeight() / newSize - 1.0f);
    cameraTransform.SetPosition(glm::vec3(newPosition, cameraTransform.GetPosition().z));

    camera.SetHeight(newSize);
    isDirty = true;
}

void SceneView::FocusOnSelectedEntities(EntitiesRegistry* entitiesRegistry)
{
    auto hierarchyViewIterator = entitiesRegistry->GetComponentIterator<HierarchyView>();
    for (int i = 0; i < hierarchyViewIterator.Size(); ++i)
    {
        hierarchyViewIterator[i].FocusOnSelectedEntities(entitiesRegistry);
    }
}
