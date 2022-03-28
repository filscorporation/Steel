#include "UIDrawCallsSource.h"
#include "Steel/UI/UIElements/UIClipping.h"
#include "Steel/UI/UIElements/UIImage.h"
#include "Steel/UI/UIElements/UIInputField.h"
#include "Steel/UI/UIElements/UIText.h"

UIDrawCallsSource::UIDrawCallsSource(Scene* scene)
{
    _scene = scene;
}

void UIDrawCallsSource::CollectDrawCalls(RenderContext* renderContext)
{
    auto images = _scene->GetEntitiesRegistry()->GetComponentIterator<UIImage>();
    renderContext->List.Reserve(images.Size());
    for (int i = 0; i < images.Size(); ++i)
        if (images[i].IsAlive())
            images[i].Draw(renderContext);

    auto uiTexts = _scene->GetEntitiesRegistry()->GetComponentIterator<UIText>();
    renderContext->List.Reserve(uiTexts.Size());
    for (int i = 0; i < uiTexts.Size(); ++i)
        if (uiTexts[i].IsAlive())
            uiTexts[i].Draw(renderContext);

    auto uiClippings = _scene->GetEntitiesRegistry()->GetComponentIterator<UIClipping>();
    renderContext->List.Reserve(uiClippings.Size());
    for (int i = 0; i < uiClippings.Size(); ++i)
        if (uiClippings[i].IsAlive())
            uiClippings[i].Draw(renderContext);

    auto inputFields = _scene->GetEntitiesRegistry()->GetComponentIterator<UIInputField>();
    renderContext->List.Reserve(inputFields.Size());
    for (int i = 0; i < inputFields.Size(); ++i)
        if (inputFields[i].IsAlive())
            inputFields[i].Draw(renderContext);
}
