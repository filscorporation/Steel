#include "Animator.h"
#include "../Core/Time.h"
#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"

void Animator::Init()
{
    if (Animations.empty() || currentAnimation == NULL_RESOURCE)
        return;

    auto current = Application::Instance->GetResourcesManager()->GetAnimation(currentAnimation);
    currentCurveFrame.resize(current->Curves.size());
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
    auto resources = Application::Instance->GetResourcesManager();
    ResourceID playID = NULL_RESOURCE;
    for (auto animationID : Animations)
    {
        auto animation = resources->GetAnimation(animationID);
        if (animation == nullptr)
            continue;
        if (animation->Name == animationName)
        {
            playID = animationID;
            break;
        }
    }

    if (playID == NULL_RESOURCE)
        return;

    if (playID == currentAnimation)
        Play();

    currentAnimation = playID;
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

void Animator::AddAndPlay(Animation* animation)
{
    if (std::find(Animations.begin(), Animations.end(), animation->ID) == Animations.end())
    {
        Animations.push_back(animation->ID);
    }

    Play(animation->Name);
}

void ApplyFrame(EntityID entity, Keyframe keyframe)
{
    auto& sr = GetComponentS<SpriteRenderer>(entity);
    if (sr.GetImage() == nullptr && keyframe.SpriteID != NULL_RESOURCE
        || sr.GetImage() != nullptr && sr.GetImage()->ID != keyframe.SpriteID)
    {
        if (keyframe.SpriteID == NULL_RESOURCE)
            sr.SetImage(nullptr);
        else
            sr.SetImage(Application::Instance->GetResourcesManager()->GetImage(keyframe.SpriteID));
    }

    sr.CurrentImageTileIndex = keyframe.TileIndex;
}

void Animator::OnUpdate()
{
    if (!IsPlaying)
        return;

    if (Animations.empty() || currentAnimation == NULL_RESOURCE)
        return;

    auto current = Application::Instance->GetResourcesManager()->GetAnimation(currentAnimation);

    float animationTime = NormalizedTime * current->Length();
    animationTime += Time::DeltaTime() * Speed;
    NormalizedTime = animationTime / current->Length();

    for (uint32_t i = 0; i < currentCurveFrame.size(); ++i)
    {
        if (Speed > 0)
        {
            if (currentCurveFrame[i] < current->Curves[i].Keyframes.size() - 1)
            {
                if (animationTime >= current->Curves[i].Keyframes[currentCurveFrame[i] + 1].Time)
                {
                    currentCurveFrame[i]++;
                    ApplyFrame(Owner, current->Curves[i].Keyframes[currentCurveFrame[i]]);
                }
            }
        }
        else
        {
            if (currentCurveFrame[i] > 0)
            {
                if (animationTime <= current->Curves[i].Keyframes[currentCurveFrame[i] - 1].Time)
                {
                    currentCurveFrame[i]--;
                    ApplyFrame(Owner, current->Curves[i].Keyframes[currentCurveFrame[i]]);
                }
            }
        }
    }

    if (NormalizedTime < 0.0f && Speed < 0 || NormalizedTime > 1.0f && Speed > 0)
    {
        if (current->Loop)
        {
            // TODO: make normalized time show number of cycles for loop animations
            NormalizedTime = Speed < 0 ? NormalizedTime + 1.0f : NormalizedTime - 1.0f;
            for (uint32_t i = 0; i < current->Curves.size(); ++i)
            {
                currentCurveFrame[i] = Speed < 0 ? current->Curves[i].Keyframes.size() : 0;
                ApplyFrame(Owner, current->Curves[i].Keyframes[0]);
            }
        }
        else
        {
            Stop();
        }
    }
}
