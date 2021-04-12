#include "Animator.h"
#include "../Core/Time.h"
#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"

void Animator::Init()
{
    if (Animations.empty())
        return;

    currentCurveFrame.resize(Animations[currentAnimation]->Curves.size());
    fill(currentCurveFrame.begin(), currentCurveFrame.end(), 0);
    initialized = true;
}

void Animator::Play()
{
    if (!initialized)
        Init();

    IsPlaying = true;
}

void Animator::Play(int animation)
{
    if (animation == currentAnimation)
        Play();

    currentAnimation = animation;
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

    if (Animations.empty() || Animations[currentAnimation] == nullptr)
        return;

    float animationTime = NormalizedTime * Animations[currentAnimation]->Length();
    animationTime += Time::DeltaTime() * Speed;
    NormalizedTime = animationTime / Animations[currentAnimation]->Length();

    for (uint32_t i = 0; i < currentCurveFrame.size(); ++i)
    {
        if (Speed > 0)
        {
            if (currentCurveFrame[i] < Animations[currentAnimation]->Curves[i].Keyframes.size() - 1)
            {
                if (animationTime >= Animations[currentAnimation]->Curves[i].Keyframes[currentCurveFrame[i] + 1].Time)
                {
                    currentCurveFrame[i]++;
                    ApplyFrame(Owner, Animations[currentAnimation]->Curves[i].Keyframes[currentCurveFrame[i]]);
                }
            }
        }
        else
        {
            if (currentCurveFrame[i] > 0)
            {
                if (animationTime <= Animations[currentAnimation]->Curves[i].Keyframes[currentCurveFrame[i] - 1].Time)
                {
                    currentCurveFrame[i]--;
                    ApplyFrame(Owner, Animations[currentAnimation]->Curves[i].Keyframes[currentCurveFrame[i]]);
                }
            }
        }
    }

    if (NormalizedTime < 0.0f && Speed < 0 || NormalizedTime > 1.0f && Speed > 0)
    {
        if (Animations[currentAnimation]->Loop)
        {
            // TODO: make normalized time show number of cycles for loop animations
            NormalizedTime = Speed < 0 ? NormalizedTime + 1.0f : NormalizedTime - 1.0f;
            for (uint32_t i = 0; i < Animations[currentAnimation]->Curves.size(); ++i)
            {
                currentCurveFrame[i] = Speed < 0 ? Animations[currentAnimation]->Curves[i].Keyframes.size() : 0;
                ApplyFrame(Owner, Animations[currentAnimation]->Curves[i].Keyframes[0]);
            }
        }
        else
        {
            Stop();
        }
    }
}
