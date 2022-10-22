#include "ResourcesInternalCalls.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Input/Input.h"

ResourceID ResourcesInternalCalls::ResourcesManager_LoadImage(MonoString* path)
{
    auto image = Application::Instance->GetResourcesManager()->LoadSprite(ScriptingCore::ToString(path));
    return image == nullptr ? NULL_RESOURCE : image->ID;
}

ResourceID ResourcesInternalCalls::ResourcesManager_LoadAsepriteData(MonoString* path, bool loopAll)
{
    auto data = Application::Instance->GetResourcesManager()->LoadAsepriteData(ScriptingCore::ToString(path), loopAll);
    return data == nullptr ? NULL_RESOURCE : data->ID;
}

ResourceID ResourcesInternalCalls::ResourcesManager_LoadAudioTrack(MonoString* path)
{
    auto track = Application::Instance->GetResourcesManager()->LoadAudioTrack(ScriptingCore::ToString(path));
    return track == nullptr ? NULL_RESOURCE : track->ID;
}

ResourceID ResourcesInternalCalls::ResourcesManager_LoadShader(MonoString* vsPath, MonoString* fsPath)
{
    auto shader = Application::Instance->GetResourcesManager()->LoadShader(ScriptingCore::ToString(vsPath), ScriptingCore::ToString(fsPath));
    return shader == nullptr ? NULL_RESOURCE : shader->ID;
}

ResourceID ResourcesInternalCalls::ResourcesManager_CreateMaterial(ResourceID shaderID)
{
    auto shader = Application::Instance->GetResourcesManager()->GetShader(shaderID);
    auto material = new Material();
    material->MainShader = shader;
    Application::Instance->GetResourcesManager()->AddResource(material);
    return material->ID;
}

float ResourcesInternalCalls::AudioTrack_GetLength(ResourceID audioTrackID)
{
    auto track = Application::Instance->GetResourcesManager()->GetAudioTrack(audioTrackID);
    if (track != nullptr)
        return track->LengthInSeconds;
    return 0;
}

void ResourcesInternalCalls::Sprite_SetAsNormal(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
        image->SetAsNormal();
}

void ResourcesInternalCalls::Sprite_SetAsSpriteSheet(ResourceID spriteID, int tileWidth, int tileHeight)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
        image->SetAsSpriteSheet(tileWidth, tileHeight);
}

void ResourcesInternalCalls::Sprite_SetAs9Sliced(ResourceID spriteID, int offset)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
        image->SetAsSliced(offset);
}

void ResourcesInternalCalls::Sprite_SetAs9Sliced2(ResourceID spriteID, int offsetTop, int offsetBottom, int offsetLeft, int offsetRight)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
        image->SetAsSliced(offsetTop, offsetBottom, offsetLeft, offsetRight);
}

uint32_t ResourcesInternalCalls::Sprite_GetTextureID(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    return image == nullptr || image->SpriteTexture == nullptr ? 0 : image->SpriteTexture->GetTextureID();
}

int ResourcesInternalCalls::Sprite_GetWidth(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    return image == nullptr || image->SpriteTexture == nullptr ? 0 : (int)image->SpriteTexture->GetWidth();
}

int ResourcesInternalCalls::Sprite_GetHeight(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    return image == nullptr || image->SpriteTexture == nullptr ? 0 : (int)image->SpriteTexture->GetHeight();
}

int ResourcesInternalCalls::Sprite_GetPixelsPerUnit(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    return image == nullptr ? 0 : image->PixelsPerUnit;
}

void ResourcesInternalCalls::Sprite_SetPixelsPerUnit(ResourceID spriteID, int pixelsPerUnit)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
        image->PixelsPerUnit = pixelsPerUnit;
}

void ResourcesInternalCalls::Sprite_GetPivot(ResourceID spriteID, glm::vec2* pivot)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
    {
        pivot->x = image->Pivot.x;
        pivot->y = image->Pivot.y;
    }
}

void ResourcesInternalCalls::Sprite_SetPivot(ResourceID spriteID, glm::vec2* pivot)
{
    auto image = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (image != nullptr)
    {
        image->Pivot.x = pivot->x;
        image->Pivot.y = pivot->y;
    }
}

ResourceID ResourcesInternalCalls::Animation_FromSpriteSheet(ResourceID spriteID, float length)
{
    auto sprite = Application::Instance->GetResourcesManager()->GetSprite(spriteID);
    if (sprite == nullptr)
        return NULL_RESOURCE;

    auto animation = new Animation(sprite, length);
    if (animation->Name.empty())
    {
        animation->Name = std::to_string(animation->ID);
    }
    Application::Instance->GetResourcesManager()->AddResource(animation);

    return animation->ID;
}

ResourceID ResourcesInternalCalls::Animation_FromSprites(MonoArray* spritesIDs, float length)
{
    std::vector<ResourceID> ids;
    ScriptingCore::FromMonoUInt64Array(spritesIDs, ids);

    std::vector<Sprite*> sprites;
    sprites.reserve(ids.size());
    for (auto id : ids)
    {
        sprites.push_back(Application::Instance->GetResourcesManager()->GetSprite(id));
    }
    auto animation = new Animation(sprites, length);
    if (animation->Name.empty())
    {
        animation->Name = std::to_string(animation->ID);
    }
    Application::Instance->GetResourcesManager()->AddResource(animation);

    return animation->ID;
}

float ResourcesInternalCalls::Animation_GetLength(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return false;

    return animation->Length();
}

void ResourcesInternalCalls::Animation_EndWithEmptyFrame(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return;

    animation->EndWithNull();
}

MonoString* ResourcesInternalCalls::Animation_GetName(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return nullptr;

    return ScriptingCore::FromString(animation->Name.c_str());
}

void ResourcesInternalCalls::Animation_SetName(ResourceID animationID, MonoString* name)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return;

    animation->Name = ScriptingCore::ToString(name);
}

bool ResourcesInternalCalls::Animation_GetLoop(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return false;

    return animation->Loop;
}

void ResourcesInternalCalls::Animation_SetLoop(ResourceID animationID, bool loop)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return;

    animation->Loop = loop;
}

MonoArray* ResourcesInternalCalls::AsepriteData_GetSprites(ResourceID resourceID)
{
    auto data = Application::Instance->GetResourcesManager()->GetAsepriteData(resourceID);
    if (data == nullptr)
        return nullptr;

    std::vector<uint64_t> spritesIDs;
    spritesIDs.reserve(data->Sprites.size());
    for (auto sprite : data->Sprites)
    {
        spritesIDs.push_back(sprite->ID);
    }

    return ScriptingCore::ToMonoUInt64Array(spritesIDs);
}

MonoArray* ResourcesInternalCalls::AsepriteData_GetAnimations(ResourceID resourceID)
{
    auto data = Application::Instance->GetResourcesManager()->GetAsepriteData(resourceID);
    if (data == nullptr)
        return nullptr;

    std::vector<uint64_t> animationsIDs;
    animationsIDs.reserve(data->Animations.size());
    for (auto animation : data->Animations)
    {
        animationsIDs.push_back(animation->ID);
    }

    return ScriptingCore::ToMonoUInt64Array(animationsIDs);
}

EntityID ResourcesInternalCalls::AsepriteData_CreateEntityFromAsepriteData(ResourceID resourceID)
{
    auto data = Application::Instance->GetResourcesManager()->GetAsepriteData(resourceID);
    if (data == nullptr)
        return NULL_ENTITY;

    return Application::Instance->GetCurrentScene()->CreateEntity(*data);
}

ResourceID ResourcesInternalCalls::Material_GetShader(ResourceID resourceID)
{
    auto material = Application::Instance->GetResourcesManager()->GetMaterial(resourceID);
    if (material == nullptr)
        return NULL_ENTITY;

    return material->MainShader == nullptr ? NULL_RESOURCE : material->MainShader->ID;
}

void ResourcesInternalCalls::Material_SetShader(ResourceID resourceID, ResourceID shaderID)
{
    auto material = Application::Instance->GetResourcesManager()->GetMaterial(resourceID);
    if (material == nullptr)
        return;
    material->MainShader = Application::Instance->GetResourcesManager()->GetShader(shaderID);
}

void ResourcesInternalCalls::Material_GetProperties(ResourceID resourceID, MaterialPropertyBlockInternal* properties)
{
    auto material = Application::Instance->GetResourcesManager()->GetMaterial(resourceID);
    if (material == nullptr)
        return;
    properties->FromMaterialPropertyBlock(material->Properties);
}

void ResourcesInternalCalls::Material_SetProperties(ResourceID resourceID, MaterialPropertyBlockInternal properties)
{
    auto material = Application::Instance->GetResourcesManager()->GetMaterial(resourceID);
    if (material == nullptr)
        return;
    MaterialPropertyBlock propertiesOut;
    properties.ToMaterialPropertyBlock(propertiesOut);
    material->Properties = propertiesOut;
}
