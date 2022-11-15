#include "Animator.h"
#include "Steel/Core/Time.h"
#include "Steel/Core/Log.h"
#include "Steel/Rendering/SpriteRenderer.h"
#include "Steel/UI/UIElements/UIImage.h"
#include "Steel/Serialization/AttributesRegistration.h"

void Animator::RegisterType()
{
    REGISTER_COMPONENT(Animator);
}

void Animator::Init()
{
    if (Animations.empty() || currentAnimation == nullptr)
        return;

    currentCurveFrame.resize(currentAnimation->Curves.size());
    fill(currentCurveFrame.begin(), currentCurveFrame.end(), 0);
    initialized = true;
}

void Animator::Play()
{
    if (!initialized)
        Init();

    IsPlaying = true;
}

void Animator::Play(const std::string& animationName)
{
    Animation* play = nullptr;
    for (auto animation : Animations)
    {
        if (animation == nullptr)
            continue;
        if (animation->Name == animationName)
        {
            play = animation;
            break;
        }
    }

    if (play == nullptr)
        return;

    if (currentAnimation != nullptr && play->ID == currentAnimation->ID)
        Play();

    currentAnimation = play;
    Restart();
}

void Animator::Pause()
{
    IsPlaying = false;
}

void Animator::Stop()
{
    initialized = false;
    NormalizedTime = 0.0f;
    IsPlaying = false;
}

void Animator::Restart()
{
    Stop();
    Play();
}

void Animator::AddAnimations(const std::vector<Animation*>& animations)
{
    for (auto& animation : animations)
    {
        if (std::find(Animations.begin(), Animations.end(), animation) == Animations.end())
            Animations.push_back(animation);
    }
}

void Animator::AddAndPlay(Animation* animation)
{
    bool exists = false;
    for (auto an : Animations)
    {
        if (an->ID == animation->ID)
        {
            exists = true;
            break;
        }
    }
    if (!exists)
        Animations.push_back(animation);

    currentAnimation = animation;
    Restart();
}

void ApplyFrame(EntityID entityID, Keyframe keyframe)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (entitiesRegistry->HasComponent<SpriteRenderer>(entityID))
    {
        auto& sr = entitiesRegistry->GetComponent<SpriteRenderer>(entityID);
        if (sr.GetImage() == nullptr && keyframe.SpriteID != NULL_RESOURCE
            || sr.GetImage() != nullptr && sr.GetImage()->ID != keyframe.SpriteID)
        {
            if (keyframe.SpriteID == NULL_RESOURCE)
                sr.SetImage(nullptr);
            else
                sr.SetImage(Application::Instance->GetResourcesManager()->GetSprite(keyframe.SpriteID));
        }

        sr.SetImageTileIndex(keyframe.TileIndex);
    }
    if (entitiesRegistry->HasComponent<UIImage>(entityID))
    {
        auto& iui = entitiesRegistry->GetComponent<UIImage>(entityID);
        if (iui.GetImage() == nullptr && keyframe.SpriteID != NULL_RESOURCE
            || iui.GetImage() != nullptr && iui.GetImage()->ID != keyframe.SpriteID)
        {
            if (keyframe.SpriteID == NULL_RESOURCE)
                iui.SetImage(nullptr);
            else
                iui.SetImage(Application::Instance->GetResourcesManager()->GetSprite(keyframe.SpriteID));
        }

        iui.SetImageTileIndex(keyframe.TileIndex);
    }
}

void Animator::OnUpdate()
{
    if (!IsPlaying)
        return;

    if (Animations.empty() || currentAnimation == nullptr)
        return;

    float animationTime = NormalizedTime * currentAnimation->Length();
    animationTime += Time::DeltaTime() * Speed;
    NormalizedTime = animationTime / currentAnimation->Length();

    for (uint32_t i = 0; i < currentCurveFrame.size(); ++i)
    {
        if (Speed > 0)
        {
            if (currentCurveFrame[i] < currentAnimation->Curves[i].Keyframes.size() - 1)
            {
                if (animationTime >= currentAnimation->Curves[i].Keyframes[currentCurveFrame[i] + 1].Time)
                {
                    currentCurveFrame[i]++;
                    ApplyFrame(Owner, currentAnimation->Curves[i].Keyframes[currentCurveFrame[i]]);
                }
            }
        }
        else
        {
            if (currentCurveFrame[i] > 0)
            {
                if (animationTime <= currentAnimation->Curves[i].Keyframes[currentCurveFrame[i] - 1].Time)
                {
                    currentCurveFrame[i]--;
                    ApplyFrame(Owner, currentAnimation->Curves[i].Keyframes[currentCurveFrame[i]]);
                }
            }
        }
    }

    if (NormalizedTime < 0.0f && Speed < 0 || NormalizedTime > 1.0f && Speed > 0)
    {
        if (currentAnimation->Loop)
        {
            // TODO: make normalized time show number of cycles for loop animations
            NormalizedTime = Speed < 0 ? NormalizedTime + 1.0f : NormalizedTime - 1.0f;
            for (uint32_t i = 0; i < currentAnimation->Curves.size(); ++i)
            {
                currentCurveFrame[i] = Speed < 0 ? currentAnimation->Curves[i].Keyframes.size() : 0;
                ApplyFrame(Owner, currentAnimation->Curves[i].Keyframes[0]);
            }
        }
        else
        {
            // Apply last frame in the end
            for (auto& curve : currentAnimation->Curves)
            {
                ApplyFrame(Owner, curve.Keyframes[Speed < 0 ? 0 : curve.Keyframes.size() - 1]);
            }
            Stop();
        }
    }
}
