#pragma once

#include <vector>
#include "Animation.h"
#include "../Scene/Component.h"

class Animator : Component
{
public:
    void Play();
    void Play(int animation);
    void Pause();
    void Stop();
    void Restart();

    void OnUpdate() override;

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
