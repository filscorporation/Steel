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
    for (int i = 0; i < images.Size(); ++i)
        images[i].Draw(renderContext);

    auto uiTexts = _scene->GetEntitiesRegistry()->GetComponentIterator<UIText>();
    for (int i = 0; i < uiTexts.Size(); ++i)
        uiTexts[i].Draw(renderContext);

    auto uiClippings = _scene->GetEntitiesRegistry()->GetComponentIterator<UIClipping>();
    for (int i = 0; i < uiClippings.Size(); ++i)
        uiClippings[i].Draw(renderContext);

    auto inputFields = _scene->GetEntitiesRegistry()->GetComponentIterator<UIInputField>();
    for (int i = 0; i < inputFields.Size(); ++i)
        inputFields[i].Draw(renderContext);
}
