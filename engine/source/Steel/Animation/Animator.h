#pragma once

#include <vector>

#include "Animation.h"
#include "Steel/EntityComponentSystem/Component.h"

class Animator : public Component
{
    DEFINE_TYPE(Animator)

public:
    explicit Animator(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void Play();
    void Play(const std::string& animationName);
    void Pause();
    void Stop();
    void Restart();

    void AddAnimations(const std::vector<Animation*>& animations);
    void AddAndPlay(Animation* animation);

    void OnUpdate();

    float Speed = 1.0f;
    float NormalizedTime = 0.0f;
    bool IsPlaying = false;
    std::vector<Animation*> Animations;

private:
    bool initialized = false;
    Animation* currentAnimation = nullptr;
    std::vector<uint32_t> currentCurveFrame;

    void Init();
};
