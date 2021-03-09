#pragma once

#include <vector>
#include "Animation.h"
#include "../EntityComponentSystem/Component.h"

class Animator : public Component
{
public:
    explicit Animator(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void Play();
    void Play(int animation);
    void Pause();
    void Stop();
    void Restart();

    void OnUpdate();

    float Speed = 1.0f;
    float NormalizedTime = 0.0f;
    bool IsPlaying = false;
    std::vector<Animation*> Animations;

private:
    bool initialized = false;
    int currentAnimation = 0;
    std::vector<int> currentCurveFrame;

    void Init();
};
