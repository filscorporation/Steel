#include "UIImage.h"
#include "../Scene/Entity.h"

UIImage::UIImage(Entity *parentEntity) : UIComponent(parentEntity)
{
    // TODO: check component collisions
    _renderer = parentEntity->AddComponent<UIRenderer>();
}

void UIImage::SetImage(Sprite *image)
{
    // TODO: update data in attached UIRenderer
    _image = image;
    _renderer->_image = _image;
    GetRectTransform()->SetSize(glm::vec2(_image->Width, _image->Height));
}

Sprite* UIImage::GetImage()
{
    return _image;
}
