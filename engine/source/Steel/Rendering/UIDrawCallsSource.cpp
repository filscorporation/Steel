#include "UIDrawCallsSource.h"
#include "Steel/UI/UIElements/UIImage.h"

UIDrawCallsSource::UIDrawCallsSource(Scene* scene)
{
    _scene = scene;
}

void UIDrawCallsSource::CollectDrawCalls(RenderContext* renderContext)
{
    auto images = _scene->GetEntitiesRegistry()->GetComponentIterator<UIImage>();
    for (int i = 0; i < images.Size(); ++i)
        images[i].Draw(renderContext);

    // TODO: add other components
}
