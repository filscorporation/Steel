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
}

Sprite* UIImage::GetImage()
{
    return _image;
}
